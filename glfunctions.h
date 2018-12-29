#pragma once

#if IBM
#include "GL\glew.h"
#include "GL\wglew.h"
#include "SOIL\include\SOIL2.h"
#endif
#if APL
#include <OpenGL/gl.h>
#include "SOIL2.h"
#endif
#include "XPLMDisplay.h"
#include "XPLMGraphics.h"
//#include <ft2build.h>
#include "XPLMUtilities.h"
//#include "airports.h"
#if IBM
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")
//#pragma comment(lib, "freetype.lib")
#endif
//#include FT_FREETYPE_H

//extern FT_Face test;

extern GLuint button_med;
extern GLuint button_med_clicked;
extern GLuint button_med_hover;
extern GLuint checkbox;
extern GLuint checkbox_checked;
extern  GLuint text_box_med;
extern  GLuint text_box_med_highlighted;
extern  GLuint text_box_small;
extern GLuint text_box_small_highlighted;
extern GLuint ts_logo;

inline XPLMCursorStatus	dummy_cursor_status_handler(XPLMWindowID in_window_id, int x, int y, void * in_refcon) { return xplm_CursorDefault; }
inline int					dummy_wheel_handler(XPLMWindowID in_window_id, int x, int y, int wheel, int clicks, void * in_refcon) { return 0; }
inline void				dummy_key_handler(XPLMWindowID in_window_id, char key, XPLMKeyFlags flags, char virtual_key, void * in_refcon, int losing_focus) { }


struct clr_t
{
	unsigned char r, g, b, a;
};

extern void loadGLTextures();
extern void loadGLFonts();
extern int InitFreeType();
extern void drawTexture(int x, int y, int w, int h, unsigned int texture);
extern void drawTextureRGB(int x, int y, int w, int h, int texture, float r, float g, float b, float a);
extern void drawTextureAngle(int x, int y, int w, int h, int texture, float angle);
extern void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius, clr_t fill, int num_segments);
extern void drawCircle(float cx, float cy, float r, clr_t fill, int num_segments, GLenum mode = GL_LINE_LOOP);
extern void drawRectangle(int x1, int y1, int x2, int y2);
extern GLuint loadTexture(const char * filename);
