#ifdef __APPLE__
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#pragma comment(lib, "glew32.lib") 
#endif

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "getbmp.h"

#define PI 3.14159265
#define N 40.0 // Number of vertices on the boundary of the disc.

//coords drawing:
//lat: (35.041462 to 35.056140)
//long: (-85.289526 to -85.304204)
//distance between: 0.014678
//distance per pixel: 0.000029356

static float y_min = 35.041462;
static float y_max = 35.052715;
static float x_min = -85.304204;
static float x_max = -85.289526;

static float width = 684.0;
static float height = 640.0;

static int line_count = 0;

static unsigned int texture[1];

static string data_array[7][300];

static vector< vector<string> > in_range;

using namespace std;

void read_file(){
	ifstream csv_file("Bicycle-Parking.csv");
	string value;
	int array_x = 0;
	int array_y = 0;

	getline(csv_file, value);

	while (!csv_file.eof()){
		getline(csv_file, value, ',');
		int pos;
		if ((pos = value.find('\n')) != string::npos)
			value.erase(pos, 1);
		data_array[array_x][array_y] = value;

		array_x++;
		array_x = array_x % 7;
		if (array_x == 0)
		{
			array_y++;
			line_count++;
		}

	}
}

float getXOffset(float longitude)
{
	return (x_max - longitude) / (x_max - x_min) * width;
}

float getYOffset(float latitude)
{
	return (y_max - latitude) / (y_max - y_min) * height;
}

float getCapacity(float capacity)
{
	if (capacity == 0)
	{
		return 5;
	}
	return capacity * 1.5;
}


void drawDisc(float R, float X, float Y, float Z)
{
	float t;
	int i;

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(X, Y, Z);
	for (i = 0; i <= N; ++i)
	{
		t = 2 * PI * i / N;
		glVertex3f(X + cos(t) * R, Y + sin(t) * R, Z);
	}
	glEnd();
}

// Load external textures.
void loadExternalTextures()
{

	// Local storage for bmp image data.
	BitMapFile *image[1];

	// Load the images.
	image[0] = getbmp("test_map.bmp");

	// Bind grass image to texture object texture[0].
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}


// Drawing routine.
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(width, 0.0, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(width, height, 0.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(0.0, height, 0.0);
	glEnd();


	float lat = (y_min + y_max) / 2;
	float lon = (x_min + x_max) / 2;




	glDisable(GL_TEXTURE_2D);
	glColor3f(1.0, 0.0, 0.0);
	/*for (const vector<string> &v : in_range)
	{
	if (!v.at(3).empty())
	{

	}
	}*/

	//drawDisc(getCapacity(3), getXOffset(lon), getYOffset(lat), 0.0);

	for (int i = 0; i < line_count; i++)
	{
		glColor3f(1.0, 0.0, 0.0);
		if (stof(data_array[5][i]) >= x_min && stof(data_array[5][i]) <= x_max &&
			stof(data_array[6][i]) >= y_min && stof(data_array[6][i]) <= y_max)
		{
			string capacity = data_array[3][i];
			float cap = getCapacity(stof(data_array[3][i]));

			if (data_array[2][i] == "Various")
			{
				//no various?
				glColor3f(1.0, 1.0, 1.0);
			}
			else if (data_array[2][i] == "Wave")
			{
				glColor3f(0.0, 0.5, 0.5);
			}
			else if (data_array[2][i] == "Custom")
			{
				//no comb?
				//glColor3f(0.0, 0.5, 0.5);
			}
			else if (data_array[2][i] == "Inverted U")
			{
				glColor3f(1.0, 1.0, 0.0);
			}
			else if (data_array[2][i] == "Comb")
			{
				//no comb?
				//glColor3f(0.0, 0.5, 0.5);
			}
			else if (data_array[2][i] == "Pi")
			{
				//no pi?
				//glColor3f(0.0, 0.5, 0.5);
			}
			else if (data_array[2][i] == "Bike Corral")
			{
				//no bike corral?
				//glColor3f(0.0, 0.5, 0.5);
			}
			else if (data_array[2][i] == "Circle")
			{
				//no circle?
				//glColor3f(0.0, 0.5, 0.5);
			}
			else if (data_array[2][i] == "Hoop")
			{
				glColor3f(0.0, 1.0, 0.0);
			}
			else if (data_array[2][i] == "Post and Ring")
			{
				glColor3f(1.0, 0.0, 0.0);
			}
			else
			{
				glColor3f(0.0, 0.0, 0.0);
			}


			drawDisc(cap, width - getXOffset(stof(data_array[5][i])), height - getYOffset(stof(data_array[6][i])), 0.0);
		}
	}
	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);

	glFlush();
}


// Initialization routine.
void setup(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);

	// Turn on OpenGL texturing.
	glEnable(GL_TEXTURE_2D);

	read_file();

	loadExternalTextures();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, width, 0.0, height, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

// Main routine.
int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitContextVersion(2, 1);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("bike_map.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}