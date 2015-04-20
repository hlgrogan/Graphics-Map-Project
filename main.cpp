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
//just copied the above from in class files -Erik

#include <iostream>
#include <fstream>

using namespace std;

//globals
string data_array[7][300];

//reads in the csv and loads it into data_array global variable
void read_file(){
  ifstream file ("Bicycle-Parking.csv");
  string value;
  int array_x = 0;
  int array_y = 0;

  while(!file.eof()){
    getline ( file, value, ',' );
    data_array[array_x][array_y] = value;
    array_x++;
    array_x = array_x % 7;
    if(array_x == 0) array_y++;
  }
}

void setup(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 
}

void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT);
   glColor3f(0.0, 0.0, 0.0);



   glFlush(); 
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch(key) 
   {
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}

int main(int argc, char **argv){
  read_file();

glutInit(&argc, argv);

   glutInitContextVersion(2, 1);
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("Chattanooga Bike Parking");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);

   glewExperimental = GL_TRUE;
   glewInit();

   setup(); 
   
   glutMainLoop(); 
}
