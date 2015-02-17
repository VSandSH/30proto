#include <windows.h>		
#include <math.h>			
#include <stdio.h>			
#include <stdlib.h>			
#include <gl\gl.h>			
#include <gl\glu.h>			
#include <gl\glaux.h>
#include "resource.h"
#include "tvector.h"
#include "tmatrix.h"
#include "tray.h"
#include <mmsystem.h>
#include "image.h"
#include <stdarg.h>  
#pragma comment(lib, "opengl32.lib")					// Link OpenGL32.lib
#pragma comment(lib, "glu32.lib")	
#pragma comment( lib, "winmm.lib" )
#define		MAP_SIZE	  1024					
#define		STEP_SIZE	  16					
#define		HEIGHT_RATIO  0.7f
#define		PI            3.1415926535
GLfloat spec[]={1.0, 1.0 ,1.0 ,1.0};      
GLfloat posl[]={0,400,0,1};    
GLfloat amb[]={0.2f, 0.2f, 0.2f ,1.0f};   
GLfloat amb2[]={0.3f, 0.3f, 0.3f ,1.0f};  

TVector dir(0,-5,-10);                     
TVector pos(0,-50,697);

float camera_rotation=0;                  
TVector veloc(0.5,-0.1,0.5);              
TVector accel(0,-0.05,0);   
TVector tochka(256,200,250);              

TVector ArrayVel[10];                    
TVector ArrayPos[10];                    
TVector OldPos[10];                      
int NrOfBalls;        
int  sounds=1;                    
double Time=0.6;
GLfloat	xrot=0;
  
int X,x,yy,z=250,zz=901;

float s=0,s1=2,velocity=1,velocity1=1;
double a,b,c;	
double alfa,alfa1;
struct Plane{
	        TVector _Position;
			TVector _Normal;
};
struct Explosion{
	   TVector _Position;
	   float   _Alpha;
	   float   _Scale;
};
Plane pl1,pl2,pl3,pl4,pl5;              //the 5 planes of the room
GLuint texture[4], dlist; 
GLUquadricObj *sphere_obj;              

Explosion ExplosionArray[20];             
int TestIntersionPlane(const Plane& plane,const TVector& position,const TVector& direction, double& lamda, TVector& pNormal);

void LoadGLTextures();                    
void InitVars();
void idle();
HINSTANCE GlobalInstance;
HDC				hDC=NULL;			
HGLRC			hRC=NULL;			
HWND			hWnd=NULL;			
HINSTANCE		hInstance;			
HMENU			Menu;
DEVMODE			DMsaved;
		
GLuint  base;      
GLfloat  cnt1;   
float cr=0.2f;  
bool			keys[256];			
bool			active=TRUE;		
bool			fullscreen=TRUE;	
bool		bRender = TRUE;	
BYTE g_HeightMap[MAP_SIZE*MAP_SIZE];					

float scaleValue = 0.15f;
long aralik=2,kural=3;
int ProcessKeys();
LRESULT CALLBACK InsertKoshumchaDlgProc(HWND hWnd, UINT msg, WPARAM wParam,LPARAM lParam);
LRESULT CALLBACK AboutDlgProc(HWND hWnd, UINT msg, WPARAM wParam,LPARAM lParam);

LRESULT CALLBACK InsertDlgProc(HWND hWnd, UINT msg, WPARAM wParam,LPARAM lParam)
{
	switch (msg)
	{		
	case WM_INITDIALOG:
		{
			SendDlgItemMessage (hWnd, IDC_RADIO1, BM_SETCHECK, BST_CHECKED, 0);
			SendDlgItemMessage (hWnd, IDC_COMBO1, CB_ADDSTRING,0, (LPARAM)"Erase Me");
			SendDlgItemMessage (hWnd, IDC_COMBO1, CB_RESETCONTENT,0, 0);
			SendDlgItemMessage (hWnd, IDC_COMBO1, CB_ADDSTRING,0, (LPARAM)"ARTKA");
			SendDlgItemMessage (hWnd, IDC_COMBO1, CB_ADDSTRING,0, (LPARAM)"ALGA");
			SendDlgItemMessage (hWnd, IDC_COMBO1, CB_SETCURSEL,0, 1);
		}break;
		case WM_COMMAND:
		{				
			if (wParam == IDOK)
			{
				char	temps[500];				
				
				GetDlgItemText (hWnd, IDC_EDIT2, temps, 500);
				sscanf (temps, "%f", &s);				
				//aralik = SendDlgItemMessage (hWnd,IDC_COMBO1, CB_GETCURSEL, 0, 0);
							
				if (SendDlgItemMessage (hWnd, IDC_RADIO1, BM_GETCHECK, 0, 0) == BST_CHECKED) 
					DialogBox ( GlobalInstance, MAKEINTRESOURCE(IDD_DIALOG3), NULL, (DLGPROC)InsertKoshumchaDlgProc );
					else if(SendDlgItemMessage (hWnd, IDC_RADIO2, BM_GETCHECK, 0, 0) == BST_CHECKED)
				{
					s1=0;
				}
			
				EndDialog (hWnd, 0);
			}
			else if (wParam == IDCANCEL)
			{				
				EndDialog (hWnd, 0);
					}			
		} break;
	}
	return (0);
}
LRESULT CALLBACK AboutDlgProc(HWND hWnd, UINT msg, WPARAM wParam,LPARAM lParam)
{
	switch (msg)
	{			
		case WM_COMMAND:
		{				
			if (wParam == IDOK)
			{				
				EndDialog (hWnd, 0);
			}					
		} break;
	}
	return (0);
}

LRESULT CALLBACK HelpDlgProc(HWND hWnd, UINT msg, WPARAM wParam,LPARAM lParam)
{
	switch (msg)
	{			
		case WM_COMMAND:
		{				
			if (wParam == IDOK)
			{				
				EndDialog (hWnd, 0);
			}					
		} break;
	}
	return (0);
}

LRESULT CALLBACK InsertKoshumchaDlgProc(HWND hWnd, UINT msg, WPARAM wParam,LPARAM lParam)
{
	switch (msg)
	{		
	case WM_INITDIALOG:
		{			
			SendDlgItemMessage (hWnd, IDC_COMBO1, CB_ADDSTRING,0, (LPARAM)"Erase Me");
			SendDlgItemMessage (hWnd, IDC_COMBO1, CB_RESETCONTENT,0, 0);
			SendDlgItemMessage (hWnd, IDC_COMBO1, CB_ADDSTRING,0, (LPARAM)"ARTKA");
			SendDlgItemMessage (hWnd, IDC_COMBO1, CB_ADDSTRING,0, (LPARAM)"ALGA");
			SendDlgItemMessage (hWnd, IDC_COMBO1, CB_SETCURSEL,0, 1);
		}break;
		case WM_COMMAND:
		{				
			if (wParam == IDOK)
			{
				char	tempv[500];
				float tt=0;
			
				GetDlgItemText (hWnd, IDC_EDIT1, tempv, 500);
				sscanf (tempv, "%f", &velocity1);				
				aralik = SendDlgItemMessage (hWnd,IDC_COMBO1, CB_GETCURSEL, 0, 0);
				tt=s/(velocity+velocity1);
				s1=velocity*tt;
				EndDialog (hWnd, 0);
			}
			else if (wParam == IDCANCEL)
			{				
				EndDialog (hWnd, 0);
			}			
		} break;
	}
	return (0);
}

void WMCommand(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (wParam == ID_FILE_EXIT) PostQuitMessage(0);
	else if(wParam==ID_INSERT) DialogBox ( GlobalInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)InsertDlgProc );
	else if(wParam==ID_HELP_ABOUT) DialogBox ( GlobalInstance, MAKEINTRESOURCE(IDD_DIALOG4), NULL, (DLGPROC)AboutDlgProc );
	else if(wParam==ID_HELP_HELP) DialogBox ( GlobalInstance, MAKEINTRESOURCE(IDD_DIALOG5), NULL, (DLGPROC)HelpDlgProc );
}
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	
GLvoid BuildFont(GLvoid)  
{
  HFONT  font;            
  
  base = glGenLists(96); 
  font = CreateFont(  -17,        
						0,
						0,
						0,
						FW_NORMAL,      
						TRUE,
						FALSE,
						FALSE,
						ANSI_CHARSET,
						OUT_TT_PRECIS,
						CLIP_DEFAULT_PRECIS,
						ANTIALIASED_QUALITY,
						FF_SCRIPT|VARIABLE_PITCH,
						"Eras Medium ITC");
  SelectObject(hDC, font);
  wglUseFontBitmaps(hDC, 32, 96, base); 
}
GLvoid KillFont(GLvoid)     
{
   glDeleteLists(base, 96); 
}
GLvoid glPrint(const char *fmt, ...)      
{
  char    text[256];      
  va_list    ap;          
  if (fmt == NULL)     
    return;            
  
  va_start(ap, fmt);      
  vsprintf(text, fmt, ap);
  va_end(ap); 
  glPushAttrib(GL_LIST_BIT);    
  glListBase(base - 32);          
  glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
  glPopAttrib();
}
GLvoid ReSizeGLScene(GLsizei width, GLsizei height)	
{
	if (height==0)									
		height=1;									

	glViewport(0,0,width,height);			

	glMatrixMode(GL_PROJECTION);			
	glLoadIdentity();						

	gluPerspective(50.0f,(GLfloat)width/(GLfloat)height,10.0f,3400.0f);

	glMatrixMode(GL_MODELVIEW);				
	glLoadIdentity();						
}
void LoadRawFile(LPSTR strName, int nSize, BYTE *pHeightMap)
{
	FILE *pFile = NULL;

	pFile = fopen( strName, "rb" );

	if ( pFile == NULL )	
	{
		MessageBox(NULL, "Can't Find The Height Map!", "Error", MB_OK);
		return;
	}
	fread( pHeightMap, 1, nSize, pFile );
	
	int result = ferror( pFile );

	if (result)
	{
		MessageBox(NULL, "Failed To Get Data!", "Error", MB_OK);
	}
	fclose(pFile);
}
int InitGL(GLvoid)		
{
   	float df=100.0;

	glClearDepth(1.0f);	
	//glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);							
	glDepthFunc(GL_LEQUAL);								
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	
	LoadRawFile("Data/Terrain.raw", MAP_SIZE * MAP_SIZE, g_HeightMap);	
		
	BuildFont();
	glClearColor(0,0,0,0);
  	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	glShadeModel(GL_SMOOTH);
	glEnable(GL_CULL_FACE);
	//glEnable(GL_DEPTH_TEST);
	
	glMaterialfv(GL_FRONT,GL_SPECULAR,spec);
	glMaterialfv(GL_FRONT,GL_SHININESS,&df);

	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0,GL_POSITION,posl);
	glLightfv(GL_LIGHT0,GL_AMBIENT,amb2);
	glEnable(GL_LIGHT0);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,amb);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
   
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
   	
	glEnable(GL_TEXTURE_2D);
    LoadGLTextures();

	
	//4 quads at right angles to each other
    glNewList(dlist=glGenLists(1), GL_COMPILE);
    glBegin(GL_QUADS);
	glRotatef(-45,0,1,0);	
    glNormal3f(-1,0,0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0,40,50);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0,40,-50);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0,-40,-50);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0,-40,50);
	glNormal3f(1,0,0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0,-40,50);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0,-40,-50);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0,40,-50);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0,40,50);

	glNormal3f(0,0,-1);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-50,40,0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(50,40,0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(50,-40,0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-50,-40,0);
	glNormal3f(0,0,1);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-50,-40,0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(50,-40,0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(50,40,0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-50,40,0);
	glEnd();
    glEndList();

	return TRUE;									
}
int Height(BYTE *pHeightMap, int X, int Y)				
{
	int x = X % MAP_SIZE;								
	int y = Y % MAP_SIZE;							

	if(!pHeightMap) return 0;							

	return pHeightMap[x + (y * MAP_SIZE)];			
}
														
void SetVertexColor(BYTE *pHeightMap, int x, int y)		
{
	if(!pHeightMap) return;								

	float fColor = -0.15f + (Height(pHeightMap, x, y ) / 256.0f);

	glColor3f(0.0f,fColor,0.0f );
}
void RenderHeightMap(BYTE pHeightMap[])					
{
	int X = 0, Y = 0;									
	int x, y, z;										

	if(!pHeightMap) return;								
	glTranslatef(-520.0f,-540.0f,-520.0f); //,,
	if(bRender)											
		glBegin( GL_QUADS );							
	else 
		glBegin( GL_LINES );							

	for ( X = 0; X < (MAP_SIZE-STEP_SIZE); X += STEP_SIZE )
		for ( Y = 0; Y < (MAP_SIZE-STEP_SIZE); Y += STEP_SIZE )
		{
			x = X;							
			y = Height(pHeightMap, X, Y );	
			z = Y;							

			SetVertexColor(pHeightMap, x, z);

			glVertex3i(x, y, z);	

			x = X;										
			y = Height(pHeightMap, X, Y + STEP_SIZE );  
			z = Y + STEP_SIZE ;							
						
			SetVertexColor(pHeightMap, x, z);

			glVertex3i(x, y, z);
			
			x = X + STEP_SIZE; 
			y = Height(pHeightMap, X + STEP_SIZE, Y + STEP_SIZE ); 
			z = Y + STEP_SIZE ;

			SetVertexColor(pHeightMap, x, z);
			
			glVertex3i(x, y, z);						

			x = X + STEP_SIZE; 
			y = Height(pHeightMap, X + STEP_SIZE, Y ); 
			z = Y;

			SetVertexColor(pHeightMap, x, z);

			glVertex3i(x, y, z);						
		}
	glEnd();						
}

void Ball(BYTE pHeightMap[]){	

	if(!pHeightMap) return;
	
			X = MAP_SIZE/4;				
			x = X;							
			yy = Height(pHeightMap, X, z/2 );				
			glPushMatrix();		
			glColor3f(0.9f,0.0f,0.0f);			
			glTranslated(x,yy+2,z/2);
			gluSphere(sphere_obj,5,20,20);			//mishen'		
			glPopMatrix();

							
			glPushMatrix();		
			glColor3f(0.0f,0.0f,0.0f);
			glTranslated(256,203,901);	
			glRotatef(180,0.0f,1.0f,1.0f);
			glRotatef(45,1.0f,0.0f,0.0f);			
			gluDisk(sphere_obj,0,3,30,40);			//disk
			glPopMatrix();
			glPushMatrix();
			glTranslated(256,((sin(xrot)*180)/PI)*3.5+203,zz);
			gluSphere(sphere_obj,3,25,25);			//snaryad
			glPopMatrix();
			

			X = MAP_SIZE/4;	
			yy = Height(pHeightMap, X, 900 );
			glPushMatrix();
			glTranslated(256,yy+20,900);
			glRotatef(45,1.0f,0.0f,0.0f);					
			gluCylinder(sphere_obj,3,3,30,40,20);	// pushka			
			glPopMatrix();
			
						
}

int DrawGLScene(GLvoid)	            // Here's Where We Do All The Drawing
{	
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	
    gluLookAt(pos.X(),pos.Y(),pos.Z(), pos.X()+dir.X(),pos.Y()+dir.Y(),pos.Z()+dir.Z(), 0,1.0,0.0);
	
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
	glPushMatrix();
	glLoadIdentity();	
    glTranslatef(4.5f,3.0f,-10.001f);
	glColor3f(0.0f,1.0f,0.0f);
	glBegin(GL_LINES);	
	for (float x = -1; x <= 1; x += 1.0f)
	{
		glVertex3f(x, 1, 0);
		glVertex3f(x,-1, 0);
	}
	for (float y = -1; y <= 1; y += 1.0f)						// y += 1.0f Stands For 1 Meter Of Space In This Example
	{
		glVertex3f( 1, y, 0);
		glVertex3f(-1, y, 0);
	}
	glEnd();
	glPopMatrix();
	
	glRotatef(camera_rotation,0,1,0);	
		
	glPushMatrix();
	glLoadIdentity();	
    glTranslatef(0.0f,0.0f,-10.001f); 
    glColor3f(0.0f,0.0f,0.0f);
    glRasterPos2f(3.8f, 3.60f);
    glPrint("rad:      %3.2f", alfa);//
	
	glLoadIdentity();
	glTranslatef(0.0f,0.0f,-10.001f); 
	glRasterPos2f(3.6f,2.60f);
	glPrint("grad:       %3.2f",alfa1 );//
    glPopMatrix();

	glPushMatrix();
	RenderHeightMap(g_HeightMap);	
	Ball(g_HeightMap);
	glPopMatrix();
	
	glEnable(GL_TEXTURE_2D);
	
	//render walls(planes) with texture
	glBindTexture(GL_TEXTURE_2D, texture[3]); 
	glColor3f(1, 1, 1);
	glFrontFace (GL_CW );
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-640,640,490);  //+z
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-640,-640,490);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(640,-640,490);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(640,640,490);
	glEnd();
	glFrontFace (GL_CCW );
	glFrontFace (GL_CW );
    glBegin(GL_QUADS);   
	glTexCoord2f(1.0f, 0.0f); glVertex3f(640,640,-530);  //-z
	glTexCoord2f(1.0f, 1.0f); glVertex3f(640,-640,-530);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-640,-640,-530);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-640,640,-530);
	glEnd();
    glFrontFace (GL_CCW );
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(490,640,-640);   //+x
	glTexCoord2f(1.0f, 1.0f); glVertex3f(490,-640,-640);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(490,-640,540);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(490,640,540);
	
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-520,640,640);  //-x
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-520,-640,640);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-520,-640,-640);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-520,640,-640);
	glEnd();	

	//render floor (plane) with colours
	glBindTexture(GL_TEXTURE_2D, texture[2]); 
    glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-450,-450,450);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(450,-450,450);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(450,-450,-530);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-450,-450,-530);
	glEnd();	

	glEnable(GL_BLEND);	
	glDepthMask(GL_FALSE);
    glBindTexture(GL_TEXTURE_2D, texture[1]);   
	for(int i=0; i<3; i++)
	{
		if(ExplosionArray[i]._Alpha>=0)
		{
		   glPushMatrix();
           ExplosionArray[i]._Alpha-=0.01f;
		   ExplosionArray[i]._Scale+=0.03f;
		   glColor4f(1,1,0,ExplosionArray[i]._Alpha);	 
		   glScalef(ExplosionArray[i]._Scale,ExplosionArray[i]._Scale,ExplosionArray[i]._Scale);
           glTranslatef((float)ExplosionArray[i]._Position.X()/ExplosionArray[i]._Scale, (float)ExplosionArray[i]._Position.Y()/ExplosionArray[i]._Scale, (float)ExplosionArray[i]._Position.Z()/ExplosionArray[i]._Scale);
           glCallList(dlist);
	       glPopMatrix();
		}
	}
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);	
	
	return TRUE;					
}
GLvoid KillGLWindow(GLvoid)			
{
	if (fullscreen)					
	{
		if (!ChangeDisplaySettings(NULL,CDS_TEST)) { 
			ChangeDisplaySettings(NULL,CDS_RESET);		
			ChangeDisplaySettings(&DMsaved,CDS_RESET);	
		} else {
			ChangeDisplaySettings(NULL,CDS_RESET);
		}			
		ShowCursor(TRUE);								
	}
	if (hRC)											
	{
		if (!wglMakeCurrent(NULL,NULL))					
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										
	}
	if (hDC && !ReleaseDC(hWnd,hDC))					
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										
	}
	if (hWnd && !DestroyWindow(hWnd))					
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;			
	}
	if (!UnregisterClass("OpenGL",hInstance))			
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									
	}
	KillFont();
}
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			
	WNDCLASS	wc;						
	DWORD		dwExStyle;				
	DWORD		dwStyle;				
	RECT		WindowRect;				
	WindowRect.left=(long)0;			
	WindowRect.right=(long)width;		
	WindowRect.top=(long)0;				
	WindowRect.bottom=(long)height;		

	fullscreen=fullscreenflag;			

	hInstance			= GetModuleHandle(NULL);			
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	
	wc.lpfnWndProc		= (WNDPROC) WndProc;					
	wc.cbClsExtra		= 0;									
	wc.cbWndExtra		= 0;									
	wc.hInstance		= hInstance;							
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			
	wc.hbrBackground	= NULL;									
	wc.lpszMenuName		= NULL;									
	wc.lpszClassName	= "OpenGL";								
	
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &DMsaved); 

	if (fullscreen)												
	{
		DEVMODE dmScreenSettings;								
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		
		dmScreenSettings.dmPelsWidth	= width;				
		dmScreenSettings.dmPelsHeight	= height;				
		dmScreenSettings.dmBitsPerPel	= bits;					
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
		
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;		
			}
			else
			{				
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									
			}
		}
	}
	if (!RegisterClass(&wc))									
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											
	}
	if (fullscreen)												
	{
		dwExStyle=WS_EX_APPWINDOW;								
		dwStyle=WS_POPUP;										
		ShowCursor(FALSE);										
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			
		dwStyle=WS_OVERLAPPEDWINDOW;							
	}
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);	

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							
								"OpenGL",							
								title,								
								dwStyle |							
								WS_CLIPSIBLINGS |					
								WS_CLIPCHILDREN,					
								0,0,								
								WindowRect.right-WindowRect.left,	
								WindowRect.bottom-WindowRect.top,	
								NULL,								
								NULL,								
								hInstance,							
								NULL)))								
	{
		KillGLWindow();								
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}
	Menu = LoadMenu (hInstance, MAKEINTRESOURCE(IDR_MENU1));
	SetMenu (hWnd, Menu);
	static	PIXELFORMATDESCRIPTOR pfd=				
	{
		sizeof(PIXELFORMATDESCRIPTOR),				
		1,											
		PFD_DRAW_TO_WINDOW |						
		PFD_SUPPORT_OPENGL |						
		PFD_DOUBLEBUFFER,							
		PFD_TYPE_RGBA,								
		bits,										
		0, 0, 0, 0, 0, 0,							
		0,											
		0,											
		0,											
		0, 0, 0, 0,									
		16,											
		0,											
		0,											
		PFD_MAIN_PLANE,								
		0,											
		0, 0, 0										
	};	
	if (!(hDC=GetDC(hWnd)))							
	{
		KillGLWindow();								
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}
	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	
	{
		KillGLWindow();								
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}
	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		
	{
		KillGLWindow();								
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}
	if (!(hRC=wglCreateContext(hDC)))				
	{
		KillGLWindow();								
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}
	if(!wglMakeCurrent(hDC,hRC))					
	{
		KillGLWindow();								
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}
	ShowWindow(hWnd,SW_SHOW);						
	SetForegroundWindow(hWnd);						
	SetFocus(hWnd);									
	ReSizeGLScene(width, height);					

    if (!InitGL())									
	{
		KillGLWindow();								
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}
	return TRUE;									
}
LRESULT CALLBACK WndProc(	HWND	hWnd,			
							UINT	uMsg,			
							WPARAM	wParam,			
							LPARAM	lParam)			
{
	switch (uMsg)									
	{
		case WM_ACTIVATE:							
		{
			if (!HIWORD(wParam))					
			{
				active=TRUE;						
			}
			else
			{
				active=FALSE;						
			}
			return 0;								
		}
		case WM_SYSCOMMAND:							
		{
			switch (wParam)							
			{
				case SC_SCREENSAVE:					
				case SC_MONITORPOWER:				
				return 0;							
			}
			break;									
		}
		case WM_CLOSE:								
		{
			PostQuitMessage(0);						
			return 0;								
		}
		case WM_KEYDOWN:							
		{
			keys[wParam] = TRUE;					
			return 0;								
		}
		case WM_KEYUP:								
		{
			keys[wParam] = FALSE;					
			return 0;							
		}
		case WM_COMMAND:
		{
			WMCommand (hWnd, uMsg, wParam, lParam); 
			return 0;
		}
		case WM_SIZE:							
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam)); 
			return 0;								
		}
	}	
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

LRESULT CALLBACK ConfigDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_INITDIALOG:
		{
			SendDlgItemMessage (hWnd, IDC_RADIO1, BM_SETCHECK, BST_CHECKED, 0);
			SendDlgItemMessage (hWnd, IDC_KURAL, CB_ADDSTRING,0, (LPARAM)"Erase Me");
			SendDlgItemMessage (hWnd, IDC_KURAL, CB_RESETCONTENT,0, 0);
			SendDlgItemMessage (hWnd, IDC_KURAL, CB_ADDSTRING,0, (LPARAM)"122-mm gaubiza M-30");
			SendDlgItemMessage (hWnd, IDC_KURAL, CB_ADDSTRING,0, (LPARAM)"122-mm polevaya pushka D-74");
			SendDlgItemMessage (hWnd, IDC_KURAL, CB_ADDSTRING,0, (LPARAM)"180-mm pushka C-23");
			SendDlgItemMessage (hWnd, IDC_KURAL, CB_SETCURSEL,0, 1);			
		} break;
		case WM_COMMAND:
		{
			if (wParam == IDOK) 
			{
				if (SendDlgItemMessage (hWnd, IDC_RADIO1, BM_GETCHECK, 0, 0) == BST_CHECKED) fullscreen=FALSE;
				else if (SendDlgItemMessage (hWnd, IDC_RADIO2, BM_GETCHECK, 0, 0) == BST_CHECKED) fullscreen=TRUE;
				
				kural = SendDlgItemMessage (hWnd,IDC_KURAL, CB_GETCURSEL, 0, 0);
				
				switch (kural){
				case 0:velocity=515.0;break;
				case 1:velocity=885.0;break;
				case 2:velocity=790.0;break;
				}

				EndDialog (hWnd, 1);
			}
			else if (wParam == IDCANCEL) EndDialog (hWnd, 0);
		} break;
	}
	return (0);
}

int WINAPI WinMain(	HINSTANCE	hInstance,			
					HINSTANCE	hPrevInstance,		
					LPSTR		lpCmdLine,			
					int			nCmdShow)			
{
	MSG		msg;									
	BOOL	done=FALSE;	
	
/*	if (MessageBox(NULL,"Ekrandi kaptasin bi?", "Chon ekran?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{}*/
	
	if (!DialogBox (hInstance, MAKEINTRESOURCE(IDD_DIALOG2), NULL, (DLGPROC)ConfigDlgProc)) 
	{
		MessageBox (NULL, "Programmaga konul burganiniz uchun RAHMAT !", "PROGRAMMADAN CHIGUU", MB_OK);
		return (0);
	}

	InitVars();                                 
	// Create Our OpenGL Window
	if (!CreateGLWindow("3-Olchomdyy Meykindik",640,480,16,fullscreen))
	{
		return 0;			
	}

	while(!done)			
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	
		{
			if (msg.message==WM_QUIT)				
			{
				done=TRUE;							
			}
			else									
			{
				TranslateMessage(&msg);				
				DispatchMessage(&msg);				
			}
		}
		else										
		    if (active)
			{
				// Draw The Scene. 
				if (keys[VK_ESCAPE])	
				{
					done=TRUE;			
				}
				else
				{
					idle();  
					DrawGLScene();     
					SwapBuffers(hDC);
				}
	
				if (!ProcessKeys()) return 0;
			}
	}


	KillGLWindow();					
    glDeleteTextures(4,texture);
	return (msg.wParam);			
}
void InitVars()
{
	
	 //create palnes
	pl1._Position=TVector(0,-325,0);
	pl1._Normal=TVector(0,1,0);

	sphere_obj= gluNewQuadric();

	NrOfBalls=1;
	ArrayVel[0]=veloc;
	ArrayPos[0]=TVector(-270,-325,-140);
	ExplosionArray[0]._Alpha=0;
	ExplosionArray[0]._Scale=1;
	ArrayVel[1]=veloc;
	ArrayPos[1]=TVector(0,150,100);
	ExplosionArray[1]._Alpha=0;
	ExplosionArray[1]._Scale=1;
	ArrayVel[2]=veloc;
	ArrayPos[2]=TVector(-100,180,-100);
	ExplosionArray[2]._Alpha=0;
	ExplosionArray[2]._Scale=1;
	for (int i=3; i<10; i++)
	{
         ArrayVel[i]=veloc;
	     ArrayPos[i]=TVector(-500+i*75, 300, -500+i*50);
		 ExplosionArray[i]._Alpha=0;
	     ExplosionArray[i]._Scale=1;
	}
	for (i=10; i<20; i++)
	{
         ExplosionArray[i]._Alpha=0;
	     ExplosionArray[i]._Scale=1;
	}
}
int TestIntersionPlane(const Plane& plane,const TVector& position,const TVector& direction, double& lamda, TVector& pNormal)
{

    double DotProduct=direction.dot(plane._Normal);
	double l2;

    //determine if ray paralle to plane
    if ((DotProduct<ZERO)&&(DotProduct>-ZERO)) 
		return 0;

    l2=(plane._Normal.dot(plane._Position-position))/DotProduct;

    if (l2<-ZERO) 
		return 0;

    pNormal=plane._Normal;
	lamda=l2;
    return 1;

}
void LoadGLTextures() {	
    
    Image *image1, *image2, *image3, *image4;
        
    image1 = (Image *) malloc(sizeof(Image));
    if (image1 == NULL) {
	printf("Error allocating space for image");
	exit(0);
    }
	image2 = (Image *) malloc(sizeof(Image));
    if (image2 == NULL) {
	printf("Error allocating space for image");
	exit(0);
    }
	image3 = (Image *) malloc(sizeof(Image));
    if (image3 == NULL) {
	printf("Error allocating space for image");
	exit(0);
    }
	image4 = (Image *) malloc(sizeof(Image));
    if (image4 == NULL) {
	printf("Error allocating space for image");
	exit(0);
    }

    if (!ImageLoad("data/sky_fr.bmp", image1)) {		//zilindri
	exit(1);
    } 
	if (!ImageLoad("data/Spark.bmp", image2)) {		//ogonki
	exit(1);
    }
	if (!ImageLoad("data/ray0.bmp", image3)) {			//zemlya
	exit(1);
    } 
	if (!ImageLoad("data/ray2.bmp", image4)) {		//steni
	exit(1);
    }
    
    glGenTextures(2, &texture[0]);
    glBindTexture(GL_TEXTURE_2D, texture[0]); 

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image1->data);

    glBindTexture(GL_TEXTURE_2D, texture[1]);   

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
        
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image2->sizeX, image2->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image2->data);

    glGenTextures(2, &texture[2]);
    glBindTexture(GL_TEXTURE_2D, texture[2]);   

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, 3, image3->sizeX, image3->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image3->data);

    glBindTexture(GL_TEXTURE_2D, texture[3]);   

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
       
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image4->sizeX, image4->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image4->data);

	free(image1->data);
	free(image1);
	free(image2->data);
	free(image2);
	free(image3->data);
	free(image3);
	free(image4->data);
	free(image4);

};
int ProcessKeys()
{
	if (keys[VK_UP])    {pos+=TVector(0,0,-2);	}
	if (keys[VK_DOWN])  {pos=TVector(0,-50,697);}
	if (keys[VK_LEFT])  cr=0.2f;
	if (keys[VK_RIGHT]) {cr=0.0f;keys[VK_LEFT]=false;}
	if (keys['y']) {
		sounds^=1;
		keys['y']=FALSE;
	}
	
	if (keys[VK_F1])						
	{
		keys[VK_F1]=FALSE;					
		KillGLWindow();						
		fullscreen=!fullscreen;				
		
		if (!CreateGLWindow("3-x mernoye prostranstvo",640,480,16,fullscreen))
		{
			return 0;						
		}
	}
	return 1;
}

void idle()
{	
	TVector normal,point,time;
	TVector norm,uveloc;
	double rt,rt2,rt4,lamda=10000,RestTime;
	TVector Pos2,Nc;
	int BallNr=0,dummy=0;
   
	camera_rotation+=cr;
	if (camera_rotation>360)
		camera_rotation=0;
	
	if(s1==0){
		z=750;
		zz-=1;
		xrot+=0.00605f;
		if(xrot>180)
			xrot=0;
	}

	if (aralik==0){		
		z+=1;
		zz-=1;
		xrot+=0.00605f;
		if(xrot>180)
			xrot=0;
	}
	else
		if (aralik==1){
			z-=1;
			zz-=1;
		xrot+=0.00605f;
		if(xrot>180)
			xrot=0;
		}		
			
		if(s1==0){
			alfa=asin((s*9.81)/(velocity*velocity));
			alfa1=(alfa*180)/PI;
		}
		else if (s1!=0 && s1!=2){
			alfa=asin((s1*9.81)/(velocity*velocity));
			alfa1=(alfa*180)/PI;
		}
		
		if(z/2>zz  ){
			z=-5;  zz=-25;xrot=0;

			RestTime=Time;
			lamda=1000;
		
			for (int j=0;j<NrOfBalls;j++)
				ArrayVel[j]+=accel*RestTime;
		
			while (RestTime>ZERO)
			{
				lamda=10000; 
				for (int i=0;i<NrOfBalls;i++)
				{					
					OldPos[i]=ArrayPos[i];
					TVector::unit(ArrayVel[i],uveloc);
					ArrayPos[i]=ArrayPos[i]+ArrayVel[i]*RestTime;
					rt2=OldPos[i].dist(ArrayPos[i]);
					
					if (TestIntersionPlane(pl1,OldPos[i],uveloc,rt,norm))
					{						
						rt4=rt*RestTime/rt2;
							if (rt4<=lamda){
							if (rt4<=RestTime+ZERO)
								if (! ((rt<=ZERO)&&(uveloc.dot(norm)>ZERO)) ){
									normal=norm;
									point=OldPos[i]+uveloc*rt;
									lamda=rt4;
									BallNr=i;/**/
								}
						}
					}
					
					if (lamda!=10000){
						RestTime-=lamda;
						for (j=0;j<NrOfBalls;j++)
							ArrayPos[j]=OldPos[j]+ArrayVel[j]*lamda;
						rt2=ArrayVel[BallNr].mag();
						ArrayVel[BallNr].unit();
						ArrayVel[BallNr]=TVector::unit( (normal*(2*normal.dot(-ArrayVel[BallNr]))) + ArrayVel[BallNr] );
						ArrayVel[BallNr]=ArrayVel[BallNr]*rt2;
						if (sounds)
							PlaySound("Data/Explode.wav",NULL,SND_FILENAME|SND_ASYNC);
					
						for(j=0;j<3;j++){
							if (ExplosionArray[j]._Alpha<=0){
								ExplosionArray[j]._Alpha=1;
								ExplosionArray[j]._Position=point;
								ExplosionArray[j]._Scale=1;/**/
								break;
							}
						}cnt1++;
					}
					else RestTime=0;
					if( cnt1>3)
						ArrayPos[0]=TVector(0,0,0);
				}
			}
		}
}	

