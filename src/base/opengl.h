/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#ifndef __OPENGL_H
#define __OPENGL_H

#define GL_GLEXT_PROTOTYPES

#ifdef HAVE_GLEW
#include <GL/glew.h>
#define WITH_GLEW
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glext.h>

#endif
