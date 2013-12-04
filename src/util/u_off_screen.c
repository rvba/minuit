/*
 * Demo of off-screen Mesa rendering
 *
 * See Mesa/include/GL/osmesa.h for documentation of the OSMesa functions.
 *
 * If you want to render BIG images you'll probably have to increase
 * MAX_WIDTH and MAX_Height in src/config.h.
 *
 * This program is in the public domain.
 *
 * Brian Paul
 *
 * PPM output provided by Joerg Schmalzl.
 * ASCII PPM output added by Brian Paul.
 *
 * Usage: osdemo [filename]
 */


#include "common.h"
#include "util.h"
#include "memory.h"

static int Width = 400;
static int Height = 400;

/*
int app_off_screen(t_app *app)
{
	OSMesaContext ctx;

	void *buffer;
	//char *filename = NULL;

	int Width=300;
	int Height=200;

	//  Create an RGBA-mode context 
	#if OSMESA_MAJOR_VERSION * 100 + OSMESA_MINOR_VERSION >= 305
	//  specify Z, stencil, accum sizes 
	printf("MAJOR\n");
	//ctx = OSMesaCreateContextExt( OSMESA_RGBA, 16, 0, 0, NULL );
	ctx = OSMesaCreateContext( OSMESA_RGBA, NULL );
	#else
	printf("%MINOR\n");
	ctx = OSMesaCreateContext( OSMESA_RGBA, NULL );
	#endif
	if (!ctx) {
	printf("OSMesaCreateContext failed!\n");
	return 0;
	}

	// Allocate the image buffer 
	buffer = mem_malloc( Width * Height * 4 * sizeof(GLubyte) );
	if (!buffer) {
	printf("Alloc image buffer failed!\n");
	return 0;
	}

	// Bind the buffer to the context and make it current 
	if (!OSMesaMakeCurrent( ctx, buffer, GL_UNSIGNED_BYTE, Width, Height )) {
	printf("OSMesaMakeCurrent failed!\n");
	return 0;
	}


	{
	int z, s, a;
	glGetIntegerv(GL_DEPTH_BITS, &z);
	glGetIntegerv(GL_STENCIL_BITS, &s);
	glGetIntegerv(GL_ACCUM_RED_BITS, &a);
	printf("Depth=%d Stencil=%d Accum=%d\n", z, s, a);
	}

	u_render_image();
	u_write_ppm("out.ppm", buffer, Width, Height);

	printf("all done\n");

	free( buffer );

	OSMesaDestroyContext( ctx );

	return 0;
}
*/

/*
static void
Sphere(float radius, int slices, int stacks)
{
   GLUquadric *q = gluNewQuadric();
   gluQuadricNormals(q, GLU_SMOOTH);
   gluSphere(q, radius, slices, stacks);
   gluDeleteQuadric(q);
}


static void
Cone(float base, float height, int slices, int stacks)
{
   GLUquadric *q = gluNewQuadric();
   gluQuadricDrawStyle(q, GLU_FILL);
   gluQuadricNormals(q, GLU_SMOOTH);
   gluCylinder(q, base, 0.0, height, slices, stacks);
   gluDeleteQuadric(q);
}
*/


void u_write_ppm(const char *filename, unsigned char *buffer, int width, int height)
{
   const int binary = 0;
   FILE *f = fopen( filename, "w" );
   if (f) {
      int i, x, y;
      //const GLubyte *ptr = buffer;
      unsigned char *ptr = buffer;
      if (binary) {
         fprintf(f,"P6\n");
         fprintf(f,"# ppm-file created by osdemo.c\n");
         fprintf(f,"%i %i\n", width,height);
         fprintf(f,"255\n");
         fclose(f);
         f = fopen( filename, "ab" );  /* reopen in binary append mode */
         for (y=height-1; y>=0; y--) {
            for (x=0; x<width; x++) {
               i = (y*width + x) * 4;
               fputc(ptr[i], f);   /* write red */
               fputc(ptr[i+1], f); /* write green */
               fputc(ptr[i+2], f); /* write blue */
            }
         }
      }
      else {
         /*ASCII*/
         int counter = 0;
         fprintf(f,"P3\n");
         fprintf(f,"# ascii ppm file created by osdemo.c\n");
         fprintf(f,"%i %i\n", width, height);
         fprintf(f,"255\n");
         for (y=height-1; y>=0; y--) {
            for (x=0; x<width; x++) {
               i = (y*width + x) * 4;
               fprintf(f, " %3d %3d %3d", ptr[i], ptr[i+1], ptr[i+2]);
               counter++;
               if (counter % 5 == 0)
                  fprintf(f, "\n");
            }
         }
      }
      fclose(f);
   }
}


//static void u_write_targa(const char *filename, const GLubyte *buffer, int width, int height)
void u_write_targa(const char *filename, unsigned char *buffer, int width, int height)
{
   FILE *f = fopen( filename, "w" );
   if (f) {
      int i, x, y;
      //const GLubyte *ptr = buffer;
      unsigned char *ptr = buffer;
      printf ("osdemo, writing tga file \n");
      fputc (0x00, f);	/* ID Length, 0 => No ID	*/
      fputc (0x00, f);	/* Color Map Type, 0 => No color map included	*/
      fputc (0x02, f);	/* Image Type, 2 => Uncompressed, True-color Image */
      fputc (0x00, f);	/* Next five bytes are about the color map entries */
      fputc (0x00, f);	/* 2 bytes Index, 2 bytes length, 1 byte size */
      fputc (0x00, f);
      fputc (0x00, f);
      fputc (0x00, f);
      fputc (0x00, f);	/* X-origin of Image	*/
      fputc (0x00, f);
      fputc (0x00, f);	/* Y-origin of Image	*/
      fputc (0x00, f);
      fputc (Width & 0xff, f);      /* Image Width	*/
      fputc ((Width>>8) & 0xff, f);
      fputc (Height & 0xff, f);     /* Image Height	*/
      fputc ((Height>>8) & 0xff, f);
      fputc (0x18, f);		/* Pixel Depth, 0x18 => 24 Bits	*/
      fputc (0x20, f);		/* Image Descriptor	*/
      fclose(f);
      f = fopen( filename, "ab" );  /* reopen in binary append mode */
      for (y=height-1; y>=0; y--) {
         for (x=0; x<width; x++) {
            i = (y*width + x) * 4;
            fputc(ptr[i+2], f); /* write blue */
            fputc(ptr[i+1], f); /* write green */
            fputc(ptr[i], f);   /* write red */
         }
      }
   }
}





