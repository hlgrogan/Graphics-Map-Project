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

#include <fstream>

//globals
int data_array[7][300];

//reads in the csv
void read_file(){
  ifsteam csv_file ("Bicycle-PArking.csv");
  string value;
  int array_x = 0;
  int array_y = 0;

  while(!csv_file.eof()){
    getline ( csv_file, value, ',' );
    data_array[array_x][array_y] = value;
    array_x++;
    array_x = array_x % 7;
    if(array_x == 0) array_y++;
  }
}

int main(int argc, char **argv){
  read_file();
}
