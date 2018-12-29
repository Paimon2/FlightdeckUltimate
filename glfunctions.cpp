#include "glfunctions.h"
#include "GlobalVars.h"
#include "fixed_string.hpp"
#include <vector>
#include <cmath>
//FT_Library ft;
//FT_Face test;

/*deg2rad() from airports.h inexplicably throws errors in glfunctions.h - please use this instead*/
#define deg2rad(deg) (deg * 3.14159265358979323846 / 180)

GLuint button_med;
GLuint button_med_clicked;
GLuint button_med_hover;
GLuint checkbox;
GLuint checkbox_checked;
GLuint text_box_med;
GLuint text_box_med_highlighted;
GLuint text_box_small;
GLuint text_box_small_highlighted;
GLuint ts_logo;

void loadGLTextures() {
	std::string texturesDir;
	texturesDir.clear();
	texturesDir.append("Resources").append(dirSep).append("plugins").append(dirSep).append("Flightdeck Ultimate").append(dirSep).append("gui").append(dirSep);

	button_med = loadTexture(fixed_string_128((texturesDir + "button_med.png")));
	button_med_clicked = loadTexture(fixed_string_128((texturesDir + "button_med_clicked.png")));
	button_med_hover = loadTexture(fixed_string_128((texturesDir + "button_med_hover.png")));
	checkbox = loadTexture(fixed_string_128((texturesDir + "checkbox.png")));
	checkbox_checked = loadTexture(fixed_string_128((texturesDir + "checkbox_checked.png")));
	text_box_med = loadTexture(fixed_string_128((texturesDir + "text_box_med.png")));
	text_box_med_highlighted = loadTexture(fixed_string_128((texturesDir + "text_box_med_highlighted.png")));
	text_box_small = loadTexture(fixed_string_128((texturesDir + "text_box_small.png")));
	text_box_small_highlighted = loadTexture(fixed_string_128((texturesDir + "text_box_small_highlighted.png")));
	ts_logo = loadTexture(fixed_string_128((texturesDir + "ts_logo.png")));
}


void drawTexture(int x, int y, int w, int h, unsigned int texture)
{
	
	XPLMSetGraphicsState(0/*Fog*/,
		1 /*TexUnits*/,
		0 /*Lighting*/,
		0 /*AlphaTesting*/,
		0 /*AlphaBlending*/,
		1 /*DepthTesting*/,
		0 /*DepthWriting*/);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor3f(1, 1, 1);

	//glClear(GL_DEPTH_BUFFER_BIT);
	XPLMBindTexture2d(texture, GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, texture);

	//glClear(GL_DEPTH_BUFFER_BIT);
	//glClear(GL_DEPTH_TEST);
	//glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glTexCoord2i(0, 0); glVertex2i(x, y);
	glTexCoord2i(0, 1); glVertex2i(x, y + h);
	glTexCoord2i(1, 1); glVertex2i(x + w, y + h);
	glTexCoord2i(1, 0); glVertex2i(x + w, y);
	//	glTexCoord2f(0, 0); glVertex2f(xx, yy);
	//	glTexCoord2f(0, 1); glVertex2f(xx, yy + hh);
	//glTexCoord2f(0, 1); glVertex2f(xx + ww, yy + hh);
	//glTexCoord2f(1, 0); glVertex2f(xx + ww, yy);
	glEnd();


}

//#DoesNothing
int InitFreeType() {

	//if (FT_Init_FreeType(&ft)) { logMsg("Could not initialize the FreeType font library!", 3, 1); return 0; }

	//if (FT_New_Face(ft, "MCP-Passengers/User Interface/Fonts/test.ttf", 0, &test)){ logMsg("Could not load a font!", 3, 1); return 0; }
	return 1;
}

void drawTextureRGB(int x, int y, int w, int h, int texture, float r, float g, float b, float a) {
	XPLMSetGraphicsState(0/*Fog*/, 1/*TexUnits*/, 0/*Lighting*/, 0/*AlphaTesting*/, 0/*AlphaBlending*/, 1/*DepthTesting*/, 0/*DepthWriting*/);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4ub(r, g, b, a);

	//glClear(GL_DEPTH_BUFFER_BIT);
	XPLMBindTexture2d(texture, GL_TEXTURE_2D);


	//glClear(GL_DEPTH_BUFFER_BIT);
	//glClear(GL_DEPTH_TEST);
	//glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glTexCoord2i(0, 0); glVertex2i(x, y);
	glTexCoord2i(0, 1); glVertex2i(x, y + h);
	glTexCoord2i(1, 1); glVertex2i(x + w, y + h);
	glTexCoord2i(1, 0); glVertex2i(x + w, y);
	//	glTexCoord2f(0, 0); glVertex2f(xx, yy);
	//	glTexCoord2f(0, 1); glVertex2f(xx, yy + hh);
	//glTexCoord2f(0, 1); glVertex2f(xx + ww, yy + hh);
	//glTexCoord2f(1, 0); glVertex2f(xx + ww, yy);
	glEnd();


}


void drawTextureAngle(int x, int y, int w, int h, int texture, float angle) {
	XPLMSetGraphicsState(0/*Fog*/, 1/*TexUnits*/, 0/*Lighting*/, 0/*AlphaTesting*/, 0/*AlphaBlending*/, 1/*DepthTesting*/, 0/*DepthWriting*/);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	//glLoadIdentity();
	glTranslatef(x, y, 0.0f); //Translate back to the original location.
	glRotatef(-angle, 0.0f, 0.0f, 1.0f);                  //Rotate.
	glTranslatef(-x, -y, 0.0f);   //Translate to the origin
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);





	//glClear(GL_DEPTH_BUFFER_BIT);



	//glClear(GL_DEPTH_BUFFER_BIT);
	//glClear(GL_DEPTH_TEST);
	//glEnable(GL_TEXTURE_2D);
	XPLMBindTexture2d(texture, GL_TEXTURE_2D);
	glBegin(GL_QUADS);

	glTexCoord2i(0, 0); glVertex2i(x, y);
	glTexCoord2i(0, 1); glVertex2i(x, y + h);
	glTexCoord2i(1, 1); glVertex2i(x + w, y + h);
	glTexCoord2i(1, 0); glVertex2i(x + w, y);
	//	glTexCoord2f(0, 0); glVertex2f(xx, yy);
	//	glTexCoord2f(0, 1); glVertex2f(xx, yy + hh);
	//glTexCoord2f(0, 1); glVertex2f(xx + ww, yy + hh);
	//glTexCoord2f(1, 0); glVertex2f(xx + ww, yy);
	glEnd();
	glPopMatrix();

}





void drawCircle(float cx, float cy, float r, clr_t fill, int num_segments, GLenum mode)
{
	struct vertex
	{
		float x, y;
	};

	using coord_type = float;

	std::vector<std::pair<coord_type, coord_type>> vertices;

	//Get the middle coords
	float Mx = cx;
	float My = cy;
	//Get the angle we are using
	float a = (num_segments - 2) * 180 / num_segments;
	float b = 180 - a;
	float c = 90 - a;

	//std::vector<vertex> vertices;



	//			logpp::Console::debug(std::to_string(start.x()) + " " + std::to_string(start.y()) + " " + std::to_string(start.z()));

	unsigned int count = 1;

	do
	{
		vertex current;
		current.x = std::cos(deg2rad(c - (count - 1) * b)) * r + Mx;
		current.y = std::cos(deg2rad(c - (count - 1) * b)) * r + My;

		vertices.emplace_back(current.x, current.y);
		
		++count;

	} while (count <= num_segments);


	//Now we have the vertices in the right order, we have to make triangles of it:
	//^Not here, as we're drawing without vertex buffers
	/*
	std::vector<Vertex> final_vertices;

	for (std::vector<Vertex>::size_type i = 0; i < vertices.size() - 1; ++i)
	{
	//				logpp::Console::log(std::to_string(i));
	final_vertices.push_back(middle);
	final_vertices.push_back(vertices[i]);
	final_vertices.push_back(vertices[(i + 1)/* % vertices.size];)*/
	/*	}

	final_vertices.push_back(middle);
	final_vertices.push_back(vertices[vertices.size() - 1]);
	final_vertices.push_back(vertices[0]);
	*/

	throw std::runtime_error("This function is not yet implemented.");
	/*glBegin(mode);
	glColor4ub(fill.r, fill.g, fill.b, fill.a);
	for (auto const&[x, y] : vertices)
	{
		glVertex2f(x, y);
	}
	glEnd();*/
}

void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius, clr_t fill, int num_segments)
{
	drawCircle(x, y, radius, fill, num_segments, GL_POLYGON);
}

void drawRectangle(int x1, int y1, int x2, int y2)
{
	glBegin(GL_POLYGON);
	glVertex2f(x1, y1);
	glVertex2f(x1, y2);
	glVertex2f(x2, y2);
	glVertex2f(x2, y1);
	glEnd();
}



GLuint loadTexture(const char * filename) {
	GLuint tex_2d = SOIL_load_OGL_texture
	(
		filename,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	if (tex_2d == 0 || tex_2d == -1) {
		char msg[200];
		snprintf(msg, 200, "Unable to load PNG file into texture - texture was NULL. The texture was %s.", filename);
		pluginLogger << msg;
		return 0;
	}
	else { return tex_2d; }
}


