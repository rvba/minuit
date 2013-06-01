/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "util.h"
#include "txt.h"
#include "sketch.h"
#include "context.h"
#include "event.h"
#include "ui.h"

#define _gLS GL_LINE_STRIP
#define _gL GL_LINES
#define _gLL GL_LINE_LOOP

#define _DEBUG 0 
#define TXT_COL 1
#define LETTER_COLUMN_TOT 4 
#define LETTER_COLUMN_LENGTH .5
#define LETTER_COLUMN_RESOLUTION 3
#define LETTER_ROW_LENGTH 0.5
#define LETTER_SPACE 9
#define LETTER_SIZE_X 3
#define LETTER_SIZE_Y 4
#define LETTER_RESOLUTION 2.5

#define _SCALE 1.5
#define _PRECISION "%-.2f"

#define _BK 25
#define TXT_LAYOUT_X 255
#define TXT_LAYOUT_RES_X 16
#define TXT_LAYOUT_RES_Y 32
#define TXT_LAYOUT_SPACING_X 1
#define TXT_LAYOUT_SPACING_Y 1

#define TXT_GRID_STEP 0.5
#define TXT_GRID_SIZE_X 4
#define TXT_GRID_SIZE_Y 4
#define TXT_GRID_SPACING 2

#define TXT_LETTER_SCALE_X 2.5
#define TXT_LETTER_SCALE_Y 4
#define TXT_LETTER_SPACING 2.8

float __zoom = 0.7;

int **LAYOUT; 

char _alphabet[]="abcdefghijklmnopqrstuvwxyz0123456789.*+/-= ";

float _qd[]={
	/*
	0.0,0.0,
	0.5,0.0,
	1.0,0.0,
	1.5,0.0,
	2.0,0.0,
	*/

	0.0,-0.1,
	0.5,-0.1,
	1.0,-0.1,
	1.5,-0.1,
	2.0,-0.1,

	0.0,0.5,
	0.5,0.5,
	1.0,0.5,
	1.5,0.5,
	2.0,0.5,

	0.0,1.0,
	0.5,1.0,
	1.0,1.0,
	1.5,1.0,
	2.0,1.0,

	0.0,1.5,
	0.5,1.5,
	1.0,1.5,
	1.5,1.5,
	2.0,1.5,

	0.0,2.5,
	0.5,2.5,
	1.0,2.5,
	1.5,2.5,
	2.0,2.5,

	};

/*

20 - 21 - 22 - 23 - 24
|    |    |    |    |
15 - 16 - 17 - 18 - 19
|    |    |    |    |
10 - 11 - 12 - 13 - 14
|    |    |    |    |
05 - 06 - 07 - 08 - 09
|    |    |    |    |
00 - 01 - 02 - 03 - 04

*/



int _underscore[]={5,9,_BK};
int _at[]={16,18,18,8,8,6,6,11,11,13,8,4,4,24,24,20,20,0,0,2,_BK};
int _period[]={8,1,_BK};
int _parent_left[]={17,11,11,7,_BK};
int _parent_right[]={17,13,13,7,_BK};


//int _b[]={20,10,10,6,6,9,9,14,14,18,18,15,_BK};
int _b[]={20,5,5,6,6,9,9,19,19,15,_BK};
int _d[]={24,14,9,9,8,5,5,15,15,19,_BK};
int _p[]={5,7,8,18,18,15,15,0,_BK};
//int _n[]={15,16,6,16,15,19,19,9,_BK};
int _j[]={15,17,17,2,2,0,_BK};
int _s[]={18,15,15,10,10,14,14,9,9,5,_BK};
//int _u[]={15,5,5,8,8,18,6,9,_BK};
//int _u[]={15,16,16,6,6,9,9,19,_BK};
int _g[]={19,16,16,6,6,9,19,4,4,0,_BK};
int _slash[]={18,6,_BK};
int _two_dots[]={16,17,6,8,_BK};


/*
int _a[]={6,16,16,18,18,8,11,13,_BK};
int _b[]={16,17,17,12,12,13,13,8,8,6,6,16,11,12,_BK};
int _c[]={18,16,16,6,6,8,_BK};
int _d[]={16,17,17,13,13,7,7,6,6,16,_BK};
int _e[]={16,18,11,12,6,8,16,6,_BK};
int _f[]={18,16,16,6,12,11,_BK};
int _g[]={18,16,16,6,6,8,8,13,13,12,_BK};
int _h[]={18,8,16,6,11,13,_BK};
int _i[]={17,7,_BK};
int _j[]={17,18,18,8,8,6,6,11,_BK};
int _k[]={16,6,11,18,11,8,_BK};
int _l[]={16,6,6,8,_BK};
int _m[]={6,16,16,7,7,18,18,8,_BK};
int _n[]={6,16,16,8,8,18,_BK};
int _o[]={16,18,18,8,8,6,6,16,_BK};
int _p[]={18,16,16,6,11,13,13,18,_BK};
int _q[]={16,18,18,8,8,6,6,16,12,8,_BK};
int _r[]={16,18,18,13,13,11,16,6,11,8,_BK};
int _s[]={18,16,16,11,11,13,13,8,8,6,_BK};
int _t[]={16,18,17,7,_BK};
int _u[]={16,6,6,8,8,18,_BK};
int _v[]={16,7,7,18,_BK};
int _w[]={16,6,6,17,17,8,8,18,_BK};
int _x[]={16,8,6,18,_BK};
int _y[]={16,12,12,18,12,7,_BK};
int _z[]={16,18,18,6,6,8,_BK};
*/



int _a[]={15,19,19,9,9,5,5,10,10,14,_BK};
//int _b[]={20,5,5,9,9,19,19,15,_BK};
//int _c[]={19,15,15,5,5,9,_BK};
int _c[]={18,15,15,5,5,8,_BK};
//int _d[]={24,9,9,5,5,15,15,19,_BK};
//int _d[]={24,9,9,5,5,15,15,19,_BK};
int _e[]={10,13,13,18,18,15,15,5,5,9,_BK};
//int _f[]={24,22,22,2,10,14,_BK};
int _f[]={15,17,23,21,21,1,_BK};
//int _g[]={19,15,15,5,5,9,19,4,4,0,_BK};
//int _g[]={19,15,15,5,5,9,19,4,4,0,_BK};
int _h[]={20,5,15,19,19,9,_BK};
int _i[]={16,17,17,7,5,9,_BK};
//int _j[]={16,17,17,2,2,0,_BK};
int _k[]={20,5,10,19,10,9,_BK};
int _l[]={20,21,21,6,6,9,_BK};
int _m[]={5,15,15,19,19,9,17,7,_BK};

//int _n[]={0,20,20,24,24,4,_BK};
int _n[]={5,15,15,19,19,9,_BK};

//int _n[]={6,16,16,19,19,9,_BK};
int _o[]={15,19,19,9,9,5,5,15,_BK};
//int _p[]={5,9,9,19,19,15,15,0,_BK};
int _q[]={9,5,5,15,15,19,19,4,_BK};
//int _r[]={15,19,16,6,5,7,_BK};
int _r[]={15,19,16,6,6,7,_BK};
//int _s[]={19,15,15,10,10,14,14,9,9,5,_BK};
//int _s[]={19,15,15,10,10,14,14,9,9,5,_BK};
int _t[]={20,5,5,9,15,18,_BK};
int _u[]={15,5,5,9,9,19,_BK};
int _v[]={15,7,7,19,_BK};
int _w[]={15,5,5,9,9,19,7,17,_BK};
int _x[]={15,9,19,5,_BK};
int _y[]={15,5,5,9,19,4,4,0,_BK};
int _z[]={15,19,19,5,5,9,_BK};


int _nul[]={0,4,_BK};
int _zero[]={15,19,19,9,9,5,5,15,5,19,_BK};
int _one[]={15,17,17,7,5,9,_BK};
int _two[]={15,19,19,14,14,10,10,5,5,9,_BK};
int _three[]={15,19,19,9,9,5,10,14,_BK};
int _four[]={15,10,10,14,17,7,_BK};
int _five[]={19,15,15,10,10,14,14,9,9,5,_BK};
int _six[]={15,5,5,9,9,14,14,10,_BK};
int _seven[]={15,19,19,9,_BK};
int _eight[]={15,19,19,9,9,5,5,15,10,14,_BK};
int _nine[]={14,10,10,15,15,19,19,9,9,5,_BK};
int _dot[]={6,8,_BK};
int _mul[]={17,7,10,14,15,9,19,5,_BK};
int _plus[]={17,7,10,14,_BK};
int _division[]={19,5,_BK}; 
int _minus[]={11,13,_BK};
int _space[]={_BK};
int _equal[]={15,18,5,8,_BK};


void draw_txt_gl_line(int i[])
{
	glLineWidth(1);
	glBegin(GL_LINES);
		int a=0;
		while(*i!=_BK)
		{
			int b = *(i+a);
			b*=2;
			glVertex2f((_qd[b])*LETTER_SIZE_X,(_qd[b+1])*LETTER_SIZE_Y);
			i++;
		}
	glEnd();
}

/** draw plain rectangle for letter selection */

void txt_draw_cell(char *string,float *pos)
{
	glTranslatef(pos[0],pos[1],pos[2]);
	char *s = string;
	float sc=4;
	glScalef(sc,sc,sc);
	float color[] ={0,0,0};
	while(*s)
	{
		float p[3]={0,0,0};

		float w=LETTER_SPACE;
		float h=LETTER_SPACE;

		skt_msh_rectangle(p,w,h,color);

		glTranslatef(LETTER_SPACE,0,0);
		s++;
	}
}

/** draw plain rectangle for letter selection */

void txt_draw_letter_cell(float *pos)
{
	glTranslatef(pos[0],pos[1],pos[2]);
	float p[3]={0,0,0};
	float w=LETTER_SIZE_X*2;
	float h=LETTER_SIZE_Y*2;
	float color[]={0,0,0};

	float s=4;
	glScalef(s,s,s);

	skt_msh_rectangle(p,w,h,color);
}

void txt_draw_letter(char letter)
{
	int indice = (int)letter;
	switch(indice)
	{
		case 97:draw_txt_gl_line(_a);break;
		case 98:draw_txt_gl_line(_b);break;
		case 99:draw_txt_gl_line(_c);break;
		case 100:draw_txt_gl_line(_d);break;
		case 101:draw_txt_gl_line(_e);break;
		case 102:draw_txt_gl_line(_f);break;
		case 103:draw_txt_gl_line(_g);break;
		case 104:draw_txt_gl_line(_h);break;
		case 105:draw_txt_gl_line(_i);break;
		case 106:draw_txt_gl_line(_j);break;
		case 107:draw_txt_gl_line(_k);break;
		case 108:draw_txt_gl_line(_l);break;
		case 109:draw_txt_gl_line(_m);break;
		case 110:draw_txt_gl_line(_n);break;
		case 111:draw_txt_gl_line(_o);break;
		case 112:draw_txt_gl_line(_p);break;
		case 113:draw_txt_gl_line(_q);break;
		case 114:draw_txt_gl_line(_r);break;
		case 115:draw_txt_gl_line(_s);break;
		case 116:draw_txt_gl_line(_t);break;
		case 117:draw_txt_gl_line(_u);break;
		case 118:draw_txt_gl_line(_v);break;
		case 119:draw_txt_gl_line(_w);break;
		case 120:draw_txt_gl_line(_x);break; 
		case 121:draw_txt_gl_line(_y);break; 
		case 122:draw_txt_gl_line(_z);break; 

		case 32:draw_txt_gl_line(_space);break;
		case 42:draw_txt_gl_line(_mul);break;
		case 43:draw_txt_gl_line(_plus);break;
		case 45:draw_txt_gl_line(_minus);break;
		case 46:draw_txt_gl_line(_dot);break;
		case 47:draw_txt_gl_line(_division);break;
		case 48:draw_txt_gl_line(_zero);break;
		case 49:draw_txt_gl_line(_one);break;
		case 50:draw_txt_gl_line(_two);break;
		case 51:draw_txt_gl_line(_three);break;
		case 52:draw_txt_gl_line(_four);break;
		case 53:draw_txt_gl_line(_five);break;
		case 54:draw_txt_gl_line(_six);break;
		case 55:draw_txt_gl_line(_seven);break;
		case 56:draw_txt_gl_line(_eight);break;
		case 57:draw_txt_gl_line(_nine);break;
		case 61:draw_txt_gl_line(_equal);break;

		default:draw_txt_gl_line(_nul);break;

	}
}

void txt_draw_direct_2d(char *string,float pos[3],float *color,float scale)
{
	char *cursor = string;

	skt_color(color[0],color[1],color[2],1);

	while(*cursor)
	{
		txt_draw_letter(*cursor);
		glTranslatef(LETTER_SPACE,0,0);
		cursor++;
	}
}

void txt_draw_list_2d(char *string,float pos[3],float scale) //XXX scale !!
{
	t_context *C = ctx_get();
	char *cursor = string;

	int *alphabet = C->ui->alphabet;

	if(pos)
	{
		glPushMatrix();
		glTranslatef(pos[0],pos[1]-2,pos[2]);
	}

	while(*cursor)
	{
		int l = (int)(*cursor);
		int p;
		p = l-97;

		// letters
		if(p>=0) glCallList(C->ui->alphabet[p]); 
		
		// special characters
		else 
		{
			switch(l)
			{
				case 32: glCallList(alphabet[41]);break; // space
				case 43: glCallList(alphabet[38]);break; // +
				case 42: glCallList(alphabet[37]);break; // *
				case 45: glCallList(alphabet[40]);break; // -
				case 46: glCallList(alphabet[36]);break; // .
				case 47: glCallList(alphabet[39]);break; // /
				case 48: glCallList(alphabet[26]);break; // 0
				case 49: glCallList(alphabet[27]);break; // 1
				case 50: glCallList(alphabet[28]);break; // 2
				case 51: glCallList(alphabet[29]);break; // 3
				case 52: glCallList(alphabet[30]);break; // 4
				case 53: glCallList(alphabet[31]);break; // 5
				case 54: glCallList(alphabet[32]);break; // 6
				case 55: glCallList(alphabet[33]);break; // 7
				case 56: glCallList(alphabet[34]);break; // 8
				case 57: glCallList(alphabet[35]);break; // 9
				default: break;
			}
		}

		glTranslatef(LETTER_SPACE,0,0);
		cursor++;

	}

	if(pos)
	{
		glPopMatrix();
	}
}

// BUILD DISPLAY LISTS

int letter_make(char letter,float *color)
{
	t_context *C = ctx_get();
	int indice = glGenLists(1);
	float width = C->ui->font_width;

	glNewList(indice,GL_COMPILE);
		glDisable(GL_LIGHTING);
		skt_color(color[0],color[1],color[2],color[3]);
		glLineWidth(width);
		txt_draw_letter(letter);
	glEndList();

	return indice;
}

void txt_alphabet_show(void)
{
	char *letter = _alphabet;

	while(*letter)
	{
		printf("%c %d | \n",*letter,(int)(*letter));
		letter+=4;
	}
}

void txt_alphabet_make(void)
{
	t_context *C = ctx_get();
	int i=0;
	float black[]={0,0,0,0};

	i=0;
	char *letter=_alphabet;

	while(*letter)
	{
		C->ui->alphabet[i]=letter_make(*letter,black);
		letter++;
		i++;
	}
}

void txt_layout_add(char letter,int  *points)
{
	int i=0;
	int psize=0;
	int *p=points;

	while(*p!=_BK)
	{
		psize++;
		p++;
	}

	psize++;//XXX
		
	LAYOUT[(int)letter]=(int *)malloc(sizeof(int)*psize);

	p=points;

	while(*p!=_BK)
	{
		LAYOUT[(int)letter][i]=*p;
		p++;
		i++;
	}

	LAYOUT[(int)letter][i]=_BK;//XXX
}

void txt_layout_init(void)
{
	LAYOUT=(int **)malloc(sizeof(int *)*TXT_LAYOUT_X);

	int i;
	for(i=0;i<TXT_LAYOUT_X;i++)
	{
		LAYOUT[i]=NULL;
	}

	txt_layout_add('a',_a);
	txt_layout_add('b',_b);
	txt_layout_add('c',_c);
	txt_layout_add('d',_d);
	txt_layout_add('e',_e);
	txt_layout_add('f',_f);
	txt_layout_add('g',_g);
	txt_layout_add('h',_h);
	txt_layout_add('i',_i);
	txt_layout_add('j',_j);
	txt_layout_add('k',_k);
	txt_layout_add('l',_l);
	txt_layout_add('m',_m);
	txt_layout_add('n',_n);
	txt_layout_add('o',_o);
	txt_layout_add('p',_p);
	txt_layout_add('q',_q);
	txt_layout_add('r',_r);
	txt_layout_add('s',_s);
	txt_layout_add('t',_t);
	txt_layout_add('u',_u);
	txt_layout_add('v',_v);
	txt_layout_add('w',_w);
	txt_layout_add('x',_x);
	txt_layout_add('y',_y);
	txt_layout_add('z',_z);

	txt_layout_add('0',_zero);
	txt_layout_add('1',_one);
	txt_layout_add('2',_two);
	txt_layout_add('3',_three);
	txt_layout_add('4',_four);
	txt_layout_add('5',_five);
	txt_layout_add('6',_six);
	txt_layout_add('7',_seven);
	txt_layout_add('8',_eight);
	txt_layout_add('9',_nine);

	txt_layout_add('.',_dot);
	txt_layout_add('*',_mul);
	txt_layout_add('+',_plus);
	txt_layout_add('/',_division);
	txt_layout_add('=',_equal);
	txt_layout_add('-',_minus);

	txt_layout_add('_',_underscore);
	txt_layout_add('@',_at);
	txt_layout_add('/',_slash);
	txt_layout_add(':',_two_dots);
	txt_layout_add(',',_period);
	txt_layout_add('(',_parent_left);
	txt_layout_add(')',_parent_right);

	txt_layout_add('A',_a);
	txt_layout_add('B',_b);
	txt_layout_add('C',_c);
	txt_layout_add('D',_d);
	txt_layout_add('E',_e);
	txt_layout_add('F',_f);
	txt_layout_add('G',_g);
	txt_layout_add('H',_h);
	txt_layout_add('I',_i);
	txt_layout_add('J',_j);
	txt_layout_add('K',_k);
	txt_layout_add('L',_l);
	txt_layout_add('M',_m);
	txt_layout_add('N',_n);
	txt_layout_add('O',_o);
	txt_layout_add('P',_p);
	txt_layout_add('Q',_q);
	txt_layout_add('R',_r);
	txt_layout_add('S',_s);
	txt_layout_add('T',_t);
	txt_layout_add('U',_u);
	txt_layout_add('V',_v);
	txt_layout_add('W',_w);
	txt_layout_add('X',_x);
	txt_layout_add('Y',_y);
	txt_layout_add('Z',_z);

}

void txt_letter_draw(char letter,float factor_x,float factor_y, int line_width)
{
	int *points=LAYOUT[(int)letter];

	if(points)
	{
		glLineWidth(line_width);
		glBegin(GL_LINES);

			while(*points!=_BK)
			{
				int p = *points;
				p*=2;
				glVertex2f((_qd[p])*factor_x,(_qd[p+1])*factor_y);
				points++;
			}

		glEnd();
	}
}

void txt_data_change(t_txt *txt,const char *name)
{
	strncpy(txt->name,name,_NAME_LONG_);
	//txt->width=(float)strlen(name)*txt->letter_width;
}

void txt_draw(t_txt *txt)
{
	t_context *C=ctx_get();
	char *letter=txt->name;
	float *c=C->ui->front_color;
	t_skt *skt = C->skt;
	float i = skt->intensity;

	int font_type;
	void * font;
	float zoom = C->ui->zoom;
	int line_width = 1;

	if(C->ui->use_bitmap_font && txt->use_bitmap_font)
	{
		font_type = 1;
		font = GLUT_BITMAP_HELVETICA_10;

		if(C->event->ui.use_scale)
		{
			if(zoom > 1.3)
			{
				font_type = 2;
			}
			else if(zoom > 1.1)
			{
				font = GLUT_BITMAP_HELVETICA_12;
			}
			else if(zoom > 0.9)
			{
				font = GLUT_BITMAP_HELVETICA_10;
			}
			else if(zoom > 0.5)
			{
				font_type = 2;
			}
			else
			{
				font_type = 0;
			}
		}

	}
	else
	{
		font_type = 2;

	
	}


	if(font_type == 1)
	{
		glColor3f(c[0]*i,c[1]*i,c[2]*i); 
		glRasterPos2i(0,0);

		//void * font = GLUT_BITMAP_HELVETICA_10;

		while(*letter)
		{
			glutBitmapCharacter(font,*letter);
			letter++;
		}
	}
	else if(font_type == 2)
	{
		glPushMatrix();

		glColor3f(c[0],c[1],c[2]); 

		float _zoom = 0.7;

		if(zoom > 1.8)
		{
			line_width = 2;
		}

		while(*letter)
		{
			txt_letter_draw(*letter,txt->letter_scale_x*_zoom,txt->letter_scale_y*_zoom, line_width);
			glTranslatef(txt->letter_width*_zoom,0,0);
			letter++;
		}

		glPopMatrix();
	}
}

float txt_get_width(t_txt *txt)
{
	if(txt->use_bitmap_font)
	{
		txt->width=(float)strlen(txt->name)*6;
	}
	else
	{
		txt->width=(float)strlen(txt->name)*txt->letter_width*__zoom;
	}
	return txt->width;
}

void txt_free(t_txt *txt)
{
	free(txt);
}

void txt_init(t_txt *txt,const char *name)
{
	if(name) strncpy(txt->name,name,_NAME_LONG_);
	else bzero(txt->name,_NAME_LONG_);

	txt->grid_step=TXT_GRID_STEP;
	txt->grid_size_x=TXT_GRID_SIZE_X;
	txt->grid_size_y=TXT_GRID_SIZE_Y;
	txt->grid_spacing=TXT_GRID_SPACING;
	txt->grid_width=txt->grid_step * (txt->grid_size_x + txt->grid_spacing);
	txt->grid_height=txt->grid_step * (txt->grid_size_y + txt->grid_spacing);

	txt->letter_scale_x=TXT_LETTER_SCALE_X;
	txt->letter_scale_y=TXT_LETTER_SCALE_Y;
	txt->letter_width=txt->grid_width * txt->letter_scale_x;
	txt->letter_height=txt->grid_height * txt->letter_scale_y;
	if(name) txt->width=(float)strlen(name)*txt->letter_width;
	else txt->width=0;
	txt->draw=txt_draw;
	txt->data_change=txt_data_change;
	txt->get_width=txt_get_width;

	txt->use_bitmap_font=1;
}

t_txt *txt_new(const char *name)
{
	t_txt *txt=(t_txt *)malloc(sizeof(t_txt));
	set_name(txt->name,name);
	txt->id=0;
	txt->id_chunk=0;

	txt->grid_step=TXT_GRID_STEP;
	txt->grid_size_x=TXT_GRID_SIZE_X;
	txt->grid_size_y=TXT_GRID_SIZE_Y;
	txt->grid_spacing=TXT_GRID_SPACING;
	txt->grid_width=txt->grid_step * (txt->grid_size_x + txt->grid_spacing);
	txt->grid_height=txt->grid_step * (txt->grid_size_y + txt->grid_spacing);

	txt->letter_scale_x=TXT_LETTER_SCALE_X;
	txt->letter_scale_y=TXT_LETTER_SCALE_Y;
	txt->letter_width=txt->grid_width * txt->letter_scale_x;
	txt->letter_height=txt->grid_height * txt->letter_scale_y;
	if(name) txt->width=(float)strlen(name)*txt->letter_width;
	else txt->width=0;
	txt->draw=txt_draw;
	txt->data_change=txt_data_change;
	txt->get_width=txt_get_width;

	txt->use_bitmap_font=1;

	return txt;
}
