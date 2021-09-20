#include "glut.h"
#include <stdio.h>
#include <math.h>


float moveSpeed = 0.01;

struct {
	GLfloat x,y,z; 
} Pos;

GLfloat W = 1.76, H=1.07, OriginZ=0.75;
//GLfloat W = 0.43, H = 0.24, OriginZ = 0.75;
float angle = 45;

float fieldOfView(float halfScreen, float cameraDistance)
{
	return (2*atan(halfScreen/(cameraDistance)))*(180/3.14157);
}

void view(int Eye)
{

  float near = 0.1;

  float scaling = near / (OriginZ + Pos.z);

  if (Eye == 0) //Left
	  Pos.x = Pos.x - 0.03;
  else
	  Pos.x = Pos.x + 0.03;

  float left = -((W / 2) + Pos.x)*scaling;
  float right = ((W / 2) - Pos.x)*scaling;
  float bottom = -(Pos.y)*scaling;
  float top = ((H)-Pos.y)*scaling;


  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();

  //gluPerspective(fieldOfView(H / 2, OriginZ + Pos.z), W / H, OriginZ + Pos.z, 10.0);

  glFrustum(left, right, bottom, top, near, 10);


  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  gluLookAt(Pos.x,Pos.y,Pos.z,  
	         Pos.x,Pos.y,-OriginZ, 
			 0,1,0);   
}

void init(void) 
{
	Pos.x = 0;
	Pos.y = H/2;
	Pos.z = 0; 


	//glutFullScreen();
	glClearColor(0.2, 0.2, 0.2, 0.0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
  
    //view();

}

void idle() {
	angle += 0.5;
	glutPostRedisplay();
}

void display(void)
{  
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_TRUE);
   view(0);
   glFrontFace(GL_CW);             // gizmo to correct teapot vertex order
   glPushMatrix();
   glTranslatef(0.05, H / 2, -(OriginZ + 0.09));
   glRotatef(angle, 0, 1, 0);
   glutSolidTeapot(0.25f);         // white teapot at eye level behind the screen
   glPopMatrix();
   glPushMatrix();
   glTranslatef(-0.05, H / 2, -(OriginZ -0.3));
   glRotatef(-angle, 0, 1, 0);
   glutSolidTeapot(0.15f);         // white teapot in front
   glPopMatrix();
   glFrontFace(GL_CCW);

   glClear(GL_DEPTH_BUFFER_BIT);
   glColorMask(GL_FALSE, GL_FALSE, GL_TRUE, GL_TRUE);
   view(1);
   glFrontFace(GL_CW);             // gizmo to correct teapot vertex order
   glPushMatrix();
   glTranslatef(0.05, H / 2, -(OriginZ + 0.09));
   glRotatef(angle, 0, 1, 0);
   glutSolidTeapot(0.25f);         // white teapot at eye level behind the screen
   glPopMatrix();
   glPushMatrix();
   glTranslatef(-0.05, H / 2, -(OriginZ -0.3));
   glRotatef(-angle, 0, 1, 0);
   glutSolidTeapot(0.15f);         // white teapot in front
   glPopMatrix();
   glFrontFace(GL_CCW);
   
   glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
   glutSwapBuffers();
}

void keyboard (unsigned char key, int x, int y)
{
   switch (key) {

      case 'a':
         Pos.x += moveSpeed;
		 glutPostRedisplay();
         break;
      case 'd':
         Pos.x -= moveSpeed;
         glutPostRedisplay();
         break;
      case 'w':
         Pos.z += moveSpeed;
         glutPostRedisplay();
         break;
      case 's':
         Pos.z -= moveSpeed;
         glutPostRedisplay();
         break;
      case 'z':
         Pos.y += moveSpeed;
         glutPostRedisplay();
         break;
      case 'x':
         Pos.y -= moveSpeed;
         glutPostRedisplay();
         break;
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
   glutInitWindowSize (1600, 900); 
   glutInitWindowPosition (0, 0);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display); 
   glutKeyboardFunc(keyboard);
   glutIdleFunc(idle);
   glutMainLoop();
   return 0;
}
