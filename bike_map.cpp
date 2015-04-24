//written by Sarah Hall, Hailey Grogan, and Erik Lundquist

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

#include "getbmp.h"

#define PI 3.14159265
#define N 40.0 // Number of vertices on the boundary of the disc.

//coords drawing:
static float y_min = 35.041462;
static float y_max = 35.052715;
static float x_min = -85.304204;
static float x_max = -85.289526;
static int inRange = 0;

static float width = 684.0;
static float height = 640.0;

static int keyDiscOffset = 30;
static int keyTextOffset = 50;
static int keyTypeOffset = 25;

static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.
static bool showMap = true;
static bool showName = false;
static int nameIndex = -1;

static unsigned int texture[1];
static string data_array[7][300];

using namespace std;


typedef struct parking
{
	string type;
	string name;
	float x;
	float y;
	int capacity;
	int selected;
} parking;

static parking allParking[300];

float getCapacity(int capacity)
{
	//for ones with unknown capacity
	if (capacity == 0)
	{
		return -7;
	}
	//modify for draw size
	return capacity * 1.5;
}

void read_file(){
	ifstream csv_file("Bicycle-Parking.csv");
	string value;
	int array_x = 0;
	int array_y = 0;
	int line_count = 0;

	//delete first line
	getline(csv_file, value);

	//read file
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

	//find spots on UTC map
	for (int i = 0; i < line_count; i++)
	{
		//if disc is in range of UTC map
		if (stof(data_array[5][i]) >= x_min && stof(data_array[5][i]) <= x_max &&
			stof(data_array[6][i]) >= y_min && stof(data_array[6][i]) <= y_max)
		{
			allParking[inRange].capacity = stof(data_array[3][i]);
			allParking[inRange].x = stof(data_array[5][i]);
			allParking[inRange].y = stof(data_array[6][i]);
			allParking[inRange].type = data_array[2][i];
			allParking[inRange].name = data_array[1][i];

			inRange++;
		}
	}
}

//get x for drawing
float getXOffset(float longitude)
{
	return (x_max - longitude) / (x_max - x_min) * width;
}

//get y for drawing
float getYOffset(float latitude)
{
	return (y_max - latitude) / (y_max - y_min) * height;
}

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string)
{
	char *c;

	for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
	cout << "Interaction:" << endl;
	cout << "Hold shift to view the key." << endl
		<< "Mouse over a location to see its name and capacity." << endl;
}

void drawParking(float R, float X, float Y, float Z)
{
	//unknown size gets cubes
	if (R <= 0)
	{
		R = -R;
		glBegin(GL_TRIANGLES);
		glVertex3f(X, Y, Z);
		glVertex3f(X + R, Y, Z);
		glVertex3f(X + R, Y + R, Z);

		glVertex3f(X, Y, Z);
		glVertex3f(X + R, Y + R, Z);
		glVertex3f(X, Y + R, Z);

		glVertex3f(X, Y, Z);
		glVertex3f(X, Y + R, Z);
		glVertex3f(X - R, Y + R, Z);

		glVertex3f(X, Y, Z);
		glVertex3f(X - R, Y + R, Z);
		glVertex3f(X - R, Y, Z);

		glVertex3f(X, Y, Z);
		glVertex3f(X - R, Y, Z);
		glVertex3f(X - R, Y - R, Z);

		glVertex3f(X, Y, Z);
		glVertex3f(X - R, Y - R, Z);
		glVertex3f(X, Y - R, Z);

		glVertex3f(X, Y, Z);
		glVertex3f(X, Y - R, Z);
		glVertex3f(X + R, Y - R, Z);

		glVertex3f(X, Y, Z);
		glVertex3f(X + R, Y - R, Z);
		glVertex3f(X + R, Y, Z);

		glEnd();
	}
	//known size gets circles
	else
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
}

// Load external textures.
void loadExternalTextures()
{

	// Local storage for bmp image data.
	BitMapFile *image[1];

	// Load the map image.
	image[0] = getbmp("test_map.bmp");

	// Bind map to texture object texture[0].
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

//writes the spot name and capacity
void writeSpotName(int index)
{
	int len = allParking[index].name.length();
	char *spotName = new char[len + 1];
	strcpy(spotName, allParking[index].name.c_str());

	glRasterPos2f(width / 2 - (len * 3.5), height - 20);
	writeBitmapString((void*)font, spotName);

	string num = to_string(allParking[index].capacity);
	len = num.length();
	char *spotCap = new char[len + 1];
	strcpy(spotCap, num.c_str());

	if (allParking[index].capacity > 0)
	{
		glRasterPos2f(width / 2 - 40, height - 40);
		writeBitmapString((void*)font, "Capacity: ");
		writeBitmapString((void*)font, spotCap);
	}
	else
	{
		glRasterPos2f(width / 2 - 60, height - 40);
		writeBitmapString((void*)font, "Capacity: Unknown");
	}

	delete[] spotName;
	delete[] spotCap;
}

//writes the info on the key screen
void writeKeyInfo()
{
	glColor3f(1.0, 1.0, 1.0);

	glRasterPos2f(width / 2 - 35, (height / 7) * 6 + 50);
	writeBitmapString((void*)font, "Capacity");

	glRasterPos2f(width / 2 + keyDiscOffset - 300, (height / 7) * 6);
	writeBitmapString((void*)font, "Capacity is Radius: ");

	glRasterPos2f(width / 2 + keyDiscOffset - 10, (height / 7) * 6);
	writeBitmapString((void*)font, "Unknown Capacity: ");


	glRasterPos2f(width / 2 - 20, (height / 7) * 5 + 50 - keyTypeOffset);
	writeBitmapString((void*)font, "Type");

	glRasterPos2f((width / 2) - keyTextOffset, (height / 7) * 5 - keyTypeOffset);
	writeBitmapString((void*)font, "Wave: ");

	glRasterPos2f((width / 2) - keyTextOffset - 50, (height / 7) * 4 - keyTypeOffset);
	writeBitmapString((void*)font, "Inverted U: ");

	glRasterPos2f((width / 2) - keyTextOffset, (height / 7) * 3 - keyTypeOffset);
	writeBitmapString((void*)font, "Hoop: ");

	glRasterPos2f((width / 2) - keyTextOffset - 75, (height / 7) * 2 - keyTypeOffset);
	writeBitmapString((void*)font, "Post and Ring: ");

	glRasterPos2f((width / 2) - keyTextOffset - 25, (height / 7) * 1 - keyTypeOffset);
	writeBitmapString((void*)font, "Unknown: ");
}


// Drawing routine.
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	if (showMap)
	{
		//draw school map (thanks Google)
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glBegin(GL_POLYGON);
		glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2f(1.0, 0.0); glVertex3f(width, 0.0, 0.0);
		glTexCoord2f(1.0, 1.0); glVertex3f(width, height, 0.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(0.0, height, 0.0);
		glEnd();

		glDisable(GL_TEXTURE_2D);

		//draw parking discs
		for (int i = 0; i < inRange; i++)
		{
			string type = allParking[i].type;
			float x = allParking[i].x;
			float y = allParking[i].y;
			float capacity = getCapacity(allParking[i].capacity);

			//disk color is controlled by type
			if (type == "Wave") //waves are teal
			{
				glColor3f(0.0, 0.6, 0.6);
			}
			else if (type == "Inverted U") //inverted u are yellow
			{
				glColor3f(1.0, 1.0, 0.15);
			}
			else if (type == "Hoop") //hoops are green
			{
				glColor3f(0.5, 1.0, 0.0);
			}
			else if (type == "Post and Ring") //post and ring are red
			{
				glColor3f(1.0, 0.1, 0.1);
			}
			else //unknown types are orange
			{
				glColor3f(1.0, 0.65, 0.0);
			}

			//disc size is controlled by capacity
			drawParking(capacity, width - getXOffset(x), height - getYOffset(y), 0.0);
		}

		glColor3f(1.0, 1.0, 1.0);

		if (showName)
		{
			writeSpotName(nameIndex);
		}

		glEnable(GL_TEXTURE_2D);
	}
	else
	{
		glDisable(GL_TEXTURE_2D);

		glColor3f(0.25, 0.25, 0.25);

		glBegin(GL_POLYGON);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(width, 0.0, 0.0);
		glVertex3f(width, height, 0.0);
		glVertex3f(0.0, height, 0.0);
		glEnd();

		writeKeyInfo();

		glColor3f(0.75, 0.75, 0.75);
		drawParking(-20, width / 2 + keyDiscOffset + 100 + 75, (height / 7) * 6, 0.0);
		drawParking(20, width / 2 + keyDiscOffset - 100, (height / 7) * 6, 0.0);

		//wave
		glColor3f(0.0, 0.6, 0.6);
		drawParking(20, width / 2 + keyDiscOffset, (height / 7) * 5 - keyTypeOffset, 0.0);

		//inverted u
		glColor3f(1.0, 1.0, 0.15);
		drawParking(20, width / 2 + keyDiscOffset, (height / 7) * 4 - keyTypeOffset, 0.0);

		//hoop
		glColor3f(0.5, 1.0, 0.0);
		drawParking(20, width / 2 + keyDiscOffset, (height / 7) * 3 - keyTypeOffset, 0.0);

		//post and ring
		glColor3f(1.0, 0.1, 0.1);
		drawParking(20, width / 2 + keyDiscOffset, (height / 7) * 2 - keyTypeOffset, 0.0);

		//unknown
		glColor3f(1.0, 0.65, 0.0);
		drawParking(20, width / 2 + keyDiscOffset, (height / 7) * 1 - keyTypeOffset, 0.0);

		glColor3f(1.0, 1.0, 1.0);

		glEnable(GL_TEXTURE_2D);
	}

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
		glutPostRedisplay();
		break;
	}
}

void specialKeyInput(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_SHIFT_L:
		showMap = false;
		break;
	case GLUT_KEY_SHIFT_R:
		showMap = false;
		break;
	default:
		glutPostRedisplay();
		break;
	}

	glutPostRedisplay();
}

void keyBoardUp(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_SHIFT_L:
		showMap = true;
		break;
	case GLUT_KEY_SHIFT_R:
		showMap = true;
	default:
		glutPostRedisplay();
		break;
	}

	glutPostRedisplay();
}

//checks if mousing over a spot
void mouseOver(int x, int y)
{
	showName = false;
	glutPostRedisplay();

	for (int i = 0; i < inRange; i++)
	{
		parking spot = allParking[i];
		float spotX = getXOffset(spot.x);
		float spotY = getYOffset(spot.y);
		float cap = getCapacity(spot.capacity);
		if (cap < 0)
		{
			cap = -cap;
		}
		if ((x >= width - spotX - cap) && (x <=  width - spotX + cap) &&
			(y >= spotY - cap) && (y <= spotY + cap))
		{
			nameIndex = i;
			showName = true;
			glutPostRedisplay();
		}
	}
}

// Main routine.
int main(int argc, char **argv)
{
	printInteraction();
	glutInit(&argc, argv);

	glutInitContextVersion(2, 1);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("bike_map.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(specialKeyInput);
	glutSpecialUpFunc(keyBoardUp);
	glutPassiveMotionFunc(mouseOver);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();


	glutMainLoop();
}