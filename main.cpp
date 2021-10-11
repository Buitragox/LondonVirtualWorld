//=============================================================================
// Sample Application: Lighting (Per Fragment Phong)
//=============================================================================

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glApplication.h"
#include "glutWindow.h"
#include <iostream>
#include "glsl.h"
#include <time.h>
#include "./classes/Objeto3D.h"
#include <vector>
#include <FreeImage.h>

//-----------------------------------------------------------------------------



class myWindow : public cwc::glutWindow
{
protected:
   cwc::glShaderManager SM;
   cwc::glShader *shader;
   cwc::glShader* shaderTex;
   GLuint ProgramObject;
   clock_t time0,time1;
   float timer010;  // timer counting 0->1->0
   bool bUp;        // flag if counting up or down.
   std::vector<Objeto3D*> modelos3d;
   GLuint texid;
   float cameraMove;
   float carriageMove;
   bool facingLeft;

public:
	myWindow(){}

    void initialize_textures(void)
    {
        GLubyte* data = 0;
        glGenTextures(1, &texid);
        glBindTexture(GL_TEXTURE_2D, texid);
        glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        // Loading JPG file
        FIBITMAP* bitmap = FreeImage_Load(
            FreeImage_GetFileType("./mallas/bark1.jpg", 0),
            "./mallas/bark1.jpg");
        FIBITMAP* pImage = FreeImage_ConvertTo32Bits(bitmap);
        int nWidth = FreeImage_GetWidth(pImage);
        int nHeight = FreeImage_GetHeight(pImage);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, nWidth, nHeight,
            0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage));
        FreeImage_Unload(pImage);
        //
        glEnable(GL_TEXTURE_2D);
    }

	virtual void OnRender(void)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	   
      //timer010 = 0.09; //for screenshot!
        glPushMatrix();
        if (shader) shader->begin();

            glTranslatef(0, 0, 1.5); //Alejar/Acercar la imagen
            glRotatef(20.0f, 1.0f, 0.0f, 0.0f); //Vista desde arriba
            glRotatef(cameraMove * 360, 0.0f, 0.001f, 0.0f);
            //glRotatef(-75, 0, 1, 0); //Vista desde el lado

            glPushMatrix();
         
            glScalef(1.1f, 1.0f, 1.0f);
            modelos3d[0]->DibujarMalla(0, 0, 0); //Ground
            glPopMatrix();

            glPushMatrix();
            modelos3d[1]->DibujarMalla(-0.1f, 0.27f, -0.7f); //Big church
            glPopMatrix();

            glPushMatrix();
            modelos3d[2]->DibujarMalla(0.0f, 0.1f, 0.85f); //Houses
            glPopMatrix();

            glPushMatrix();
            modelos3d[3]->DibujarMalla(-0.6f, 0.23f, -0.23f); //Small church
            glPopMatrix();

            glPushMatrix();
            modelos3d[5]->DibujarMalla(0.55f, 0.04f, -0.33f); //Bench
            glPopMatrix();

            glPushMatrix();
            glScalef(0.2f, 0.2f, 0.2f);
            if (facingLeft)
            {
                //glTranslatef(0.0f, 0.0f, -1.5f);
                glRotatef(180, 0.0f, 1.0f, 0.0f);
                
                modelos3d[6]->DibujarMalla(-carriageMove, 0.375f, -1.2f); //Carriage
                //glTranslatef(0.0f, 0.0f, 1.5f);
            }
            else
            {
                modelos3d[6]->DibujarMalla(carriageMove, 0.375f, 1.2f); //Carriage
            }
            
            
            glPopMatrix();

        if (shader) shader->end();

        //Mallas con textura
        if (shaderTex) shaderTex->begin();
            glPushMatrix();
            modelos3d[4]->SetHasTexture(true);
            modelos3d[4]->DibujarMalla(-0.0f, 0.15f, -0.3f, texid); //Trees
            glPopMatrix();
        if (shaderTex) shaderTex->end();

        
        glutSwapBuffers();
        glPopMatrix();

        UpdateTimer();

		Repaint();
	}

	virtual void OnIdle() {}

	// When OnInit is called, a render context (in this case GLUT-Window) 
	// is already available!
	virtual void OnInit()
	{
	    glClearColor(0.5f, 0.5f, 1.0f, 0.0f);
	    glShadeModel(GL_SMOOTH);
	    glEnable(GL_DEPTH_TEST);

	    shader = SM.loadfromFile("vertexshader.txt","fragmentshader.txt"); // load (and compile, link) from file
	    if (shader==0) 
            std::cout << "Error Loading, compiling or linking shader\n";
        else
        {
            ProgramObject = shader->GetProgramObject();
        }

        shaderTex = SM.loadfromFile("vertexshaderTex.txt", "fragmentshaderTex.txt"); // load (and compile, link) from file
        if (shader == 0)
            std::cout << "Error Loading, compiling or linking shader\n";
        else
        {
            ProgramObject = shader->GetProgramObject();
        }

        time0 = clock();
        timer010 = 0.0f;
        bUp = true;

        facingLeft = false;
        carriageMove = 0.0f;
        cameraMove = 0.0f;

        std::vector<std::string> filenames{"ground.obj", "bigchurch2.obj", "houses.obj",
                                        "smallchurch.obj", "trees.obj", "bench.obj", 
                                        "horse_and_carriage.obj"};
        std::string fullpath;
        modelos3d = std::vector<Objeto3D*>();
        for (int i = 0; i < filenames.size(); i++)
        {
            modelos3d.push_back(new Objeto3D());
            fullpath = "./mallas/" + filenames[i];
            //std::cout << fullpath << "\n";
            modelos3d[i]->AbrirMalla(fullpath);
        }
        initialize_textures();
        DemoLight();

	}

	virtual void OnResize(int w, int h)
   {
      if(h == 0) h = 1;
	   float ratio = 1.0f * (float)w / (float)h;

      glMatrixMode(GL_PROJECTION);
	   glLoadIdentity();
	
	   glViewport(0, 0, w, h);

      gluPerspective(45,ratio,1,100);
	   glMatrixMode(GL_MODELVIEW);
	   glLoadIdentity();
	   gluLookAt(0.0f,0.0f,4.0f, 
		          0.0,0.0,-1.0,
			       0.0f,1.0f,0.0f);
   }
	virtual void OnClose(void){}
	virtual void OnMouseDown(int button, int x, int y) {}    
	virtual void OnMouseUp(int button, int x, int y) {}
	virtual void OnMouseWheel(int nWheelNumber, int nDirection, int x, int y){}

	virtual void OnKeyDown(int nKey, char cAscii)
	{       
        if (cAscii == 27) // 0x1b = ESC
        {
            this->Close(); // Close Window!
        }
        else if (cAscii == 'a')
        {
            cameraMove += 0.008f;
            if (cameraMove > 1.0f)
                cameraMove = 0.0f;
            std::cout << cameraMove << "\n";
            
        }
        else if (cAscii == 'd')
        {
            cameraMove -= 0.008f;
            if (cameraMove < 0.0f)
                cameraMove = 1.0f;
            std::cout << cameraMove << "\n";
        }
        else if (cAscii == 'j')
        {
            facingLeft = true;
            if (carriageMove - 0.05f > -4.0f)
                carriageMove -= 0.05f;
        }
        else if (cAscii == 'l')
        {
            facingLeft = false;
            if (carriageMove + 0.05f < 4.0f)
                carriageMove += 0.05f;
        }
	};

	virtual void OnKeyUp(int nKey, char cAscii)
	{
      if (cAscii == 's')      // s: Shader
         shader->enable();
      else if (cAscii == 'f') // f: Fixed Function
         shader->disable();
      

	}


   void UpdateTimer()
   {
      time1 = clock();
      float delta = static_cast<float>(static_cast<double>(time1-time0)/static_cast<double>(CLOCKS_PER_SEC));
      delta = delta / 4;
      if (delta > 0.00005f)
      {
         time0 = clock();
         if (bUp)
         {
            timer010 += delta;
            if (timer010>=1.0f) { timer010 = 1.0f; bUp = false;}
         }
         else
         {
            timer010 -= delta;
            if (timer010<=0.0f) { timer010 = 0.0f; bUp = true;}
         }
      }
   }

   void DemoLight(void)
   {
     glEnable(GL_LIGHTING);
     glEnable(GL_LIGHT0);
     glEnable(GL_NORMALIZE);
     
     // Light model parameters:
     // -------------------------------------------
     
     /*GLfloat lmKa[] = {0.0, 0.0, 0.0, 0.0 };
     glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmKa);
     
     glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
     glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);*/
     
     // -------------------------------------------
     // Spotlight Attenuation
     
     /*GLfloat spot_direction[] = {1.0, -1.0, -1.0 };
     GLint spot_exponent = 30;
     GLint spot_cutoff = 180;
     
     glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
     glLighti(GL_LIGHT0, GL_SPOT_EXPONENT, spot_exponent);
     glLighti(GL_LIGHT0, GL_SPOT_CUTOFF, spot_cutoff);
    
     GLfloat Kc = 1.0;
     GLfloat Kl = 0.0;
     GLfloat Kq = 0.0;
     
     glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION,Kc);
     glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, Kl);
     glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, Kq);*/
     
     
     // ------------------------------------------- 
     // Lighting parameters:

     //GLfloat light_pos[] = {0.0f, 5.0f, 5.0f, 1.0f};
     //GLfloat light_Ka[]  = {0.3f, 0.5f, 0.5f, 1.0f};
     //GLfloat light_Kd[]  = {0.3f, 0.1f, 0.1f, 1.0f};
     //GLfloat light_Ks[]  = {0.3f, 1.0f, 1.0f, 1.0f};

     //glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
     //glLightfv(GL_LIGHT0, GL_AMBIENT, light_Ka);
     //glLightfv(GL_LIGHT0, GL_DIFFUSE, light_Kd);
     //glLightfv(GL_LIGHT0, GL_SPECULAR, light_Ks);

     // -------------------------------------------
     // Material parameters:

     //GLfloat material_Ka[] = {0.5f, 0.0f, 0.0f, 1.0f}; //Luz ambiental
     //GLfloat material_Kd[] = {0.4f, 0.4f, 0.5f, 1.0f}; //Luz difusa
     //GLfloat material_Ks[] = {0.8f, 0.8f, 0.0f, 1.0f}; //Poner en 0 para anular luz especular
     //GLfloat material_Ke[] = {0.1f, 0.0f, 0.0f, 0.0f}; //Opcional
     //GLfloat material_Se = 20.0f; //Opcional

     //glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_Ka); //Aplicar ambiental
     //glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_Kd); //Aplicar difusa
     //glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_Ks); //Aplicar especular
     //glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material_Ke);
     //glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material_Se); //Brillo del objeto
   }
};

//-----------------------------------------------------------------------------

class myApplication : public cwc::glApplication
{
public:
	virtual void OnInit() {std::cout << "Hello World!\n"; }
};

//-----------------------------------------------------------------------------

int main(void)
{
	myApplication*  pApp = new myApplication;
	myWindow* myWin = new myWindow();

	pApp->run();
	delete pApp;
	return 0;
}

//-----------------------------------------------------------------------------

