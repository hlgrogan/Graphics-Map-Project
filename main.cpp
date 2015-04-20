
//I copied this from our class code -Erik
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


void read_file(){
  ifstream csv_file ("Bicycle-Parking.csv"); 
  string value;
  int array_x = 0;
  int array_y = 0;
  int data_array[7][300];
  
  while(!file.eof()){
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
