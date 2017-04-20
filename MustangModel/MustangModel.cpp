// MustangModel.cpp: ���������� ����� ����� ��� ����������.
//

#include "stdafx.h"
#include "MustangModel.h"
#include <windows.h>                      // ������������ ���� ��� Windows
#include <gl\gl.h>                        // ������������ ���� ��� OpenGL32 ����������
#include <gl\glu.h>                       // ������������ ���� ��� GLu32 ����������
#include <gl\glaux.h>                     // ������������ ���� ��� GLaux ����������
#include <math.h>

#define MAX_LOADSTRING 100
#define M_PI 3.1415926535897932384626433832795

// ���������� ����������:
HINSTANCE hInst;								// ������� ���������
TCHAR szTitle[MAX_LOADSTRING];					// ����� ������ ���������
TCHAR szWindowClass[MAX_LOADSTRING];			// ��� ������ �������� ����
static HGLRC hRC;               // ���������� �������� ����������
static HDC hDC;                 // ��������� �������� ���������� GDI
BOOL    keys[256];              // ������ ��� ��������� ��������� ����������
HWND hWnd;
GLfloat rtri;           // ���� ��� �����������
GLfloat rquad;          // ���� ��� ����������������
GLfloat camXZ;
GLfloat camY;
GLfloat zoom;
GLfloat wheelsA, doorsA, hoodA, trunkA;
static int wheels, doors, hood, trunk;

// ��������� ���������� �������, ���������� � ���� ������ ����:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

GLvoid InitGL(GLsizei Width, GLsizei Height)    // ������� ����� �������� ���� GL
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);   // ������� ������ � ������ ����
	glClearDepth(1.0);                      // ��������� ������� ������ �������
    glDepthFunc(GL_LESS);                   // ��� ����� �������
    glEnable(GL_DEPTH_TEST);                // ��������� ���� �������
	glShadeModel(GL_SMOOTH);        // ��������� ������� �������� �����������
    glMatrixMode(GL_PROJECTION);    // ����� ������� ��������
    glLoadIdentity();               // ����� ������� ��������
    gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);
                            // ��������� ����������� �������������� �������� ��� ����
    glMatrixMode(GL_MODELVIEW);     // ����� ������� ��������� ������
}

GLvoid ReSizeGLScene(GLsizei Width, GLsizei Height)
{
        if (Height==0)          // �������������� ������� �� ����, ���� ���� ������� ����
                Height=1;

        glViewport(0, 0, Width, Height);
                // ����� ������� ������� ������ � ������������� ��������������

        glMatrixMode(GL_PROJECTION);            // ����� ������� ��������
        glLoadIdentity();                       // ����� ������� ��������

        gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);
                // ���������� ����������� �������������� �������� ��� ����
        glMatrixMode(GL_MODELVIEW);     // ����� ������� ��������� ������
}

GLvoid DrawGLScene(GLvoid)
{
		int i;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                // ������� ������ � ������ �������
        //glLoadIdentity();
		if (keys[VK_LEFT])                // ������� ����� ���� ������?
        {
			if (camXZ > 0.05f)
				camXZ -= 0.5f;             // ������� ����� �����
			else camXZ = 360.0f;
        }
        if (keys[VK_RIGHT])              // ������� ������ ������?
        {
			if (camXZ < 359.95f)
                camXZ += 0.5f;             // ������� ����� ������
			else camXZ = 0.0f;
        }
		if (keys[VK_DOWN])                // ������� ���� ���� ������?
        {
			if (camY >= -89.95f)
				camY -= 0.5f;             // ������� ����� �����
        }
        if (keys[VK_UP])              // ������� ����� ������?
        {
			if (camY <= 89.95f)
                camY += 0.5f;             // ������� ����� ����
        }
		if (keys[0x58])                // X ������?
        {
			if (zoom >= 5.005f)
				zoom -= 0.05f;             // ���������� �����
        }
        if (keys[0x5A])              // Z ������?
        {
			if (zoom <= 17.995f)
                zoom += 0.05f;             // �������� �����
        }
		switch (wheels)                // W (wheels) ������?
        {
			case 1:
				if (wheelsA < 350.0f)
					wheelsA += 1.0f;
				else wheelsA = 0.0f;
				break;
			case 2:
				if (wheelsA < 350.0f)
					wheelsA += 2.0f;
				else wheelsA = 0.0f;
				break;
			case 3:
				if (wheelsA < 350.0f)
					wheelsA += 4.0f;
				else wheelsA = 0.0f;
				break;
        }
        if (doors)              // D (doors) ������?
		{
			if (doorsA < 35.0f)
				doorsA += 1.0f;
		}
		else
		{
			if (doorsA > 0.0f)
				doorsA -= 1.0f;
		}
		if (hood)                // H (hood) ������?
        {
			if (hoodA < 45.0f)
				hoodA += 1.0f;
        }
		else
		{
			if (hoodA > 0.0f)
				hoodA -= 1.0f;
		}
        if (trunk)              // T (trunk) ������?
        {
			if (trunkA < 45.0f)
				trunkA += 1.0f;
        }
		else
		{
			if (trunkA > 0.0f)
				trunkA -= 1.0f;
		}
		// ������� ���������
        glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glShadeModel(GL_SMOOTH); 
		glEnable(GL_NORMALIZE); //����� ������� ���������� �������� �� ��������� ����������
		GLfloat ambientLight[] = { 0.4f, 0.4f, 0.4f, 1.0f };
		GLfloat diffuseLight[] = { 0.9f, 0.9f, 0.9f, 1.0f };
        GLfloat lightPosition[] = { 3.0f, 2.0f, 2.5f, 1.0f };
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
		glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.99);
		glEnable(GL_LIGHT0);

		glEnable(GL_ALPHA_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glLoadIdentity();
		gluLookAt(zoom*cos(M_PI*camY/180.0)*sin(M_PI*camXZ/180.0), zoom*sin(M_PI*camY/180.0), zoom*cos(M_PI*camY/180.0)*cos(M_PI*camXZ/180.0), // ��������� ����, ������ "��"
			0.0f, 0.0f, 0.0f, // ����, ������ "��"
			0.0f, 1.0f, 0.0f);
		
		/*glTranslatef(4.784f,0.0f,0.0f);
		gluSphere(gluNewQuadric(), 0.1, 10, 10);
		glTranslatef(-4.784f,0.0f,0.0f);
		glTranslatef(0.0f,1.381f,0.0f);
		gluSphere(gluNewQuadric(), 0.1, 10, 10);
		glTranslatef(0.0f,-1.381f,0.0f);
		glTranslatef(0.0f,0.0f,1.916f);
		gluSphere(gluNewQuadric(), 0.1, 10, 10);
		glTranslatef(0.0f,0.0f,-1.916f);
		glBegin(GL_LINES);
			glVertex3f(0.0f, 1.381f, 0.0f);
			glVertex3f(0.0f, -1.381f, 0.0f);
		glEnd();
		glBegin(GL_LINES);
			glVertex3f(4.784f, 0.0f, 0.0f);
			glVertex3f(-4.784f, 0.0f, 0.0f);
		glEnd();
		glBegin(GL_LINES);
			glVertex3f(0.0f, 0.0f, 1.916f);
			glVertex3f(0.0f, 0.0f, -1.916f);
		glEnd();*/
		
		//������
		glDisable(GL_LIGHTING);
		glColor3f(0xff, 0xd7, 0x00);
		glTranslatef(0.0f,-1.381f,0.0f);
		glRotatef(90,1.0f,0.0f,0.0f);
		gluDisk(gluNewQuadric(), 0.0, 5.0, 30, 5); // ����
		glRotatef(-90,1.0f,0.0f,0.0f);
		glTranslatef(0.0f,1.381f,0.0f);
		//������ �������� ������
		glColor3f(0.02f, 0.02f, 0.02f);
		glTranslatef(2.84f, -0.6905f, 1.420f);
		glRotatef(-wheelsA, 0.0f, 0.0f, 1.0f);
		gluCylinder(gluNewQuadric(), 0.6905f, 0.6905f, 0.3f, 20, 20); // �������
		glRotatef(wheelsA, 0.0f, 0.0f, 1.0f);
		glColor3f(0.8f, 0.8f, 0.8f);
		glTranslatef(0.0f, 0.0f, 0.18f);
		glRotatef(-wheelsA, 0.0f, 0.0f, 1.0f);
		gluDisk(gluNewQuadric(), 0.0f, 0.45f, 30, 5); // ����
		glColor3f(0.4f, 0.4f, 0.4f);
		gluCylinder(gluNewQuadric(), 0.45f, 0.45f, 0.12f, 20, 20); // �������
		for (i = 0; i < 5; i++)
		{
			glColor3f(0.4f, 0.4f, 0.4f);
			glBegin(GL_POLYGON);
				glVertex3f(0.3f*cos(M_PI*(21.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(21.0 + i*72.0)/180.0), 0.1f);
				glVertex3f(0.45f*cos(M_PI*(11.0 + i*72.0)/180.0), 0.45f*sin(M_PI*(11.0 + i*72.0)/180.0), 0.12f);
				glVertex3f(0.45f*cos(M_PI*(21.0 + i*72.0)/180.0), 0.45f*sin(M_PI*(21.0 + i*72.0)/180.0), 0.12f);
				glVertex3f(0.45f*cos(M_PI*(31.0 + i*72.0)/180.0), 0.45f*sin(M_PI*(31.0 + i*72.0)/180.0), 0.12f);
				glVertex3f(0.45f*cos(M_PI*(41.0 + i*72.0)/180.0), 0.45f*sin(M_PI*(41.0 + i*72.0)/180.0), 0.12f);
				glVertex3f(0.45f*cos(M_PI*(51.0 + i*72.0)/180.0), 0.45f*sin(M_PI*(51.0 + i*72.0)/180.0), 0.12f);
				glVertex3f(0.45f*cos(M_PI*(61.0 + i*72.0)/180.0), 0.45f*sin(M_PI*(61.0 + i*72.0)/180.0), 0.12f);
				glVertex3f(0.3f*cos(M_PI*(51.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(51.0 + i*72.0)/180.0), 0.1f);
			glEnd();
			glBegin(GL_POLYGON);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glVertex3f(0.3f*cos(M_PI*(21.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(21.0 + i*72.0)/180.0), 0.1f);
				glVertex3f(0.3f*cos(M_PI*(51.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(51.0 + i*72.0)/180.0), 0.1f);
			glEnd();
			glColor3f(0.2f, 0.2f, 0.2f);
			glBegin(GL_TRIANGLES);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glVertex3f(0.3f*cos(M_PI*(21.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(21.0 + i*72.0)/180.0), 0.0f);
				glVertex3f(0.3f*cos(M_PI*(21.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(21.0 + i*72.0)/180.0), 0.1f);
			glEnd();
			glBegin(GL_QUADS);
				glVertex3f(0.3f*cos(M_PI*(21.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(21.0 + i*72.0)/180.0), 0.0f);
				glVertex3f(0.45f*cos(M_PI*(11.0 + i*72.0)/180.0), 0.45f*sin(M_PI*(11.0 + i*72.0)/180.0), 0.0f);
				glVertex3f(0.45f*cos(M_PI*(11.0 + i*72.0)/180.0), 0.45f*sin(M_PI*(11.0 + i*72.0)/180.0), 0.12f);
				glVertex3f(0.3f*cos(M_PI*(21.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(21.0 + i*72.0)/180.0), 0.1f);
			glEnd();
			glBegin(GL_TRIANGLES);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glVertex3f(0.3f*cos(M_PI*(51.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(51.0 + i*72.0)/180.0), 0.0f);
				glVertex3f(0.3f*cos(M_PI*(51.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(51.0 + i*72.0)/180.0), 0.1f);
			glEnd();
			glBegin(GL_QUADS);
				glVertex3f(0.3f*cos(M_PI*(51.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(51.0 + i*72.0)/180.0), 0.0f);
				glVertex3f(0.45f*cos(M_PI*(61.0 + i*72.0)/180.0), 0.45f*sin(M_PI*(61.0 + i*72.0)/180.0), 0.0f);
				glVertex3f(0.45f*cos(M_PI*(61.0 + i*72.0)/180.0), 0.45f*sin(M_PI*(61.0 + i*72.0)/180.0), 0.12f);
				glVertex3f(0.3f*cos(M_PI*(51.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(51.0 + i*72.0)/180.0), 0.1f);
			glEnd();
			glColor3f(0.95f, 0.95f, 0.95f);
			glBegin(GL_LINE_STRIP);
				glVertex3f(0.06f*cos(M_PI*(34.0 + i*72.0)/180.0), 0.06f*sin(M_PI*(34.0 + i*72.0)/180.0), 0.02f);
				glVertex3f(0.3f*cos(M_PI*(34.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(34.0 + i*72.0)/180.0), 0.11f);
				glVertex3f(0.4f*cos(M_PI*(34.0 + i*72.0)/180.0), 0.4f*sin(M_PI*(34.0 + i*72.0)/180.0), 0.123f);
			glEnd();
			glBegin(GL_LINE_STRIP);
				glVertex3f(0.06f*cos(M_PI*(36.0 + i*72.0)/180.0), 0.06f*sin(M_PI*(36.0 + i*72.0)/180.0), 0.02f);
				glVertex3f(0.3f*cos(M_PI*(36.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(36.0 + i*72.0)/180.0), 0.11f);
				glVertex3f(0.4f*cos(M_PI*(36.0 + i*72.0)/180.0), 0.4f*sin(M_PI*(36.0 + i*72.0)/180.0), 0.123f);
			glEnd();
		}
		glRotatef(wheelsA, 0.0f, 0.0f, 1.0f);
		glColor3f(0.02f, 0.02f, 0.02f);
		glTranslatef(0.0f, 0.0f, 0.12f);
		glRotatef(-wheelsA, 0.0f, 0.0f, 1.0f);
		gluDisk(gluNewQuadric(), 0.45f, 0.6905f, 30, 5); // ����
		glRotatef(wheelsA, 0.0f, 0.0f, 1.0f);
		glColor3f(0.0f, 0.0f, 0.65f);
		for (i = -3; i < 183; i++)
		{
			glEnable(GL_LIGHTING);
			glBegin(GL_QUADS);
				glNormal3f(0, 0, 1);
				glVertex3f(0.71f*cos(M_PI*(i)/180.0), 0.71f*sin(M_PI*(i)/180.0), 0.12f);
				glVertex3f(0.71*cos(M_PI*(i+1)/180.0), 0.71f*sin(M_PI*(i+1)/180.0), 0.12f);
				glVertex3f(0.81f*cos(M_PI*(i+1)/180.0), 0.81f*sin(M_PI*(i+1)/180.0), 0.12f);
				glVertex3f(0.81f*cos(M_PI*(i)/180.0), 0.81f*sin(M_PI*(i)/180.0), 0.12f);
			glEnd();
			glBegin(GL_QUADS);
				glNormal3f(0, 0, 1);
				glVertex3f(0.71f*cos(M_PI*(i)/180.0), 0.71f*sin(M_PI*(i)/180.0), 0.12f);
				glVertex3f(0.71*cos(M_PI*(i+1)/180.0), 0.71f*sin(M_PI*(i+1)/180.0), 0.12f);
				glVertex3f(0.71*cos(M_PI*(i+1)/180.0), 0.71f*sin(M_PI*(i+1)/180.0), -0.32f);
				glVertex3f(0.71f*cos(M_PI*(i)/180.0), 0.71f*sin(M_PI*(i)/180.0), -0.32f);
			glEnd();
			glBegin(GL_QUADS);
				glNormal3f(0, 0, 1);
				glVertex3f(0.81f*cos(M_PI*(i+1)/180.0), 0.81f*sin(M_PI*(i+1)/180.0), 0.12f);
				glVertex3f(0.81f*cos(M_PI*(i)/180.0), 0.81f*sin(M_PI*(i)/180.0), 0.12f);
				glVertex3f(0.96f*cos(M_PI*(i)/180.0), 0.96f*sin(M_PI*(i)/180.0), -0.22f);
				glVertex3f(0.96f*cos(M_PI*(i+1)/180.0), 0.96f*sin(M_PI*(i+1)/180.0), -0.22f);
			glEnd();
			glDisable(GL_LIGHTING);
		}
		glTranslatef(0.0f, 0.0f, -0.3f);
		glTranslatef(-2.84f, 0.6905f, -1.420f);
		//������ �������� �����
		glColor3f(0.02f, 0.02f, 0.02f);
		glTranslatef(2.84f, -0.6905f, -1.420f);
		glRotatef(180.0, 0.0f, 1.0f, 0.0f);
		glRotatef(wheelsA, 0.0f, 0.0f, 1.0f);
		gluCylinder(gluNewQuadric(), 0.6905f, 0.6905f, 0.3f, 20, 20); // �������
		glRotatef(-wheelsA, 0.0f, 0.0f, 1.0f);
		glColor3f(0.8f, 0.8f, 0.8f);
		glTranslatef(0.0f, 0.0f, 0.18f);
		glRotatef(wheelsA, 0.0f, 0.0f, 1.0f);
		gluDisk(gluNewQuadric(), 0.0f, 0.45f, 30, 5); // ����
		glColor3f(0.4f, 0.4f, 0.4f);
		gluCylinder(gluNewQuadric(), 0.45f, 0.45f, 0.12f, 20, 20); // �������
		for (i = 0; i < 5; i++)
		{
			glColor3f(0.4f, 0.4f, 0.4f);
			glBegin(GL_POLYGON);
				glVertex3f(0.3f*cos(M_PI*(21.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(21.0 + i*72.0)/180.0), 0.1f);
				glVertex3f(0.45f*cos(M_PI*(11.0 + i*72.0)/180.0), 0.45f*sin(M_PI*(11.0 + i*72.0)/180.0), 0.12f);
				glVertex3f(0.45f*cos(M_PI*(21.0 + i*72.0)/180.0), 0.45f*sin(M_PI*(21.0 + i*72.0)/180.0), 0.12f);
				glVertex3f(0.45f*cos(M_PI*(31.0 + i*72.0)/180.0), 0.45f*sin(M_PI*(31.0 + i*72.0)/180.0), 0.12f);
				glVertex3f(0.45f*cos(M_PI*(41.0 + i*72.0)/180.0), 0.45f*sin(M_PI*(41.0 + i*72.0)/180.0), 0.12f);
				glVertex3f(0.45f*cos(M_PI*(51.0 + i*72.0)/180.0), 0.45f*sin(M_PI*(51.0 + i*72.0)/180.0), 0.12f);
				glVertex3f(0.45f*cos(M_PI*(61.0 + i*72.0)/180.0), 0.45f*sin(M_PI*(61.0 + i*72.0)/180.0), 0.12f);
				glVertex3f(0.3f*cos(M_PI*(51.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(51.0 + i*72.0)/180.0), 0.1f);
			glEnd();
			glBegin(GL_POLYGON);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glVertex3f(0.3f*cos(M_PI*(21.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(21.0 + i*72.0)/180.0), 0.1f);
				glVertex3f(0.3f*cos(M_PI*(51.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(51.0 + i*72.0)/180.0), 0.1f);
			glEnd();
			glColor3f(0.2f, 0.2f, 0.2f);
			glBegin(GL_TRIANGLES);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glVertex3f(0.3f*cos(M_PI*(21.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(21.0 + i*72.0)/180.0), 0.0f);
				glVertex3f(0.3f*cos(M_PI*(21.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(21.0 + i*72.0)/180.0), 0.1f);
			glEnd();
			glBegin(GL_QUADS);
				glVertex3f(0.3f*cos(M_PI*(21.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(21.0 + i*72.0)/180.0), 0.0f);
				glVertex3f(0.45f*cos(M_PI*(11.0 + i*72.0)/180.0), 0.45f*sin(M_PI*(11.0 + i*72.0)/180.0), 0.0f);
				glVertex3f(0.45f*cos(M_PI*(11.0 + i*72.0)/180.0), 0.45f*sin(M_PI*(11.0 + i*72.0)/180.0), 0.12f);
				glVertex3f(0.3f*cos(M_PI*(21.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(21.0 + i*72.0)/180.0), 0.1f);
			glEnd();
			glBegin(GL_TRIANGLES);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glVertex3f(0.3f*cos(M_PI*(51.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(51.0 + i*72.0)/180.0), 0.0f);
				glVertex3f(0.3f*cos(M_PI*(51.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(51.0 + i*72.0)/180.0), 0.1f);
			glEnd();
			glBegin(GL_QUADS);
				glVertex3f(0.3f*cos(M_PI*(51.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(51.0 + i*72.0)/180.0), 0.0f);
				glVertex3f(0.45f*cos(M_PI*(61.0 + i*72.0)/180.0), 0.45f*sin(M_PI*(61.0 + i*72.0)/180.0), 0.0f);
				glVertex3f(0.45f*cos(M_PI*(61.0 + i*72.0)/180.0), 0.45f*sin(M_PI*(61.0 + i*72.0)/180.0), 0.12f);
				glVertex3f(0.3f*cos(M_PI*(51.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(51.0 + i*72.0)/180.0), 0.1f);
			glEnd();
			glColor3f(0.95f, 0.95f, 0.95f);
			glBegin(GL_LINE_STRIP);
				glVertex3f(0.06f*cos(M_PI*(34.0 + i*72.0)/180.0), 0.06f*sin(M_PI*(34.0 + i*72.0)/180.0), 0.02f);
				glVertex3f(0.3f*cos(M_PI*(34.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(34.0 + i*72.0)/180.0), 0.11f);
				glVertex3f(0.4f*cos(M_PI*(34.0 + i*72.0)/180.0), 0.4f*sin(M_PI*(34.0 + i*72.0)/180.0), 0.123f);
			glEnd();
			glBegin(GL_LINE_STRIP);
				glVertex3f(0.06f*cos(M_PI*(36.0 + i*72.0)/180.0), 0.06f*sin(M_PI*(36.0 + i*72.0)/180.0), 0.02f);
				glVertex3f(0.3f*cos(M_PI*(36.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(36.0 + i*72.0)/180.0), 0.11f);
				glVertex3f(0.4f*cos(M_PI*(36.0 + i*72.0)/180.0), 0.4f*sin(M_PI*(36.0 + i*72.0)/180.0), 0.123f);
			glEnd();
		}
		glRotatef(-wheelsA, 0.0f, 0.0f, 1.0f);
		glColor3f(0.02f, 0.02f, 0.02f);
		glTranslatef(0.0f, 0.0f, 0.12f);
		glRotatef(wheelsA, 0.0f, 0.0f, 1.0f);
		gluDisk(gluNewQuadric(), 0.45f, 0.6905f, 30, 5); // ����
		glRotatef(-wheelsA, 0.0f, 0.0f, 1.0f);
		glColor3f(0.0f, 0.0f, 0.65f);
		for (i = -3; i < 183; i++)
		{
			glEnable(GL_LIGHTING);
			glBegin(GL_QUADS);
				glNormal3f(0, 0, -1);
				glVertex3f(0.71f*cos(M_PI*(i)/180.0), 0.71f*sin(M_PI*(i)/180.0), 0.12f);
				glVertex3f(0.71*cos(M_PI*(i+1)/180.0), 0.71f*sin(M_PI*(i+1)/180.0), 0.12f);
				glVertex3f(0.81f*cos(M_PI*(i+1)/180.0), 0.81f*sin(M_PI*(i+1)/180.0), 0.12f);
				glVertex3f(0.81f*cos(M_PI*(i)/180.0), 0.81f*sin(M_PI*(i)/180.0), 0.12f);
			glEnd();
			glBegin(GL_QUADS);
				glNormal3f(0, 0, -1);
				glVertex3f(0.71f*cos(M_PI*(i)/180.0), 0.71f*sin(M_PI*(i)/180.0), 0.12f);
				glVertex3f(0.71*cos(M_PI*(i+1)/180.0), 0.71f*sin(M_PI*(i+1)/180.0), 0.12f);
				glVertex3f(0.71*cos(M_PI*(i+1)/180.0), 0.71f*sin(M_PI*(i+1)/180.0), -0.32f);
				glVertex3f(0.71f*cos(M_PI*(i)/180.0), 0.71f*sin(M_PI*(i)/180.0), -0.32f);
			glEnd();
			glBegin(GL_QUADS);
				glNormal3f(0, 0, -1);
				glVertex3f(0.81f*cos(M_PI*(i+1)/180.0), 0.81f*sin(M_PI*(i+1)/180.0), 0.12f);
				glVertex3f(0.81f*cos(M_PI*(i)/180.0), 0.81f*sin(M_PI*(i)/180.0), 0.12f);
				glVertex3f(0.96f*cos(M_PI*(i)/180.0), 0.96f*sin(M_PI*(i)/180.0), -0.22f);
				glVertex3f(0.96f*cos(M_PI*(i+1)/180.0), 0.96f*sin(M_PI*(i+1)/180.0), -0.22f);
			glEnd();
			glDisable(GL_LIGHTING);
		}
		glTranslatef(0.0f, 0.0f, -0.3f);
		glRotatef(-180.0, 0.0f, 1.0f, 0.0f);
		glTranslatef(-2.84f, 0.6905f, 1.420f);
		//������ ������ ������
		glColor3f(0.02f, 0.02f, 0.02f);
		glTranslatef(-2.6f, -0.6905f, 1.447f);
		glRotatef(-wheelsA, 0.0f, 0.0f, 1.0f);
		gluCylinder(gluNewQuadric(), 0.6905f, 0.6905f, 0.34f, 20, 20); // �������
		glRotatef(wheelsA, 0.0f, 0.0f, 1.0f);
		glColor3f(0.8f, 0.8f, 0.8f);
		glTranslatef(0.0f, 0.0f, 0.22f);
		glRotatef(-wheelsA, 0.0f, 0.0f, 1.0f);
		gluDisk(gluNewQuadric(), 0.0f, 0.45f, 30, 5); // ����
		glColor3f(0.4f, 0.4f, 0.4f);
		gluCylinder(gluNewQuadric(), 0.45f, 0.45f, 0.12f, 20, 20); // �������
		for (i = 0; i < 5; i++)
		{
			glColor3f(0.4f, 0.4f, 0.4f);
			glBegin(GL_POLYGON);
				glVertex3f(0.3f*cos(M_PI*(21.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(21.0 + i*72.0)/180.0), 0.1f);
				glVertex3f(0.45f*cos(M_PI*(11.0 + i*72.0)/180.0), 0.45f*sin(M_PI*(11.0 + i*72.0)/180.0), 0.12f);
				glVertex3f(0.45f*cos(M_PI*(21.0 + i*72.0)/180.0), 0.45f*sin(M_PI*(21.0 + i*72.0)/180.0), 0.12f);
				glVertex3f(0.45f*cos(M_PI*(31.0 + i*72.0)/180.0), 0.45f*sin(M_PI*(31.0 + i*72.0)/180.0), 0.12f);
				glVertex3f(0.45f*cos(M_PI*(41.0 + i*72.0)/180.0), 0.45f*sin(M_PI*(41.0 + i*72.0)/180.0), 0.12f);
				glVertex3f(0.45f*cos(M_PI*(51.0 + i*72.0)/180.0), 0.45f*sin(M_PI*(51.0 + i*72.0)/180.0), 0.12f);
				glVertex3f(0.45f*cos(M_PI*(61.0 + i*72.0)/180.0), 0.45f*sin(M_PI*(61.0 + i*72.0)/180.0), 0.12f);
				glVertex3f(0.3f*cos(M_PI*(51.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(51.0 + i*72.0)/180.0), 0.1f);
			glEnd();
			glBegin(GL_POLYGON);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glVertex3f(0.3f*cos(M_PI*(21.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(21.0 + i*72.0)/180.0), 0.1f);
				glVertex3f(0.3f*cos(M_PI*(51.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(51.0 + i*72.0)/180.0), 0.1f);
			glEnd();
			glColor3f(0.2f, 0.2f, 0.2f);
			glBegin(GL_TRIANGLES);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glVertex3f(0.3f*cos(M_PI*(21.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(21.0 + i*72.0)/180.0), 0.0f);
				glVertex3f(0.3f*cos(M_PI*(21.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(21.0 + i*72.0)/180.0), 0.1f);
			glEnd();
			glBegin(GL_QUADS);
				glVertex3f(0.3f*cos(M_PI*(21.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(21.0 + i*72.0)/180.0), 0.0f);
				glVertex3f(0.45f*cos(M_PI*(11.0 + i*72.0)/180.0), 0.45f*sin(M_PI*(11.0 + i*72.0)/180.0), 0.0f);
				glVertex3f(0.45f*cos(M_PI*(11.0 + i*72.0)/180.0), 0.45f*sin(M_PI*(11.0 + i*72.0)/180.0), 0.12f);
				glVertex3f(0.3f*cos(M_PI*(21.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(21.0 + i*72.0)/180.0), 0.1f);
			glEnd();
			glBegin(GL_TRIANGLES);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glVertex3f(0.3f*cos(M_PI*(51.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(51.0 + i*72.0)/180.0), 0.0f);
				glVertex3f(0.3f*cos(M_PI*(51.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(51.0 + i*72.0)/180.0), 0.1f);
			glEnd();
			glBegin(GL_QUADS);
				glVertex3f(0.3f*cos(M_PI*(51.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(51.0 + i*72.0)/180.0), 0.0f);
				glVertex3f(0.45f*cos(M_PI*(61.0 + i*72.0)/180.0), 0.45f*sin(M_PI*(61.0 + i*72.0)/180.0), 0.0f);
				glVertex3f(0.45f*cos(M_PI*(61.0 + i*72.0)/180.0), 0.45f*sin(M_PI*(61.0 + i*72.0)/180.0), 0.12f);
				glVertex3f(0.3f*cos(M_PI*(51.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(51.0 + i*72.0)/180.0), 0.1f);
			glEnd();
			glColor3f(0.95f, 0.95f, 0.95f);
			glBegin(GL_LINE_STRIP);
				glVertex3f(0.06f*cos(M_PI*(34.0 + i*72.0)/180.0), 0.06f*sin(M_PI*(34.0 + i*72.0)/180.0), 0.02f);
				glVertex3f(0.3f*cos(M_PI*(34.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(34.0 + i*72.0)/180.0), 0.11f);
				glVertex3f(0.4f*cos(M_PI*(34.0 + i*72.0)/180.0), 0.4f*sin(M_PI*(34.0 + i*72.0)/180.0), 0.123f);
			glEnd();
			glBegin(GL_LINE_STRIP);
				glVertex3f(0.06f*cos(M_PI*(36.0 + i*72.0)/180.0), 0.06f*sin(M_PI*(36.0 + i*72.0)/180.0), 0.02f);
				glVertex3f(0.3f*cos(M_PI*(36.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(36.0 + i*72.0)/180.0), 0.11f);
				glVertex3f(0.4f*cos(M_PI*(36.0 + i*72.0)/180.0), 0.4f*sin(M_PI*(36.0 + i*72.0)/180.0), 0.123f);
			glEnd();
		}
		glRotatef(wheelsA, 0.0f, 0.0f, 1.0f);
		glColor3f(0.02f, 0.02f, 0.02f);
		glTranslatef(0.0f, 0.0f, 0.12f);
		glRotatef(-wheelsA, 0.0f, 0.0f, 1.0f);
		gluDisk(gluNewQuadric(), 0.45f, 0.6905f, 30, 5); // ����
		glRotatef(wheelsA, 0.0f, 0.0f, 1.0f);
		glColor3f(0.0f, 0.0f, 0.65f);
		for (i = -3; i < 183; i++)
		{
			glEnable(GL_LIGHTING);
			glBegin(GL_QUADS);
				glNormal3f(0, 0, 1);
				glVertex3f(0.71f*cos(M_PI*(i)/180.0), 0.71f*sin(M_PI*(i)/180.0), 0.12f);
				glVertex3f(0.71*cos(M_PI*(i+1)/180.0), 0.71f*sin(M_PI*(i+1)/180.0), 0.12f);
				glVertex3f(0.81f*cos(M_PI*(i+1)/180.0), 0.81f*sin(M_PI*(i+1)/180.0), 0.12f);
				glVertex3f(0.81f*cos(M_PI*(i)/180.0), 0.81f*sin(M_PI*(i)/180.0), 0.12f);
			glEnd();
			glBegin(GL_QUADS);
				glNormal3f(0, 0, 1);
				glVertex3f(0.71f*cos(M_PI*(i)/180.0), 0.71f*sin(M_PI*(i)/180.0), 0.12f);
				glVertex3f(0.71*cos(M_PI*(i+1)/180.0), 0.71f*sin(M_PI*(i+1)/180.0), 0.12f);
				glVertex3f(0.71*cos(M_PI*(i+1)/180.0), 0.71f*sin(M_PI*(i+1)/180.0), -0.32f);
				glVertex3f(0.71f*cos(M_PI*(i)/180.0), 0.71f*sin(M_PI*(i)/180.0), -0.32f);
			glEnd();
			glBegin(GL_QUADS);
				glNormal3f(0, 0, 1);
				glVertex3f(0.81f*cos(M_PI*(i+1)/180.0), 0.81f*sin(M_PI*(i+1)/180.0), 0.12f);
				glVertex3f(0.81f*cos(M_PI*(i)/180.0), 0.81f*sin(M_PI*(i)/180.0), 0.12f);
				glVertex3f(1.0f*cos(M_PI*(i)/180.0), 1.0f*sin(M_PI*(i)/180.0), -0.22f);
				glVertex3f(1.0f*cos(M_PI*(i+1)/180.0), 1.0f*sin(M_PI*(i+1)/180.0), -0.22f);
			glEnd();
			glDisable(GL_LIGHTING);
		}
		glTranslatef(0.0f, 0.0f, -0.3f);
		glTranslatef(2.6f, 0.6905f, -1.447f);

		//������ ������ �����
		glColor3f(0.02f, 0.02f, 0.02f);
		glTranslatef(-2.6f, -0.6905f, -1.447f);
		glRotatef(180.0, 0.0f, 1.0f, 0.0f);
		glRotatef(wheelsA, 0.0f, 0.0f, 1.0f);
		gluCylinder(gluNewQuadric(), 0.6905f, 0.6905f, 0.34f, 20, 20); // �������
		glRotatef(-wheelsA, 0.0f, 0.0f, 1.0f);
		glColor3f(0.8f, 0.8f, 0.8f);
		glTranslatef(0.0f, 0.0f, 0.22f);
		glRotatef(wheelsA, 0.0f, 0.0f, 1.0f);
		gluDisk(gluNewQuadric(), 0.0f, 0.45f, 30, 5); // ����
		glColor3f(0.4f, 0.4f, 0.4f);
		gluCylinder(gluNewQuadric(), 0.45f, 0.45f, 0.12f, 20, 20); // �������
		for (i = 0; i < 5; i++)
		{
			glColor3f(0.4f, 0.4f, 0.4f);
			glBegin(GL_POLYGON);
				glVertex3f(0.3f*cos(M_PI*(21.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(21.0 + i*72.0)/180.0), 0.1f);
				glVertex3f(0.45f*cos(M_PI*(11.0 + i*72.0)/180.0), 0.45f*sin(M_PI*(11.0 + i*72.0)/180.0), 0.12f);
				glVertex3f(0.45f*cos(M_PI*(21.0 + i*72.0)/180.0), 0.45f*sin(M_PI*(21.0 + i*72.0)/180.0), 0.12f);
				glVertex3f(0.45f*cos(M_PI*(31.0 + i*72.0)/180.0), 0.45f*sin(M_PI*(31.0 + i*72.0)/180.0), 0.12f);
				glVertex3f(0.45f*cos(M_PI*(41.0 + i*72.0)/180.0), 0.45f*sin(M_PI*(41.0 + i*72.0)/180.0), 0.12f);
				glVertex3f(0.45f*cos(M_PI*(51.0 + i*72.0)/180.0), 0.45f*sin(M_PI*(51.0 + i*72.0)/180.0), 0.12f);
				glVertex3f(0.45f*cos(M_PI*(61.0 + i*72.0)/180.0), 0.45f*sin(M_PI*(61.0 + i*72.0)/180.0), 0.12f);
				glVertex3f(0.3f*cos(M_PI*(51.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(51.0 + i*72.0)/180.0), 0.1f);
			glEnd();
			glBegin(GL_POLYGON);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glVertex3f(0.3f*cos(M_PI*(21.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(21.0 + i*72.0)/180.0), 0.1f);
				glVertex3f(0.3f*cos(M_PI*(51.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(51.0 + i*72.0)/180.0), 0.1f);
			glEnd();
			glColor3f(0.2f, 0.2f, 0.2f);
			glBegin(GL_TRIANGLES);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glVertex3f(0.3f*cos(M_PI*(21.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(21.0 + i*72.0)/180.0), 0.0f);
				glVertex3f(0.3f*cos(M_PI*(21.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(21.0 + i*72.0)/180.0), 0.1f);
			glEnd();
			glBegin(GL_QUADS);
				glVertex3f(0.3f*cos(M_PI*(21.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(21.0 + i*72.0)/180.0), 0.0f);
				glVertex3f(0.45f*cos(M_PI*(11.0 + i*72.0)/180.0), 0.45f*sin(M_PI*(11.0 + i*72.0)/180.0), 0.0f);
				glVertex3f(0.45f*cos(M_PI*(11.0 + i*72.0)/180.0), 0.45f*sin(M_PI*(11.0 + i*72.0)/180.0), 0.12f);
				glVertex3f(0.3f*cos(M_PI*(21.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(21.0 + i*72.0)/180.0), 0.1f);
			glEnd();
			glBegin(GL_TRIANGLES);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glVertex3f(0.3f*cos(M_PI*(51.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(51.0 + i*72.0)/180.0), 0.0f);
				glVertex3f(0.3f*cos(M_PI*(51.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(51.0 + i*72.0)/180.0), 0.1f);
			glEnd();
			glBegin(GL_QUADS);
				glVertex3f(0.3f*cos(M_PI*(51.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(51.0 + i*72.0)/180.0), 0.0f);
				glVertex3f(0.45f*cos(M_PI*(61.0 + i*72.0)/180.0), 0.45f*sin(M_PI*(61.0 + i*72.0)/180.0), 0.0f);
				glVertex3f(0.45f*cos(M_PI*(61.0 + i*72.0)/180.0), 0.45f*sin(M_PI*(61.0 + i*72.0)/180.0), 0.12f);
				glVertex3f(0.3f*cos(M_PI*(51.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(51.0 + i*72.0)/180.0), 0.1f);
			glEnd();
			glColor3f(0.95f, 0.95f, 0.95f);
			glBegin(GL_LINE_STRIP);
				glVertex3f(0.06f*cos(M_PI*(34.0 + i*72.0)/180.0), 0.06f*sin(M_PI*(34.0 + i*72.0)/180.0), 0.02f);
				glVertex3f(0.3f*cos(M_PI*(34.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(34.0 + i*72.0)/180.0), 0.11f);
				glVertex3f(0.4f*cos(M_PI*(34.0 + i*72.0)/180.0), 0.4f*sin(M_PI*(34.0 + i*72.0)/180.0), 0.123f);
			glEnd();
			glBegin(GL_LINE_STRIP);
				glVertex3f(0.06f*cos(M_PI*(36.0 + i*72.0)/180.0), 0.06f*sin(M_PI*(36.0 + i*72.0)/180.0), 0.02f);
				glVertex3f(0.3f*cos(M_PI*(36.0 + i*72.0)/180.0), 0.3f*sin(M_PI*(36.0 + i*72.0)/180.0), 0.11f);
				glVertex3f(0.4f*cos(M_PI*(36.0 + i*72.0)/180.0), 0.4f*sin(M_PI*(36.0 + i*72.0)/180.0), 0.123f);
			glEnd();
		}
		glRotatef(-wheelsA, 0.0f, 0.0f, 1.0f);
		glColor3f(0.02f, 0.02f, 0.02f);
		glTranslatef(0.0f, 0.0f, 0.12f);
		glRotatef(wheelsA, 0.0f, 0.0f, 1.0f);
		gluDisk(gluNewQuadric(), 0.45f, 0.6905f, 30, 5); // ����
		glRotatef(-wheelsA, 0.0f, 0.0f, 1.0f);
		glColor3f(0.0f, 0.0f, 0.65f);
		//glTranslatef(0.0f, 0.0f, 0.005f);
		for (i = -3; i < 183; i++)
		{
			glEnable(GL_LIGHTING);
			glBegin(GL_QUADS);
				glNormal3f(0, 0, -1);
				glVertex3f(0.71f*cos(M_PI*(i)/180.0), 0.71f*sin(M_PI*(i)/180.0), 0.13f);
				glVertex3f(0.71*cos(M_PI*(i+1)/180.0), 0.71f*sin(M_PI*(i+1)/180.0), 0.13f);
				glVertex3f(0.81f*cos(M_PI*(i+1)/180.0), 0.81f*sin(M_PI*(i+1)/180.0), 0.13f);
				glVertex3f(0.81f*cos(M_PI*(i)/180.0), 0.81f*sin(M_PI*(i)/180.0), 0.13f);
			glEnd();
			glBegin(GL_QUADS);
				glNormal3f(0, 0, -1);
				glVertex3f(0.71f*cos(M_PI*(i)/180.0), 0.71f*sin(M_PI*(i)/180.0), 0.13f);
				glVertex3f(0.71*cos(M_PI*(i+1)/180.0), 0.71f*sin(M_PI*(i+1)/180.0), 0.13f);
				glVertex3f(0.71*cos(M_PI*(i+1)/180.0), 0.71f*sin(M_PI*(i+1)/180.0), -0.32f);
				glVertex3f(0.71f*cos(M_PI*(i)/180.0), 0.71f*sin(M_PI*(i)/180.0), -0.32f);
			glEnd();
			glBegin(GL_QUADS);
				glNormal3f(0, 0, -1);
				glVertex3f(0.81f*cos(M_PI*(i+1)/180.0), 0.81f*sin(M_PI*(i+1)/180.0), 0.13f);
				glVertex3f(0.81f*cos(M_PI*(i)/180.0), 0.81f*sin(M_PI*(i)/180.0), 0.13f);
				glVertex3f(1.0f*cos(M_PI*(i)/180.0), 1.0f*sin(M_PI*(i)/180.0), -0.22f);
				glVertex3f(1.0f*cos(M_PI*(i+1)/180.0), 1.0f*sin(M_PI*(i+1)/180.0), -0.22f);
			glEnd();
			glDisable(GL_LIGHTING);
		}
		//glTranslatef(0.0f, 0.0f, -0.005f);
		glTranslatef(0.0f, 0.0f, -0.3f);
		glRotatef(-180.0, 0.0f, 1.0f, 0.0f);
		glTranslatef(2.6f, 0.6905f, 1.447f);
		glEnable(GL_LIGHTING);

		//������ ���
		glColor3f(0.0f, 0.0f, 0.65f);
		glTranslatef(-1.87f, -0.92f, 1.87f);
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(0.0f, 0.0f, -0.15f);
			glVertex3f(4.0f, 0.0f, -0.15f);
			glVertex3f(4.0f, 0.2f, 0.0f);
			glVertex3f(-0.02f, 0.2f, 0.0f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(0.0f, 0.0f, -0.15f);
			glVertex3f(4.0f, 0.0f, -0.15f);
			glVertex3f(4.0f, 0.0f, -0.45f);
			glVertex3f(0.0f, 0.0f, -0.45f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(4.0f, 0.2f, 0.0f);
			glVertex3f(0.0f, 0.2f, 0.0f);
			glVertex3f(0.0f, 0.2f, -0.1f);
			glVertex3f(4.0f, 0.2f, -0.1f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(4.0f, 0.0f, -0.15f);
			glVertex3f(4.0f, 0.2f, 0.0f);
			glVertex3f(4.0f, 0.2f, -0.45f);
			glVertex3f(4.0f, 0.0f, -0.45f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(0.0f, 0.0f, -0.15f);
			glVertex3f(-0.02f, 0.2f, 0.0f);
			glVertex3f(-0.02f, 0.2f, -0.45f);
			glVertex3f(0.0f, 0.0f, -0.45f);
		glEnd();
		//�����
		glTranslatef(3.3f, 0.0f, 0.0f);
		glRotatef(doorsA, 0.0f, 1.0f, 0.0f);
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(0.75f - 3.3f, 0.2f, -0.05f);
			glVertex3f(3.2f - 3.3f, 0.2f, -0.05f);
			glVertex3f(3.28 - 3.3f, 0.5f, 0.05f);
			glVertex3f(0.65 - 3.3f, 0.5f, 0.05f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(0.75f - 3.3f, 0.2f, -0.05f);
			glVertex3f(3.2f - 3.3f, 0.2f, -0.05f);
			glVertex3f(3.2f - 3.3f, 0.2f, -0.15f);
			glVertex3f(0.75f - 3.3f, 0.2f, -0.15f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(3.28f - 3.3f, 0.5f, 0.05f);
			glVertex3f(0.65f - 3.3f, 0.5f, 0.05f);
			glVertex3f(0.65f - 3.3f, 0.5f, -0.15f);
			glVertex3f(3.28f - 3.3f, 0.5f, -0.15f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(3.28f - 3.3f, 0.5f, -0.05f);
			glVertex3f(0.65f - 3.3f, 0.5f, -0.05f);
			glVertex3f(0.57f - 3.3f, 1.05f, -0.05f);
			glVertex3f(3.3f - 3.3f, 1.05f, -0.05f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(0.57f - 3.3f, 1.05f, -0.05f);
			glVertex3f(3.3f - 3.3f, 1.05f, -0.05f);
			glVertex3f(3.3f - 3.3f, 1.05f, -0.15f);
			glVertex3f(0.57f - 3.3f, 1.05f, -0.15f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(0.57f - 3.3f, 1.05f, 0.0f);
			glVertex3f(3.3f - 3.3f, 1.05f, 0.0f);
			glVertex3f(3.25f - 3.3f, 1.3f, -0.1f);
			glVertex3f(0.62f - 3.3f, 1.3f, -0.1f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(0.57f - 3.3f, 1.05f, 0.0f);
			glVertex3f(3.3f - 3.3f, 1.05f, 0.0f);
			glVertex3f(3.3f - 3.3f, 1.05f, -0.15f);
			glVertex3f(0.57f - 3.3f, 1.05f, -0.15f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(3.25f - 3.3f, 1.3f, -0.1f);
			glVertex3f(0.62f - 3.3f, 1.3f, -0.1f);
			glVertex3f(0.62f - 3.3f, 1.3f, -0.35f);
			glVertex3f(3.25f - 3.3f, 1.3f, -0.35f);
		glEnd();
		glColor4f(0.05f, 0.05f, 0.05f, 0.8f);
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(3.25f - 3.3f, 1.3f, -0.15f);
			glVertex3f(0.62f - 3.3f, 1.3f, -0.15f);
			glVertex3f(0.52f - 3.3f, 1.95f, -0.4f);
			glVertex3f(1.1f - 3.3f, 2.02f, -0.41f);
			glVertex3f(1.6f - 3.3f, 2.05f, -0.41f);
			glVertex3f(2.0f - 3.3f, 2.00f, -0.42f);
		glEnd();
		//�������
		glColor3f(0.1f, 0.1f, 0.1f);
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(3.25f - 3.3f, 1.3f, -0.145f);
			glVertex3f(2.7f - 3.3f, 1.3f, -0.145f);
			glVertex3f(2.7f - 3.3f, 1.6f, -0.24f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(3.25f - 3.3f, 1.3f, -0.145f);
			glVertex3f(2.7f - 3.3f, 1.6f, -0.24f);
			glVertex3f(2.7f - 3.3f, 1.61f, -0.28f);
		glEnd();
		glColor3f(0.0f, 0.0f, 0.65f);
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(2.9f - 3.3f, 1.32f, -0.165f);
			glVertex3f(2.7f - 3.3f, 1.32f, 0.2f);
			glVertex3f(2.7f - 3.3f, 1.5f, 0.15f);
			glVertex3f(2.9f - 3.3f, 1.55f, -0.165f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(2.9f - 3.3f, 1.32f, -0.165f);
			glVertex3f(2.8f - 3.3f, 1.32f, -0.165f);
			glVertex3f(2.6f - 3.3f, 1.32f, 0.2f);
			glVertex3f(2.7f - 3.3f, 1.32f, 0.2f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(2.7f - 3.3f, 1.5f, 0.15f);
			glVertex3f(2.9f - 3.3f, 1.55f, -0.165f);
			glVertex3f(2.8f - 3.3f, 1.55f, -0.165f);
			glVertex3f(2.6f - 3.3f, 1.5f, 0.15f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(2.7f - 3.3f, 1.32f, 0.2f);
			glVertex3f(2.7f - 3.3f, 1.5f, 0.15f);
			glVertex3f(2.6f - 3.3f, 1.5f, 0.15f);
			glVertex3f(2.6f - 3.3f, 1.32f, 0.2f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(2.9f - 3.3f, 1.32f, -0.165f);
			glVertex3f(2.9f - 3.3f, 1.55f, -0.165f);
			glVertex3f(2.8f - 3.3f, 1.55f, -0.165f);
			glVertex3f(2.8f - 3.3f, 1.32f, -0.165f);
		glEnd();
		//�������
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(2.83f - 3.3f, 1.32f, -0.165f);
			glVertex3f(2.63f - 3.3f, 1.32f, 0.2f);
			glVertex3f(2.63f - 3.3f, 1.5f, 0.15f);
			glVertex3f(2.83f - 3.3f, 1.55f, -0.165f);
		glEnd();
		//�����
		glColor3f(0.02f, 0.02f, 0.02f);
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(1.3f - 3.3f, 1.2f, -0.02f);
			glVertex3f(0.9f - 3.3f, 1.2f, -0.02f);
			glVertex3f(0.9f - 3.3f, 1.1f, -0.02f);
			glVertex3f(1.3f - 3.3f, 1.1f, -0.02f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(1.3f - 3.3f, 1.2f, -0.02f);
			glVertex3f(0.9f - 3.3f, 1.2f, -0.02f);
			glVertex3f(0.9f - 3.3f, 1.2f, -0.1f);
			glVertex3f(1.3f - 3.3f, 1.2f, -0.1f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(0.9f - 3.3f, 1.2f, -0.02f);
			glVertex3f(0.9f - 3.3f, 1.1f, -0.02f);
			glVertex3f(0.9f - 3.3f, 1.1f, -0.1f);
			glVertex3f(0.9f - 3.3f, 1.2f, -0.1f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(1.3f - 3.3f, 1.2f, -0.02f);
			glVertex3f(1.3f - 3.3f, 1.1f, -0.02f);
			glVertex3f(1.3f - 3.3f, 1.1f, -0.1f);
			glVertex3f(1.3f - 3.3f, 1.2f, -0.1f);
		glEnd();
		glRotatef(-doorsA, 0.0f, 1.0f, 0.0f);
		//������ �����
		glColor3f(0.0f, 0.0f, 0.65f);
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(-0.1f, 0.2f, -0.05f);
			glVertex3f(0.6f, 0.2f, -0.05f);
			glVertex3f(0.65f, 0.5f, -0.05f);
			glVertex3f(-0.02f, 0.5f, 0.05f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(0.65f, 0.5f, -0.05f);
			glVertex3f(-0.02f, 0.5f, 0.05f);
			glVertex3f(-0.02f, 0.5f, -0.15f);
			glVertex3f(0.65f, 0.5f, -0.25f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(-0.02f, 0.5f, -0.05f);
			glVertex3f(0.65f, 0.5f, -0.05f);
			glVertex3f(0.9f, 0.8f, -0.05f);
			glVertex3f(1.35f, 1.05f, -0.05f);
			glVertex3f(0.0f, 1.05f, -0.05f);
		glEnd();

		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(1.35f, 1.05f, -0.05f);
			glVertex3f(0.0f, 1.05f, 0.0f);
			glVertex3f(0.0f, 1.05f, -0.15f);
			glVertex3f(1.35f, 1.05f, -0.15f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(1.5f, 1.05f, -0.06f);
			glVertex3f(0.0f, 1.05f, 0.0f);
			glVertex3f(-0.05f, 1.3f, -0.1f);
			glVertex3f(1.5f, 1.2f, -0.2f);//������ ����� �������!!!!!!!!
		glEnd();

		//������ �����
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(0.75f - 3.3f, 0.2f, -0.05f);
			glVertex3f(0.65 - 3.3f, 0.5f, 0.05f);
			glVertex3f(0.05f - 3.3f, 0.5f, 0.0f);
			glVertex3f(0.1f - 3.3f, 0.2f, -0.05f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(0.65 - 3.3f, 0.5f, 0.05f);
			glVertex3f(0.05f - 3.3f, 0.5f, 0.0f);
			glVertex3f(0.05f - 3.3f, 0.5f, -0.2f);
			glVertex3f(0.65 - 3.3f, 0.5f, -0.15f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(0.05f - 3.3f, 0.5f, 0.0f);
			glVertex3f(0.65f - 3.3f, 0.5f, 0.05f);
			glVertex3f(0.57f - 3.3f, 1.05f, 0.0f);
			glVertex3f(-0.6f - 3.3f, 1.05f, 0.0f);
		glEnd();
		glColor3f(0.02f, 0.02f, 0.02f);
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(0.65f - 3.3f, 0.5f, 0.05f);
			glVertex3f(0.57f - 3.3f, 1.05f, 0.0f);
			glVertex3f(0.57f - 3.3f, 1.05f, -0.2f);
			glVertex3f(0.65f - 3.3f, 0.5f, -0.15f);
		glEnd();
		glColor3f(0.0f, 0.0f, 0.65f);
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(0.57f - 3.3f, 1.05f, 0.0f);
			glVertex3f(-1.9f - 3.3f, 1.05f, 0.0f);
			glVertex3f(-1.9f - 3.3f, 1.3f, -0.2f);
			glVertex3f(0.62f - 3.3f, 1.3f, -0.1f);
		glEnd();
		glTranslatef(-3.3f, 0.0f, 0.0f);

		glTranslatef(1.87f, 0.92f, -1.87f);

		//����� ���
		glColor3f(0.0f, 0.0f, 0.65f);
		glTranslatef(-1.87f, -0.92f, -1.87f);
		//glRotatef(-180.0, 0.0f, 1.0f, 0.0f);
		//���
		glBegin(GL_POLYGON);
			glNormal3f(0, 1, 0);
			glVertex3f(0.0f, 0.0f, 0.15f);
			glVertex3f(4.0f, 0.0f, 0.15f);
			glVertex3f(4.0f, 0.0f, 3.59f);
			glVertex3f(0.0f, 0.0f, 3.59f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 1, 0);
			glVertex3f(0.0f, 0.0f, 0.45f);
			glVertex3f(0.0f, 0.0f, 3.29f);
			glVertex3f(-1.5f, 0.08f, 3.29f);
			glVertex3f(-1.5f, 0.08f, 0.45f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 1, 0);
			glVertex3f(4.0f, 0.0f, 0.45f);
			glVertex3f(4.0f, 0.0f, 3.29f);
			glVertex3f(5.0f, 0.2f, 3.29f);
			glVertex3f(5.0f, 0.2f, 0.45f);
		glEnd();
		//����� ��� (�����������)
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(0.0f, 0.0f, 0.15f);
			glVertex3f(4.0f, 0.0f, 0.15f);
			glVertex3f(4.0f, 0.2f, -0.0f);
			glVertex3f(-0.02f, 0.2f, -0.0f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(0.0f, 0.0f, 0.15f);
			glVertex3f(4.0f, 0.0f, 0.15f);
			glVertex3f(4.0f, 0.0f, 0.45f);
			glVertex3f(0.0f, 0.0f, 0.45f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(4.0f, 0.2f, -0.0f);
			glVertex3f(0.0f, 0.2f, -0.0f);
			glVertex3f(0.0f, 0.2f, 0.1f);
			glVertex3f(4.0f, 0.2f, 0.1f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(4.0f, 0.0f, 0.15f);
			glVertex3f(4.0f, 0.2f, -0.0f);
			glVertex3f(4.0f, 0.2f, 0.45f);
			glVertex3f(4.0f, 0.0f, 0.45f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(0.0f, 0.0f, 0.15f);
			glVertex3f(-0.02f, 0.2f, -0.0f);
			glVertex3f(-0.02f, 0.2f, 0.45f);
			glVertex3f(0.0f, 0.0f, 0.45f);
		glEnd();
		//�����
		glTranslatef(3.3f, 0.0f, 0.0f);
		glRotatef(-doorsA, 0.0f, 1.0f, 0.0f);
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(0.75f - 3.3f, 0.2f, 0.05f);
			glVertex3f(3.2f - 3.3f, 0.2f, 0.05f);
			glVertex3f(3.28 - 3.3f, 0.5f, -0.05f);
			glVertex3f(0.65 - 3.3f, 0.5f, -0.05f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(0.75f - 3.3f, 0.2f, 0.05f);
			glVertex3f(3.2f - 3.3f, 0.2f, 0.05f);
			glVertex3f(3.2f - 3.3f, 0.2f, 0.15f);
			glVertex3f(0.75f - 3.3f, 0.2f, 0.15f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(3.28f - 3.3f, 0.5f, -0.05f);
			glVertex3f(0.65f - 3.3f, 0.5f, -0.05f);
			glVertex3f(0.65f - 3.3f, 0.5f, 0.15f);
			glVertex3f(3.28f - 3.3f, 0.5f, 0.15f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(3.28f - 3.3f, 0.5f, 0.05f);
			glVertex3f(0.65f - 3.3f, 0.5f, 0.05f);
			glVertex3f(0.57f - 3.3f, 1.05f, 0.05f);
			glVertex3f(3.3f - 3.3f, 1.05f, 0.05f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(0.57f - 3.3f, 1.05f, 0.05f);
			glVertex3f(3.3f - 3.3f, 1.05f, 0.05f);
			glVertex3f(3.3f - 3.3f, 1.05f, 0.15f);
			glVertex3f(0.57f - 3.3f, 1.05f, 0.15f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(0.57f - 3.3f, 1.05f, -0.0f);
			glVertex3f(3.3f - 3.3f, 1.05f, -0.0f);
			glVertex3f(3.25f - 3.3f, 1.3f, 0.1f);
			glVertex3f(0.62f - 3.3f, 1.3f, 0.1f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(0.57f - 3.3f, 1.05f, -0.0f);
			glVertex3f(3.3f - 3.3f, 1.05f, -0.0f);
			glVertex3f(3.3f - 3.3f, 1.05f, 0.15f);
			glVertex3f(0.57f - 3.3f, 1.05f, 0.15f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(3.25f - 3.3f, 1.3f, 0.1f);
			glVertex3f(0.62f - 3.3f, 1.3f, 0.1f);
			glVertex3f(0.62f - 3.3f, 1.3f, 0.35f);
			glVertex3f(3.25f - 3.3f, 1.3f, 0.35f);
		glEnd();
		glColor4f(0.05f, 0.05f, 0.05f, 0.8f);
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(3.25f - 3.3f, 1.3f, 0.15f);
			glVertex3f(0.62f - 3.3f, 1.3f, 0.15f);
			glVertex3f(0.52f - 3.3f, 1.95f, 0.4f);
			glVertex3f(1.1f - 3.3f, 2.02f, 0.41f);
			glVertex3f(1.6f - 3.3f, 2.05f, 0.41f);
			glVertex3f(2.0f - 3.3f, 2.00f, 0.42f);
		glEnd();
		//�������
		glColor3f(0.1f, 0.1f, 0.1f);
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(3.25f - 3.3f, 1.3f, 0.145f);
			glVertex3f(2.7f - 3.3f, 1.3f, 0.145f);
			glVertex3f(2.7f - 3.3f, 1.6f, 0.24f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(3.25f - 3.3f, 1.3f, 0.145f);
			glVertex3f(2.7f - 3.3f, 1.6f, 0.24f);
			glVertex3f(2.7f - 3.3f, 1.61f, 0.28f);
		glEnd();
		glColor3f(0.0f, 0.0f, 0.65f);
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(2.9f - 3.3f, 1.32f, 0.165f);
			glVertex3f(2.7f - 3.3f, 1.32f, -0.2f);
			glVertex3f(2.7f - 3.3f, 1.5f, -0.15f);
			glVertex3f(2.9f - 3.3f, 1.55f, 0.165f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(2.9f - 3.3f, 1.32f, 0.165f);
			glVertex3f(2.8f - 3.3f, 1.32f, 0.165f);
			glVertex3f(2.6f - 3.3f, 1.32f, -0.2f);
			glVertex3f(2.7f - 3.3f, 1.32f, -0.2f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(2.7f - 3.3f, 1.5f, -0.15f);
			glVertex3f(2.9f - 3.3f, 1.55f, 0.165f);
			glVertex3f(2.8f - 3.3f, 1.55f, 0.165f);
			glVertex3f(2.6f - 3.3f, 1.5f, -0.15f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(2.7f - 3.3f, 1.32f, -0.2f);
			glVertex3f(2.7f - 3.3f, 1.5f, -0.15f);
			glVertex3f(2.6f - 3.3f, 1.5f, -0.15f);
			glVertex3f(2.6f - 3.3f, 1.32f, -0.2f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(2.9f - 3.3f, 1.32f, 0.165f);
			glVertex3f(2.9f - 3.3f, 1.55f, 0.165f);
			glVertex3f(2.8f - 3.3f, 1.55f, 0.165f);
			glVertex3f(2.8f - 3.3f, 1.32f, 0.165f);
		glEnd();
		//�������
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(2.83f - 3.3f, 1.32f, 0.165f);
			glVertex3f(2.63f - 3.3f, 1.32f, -0.2f);
			glVertex3f(2.63f - 3.3f, 1.5f, -0.15f);
			glVertex3f(2.83f - 3.3f, 1.55f, 0.165f);
		glEnd();
		//�����
		glColor3f(0.02f, 0.02f, 0.02f);
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(1.3f - 3.3f, 1.2f, 0.02f);
			glVertex3f(0.9f - 3.3f, 1.2f, 0.02f);
			glVertex3f(0.9f - 3.3f, 1.1f, 0.02f);
			glVertex3f(1.3f - 3.3f, 1.1f, 0.02f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(1.3f - 3.3f, 1.2f, 0.02f);
			glVertex3f(0.9f - 3.3f, 1.2f, 0.02f);
			glVertex3f(0.9f - 3.3f, 1.2f, 0.1f);
			glVertex3f(1.3f - 3.3f, 1.2f, 0.1f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(0.9f - 3.3f, 1.2f, 0.02f);
			glVertex3f(0.9f - 3.3f, 1.1f, 0.02f);
			glVertex3f(0.9f - 3.3f, 1.1f, 0.1f);
			glVertex3f(0.9f - 3.3f, 1.2f, 0.1f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(1.3f - 3.3f, 1.2f, 0.02f);
			glVertex3f(1.3f - 3.3f, 1.1f, 0.02f);
			glVertex3f(1.3f - 3.3f, 1.1f, 0.1f);
			glVertex3f(1.3f - 3.3f, 1.2f, 0.1f);
		glEnd();
		glRotatef(doorsA, 0.0f, 1.0f, 0.0f);
		//����� �����
		glColor3f(0.0f, 0.0f, 0.65f);
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(-0.1f, 0.2f, 0.05f);
			glVertex3f(0.6f, 0.2f, 0.05f);
			glVertex3f(0.65f, 0.5f, 0.05f);
			glVertex3f(-0.02f, 0.5f, -0.05f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(0.65f, 0.5f, 0.05f);
			glVertex3f(-0.02f, 0.5f, -0.05f);
			glVertex3f(-0.02f, 0.5f, 0.15f);
			glVertex3f(0.65f, 0.5f, 0.25f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(-0.02f, 0.5f, 0.05f);
			glVertex3f(0.65f, 0.5f, 0.05f);
			glVertex3f(0.9f, 0.8f, 0.05f);
			glVertex3f(1.35f, 1.05f, 0.05f);
			glVertex3f(0.0f, 1.05f, 0.05f);
		glEnd();

		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(1.35f, 1.05f, 0.05f);
			glVertex3f(0.0f, 1.05f, -0.0f);
			glVertex3f(0.0f, 1.05f, 0.15f);
			glVertex3f(1.35f, 1.05f, 0.15f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(1.5f, 1.05f, 0.06f);
			glVertex3f(0.0f, 1.05f, -0.0f);
			glVertex3f(-0.05f, 1.3f, 0.1f);
			glVertex3f(1.5f, 1.2f, 0.2f);
		glEnd();

		//����� �����
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(0.75f - 3.3f, 0.2f, 0.05f);
			glVertex3f(0.65 - 3.3f, 0.5f, -0.05f);
			glVertex3f(0.05f - 3.3f, 0.5f, -0.0f);
			glVertex3f(0.1f - 3.3f, 0.2f, 0.05f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(0.65 - 3.3f, 0.5f, -0.05f);
			glVertex3f(0.05f - 3.3f, 0.5f, -0.0f);
			glVertex3f(0.05f - 3.3f, 0.5f, 0.2f);
			glVertex3f(0.65 - 3.3f, 0.5f, 0.15f);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(0.05f - 3.3f, 0.5f, -0.0f);
			glVertex3f(0.65f - 3.3f, 0.5f, -0.05f);
			glVertex3f(0.57f - 3.3f, 1.05f, -0.0f);
			glVertex3f(-0.6f - 3.3f, 1.05f, -0.0f);
		glEnd();
		glColor3f(0.02f, 0.02f, 0.02f);
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(0.65f - 3.3f, 0.5f, -0.05f);
			glVertex3f(0.57f - 3.3f, 1.05f, -0.0f);
			glVertex3f(0.57f - 3.3f, 1.05f, 0.2f);
			glVertex3f(0.65f - 3.3f, 0.5f, 0.15f);
		glEnd();
		glColor3f(0.0f, 0.0f, 0.65f);
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(0.57f - 3.3f, 1.05f, -0.0f);
			glVertex3f(-1.9f - 3.3f, 1.05f, -0.0f);
			glVertex3f(-1.9f - 3.3f, 1.3f, 0.2f);
			glVertex3f(0.62f - 3.3f, 1.3f, 0.1f);
		glEnd();
		glTranslatef(-3.3f, 0.0f, 0.0f);

		glTranslatef(1.87f, 0.92f, 1.87f);
		//�����
		glTranslatef(-3.784f, 0.0f, 0.0f);

		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(-0.95f, -0.4f, -1.2f);
			glVertex3f(-1.0f, 0.0f, -1.2f);
			glVertex3f(-1.0f, 0.0f, 1.2f);
			glVertex3f(-0.95f, -0.4f, 1.2f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-0.95f, -0.4f, -1.2f);
			glVertex3f(-0.95f, -0.4f, 1.2f);
			glVertex3f(-1.0f, -0.6f, 1.2f);
			glVertex3f(-1.0f, -0.6f, -1.2f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-1.0f, -0.6f, 1.2f);
			glVertex3f(-1.0f, -0.6f, -1.2f);
			glVertex3f(-0.6f, -0.6f, -1.2f);
			glVertex3f(-0.6f, -0.6f, 1.2f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-0.95f, -0.4f, 1.2f);
			glVertex3f(-1.0f, -0.6f, 1.2f);
			glVertex3f(-0.5f, -0.65f, 1.72f);
			glVertex3f(-0.5f, -0.45f, 1.8f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-1.0f, -0.6f, 1.2f);
			glVertex3f(-0.5f, -0.65f, 1.72f);
			glVertex3f(-0.1f, -0.65f, 1.72f);
			glVertex3f(-0.6f, -0.6f, 1.2f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-0.95f, -0.4f, -1.2f);
			glVertex3f(-1.0f, -0.6f, -1.2f);
			glVertex3f(-0.5f, -0.65f, -1.72f);
			glVertex3f(-0.5f, -0.45f, -1.8f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-1.0f, -0.6f, -1.2f);
			glVertex3f(-0.5f, -0.65f, -1.72f);
			glVertex3f(-0.1f, -0.65f, -1.72f);
			glVertex3f(-0.6f, -0.6f, -1.2f);
		glEnd();

		glColor3f(0.0f, 0.0f, 0.65f);
		glBegin(GL_POLYGON);
			glVertex3f(0.48f, -0.85f, 1.7f);
			glVertex3f(-0.5f, -0.8f, 1.65f);
			glVertex3f(-0.5f, -0.45f, 1.8f);
			glVertex3f(0.48f, -0.5f, 1.8f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-0.5f, -0.8f, 1.65f);
			glVertex3f(-0.5f, -0.45f, 1.8f);
			glVertex3f(-0.5f, -0.45f, 1.5f);
			glVertex3f(-0.5f, -0.8f, 1.4f);
		glEnd();
		
		glBegin(GL_POLYGON);
			glVertex3f(0.48f, -0.85f, -1.7f);
			glVertex3f(-0.5f, -0.8f, -1.65f);
			glVertex3f(-0.5f, -0.45f, -1.8f);
			glVertex3f(0.48f, -0.5f, -1.8f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-0.5f, -0.8f, -1.65f);
			glVertex3f(-0.5f, -0.45f, -1.8f);
			glVertex3f(-0.5f, -0.45f, -1.5f);
			glVertex3f(-0.5f, -0.8f, -1.4f);
		glEnd();

		//glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_POLYGON);
			glVertex3f(-0.5f, -0.8f, 1.5f);
			glVertex3f(-0.5f, -0.45f, 1.6f);
			glVertex3f(-0.9f, -0.45f, 1.2f);
			glVertex3f(-0.8f, -0.8f, 1.2f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-0.5f, -0.8f, -1.5f);
			glVertex3f(-0.5f, -0.45f, -1.6f);
			glVertex3f(-0.9f, -0.45f, -1.2f);
			glVertex3f(-0.8f, -0.8f, -1.2f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-0.9f, -0.45f, 1.2f);
			glVertex3f(-0.8f, -0.8f, 1.2f);
			glVertex3f(-0.8f, -0.8f, -1.2f);
			glVertex3f(-0.9f, -0.45f, -1.2f);
		glEnd();

		//�������� ��������� �����
		glColor3f(0.02f, 0.02f, 0.02f);
		glTranslatef(-0.7f, -0.7f, 1.0f);
		glRotatef(270.0, 0.0f, 1.0f, 0.0f);
		gluCylinder(gluNewQuadric(), 0.09f, 0.09f, 0.25f, 20, 20); // �������
		glRotatef(-270.0, 0.0f, 1.0f, 0.0f);
		glTranslatef(-0.2f, 0.0f, 0.0f);
		glRotatef(270.0, 0.0f, 1.0f, 0.0f);
		gluDisk(gluNewQuadric(), 0.0f, 0.09, 30, 5); // ����
		glRotatef(-270.0, 0.0f, 1.0f, 0.0f);
		glTranslatef(0.2f, 0.0f, 0.0f);
		glTranslatef(0.7f, 0.7f, -1.0f);
		//glColor3f(0.0f, 0.0f, 0.8f);

		glTranslatef(-0.7f, -0.7f, -1.0f);
		glRotatef(270.0, 0.0f, 1.0f, 0.0f);
		gluCylinder(gluNewQuadric(), 0.09f, 0.09f, 0.25f, 20, 20); // �������
		glRotatef(-270.0, 0.0f, 1.0f, 0.0f);
		glTranslatef(-0.2f, 0.0f, 0.0f);
		glRotatef(270.0, 0.0f, 1.0f, 0.0f);
		gluDisk(gluNewQuadric(), 0.0f, 0.09, 30, 5); // ����
		glRotatef(-270.0, 0.0f, 1.0f, 0.0f);
		glTranslatef(0.2f, 0.0f, 0.0f);
		glTranslatef(0.7f, 0.7f, 1.0f);
		
		glColor3f(0.0f, 0.0f, 0.65f);

		//���
		glBegin(GL_POLYGON);
			glVertex3f(-0.5f, -0.8f, 1.5f);
			glVertex3f(-0.8f, -0.8f, 1.2f);
			glVertex3f(-0.4f, -0.8f, 1.2f);
			glVertex3f(-0.1f, -0.8f, 1.5f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-0.5f, -0.8f, -1.5f);
			glVertex3f(-0.8f, -0.8f, -1.2f);
			glVertex3f(-0.4f, -0.8f, -1.2f);
			glVertex3f(-0.1f, -0.8f, -1.5f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-0.8f, -0.8f, 1.2f);
			glVertex3f(-0.8f, -0.8f, -1.2f);
			glVertex3f(-0.4f, -0.8f, -1.2f);
			glVertex3f(-0.4f, -0.8f, 1.2f);
		glEnd();
		
		glBegin(GL_POLYGON);
			glVertex3f(0.48f, -0.85f, 1.7f);
			glVertex3f(-0.5f, -0.8f, 1.65f);
			glVertex3f(-0.5f, -0.8f, -1.65f);
			glVertex3f(0.48f, -0.85f, -1.7f);
		glEnd();

		//������ ���������� ������
		
		glBegin(GL_POLYGON);
			glVertex3f(-0.9f, 0.1f, -1.2f);
			glVertex3f(-1.0f, 0.0f, -1.2f);
			glVertex3f(-1.0f, 0.0f, 1.2f);
			glVertex3f(-0.9f, 0.1f, 1.2f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-0.9f, 0.1f, -1.2f);
			glVertex3f(-0.9f, 0.1f, 1.2f);
			glVertex3f(-0.1f, 0.1f, 1.2f);
			glVertex3f(-0.1f, 0.1f, -1.2f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-1.0f, 0.0f, 1.2f);
			glVertex3f(-0.9f, 0.1f, 1.2f);
			glVertex3f(-0.5f, 0.2f, 1.6f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-1.0f, 0.0f, -1.2f);
			glVertex3f(-0.9f, 0.1f, -1.2f);
			glVertex3f(-0.5f, 0.2f, -1.6f);
		glEnd();
		
		//������ ������
		glColor4f(0.7f, 0.0f, 0.0f, 0.8f);
		glBegin(GL_POLYGON);
			glVertex3f(-0.9f, 0.1f, 0.7f);
			glVertex3f(-0.9f, 0.1f, 1.2f);
			glVertex3f(-0.75f, 0.45f, 1.2f);
			glVertex3f(-0.75f, 0.45f, 0.7f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-0.9f, 0.1f, 1.2f);
			glVertex3f(-0.74f, 0.14f, 1.36f);
			glVertex3f(-0.64f, 0.45f, 1.36f);
			glVertex3f(-0.75f, 0.45f, 1.2f);
		glEnd();

		glColor3f(0.02f, 0.02f, 0.02);
		glBegin(GL_POLYGON);
			glVertex3f(-0.9f, 0.1f, 0.7f);
			glVertex3f(-0.75f, 0.45f, 0.7f);
			glVertex3f(-0.6f, 0.45f, 0.7f);
			glVertex3f(-0.6f, 0.1f, 0.7f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-0.6f, 0.45f, 0.7f);
			glVertex3f(-0.6f, 0.1f, 0.7f);
			glVertex3f(-0.6f, 0.14f, 1.36f);
			glVertex3f(-0.6f, 0.45f, 1.36f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-0.74f, 0.14f, 1.36f);
			glVertex3f(-0.64f, 0.45f, 1.36f);
			glVertex3f(-0.6f, 0.14f, 1.36f);
			glVertex3f(-0.6f, 0.45f, 1.36f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-0.6f, 0.45f, 0.7f);
			glVertex3f(-0.6f, 0.45f, 1.36f);
			glVertex3f(-0.64f, 0.45f, 1.36f);
			glVertex3f(-0.75f, 0.45f, 1.2f);
			glVertex3f(-0.75f, 0.45f, 0.7f);
		glEnd();

		glColor3f(0.95f, 0.95f, 0.95f);
		glBegin(GL_LINE_STRIP);
			glVertex3f(-0.9f, 0.1f, 1.205f);
			glVertex3f(-0.755f, 0.45f, 1.205f);
		glEnd();
		glBegin(GL_LINE_STRIP);
			glVertex3f(-0.905f, 0.1f, 1.195f);
			glVertex3f(-0.755f, 0.45f, 1.195f);
		glEnd();
		glBegin(GL_LINE_STRIP);
			glVertex3f(-0.905f, 0.1f, 0.955f);
			glVertex3f(-0.755f, 0.45f, 0.955f);
		glEnd();
		glBegin(GL_LINE_STRIP);
			glVertex3f(-0.905f, 0.1f, 0.945f);
			glVertex3f(-0.755f, 0.45f, 0.945f);
		glEnd();
		glBegin(GL_LINE_STRIP);
			glVertex3f(-0.905f, 0.105f, 0.705f);
			glVertex3f(-0.905f, 0.105f, 1.2f);
			glVertex3f(-0.745f, 0.145f, 1.355f);
			glVertex3f(-0.65f, 0.445f, 1.355f);
			glVertex3f(-0.755f, 0.445f, 1.2f);
			glVertex3f(-0.755f, 0.445f, 0.705f);
			glVertex3f(-0.905f, 0.105f, 0.705f);
		glEnd();
		//����� ������
		glColor4f(0.7f, 0.0f, 0.0f, 0.8f);
		glBegin(GL_POLYGON);
			glVertex3f(-0.9f, 0.1f, -0.7f);
			glVertex3f(-0.9f, 0.1f, -1.2f);
			glVertex3f(-0.75f, 0.45f, -1.2f);
			glVertex3f(-0.75f, 0.45f, -0.7f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-0.9f, 0.1f, -1.2f);
			glVertex3f(-0.74f, 0.14f, -1.36f);
			glVertex3f(-0.64f, 0.45f, -1.36f);
			glVertex3f(-0.75f, 0.45f, -1.2f);
		glEnd();

		glColor3f(0.02f, 0.02f, 0.02);
		glBegin(GL_POLYGON);
			glVertex3f(-0.9f, 0.1f, -0.7f);
			glVertex3f(-0.75f, 0.45f, -0.7f);
			glVertex3f(-0.6f, 0.45f, -0.7f);
			glVertex3f(-0.6f, 0.1f, -0.7f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-0.6f, 0.45f, -0.7f);
			glVertex3f(-0.6f, 0.1f, -0.7f);
			glVertex3f(-0.6f, 0.14f, -1.36f);
			glVertex3f(-0.6f, 0.45f, -1.36f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-0.74f, 0.14f, -1.36f);
			glVertex3f(-0.64f, 0.45f, -1.36f);
			glVertex3f(-0.6f, 0.14f, -1.36f);
			glVertex3f(-0.6f, 0.45f, -1.36f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-0.6f, 0.45f, -0.7f);
			glVertex3f(-0.6f, 0.45f, -1.36f);
			glVertex3f(-0.64f, 0.45f, -1.36f);
			glVertex3f(-0.75f, 0.45f, -1.2f);
			glVertex3f(-0.75f, 0.45f, -0.7f);
		glEnd();
		glColor3f(0.95f, 0.95f, 0.95f);
		glBegin(GL_LINE_STRIP);
			glVertex3f(-0.9f, 0.1f, -1.205f);
			glVertex3f(-0.755f, 0.45f, -1.205f);
		glEnd();
		glBegin(GL_LINE_STRIP);
			glVertex3f(-0.905f, 0.1f, -1.195f);
			glVertex3f(-0.755f, 0.45f, -1.195f);
		glEnd();
		glBegin(GL_LINE_STRIP);
			glVertex3f(-0.905f, 0.1f, -0.955f);
			glVertex3f(-0.755f, 0.45f, -0.955f);
		glEnd();
		glBegin(GL_LINE_STRIP);
			glVertex3f(-0.905f, 0.1f, -0.945f);
			glVertex3f(-0.755f, 0.45f, -0.945f);
		glEnd();
		glBegin(GL_LINE_STRIP);
			glVertex3f(-0.905f, 0.105f, -0.705f);
			glVertex3f(-0.905f, 0.105f, -1.2f);
			glVertex3f(-0.745f, 0.145f, -1.355f);
			glVertex3f(-0.65f, 0.445f, -1.355f);
			glVertex3f(-0.755f, 0.445f, -1.2f);
			glVertex3f(-0.755f, 0.445f, -0.705f);
			glVertex3f(-0.905f, 0.105f, -0.705f);
		glEnd();

		glColor3f(0.0f, 0.0f, 0.65f);
		glBegin(GL_POLYGON);
			glVertex3f(-0.74f, 0.14f, 1.36f);
			glVertex3f(-0.64f, 0.45f, 1.36f);
			glVertex3f(-0.54f, 0.45f, 1.46f);
			glVertex3f(-0.5f, 0.2f, 1.6f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-0.54f, 0.45f, 1.46f);
			glVertex3f(-0.5f, 0.2f, 1.6f);
			glVertex3f(0.02f, 0.13f, 1.87f);
			glVertex3f(0.02f, 0.38f, 1.67f);
		glEnd();

		glBegin(GL_POLYGON);
			glVertex3f(-0.74f, 0.14f, -1.36f);
			glVertex3f(-0.64f, 0.45f, -1.36f);
			glVertex3f(-0.54f, 0.45f, -1.46f);
			glVertex3f(-0.5f, 0.2f, -1.6f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-0.54f, 0.45f, -1.46f);
			glVertex3f(-0.5f, 0.2f, -1.6f);
			glVertex3f(0.02f, 0.13f, -1.87f);
			glVertex3f(0.02f, 0.38f, -1.67f);
		glEnd();

		glBegin(GL_POLYGON);
			glVertex3f(-1.0f, 0.0f, 1.2f);
			glVertex3f(-0.95f, -0.4f, 1.2f);
			glVertex3f(-0.5f, -0.45f, 1.8f);
			glVertex3f(-0.5f, 0.2f, 1.6f);
		glEnd();
		glBegin(GL_TRIANGLES);
			glVertex3f(-0.5f, -0.45f, 1.8f);
			glVertex3f(-0.5f, 0.2f, 1.6f);
			glVertex3f(0.02f, 0.13f, 1.87f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-0.5f, -0.45f, 1.8f);
			glVertex3f(0.02f, 0.13f, 1.87f);
			glVertex3f(1.3f, 0.13f, 1.87f);
			glVertex3f(0.8f, 0.11f, 1.87f);
			glVertex3f(0.48f, -0.5f, 1.8f);
		glEnd();

		glBegin(GL_POLYGON);
			glVertex3f(-1.0f, 0.0f, -1.2f);
			glVertex3f(-0.95f, -0.4f, -1.2f);
			glVertex3f(-0.5f, -0.45f, -1.8f);
			glVertex3f(-0.5f, 0.2f, -1.6f);
		glEnd();
		glBegin(GL_TRIANGLES);
			glVertex3f(-0.5f, -0.45f, -1.8f);
			glVertex3f(-0.5f, 0.2f, -1.6f);
			glVertex3f(0.02f, 0.13f, -1.87f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-0.5f, -0.45f, -1.8f);
			glVertex3f(0.02f, 0.13f, -1.87f);
			glVertex3f(1.3f, 0.13f, -1.87f);
			glVertex3f(0.8f, 0.11f, -1.87f);
			glVertex3f(0.48f, -0.5f, -1.8f);
		glEnd();

		//��������
		glColor3f(0.8f, 0.8f, 0.8f);
		glTranslatef(0.5f, 0.6f, 0.0f);
		glRotatef(-trunkA, 0.0f, 0.0f, 1.0f);
		glBegin(GL_POLYGON);
			glVertex3f(-1.1f, -0.05f, 0.8f);
			glVertex3f(-0.1f, 0.0f, 0.8f);
			glVertex3f(-0.1f, 0.0f, -0.8f);
			glVertex3f(-1.1f, -0.05f, -0.8f);
		glEnd();
		
		glColor3f(0.0f, 0.0f, 0.65f);
		glBegin(GL_POLYGON);
			glVertex3f(-1.1f, -0.05f, 0.8f);
			glVertex3f(-0.1f, 0.0f, 0.8f);
			glVertex3f(-0.0f, -0.05f, 1.4f);
			glVertex3f(-1.0f, -0.05f, 1.3f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-1.1f, -0.05f, -0.8f);
			glVertex3f(-0.1f, 0.0f, -0.8f);
			glVertex3f(-0.0f, -0.05f, -1.4f);
			glVertex3f(-1.0f, -0.05f, -1.3f);
		glEnd();

		glBegin(GL_POLYGON);
			glVertex3f(-1.1f, -0.05f, 0.8f);
			glVertex3f(-1.0f, -0.05f, 1.3f);
			glVertex3f(-1.25f, -0.15f, 1.2f);
			glVertex3f(-1.25f, -0.15f, 0.7f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-1.1f, -0.05f, -0.8f);
			glVertex3f(-1.0f, -0.05f, -1.3f);
			glVertex3f(-1.25f, -0.15f, -1.2f);
			glVertex3f(-1.25f, -0.15f, -0.7f);
		glEnd();
		
		glColor3f(0.8f, 0.8f, 0.8f);
		glBegin(GL_POLYGON);
			glVertex3f(-1.1f, -0.05f, 0.8f);
			glVertex3f(-1.1f, -0.05f, -0.8f);
			glVertex3f(-1.25f, -0.15f, -0.7f);
			glVertex3f(-1.25f, -0.15f, 0.7f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-1.18f, -0.15f, -0.7f);
			glVertex3f(-1.18f, -0.15f, 0.7f);
			glVertex3f(-1.33f, -0.5f, 0.7f);
			glVertex3f(-1.33f, -0.5f, -0.7f);
		glEnd();
		glColor3f(0.02f, 0.02f, 0.02f);
		glBegin(GL_POLYGON);
			glVertex3f(-1.25f, -0.15f, -0.7f);
			glVertex3f(-1.25f, -0.15f, 0.7f);
			glVertex3f(-0.1f, -0.15f, 0.7f);
			glVertex3f(-0.1f, -0.15f, -0.7f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-0.1f, -0.15f, 0.7f);
			glVertex3f(-0.1f, -0.15f, -0.7f);
			glVertex3f(-0.1f, 0.0f, -0.8f);
			glVertex3f(-0.1f, 0.0f, 0.8f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-0.1f, -0.15f, 0.7f);
			glVertex3f(-0.1f, 0.0f, 0.8f);
			glVertex3f(-0.0f, -0.05f, 1.4f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-0.1f, -0.15f, 0.7f);
			glVertex3f(-0.0f, -0.05f, 1.4f);
			glVertex3f(-1.25f, -0.15f, 1.2f);
			glVertex3f(-1.25f, -0.15f, 0.7f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-0.0f, -0.05f, 1.4f);
			glVertex3f(-1.25f, -0.15f, 1.2f);
			glVertex3f(-1.0f, -0.05f, 1.3f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-0.1f, -0.15f, -0.7f);
			glVertex3f(-0.1f, 0.0f, -0.8f);
			glVertex3f(-0.0f, -0.05f, -1.4f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-0.1f, -0.15f, -0.7f);
			glVertex3f(-0.0f, -0.05f, -1.4f);
			glVertex3f(-1.25f, -0.15f, -1.2f);
			glVertex3f(-1.25f, -0.15f, -0.7f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-0.0f, -0.05f, -1.4f);
			glVertex3f(-1.25f, -0.15f, -1.2f);
			glVertex3f(-1.0f, -0.05f, -1.3f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-1.15f, -0.15f, -0.7f);
			glVertex3f(-1.15f, -0.15f, 0.7f);
			glVertex3f(-1.3f, -0.5f, 0.7f);
			glVertex3f(-1.3f, -0.5f, -0.7f);
		glEnd();
		glRotatef(trunkA, 0.0f, 0.0f, 1.0f);
		glColor3f(0.0f, 0.0f, 0.65f);
		glBegin(GL_POLYGON);
			glVertex3f(-0.0f, -0.05f, 1.4f);
			glVertex3f(-1.0f, -0.05f, 1.3f);
			glVertex3f(-1.04f, -0.15f, 1.46f);
			glVertex3f(-0.48f, -0.22f, 1.67f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-1.0f, -0.05f, 1.3f);
			glVertex3f(-1.04f, -0.15f, 1.46f);
			glVertex3f(-1.25f, -0.15f, 1.2f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-0.0f, -0.05f, -1.4f);
			glVertex3f(-1.0f, -0.05f, -1.3f);
			glVertex3f(-1.04f, -0.15f, -1.46f);
			glVertex3f(-0.48f, -0.22f, -1.67f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-1.0f, -0.05f, -1.3f);
			glVertex3f(-1.04f, -0.15f, -1.46f);
			glVertex3f(-1.25f, -0.15f, -1.2f);
		glEnd();

		//����
		//������ ����
		
		glColor3f(0.8f, 0.8f, 0.8f);
		glBegin(GL_POLYGON);
			glVertex3f(0.0f, 0.05f, 0.8f);
			glVertex3f(0.0f, 0.05f, -0.8f);
			glVertex3f(-0.1f, 0.0f, -0.8f);
			glVertex3f(-0.1f, 0.0f, 0.8f);
		glEnd();
		glColor3f(0.0f, 0.0f, 0.65f);
		glBegin(GL_POLYGON);
			glVertex3f(0.0f, 0.05f, 0.8f);
			glVertex3f(0.1f, 0.05f, 1.1f);
			glVertex3f(-0.0f, -0.05f, 1.4f);
			glVertex3f(-0.1f, 0.0f, 0.8f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(0.1f, 0.05f, 1.1f);
			glVertex3f(0.3f, 0.1f, 1.3f);
			glVertex3f(-0.0f, -0.05f, 1.4f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(0.0f, 0.05f, -0.8f);
			glVertex3f(0.1f, 0.05f, -1.1f);
			glVertex3f(-0.0f, -0.05f, -1.4f);
			glVertex3f(-0.1f, 0.0f, -0.8f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(0.1f, 0.05f, -1.1f);
			glVertex3f(0.3f, 0.1f, -1.3f);
			glVertex3f(-0.0f, -0.05f, -1.4f);
		glEnd();

		//���� ����
		
		glColor3f(0.8f, 0.8f, 0.8f);
		glBegin(GL_POLYGON);
			glVertex3f(1.5f, 0.55f, -0.8f);
			glVertex3f(1.5f, 0.55f, 0.8f);
			glVertex3f(2.9f, 0.7f, 0.8f);
			glVertex3f(2.9f, 0.7f, -0.8f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(1.5f, 0.55f, -0.8f);
			glVertex3f(1.5f, 0.55f, 0.8f);
			glVertex3f(1.3f, 0.5f, 0.8f);
			glVertex3f(1.3f, 0.5f, -0.8f);
		glEnd();
		glColor3f(0.0f, 0.0f, 0.65f);
		glBegin(GL_POLYGON);
			glVertex3f(1.5f, 0.55f, 0.8f);
			glVertex3f(2.9f, 0.7f, 0.8f);
			glVertex3f(2.9f, 0.65f, 1.3f);
			glVertex3f(1.55f, 0.5f, 1.4f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(1.5f, 0.55f, 0.8f);
			glVertex3f(1.55f, 0.5f, 1.4f);
			glVertex3f(1.3f, 0.45f, 1.3f);
			glVertex3f(1.3f, 0.5f, 0.8f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(1.5f, 0.55f, -0.8f);
			glVertex3f(2.9f, 0.7f, -0.8f);
			glVertex3f(2.9f, 0.65f, -1.3f);
			glVertex3f(1.55f, 0.5f, -1.4f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(1.5f, 0.55f, -0.8f);
			glVertex3f(1.55f, 0.5f, -1.4f);
			glVertex3f(1.3f, 0.45f, -1.3f);
			glVertex3f(1.3f, 0.5f, -0.8f);
		glEnd();
		
		glColor3f(0.8f, 0.8f, 0.8f);
		glBegin(GL_POLYGON);
			glVertex3f(2.9f, 0.7f, 0.8f);
			glVertex3f(2.9f, 0.7f, -0.8f);
			glVertex3f(3.52f, 0.6f, -0.8f);
			glVertex3f(3.52f, 0.6f, 0.8f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(3.52f, 0.6f, -0.8f);
			glVertex3f(3.52f, 0.6f, 0.8f);
			glVertex3f(3.6f, 0.55f, 0.8f);
			glVertex3f(3.6f, 0.55f, -0.8f);
		glEnd();
		glColor3f(0.0f, 0.0f, 0.65f);
		glBegin(GL_POLYGON);
			glVertex3f(2.9f, 0.7f, 0.8f);
			glVertex3f(3.52f, 0.6f, 0.8f);
			glVertex3f(3.52f, 0.55f, 1.4f);
			glVertex3f(2.9f, 0.65f, 1.3f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(3.52f, 0.6f, 0.8f);
			glVertex3f(3.52f, 0.55f, 1.4f);
			glVertex3f(3.6f, 0.5f, 1.35f);
			glVertex3f(3.6f, 0.55f, 0.8f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(2.9f, 0.7f, -0.8f);
			glVertex3f(3.52f, 0.6f, -0.8f);
			glVertex3f(3.52f, 0.55f, -1.4f);
			glVertex3f(2.9f, 0.65f, -1.3f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(3.52f, 0.6f, -0.8f);
			glVertex3f(3.52f, 0.55f, -1.4f);
			glVertex3f(3.6f, 0.5f, -1.35f);
			glVertex3f(3.6f, 0.55f, -0.8f);
		glEnd();
		//glColor3f(0.8f, 0.0f, 0.7f);
		//right side
		glBegin(GL_POLYGON);
			glVertex3f(0.5f, -0.1f, 1.55f);
			glVertex3f(0.3f, 0.1f, 1.3f);
			glVertex3f(-0.0f, -0.05f, 1.4f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(1.5f, 0.3f, 1.5f);
			glVertex3f(0.5f, -0.1f, 1.55f);
			glVertex3f(0.3f, 0.1f, 1.3f);
			glVertex3f(1.3f, 0.45f, 1.3f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(1.5f, 0.3f, 1.5f);
			glVertex3f(1.3f, 0.45f, 1.3f);
			glVertex3f(1.55f, 0.5f, 1.4f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(1.5f, 0.3f, 1.5f);
			glVertex3f(1.55f, 0.5f, 1.4f);
			glVertex3f(2.05f, -0.25f, 1.76f);
			glVertex3f(1.6f, -0.1f, 1.65f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(1.935f, 0.43f, 1.47f);
			glVertex3f(1.55f, 0.5f, 1.4f);
			glVertex3f(2.05f, -0.25f, 1.76f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(1.935f, 0.43f, 1.47f);
			glVertex3f(1.55f, 0.5f, 1.4f);
			glVertex3f(2.9f, 0.65f, 1.3f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(1.935f, 0.43f, 1.47f);
			glVertex3f(2.9f, 0.65f, 1.3f);
			glVertex3f(3.0f, 0.53f, 1.46f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(3.52f, 0.55f, 1.4f);
			glVertex3f(2.9f, 0.65f, 1.3f);
			glVertex3f(3.0f, 0.53f, 1.46f);
			glVertex3f(3.4f, 0.48f, 1.46f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(3.52f, 0.55f, 1.4f);
			glVertex3f(3.4f, 0.48f, 1.46f);
			glVertex3f(4.67f, -0.23f, 1.72f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(3.6f, 0.5f, 1.35f);
			glVertex3f(3.52f, 0.55f, 1.4f);
			glVertex3f(4.67f, -0.23f, 1.72f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(4.5f, -0.05f, 1.5f);
			glVertex3f(3.6f, 0.5f, 1.35f);
			glVertex3f(4.67f, -0.23f, 1.72f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(4.5f, -0.05f, 1.5f);
			glVertex3f(4.6f, -0.1f, 1.35f);
			glVertex3f(4.67f, -0.23f, 1.72f);
		glEnd();

		glBegin(GL_POLYGON);
			glVertex3f(1.935f, 0.43f, 1.4f);
			glVertex3f(1.935f, 0.43f, 1.47f);
			glVertex3f(2.05f, -0.25f, 1.76f);
			glVertex3f(2.05f, -0.25f, 1.5f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(0.5f, -0.1f, 1.55f);
			glVertex3f(0.5f, -0.25f, 1.7f);
			glVertex3f(-0.6f, -0.25f, 1.66f);
			glVertex3f(-0.0f, -0.05f, 1.4f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(0.5f, -0.1f, 1.55f);
			glVertex3f(0.5f, -0.25f, 1.7f);
			glVertex3f(2.05f, -0.25f, 1.76f);
			glVertex3f(1.6f, -0.1f, 1.65f);
		glEnd();

		//left side
		glBegin(GL_POLYGON);
			glVertex3f(0.5f, -0.1f, -1.55f);
			glVertex3f(0.3f, 0.1f, -1.3f);
			glVertex3f(-0.0f, -0.05f, -1.4f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(1.5f, 0.3f, -1.5f);
			glVertex3f(0.5f, -0.1f, -1.55f);
			glVertex3f(0.3f, 0.1f, -1.3f);
			glVertex3f(1.3f, 0.45f, -1.3f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(1.5f, 0.3f, -1.5f);
			glVertex3f(1.3f, 0.45f, -1.3f);
			glVertex3f(1.55f, 0.5f, -1.4f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(1.5f, 0.3f, -1.5f);
			glVertex3f(1.55f, 0.5f, -1.4f);
			glVertex3f(2.05f, -0.25f, -1.76f);
			glVertex3f(1.6f, -0.1f, -1.65f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(1.935f, 0.43f, -1.47f);
			glVertex3f(1.55f, 0.5f, -1.4f);
			glVertex3f(2.05f, -0.25f, -1.76f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(1.935f, 0.43f, -1.47f);
			glVertex3f(1.55f, 0.5f, -1.4f);
			glVertex3f(2.9f, 0.65f, -1.3f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(1.935f, 0.43f, -1.47f);
			glVertex3f(2.9f, 0.65f, -1.3f);
			glVertex3f(3.0f, 0.53f, -1.46f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(3.52f, 0.55f, -1.4f);
			glVertex3f(2.9f, 0.65f, -1.3f);
			glVertex3f(3.0f, 0.53f, -1.46f);
			glVertex3f(3.4f, 0.48f, -1.46f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(3.52f, 0.55f, -1.4f);
			glVertex3f(3.4f, 0.48f, -1.46f);
			glVertex3f(4.67f, -0.23f, -1.72f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(3.6f, 0.5f, -1.35f);
			glVertex3f(3.52f, 0.55f, -1.4f);
			glVertex3f(4.67f, -0.23f, -1.72f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(4.5f, -0.05f, -1.5f);
			glVertex3f(3.6f, 0.5f, -1.35f);
			glVertex3f(4.67f, -0.23f, -1.72f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(4.5f, -0.05f, -1.5f);
			glVertex3f(4.6f, -0.1f, -1.35f);
			glVertex3f(4.67f, -0.23f, -1.72f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(1.935f, 0.43f, -1.4f);
			glVertex3f(1.935f, 0.43f, -1.47f);
			glVertex3f(2.05f, -0.25f, -1.76f);
			glVertex3f(2.05f, -0.25f, -1.5f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(0.5f, -0.1f, -1.55f);
			glVertex3f(0.5f, -0.25f, -1.7f);
			glVertex3f(-0.6f, -0.25f, -1.66f);
			glVertex3f(-0.0f, -0.05f, -1.4f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(0.5f, -0.1f, -1.55f);
			glVertex3f(0.5f, -0.25f, -1.7f);
			glVertex3f(2.05f, -0.25f, -1.76f);
			glVertex3f(1.6f, -0.1f, -1.65f);
		glEnd();

		//front ������ ������
		/*glBegin(GL_POLYGON);
			glVertex3f(3.6f, 0.55f, 0.8f);
			glVertex3f(4.7f, -0.1f, 0.8f);
			glVertex3f(4.6f, -0.1f, 1.35f);
			glVertex3f(4.5f, -0.05f, 1.5f);
			glVertex3f(3.6f, 0.5f, 1.35f);
		glEnd();*/

		//������ ������
		glColor4f(0.05f, 0.05f, 0.05f, 0.8f);
		glBegin(GL_POLYGON);
			glVertex3f(0.0f, 0.05f, 0.8f);
			glVertex3f(0.0f, 0.05f, -0.8f);
			glVertex3f(1.3f, 0.5f, -0.8f);
			glVertex3f(1.3f, 0.5f, 0.8f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(0.0f, 0.05f, 0.8f);
			glVertex3f(1.3f, 0.5f, 0.8f);
			glVertex3f(1.3f, 0.45f, 1.3f);
			glVertex3f(0.3f, 0.1f, 1.3f);
			glVertex3f(0.1f, 0.05f, 1.1f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(0.0f, 0.05f, -0.8f);
			glVertex3f(1.3f, 0.5f, -0.8f);
			glVertex3f(1.3f, 0.45f, -1.3f);
			glVertex3f(0.3f, 0.1f, -1.3f);
			glVertex3f(0.1f, 0.05f, -1.1f);
		glEnd();

		//������ ������ ������
		glBegin(GL_POLYGON);
			glVertex3f(1.5f, 0.3f, 1.5f);
			glVertex3f(0.5f, -0.1f, 1.55f);
			glVertex3f(1.6f, -0.1f, 1.65f);
		glEnd();

		//������ ����e ������
		glBegin(GL_POLYGON);
			glVertex3f(1.5f, 0.3f, -1.5f);
			glVertex3f(0.5f, -0.1f, -1.55f);
			glVertex3f(1.6f, -0.1f, -1.65f);
		glEnd();

		//�������� ������
		glBegin(GL_POLYGON);
			glVertex3f(3.6f, 0.55f, -0.8f);
			glVertex3f(3.6f, 0.55f, 0.8f);
			glVertex3f(4.7f, -0.1f, 0.8f);
			glVertex3f(4.7f, -0.1f, -0.8f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(3.6f, 0.55f, 0.8f);
			glVertex3f(4.7f, -0.1f, 0.8f);
			glVertex3f(4.6f, -0.1f, 1.35f);
			glVertex3f(4.5f, -0.05f, 1.5f);
			glVertex3f(3.6f, 0.5f, 1.35f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(3.6f, 0.55f, -0.8f);
			glVertex3f(4.7f, -0.1f, -0.8f);
			glVertex3f(4.6f, -0.1f, -1.35f);
			glVertex3f(4.5f, -0.05f, -1.5f);
			glVertex3f(3.6f, 0.5f, -1.35f);
		glEnd();

		
		glColor3f(0.8f, 0.8f, 0.8f);
		glTranslatef(4.85f, -0.1f, 0.0f);
		glBegin(GL_POLYGON);
			glVertex3f(-0.15f, 0.0f, 0.8f);
			glVertex3f(-0.15f, 0.0f, -0.8f);
			glVertex3f(0.0f, -0.1f, -0.8f);
			glVertex3f(0.0f, -0.1f, 0.8f);
		glEnd();
		glColor3f(0.0f, 0.0f, 0.65f);
		glBegin(GL_POLYGON);
			glVertex3f(-0.15f, 0.0f, 0.8f);
			glVertex3f(0.0f, -0.1f, 0.8f);
			glVertex3f(-0.18f, -0.13f, 1.72f);
			glVertex3f(-0.25f, 0.0f, 1.35f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-0.15f, 0.0f, -0.8f);
			glVertex3f(0.0f, -0.1f, -0.8f);
			glVertex3f(-0.18f, -0.13f, -1.72f);
			glVertex3f(-0.25f, 0.0f, -1.35f);
		glEnd();

		//�����
		glRotatef(hoodA, 0.0f, 0.0f, 1.0f);
		glColor3f(0.8f, 0.8f, 0.8f);
		glBegin(GL_POLYGON);
			glVertex3f(0.0f, -0.1f, -0.8f);
			glVertex3f(0.0f, -0.1f, 0.8f);
			glVertex3f(0.05f, 0.05f, 0.8f);
			glVertex3f(0.05f, 0.05f, -0.8f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(0.05f, 0.05f, 0.8f);
			glVertex3f(0.05f, 0.05f, -0.8f);
			glVertex3f(2.0f, -0.05f, -0.5f);
			glVertex3f(2.0f, -0.05f, 0.5f);
		glEnd();
		glColor3f(0.07f, 0.07f, 0.07f);
		glBegin(GL_POLYGON);
			glVertex3f(2.0f, 0.0f -0.05f, -0.5f);
			glVertex3f(2.0f, 0.0f -0.05f, 0.5f);
			glVertex3f(2.2f, -0.15f -0.05f, 0.45f);
			glVertex3f(2.2f, -0.15f -0.05f, -0.45f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(2.2f, -0.05f -0.05f, -0.45f);
			glVertex3f(2.2f, -0.05f -0.05f, 0.45f);
			glVertex3f(2.4f, -0.2f -0.05f, 0.4f);
			glVertex3f(2.4f, -0.2f -0.05f, -0.4f);
		glEnd();
		glColor3f(0.02f, 0.02f, 0.02f);
		glBegin(GL_POLYGON);
			glVertex3f(2.2f, -0.15f -0.05f, 0.45f);
			glVertex3f(2.2f, -0.15f -0.05f, -0.45f);
			glVertex3f(2.2f, -0.05f -0.05f, -0.45f);
			glVertex3f(2.2f, -0.05f -0.05f, 0.45f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(2.4f, -0.2f -0.05f, 0.4f);
			glVertex3f(2.4f, -0.2f -0.05f, -0.4f);
			glVertex3f(2.4f, -0.1f -0.05f, -0.4f);
			glVertex3f(2.4f, -0.1f -0.05f, 0.4f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(2.0f, 0.0f -0.05f, 0.5f);
			glVertex3f(2.4f, -0.1f -0.05f, 0.4f);
			glVertex3f(2.4f, -0.2f -0.05f, 0.4f);
			glVertex3f(2.0f, -0.15f -0.05f, 0.5f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(2.0f, 0.0f -0.05f, -0.5f);
			glVertex3f(2.4f, -0.1f -0.05f, -0.4f);
			glVertex3f(2.4f, -0.2f -0.05f, -0.4f);
			glVertex3f(2.0f, -0.15f -0.05f, -0.5f);
		glEnd();
		glColor3f(0.8f, 0.8f, 0.8f);
		glBegin(GL_POLYGON);
			glVertex3f(2.4f, -0.1f -0.05f, 0.4f);
			glVertex3f(2.4f, -0.1f -0.05f, -0.4f);
			glVertex3f(3.0f, -0.3f -0.05f, -0.3f);
			glVertex3f(3.0f, -0.3f -0.05f, 0.3f);
		glEnd();
		glColor3f(0.0f, 0.0f, 0.65f);
		glBegin(GL_POLYGON);
			glVertex3f(3.0f, -0.3f -0.05f, -0.3f);
			glVertex3f(3.0f, -0.3f -0.05f, 0.3f);
			glVertex3f(0.05f, -0.3f -0.05f, 0.3f);
			glVertex3f(0.05f, -0.3f -0.05f, -0.3f);
		glEnd();
		//�������� ������...
		glColor3f(0.0f, 0.0f, 0.65f);
		glBegin(GL_POLYGON);
			glVertex3f(0.0f, -0.1f, 0.8f);
			glVertex3f(0.05f, 0.05f, 0.8f);
			glVertex3f(-0.04f, 0.0f, 1.26f);
			glVertex3f(-0.09f, -0.115f, 1.26f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-0.09f, -0.06f, 1.36f);
			glVertex3f(-0.04f, 0.0f, 1.26f);
			glVertex3f(-0.09f, -0.115f, 1.26f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-0.18f, -0.13f, 1.72f);
			glVertex3f(-0.09f, -0.06f, 1.36f);
			glVertex3f(-0.09f, -0.115f, 1.26f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(0.05f, 0.05f, 0.8f);
			glVertex3f(-0.04f, 0.0f, 1.26f);
			glVertex3f(2.0f, -0.05f -0.05f, 0.86f);
			glVertex3f(2.0f, 0.0f -0.05f, 0.5f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-0.09f, -0.06f, 1.36f);
			glVertex3f(-0.04f, 0.0f, 1.26f);
			glVertex3f(2.0f, -0.05f -0.05f, 0.86f);
			glVertex3f(1.95f, -0.11f -0.05f, 0.96f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-0.09f, -0.06f, 1.36f);
			glVertex3f(-0.18f, -0.13f, 1.72f);
			glVertex3f(1.86f, -0.18f -0.05f, 1.46f);
			glVertex3f(1.95f, -0.11f -0.05f, 0.96f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(2.0f, -0.05f -0.05f, 0.86f);
			glVertex3f(2.0f, 0.0f -0.05f, 0.5f);
			glVertex3f(2.4f, -0.1f -0.05f, 0.4f);
			glVertex3f(2.4f, -0.15f -0.05f, 0.76f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(2.0f, -0.05f -0.05f, 0.86f);
			glVertex3f(2.4f, -0.15f -0.05f, 0.76f);
			glVertex3f(2.35f, -0.21f -0.05f, 0.86f);
			glVertex3f(1.95f, -0.11f -0.05f, 0.96f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(2.35f, -0.21f -0.05f, 0.86f);
			glVertex3f(1.95f, -0.11f -0.05f, 0.96f);
			glVertex3f(1.86f, -0.18f -0.05f, 1.46f);
			glVertex3f(2.26f, -0.28f -0.05f, 1.36f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(2.4f, -0.1f -0.05f, 0.4f);
			glVertex3f(3.0f, -0.3f -0.05f, 0.3f);
			glVertex3f(3.0f, -0.35f -0.05f, 0.66f);
			glVertex3f(2.4f, -0.15f -0.05f, 0.76f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(3.0f, -0.35f -0.05f, 0.66f);
			glVertex3f(2.4f, -0.15f -0.05f, 0.76f);
			glVertex3f(2.35f, -0.21f -0.05f, 0.86f);
			glVertex3f(2.95f, -0.41f -0.05f, 0.76f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(2.35f, -0.21f -0.05f, 0.86f);
			glVertex3f(2.95f, -0.41f -0.05f, 0.76f);
			glVertex3f(2.5f, -0.41f -0.05f, 1.26f);
			glVertex3f(2.26f, -0.28f -0.05f, 1.36f);
		glEnd();

		glBegin(GL_POLYGON);
			glVertex3f(3.0f, -0.3f -0.05f, 0.3f);
			glVertex3f(3.0f, -0.35f -0.05f, 0.66f);
			glVertex3f(-0.04f, -0.35f -0.05f, 0.66f);
			glVertex3f(0.05f, -0.3f -0.05f, 0.3f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(3.0f, -0.35f -0.05f, 0.66f);
			glVertex3f(2.95f, -0.41f -0.05f, 0.76f);
			glVertex3f(-0.09f, -0.41f -0.05f, 0.76f);
			glVertex3f(-0.04f, -0.35f -0.05f, 0.66f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(2.95f, -0.41f -0.05f, 0.76f);
			glVertex3f(2.5f, -0.41f -0.05f, 1.26f);
			glVertex3f(-0.18f, -0.41f -0.05f, 1.26f);
			glVertex3f(-0.09f, -0.41f -0.05f, 0.76f);
		glEnd();
		//������ ^
		glBegin(GL_POLYGON);
			glVertex3f(0.0f, -0.1f, -0.8f);
			glVertex3f(0.05f, 0.05f, -0.8f);
			glVertex3f(-0.04f, 0.0f, -1.26f);
			glVertex3f(-0.09f, -0.115f, -1.26f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-0.09f, -0.06f, -1.36f);
			glVertex3f(-0.04f, 0.0f, -1.26f);
			glVertex3f(-0.09f, -0.115f, -1.26f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-0.18f, -0.13f, -1.72f);
			glVertex3f(-0.09f, -0.06f, -1.36f);
			glVertex3f(-0.09f, -0.115f, -1.26f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(0.05f, 0.05f, -0.8f);
			glVertex3f(-0.04f, 0.0f, -1.26f);
			glVertex3f(2.0f, -0.05f -0.05f, -0.86f);
			glVertex3f(2.0f, 0.0f -0.05f, -0.5f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-0.09f, -0.06f, -1.36f);
			glVertex3f(-0.04f, 0.0f, -1.26f);
			glVertex3f(2.0f, -0.05f -0.05f, -0.86f);
			glVertex3f(1.95f, -0.11f -0.05f, -0.96f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(-0.09f, -0.06f, -1.36f);
			glVertex3f(-0.18f, -0.13f, -1.72f);
			glVertex3f(1.86f, -0.18f -0.05f, -1.46f);
			glVertex3f(1.95f, -0.11f -0.05f, -0.96f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(2.0f, -0.05f -0.05f, -0.86f);
			glVertex3f(2.0f, 0.0f -0.05f, -0.5f);
			glVertex3f(2.4f, -0.1f -0.05f, -0.4f);
			glVertex3f(2.4f, -0.15f -0.05f, -0.76f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(2.0f, -0.05f -0.05f, -0.86f);
			glVertex3f(2.4f, -0.15f -0.05f, -0.76f);
			glVertex3f(2.35f, -0.21f -0.05f, -0.86f);
			glVertex3f(1.95f, -0.11f -0.05f, -0.96f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(2.35f, -0.21f -0.05f, -0.86f);
			glVertex3f(1.95f, -0.11f -0.05f, -0.96f);
			glVertex3f(1.86f, -0.18f -0.05f, -1.46f);
			glVertex3f(2.26f, -0.28f -0.05f, -1.36f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(2.4f, -0.1f -0.05f, -0.4f);
			glVertex3f(3.0f, -0.3f -0.05f, -0.3f);
			glVertex3f(3.0f, -0.35f -0.05f, -0.66f);
			glVertex3f(2.4f, -0.15f -0.05f, -0.76f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(3.0f, -0.35f -0.05f, -0.66f);
			glVertex3f(2.4f, -0.15f -0.05f, -0.76f);
			glVertex3f(2.35f, -0.21f -0.05f, -0.86f);
			glVertex3f(2.95f, -0.41f -0.05f, -0.76f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(2.35f, -0.21f -0.05f, -0.86f);
			glVertex3f(2.95f, -0.41f -0.05f, -0.76f);
			glVertex3f(2.5f, -0.41f -0.05f, -1.26f);
			glVertex3f(2.26f, -0.28f -0.05f, -1.36f);
		glEnd();

		glBegin(GL_POLYGON);
			glVertex3f(3.0f, -0.3f -0.05f, -0.3f);
			glVertex3f(3.0f, -0.35f -0.05f, -0.66f);
			glVertex3f(-0.04f, -0.35f -0.05f, -0.66f);
			glVertex3f(0.05f, -0.3f -0.05f, -0.3f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(3.0f, -0.35f -0.05f, -0.66f);
			glVertex3f(2.95f, -0.41f -0.05f, -0.76f);
			glVertex3f(-0.09f, -0.41f -0.05f, -0.76f);
			glVertex3f(-0.04f, -0.35f -0.05f, -0.66f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(2.95f, -0.41f -0.05f, -0.76f);
			glVertex3f(2.5f, -0.41f -0.05f, -1.26f);
			glVertex3f(-0.18f, -0.41f -0.05f, -1.26f);
			glVertex3f(-0.09f, -0.41f -0.05f, -0.76f);
		glEnd();
		//����� ^
		glRotatef(-hoodA, 0.0f, 0.0f, 1.0f);

		//�����
		//������� �������
		glColor3f(0.07f, 0.07f, 0.07f);
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(2.75f, -0.36f, -0.3f);
			glVertex3f(2.75f, -0.36f, 0.3f);
			glVertex3f(2.75f, -0.7f, 0.3f);
			glVertex3f(2.75f, -0.7f, -0.3f);
		glEnd();

		glBegin(GL_POLYGON);
			glVertex3f(2.75f, -0.36f, 0.3f);
			glVertex3f(2.75f, -0.41f, 0.66f);
			glVertex3f(2.75f, -0.7f, 0.66f);
			glVertex3f(2.75f, -0.7f, 0.3f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(2.75f, -0.41f, 0.66f);
			glVertex3f(2.75f, -0.47f, 0.76f);
			glVertex3f(2.75f, -0.7f, 0.76f);
			glVertex3f(2.75f, -0.7f, 0.66f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(2.75f, -0.47f, 0.76f);
			glVertex3f(2.75f, -0.7f, 0.76f);
			glVertex3f(2.75f, -0.7f, 0.82f);
		glEnd();

		glBegin(GL_POLYGON);
			glVertex3f(2.75f, -0.36f, -0.3f);
			glVertex3f(2.75f, -0.41f, -0.66f);
			glVertex3f(2.75f, -0.7f, -0.66f);
			glVertex3f(2.75f, -0.7f, -0.3f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(2.75f, -0.41f, -0.66f);
			glVertex3f(2.75f, -0.47f, -0.76f);
			glVertex3f(2.75f, -0.7f, -0.76f);
			glVertex3f(2.75f, -0.7f, -0.66f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(2.75f, -0.47f, -0.76f);
			glVertex3f(2.75f, -0.7f, -0.76f);
			glVertex3f(2.75f, -0.7f, -0.82f);
		glEnd();

		//������ �������
		glBegin(GL_POLYGON);
			glVertex3f(3.0f, -0.9f, -0.3f);
			glVertex3f(3.0f, -0.9f, 0.3f);
			glVertex3f(3.0f, -1.15f, 0.3f);
			glVertex3f(3.0f, -1.15f, -0.3f);
		glEnd();

		glBegin(GL_POLYGON);
			glVertex3f(3.0f, -0.9f, 0.3f);
			glVertex3f(3.0f, -0.9f, 0.66f);
			glVertex3f(3.0f, -1.13f, 0.66f);
			glVertex3f(3.0f, -1.15f, 0.3f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(3.0f, -0.9f, 0.66f);
			glVertex3f(3.0f, -0.9f, 0.76f);
			glVertex3f(3.0f, -1.07f, 0.76f);
			glVertex3f(3.0f, -1.13f, 0.66f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(3.0f, -0.9f, 0.76f);
			glVertex3f(3.0f, -1.07f, 0.76f);
			glVertex3f(3.0f, -0.9f, 0.78f);
		glEnd();

		glBegin(GL_POLYGON);
			glVertex3f(3.0f, -0.9f, -0.3f);
			glVertex3f(3.0f, -0.9f, -0.66f);
			glVertex3f(3.0f, -1.13f, -0.66f);
			glVertex3f(3.0f, -1.15f, -0.3f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(3.0f, -0.9f, -0.66f);
			glVertex3f(3.0f, -0.9f, -0.76f);
			glVertex3f(3.0f, -1.07f, -0.76f);
			glVertex3f(3.0f, -1.13f, -0.66f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(3.0f, -0.9f, -0.76f);
			glVertex3f(3.0f, -1.07f, -0.76f);
			glVertex3f(3.0f, -0.9f, -0.78f);
		glEnd();

		//��������� ������� �������
		glColor3f(0.0f, 0.0f, 0.65f);
		glBegin(GL_POLYGON);
			glVertex3f(2.6f, -0.355f, -0.3f);
			glVertex3f(2.6f, -0.355f, 0.3f);
			glVertex3f(3.0f, -0.355f, 0.3f);
			glVertex3f(3.0f, -0.355f, -0.3f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(2.6f, -0.355f, 0.3f);
			glVertex3f(2.6f, -0.405f, 0.66f);
			glVertex3f(3.0f, -0.405f, 0.66f);
			glVertex3f(3.0f, -0.355f, 0.3f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(2.6f, -0.405f, 0.66f);
			glVertex3f(2.6f, -0.465f, 0.76f);
			glVertex3f(3.0f, -0.465f, 0.76f);
			glVertex3f(3.0f, -0.405f, 0.66f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(2.6f, -0.465f, 0.76f);
			glVertex3f(2.6f, -0.7f, 0.82f);
			glVertex3f(2.8f, -0.7f, 0.82f);
			glVertex3f(3.0f, -0.465f, 0.76f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(2.6f, -0.465f, 0.8f);
			glVertex3f(2.6f, -0.7f, 0.86f);
			glVertex3f(2.8f, -0.7f, 0.86f);
			glVertex3f(3.0f, -0.465f, 0.8f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(2.8f, -0.7f, 0.82f);
			glVertex3f(3.0f, -0.465f, 0.76f);
			glVertex3f(3.0f, -0.465f, 0.8f);
			glVertex3f(2.8f, -0.7f, 0.86f);
		glEnd();

		glBegin(GL_POLYGON);
			glVertex3f(2.6f, -0.355f, -0.3f);
			glVertex3f(2.6f, -0.405f, -0.66f);
			glVertex3f(3.0f, -0.405f, -0.66f);
			glVertex3f(3.0f, -0.355f, -0.3f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(2.6f, -0.405f, -0.66f);
			glVertex3f(2.6f, -0.465f, -0.76f);
			glVertex3f(3.0f, -0.465f, -0.76f);
			glVertex3f(3.0f, -0.405f, -0.66f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(2.6f, -0.465f, -0.76f);
			glVertex3f(2.6f, -0.7f, -0.82f);
			glVertex3f(2.8f, -0.7f, -0.82f);
			glVertex3f(3.0f, -0.465f, -0.76f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(2.6f, -0.465f, -0.8f);
			glVertex3f(2.6f, -0.7f, -0.86f);
			glVertex3f(2.8f, -0.7f, -0.86f);
			glVertex3f(3.0f, -0.465f, -0.8f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(2.8f, -0.7f, -0.82f);
			glVertex3f(3.0f, -0.465f, -0.76f);
			glVertex3f(3.0f, -0.465f, -0.8f);
			glVertex3f(2.8f, -0.7f, -0.86f);
		glEnd();

		glBegin(GL_POLYGON);
			glVertex3f(2.6f, -0.7f, -0.86f);
			glVertex3f(2.6f, -0.7f, 0.86f);
			glVertex3f(3.0f, -0.7f, 0.86f);
			glVertex3f(3.0f, -0.7f, -0.86f);
		glEnd();

		//����� ������� ������� (�������� ������)
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(3.12f, -0.75f, -0.86f);
			glVertex3f(3.12f, -0.75f, 0.86f);
			glVertex3f(3.1f, -0.85f, 0.86f);
			glVertex3f(3.1f, -0.85f, -0.86f);
		glEnd();


		glBegin(GL_POLYGON);
			glVertex3f(3.0f, -0.7f, 0.86f);
			glVertex3f(3.0f, -0.7f, -0.86f);
			glVertex3f(3.12f, -0.75f, -0.86f);
			glVertex3f(3.12f, -0.75f, 0.86f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(3.0f, -0.9f, -0.78f);
			glVertex3f(3.0f, -0.9f, 0.78f);
			glVertex3f(3.1f, -0.85f, 0.86f);
			glVertex3f(3.1f, -0.85f, -0.86f);
		glEnd();

		//glColor3f(0.0f, 0.8f, 0.0f);
		glBegin(GL_POLYGON);
			glVertex3f(3.12f, -0.75f, 0.86f);
			glVertex3f(3.1f, -0.85f, 0.86f);
			glVertex3f(2.8f, -0.85f, 1.3f);
			glVertex3f(2.8f, -0.66f, 1.34f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(3.0f, -0.7f, 0.86f);
			glVertex3f(3.12f, -0.75f, 0.86f);
			glVertex3f(2.8f, -0.66f, 1.34f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(2.6f, -0.7f, 0.86f);
			glVertex3f(3.0f, -0.7f, 0.86f);
			glVertex3f(2.8f, -0.66f, 1.34f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(2.6f, -0.7f, 0.86f);
			glVertex3f(1.95f, -0.66f, 1.74f);
			glVertex3f(2.8f, -0.66f, 1.34f);
		glEnd();

		glBegin(GL_POLYGON);
			glVertex3f(2.8f, -0.85f, 1.3f);
			glVertex3f(2.8f, -0.66f, 1.34f);
			glVertex3f(1.95f, -0.66f, 1.74f);
			glVertex3f(2.05f, -0.85f, 1.7f);
		glEnd();

		//glVertex3f(2.5f, -0.41f -0.05f, -1.26f);
		glColor3f(0.0f, 0.0f, 0.65f);
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(-0.18f, -0.12f, 1.775f);
			glVertex3f(-0.18f, -0.13f, 1.72f);
			glVertex3f(-0.2f, -0.12f, 1.71f);
			glVertex3f(-0.2f, -0.12f, 1.77f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(1.86f, -0.18f -0.05f, 1.46f);
			glVertex3f(1.36f, -0.17f -0.05f, 1.67f);
			glVertex3f(-0.18f, -0.12f, 1.775f);
			glVertex3f(-0.18f, -0.13f, 1.72f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(1.67f, -0.5f, 1.82f);
			glVertex3f(1.86f, -0.18f -0.05f, 1.46f);
			glVertex3f(1.36f, -0.17f -0.05f, 1.67f);
			glVertex3f(1.36f, -0.32f -0.05f, 1.81f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(1.67f, -0.5f, 1.82f);
			glVertex3f(1.95f, -0.66f, 1.74f);
			glVertex3f(1.86f, -0.18f -0.05f, 1.46f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(1.95f, -0.66f, 1.74f);
			glVertex3f(1.86f, -0.18f -0.05f, 1.46f);
			glVertex3f(2.26f, -0.28f -0.05f, 1.36f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(2.375f, -0.67f, 1.44f);
			glVertex3f(1.95f, -0.66f, 1.74f);
			glVertex3f(2.26f, -0.28f -0.05f, 1.36f);
			glVertex3f(2.5f, -0.46f, 1.26f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(2.375f, -0.67f, 1.44f);
			glVertex3f(2.375f, -0.67f, 1.36f);
			glVertex3f(2.5f, -0.46f, 1.26f);
		glEnd();

		//������ ����
		glDisable(GL_LIGHTING);
		glColor4f(1.0f, 1.0f, 1.0f, 0.6f);
		glBegin(GL_POLYGON);
			glVertex3f(2.375f, -0.67f, 1.36f);
			glVertex3f(2.5f, -0.46f, 1.26f);
			glVertex3f(2.6f, -0.465f, 0.8f);
			glVertex3f(2.6f, -0.7f, 0.86f);
		glEnd();
		glTranslatef(2.48f, -0.58f, 0.98f);
		gluSphere(gluNewQuadric(), 0.14, 20, 20);
		glTranslatef(-2.48f, 0.58f, -0.98f);

		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(2.72f, -1.0f, 1.1f);
		gluSphere(gluNewQuadric(), 0.14, 20, 20);
		glTranslatef(-2.72f, 1.0f, -1.1f);
		glEnable(GL_LIGHTING);

		glColor3f(0.0f, 0.0f, 0.65f);
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(-0.18f, -0.12f, -1.775f);
			glVertex3f(-0.18f, -0.13f, -1.72f);
			glVertex3f(-0.2f, -0.12f, -1.71f);
			glVertex3f(-0.2f, -0.12f, -1.77f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(1.86f, -0.18f -0.05f, -1.46f);
			glVertex3f(1.36f, -0.17f -0.05f, -1.67f);
			glVertex3f(-0.18f, -0.12f, -1.775f);
			glVertex3f(-0.18f, -0.13f, -1.72f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(1.67f, -0.5f, -1.82f);
			glVertex3f(1.86f, -0.18f -0.05f, -1.46f);
			glVertex3f(1.36f, -0.17f -0.05f, -1.67f);
			glVertex3f(1.36f, -0.32f -0.05f, -1.81f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(1.67f, -0.5f, -1.82f);
			glVertex3f(1.95f, -0.66f, -1.74f);
			glVertex3f(1.86f, -0.18f -0.05f, -1.46f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(1.95f, -0.66f, -1.74f);
			glVertex3f(1.86f, -0.18f -0.05f, -1.46f);
			glVertex3f(2.26f, -0.28f -0.05f, -1.36f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(2.375f, -0.67f, -1.44f);
			glVertex3f(1.95f, -0.66f, -1.74f);
			glVertex3f(2.26f, -0.28f -0.05f, -1.36f);
			glVertex3f(2.5f, -0.46f, -1.26f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(2.375f, -0.67f, -1.44f);
			glVertex3f(2.375f, -0.67f, -1.36f);
			glVertex3f(2.5f, -0.46f, -1.26f);
		glEnd();
		//����� ����
		glDisable(GL_LIGHTING);
		glColor4f(1.0f, 1.0f, 1.0f, 0.6f);
		glBegin(GL_POLYGON);
			glVertex3f(2.375f, -0.67f, -1.36f);
			glVertex3f(2.5f, -0.46f, -1.26f);
			glVertex3f(2.6f, -0.465f, -0.8f);
			glVertex3f(2.6f, -0.7f, -0.86f);
		glEnd();
		glTranslatef(2.48f, -0.58f, -0.98f);
		gluSphere(gluNewQuadric(), 0.14, 20, 20);
		glTranslatef(-2.48f, 0.58f, 0.98f);

		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(2.72f, -1.0f, -1.1f);
		gluSphere(gluNewQuadric(), 0.14, 20, 20);
		glTranslatef(-2.72f, 1.0f, 1.1f);
		glEnable(GL_LIGHTING);
		glColor3f(0.0f, 0.0f, 0.65f);


		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(3.12f, -0.75f, -0.86f);
			glVertex3f(3.1f, -0.85f, -0.86f);
			glVertex3f(2.8f, -0.85f, -1.3f);
			glVertex3f(2.8f, -0.66f, -1.34f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(3.0f, -0.7f, -0.86f);
			glVertex3f(3.12f, -0.75f, -0.86f);
			glVertex3f(2.8f, -0.66f, -1.34f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(2.6f, -0.7f, -0.86f);
			glVertex3f(3.0f, -0.7f, -0.86f);
			glVertex3f(2.8f, -0.66f, -1.34f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(2.6f, -0.7f, -0.86f);
			glVertex3f(1.95f, -0.66f, -1.74f);
			glVertex3f(2.8f, -0.66f, -1.34f);
		glEnd();

		glBegin(GL_POLYGON);
			glVertex3f(2.8f, -0.85f, -1.3f);
			glVertex3f(2.8f, -0.66f, -1.34f);
			glVertex3f(1.95f, -0.66f, -1.74f);
			glVertex3f(2.05f, -0.85f, -1.7f);
		glEnd();

		//������ ���������
		glColor3f(0.8f, 0.8f, 0.8f);
		glBegin(GL_POLYGON);
			glVertex3f(3.0f, -1.15f, 0.3f);
			glVertex3f(3.0f, -1.15f, -0.3f);
			glVertex3f(3.06f, -1.25f, -0.3f);
			glVertex3f(3.06f, -1.25f, 0.3f);
		glEnd();
		glColor3f(0.0f, 0.0f, 0.65f);
		glBegin(GL_POLYGON);
			glVertex3f(3.06f, -1.25f, -0.3f);
			glVertex3f(3.06f, -1.25f, 0.3f);
			glVertex3f(1.0f, -1.25f, 0.3f);
			glVertex3f(1.0f, -1.25f, -0.3f);
		glEnd();
		//right
		glBegin(GL_POLYGON);
			glVertex3f(3.0f, -1.13f, 0.66f);
			glVertex3f(3.0f, -1.15f, 0.3f);
			glVertex3f(3.06f, -1.25f, 0.3f);
			glVertex3f(3.06f, -1.25f, 0.66f);
		glEnd();

		glBegin(GL_POLYGON);
			glVertex3f(3.0f, -1.07f, 0.76f);
			glVertex3f(3.0f, -1.13f, 0.66f);
			glVertex3f(3.06f, -1.25f, 0.66f);
			glVertex3f(3.06f, -1.25f, 0.76f);
		glEnd();

		glBegin(GL_POLYGON);
			glVertex3f(3.06f, -1.25f, 0.3f);
			glVertex3f(3.06f, -1.25f, 0.66f);
			glVertex3f(1.0f, -1.25f, 0.66f);
			glVertex3f(1.0f, -1.25f, 0.3f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(3.06f, -1.25f, 0.66f);
			glVertex3f(3.06f, -1.25f, 0.76f);
			glVertex3f(1.0f, -1.25f, 0.76f);
			glVertex3f(1.0f, -1.25f, 0.66f);
		glEnd();
		
		glBegin(GL_POLYGON);
			glVertex3f(3.1f, -0.85f, 0.86f);
			glVertex3f(2.8f, -0.85f, 1.3f);
			glVertex3f(3.0f, -0.9f, 0.78f);
		glEnd();


		glBegin(GL_POLYGON);
			glVertex3f(3.0f, -1.07f, 0.76f);
			glVertex3f(3.06f, -1.25f, 0.76f);
			glVertex3f(2.76f, -1.25f, 1.3f);
			glVertex3f(2.7f, -1.07f, 1.36f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(3.0f, -0.9f, 0.78f);
			glVertex3f(3.0f, -1.07f, 0.76f);
			glVertex3f(2.7f, -1.07f, 1.36f);
			glVertex3f(2.8f, -0.85f, 1.3f);
		glEnd();

		glBegin(GL_POLYGON);
			glVertex3f(2.76f, -1.25f, 1.3f);
			glVertex3f(2.7f, -1.07f, 1.36f);
			glVertex3f(2.15f, -1.07f, 1.66f);
			glVertex3f(2.1f, -1.23f, 1.76f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(2.7f, -1.07f, 1.36f);
			glVertex3f(2.8f, -0.85f, 1.3f);
			glVertex3f(2.05f, -0.85f, 1.7f);
			glVertex3f(2.15f, -1.07f, 1.66f);
		glEnd();

		glBegin(GL_POLYGON);
			glVertex3f(3.06f, -1.25f, 0.76f);
			glVertex3f(2.76f, -1.25f, 1.3f);
			glVertex3f(1.0f, -1.25f, 1.3f);
			glVertex3f(1.0f, -1.25f, 0.76f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(2.76f, -1.25f, 1.3f);
			glVertex3f(2.1f, -1.23f, 1.76f);
			glVertex3f(2.1f, -1.25f, 1.3f);
		glEnd();

		//left
		glBegin(GL_POLYGON);
			glVertex3f(3.0f, -1.13f, -0.66f);
			glVertex3f(3.0f, -1.15f, -0.3f);
			glVertex3f(3.06f, -1.25f, -0.3f);
			glVertex3f(3.06f, -1.25f, -0.66f);
		glEnd();

		glBegin(GL_POLYGON);
			glVertex3f(3.0f, -1.07f, -0.76f);
			glVertex3f(3.0f, -1.13f, -0.66f);
			glVertex3f(3.06f, -1.25f, -0.66f);
			glVertex3f(3.06f, -1.25f, -0.76f);
		glEnd();

		glBegin(GL_POLYGON);
			glVertex3f(3.06f, -1.25f, -0.3f);
			glVertex3f(3.06f, -1.25f, -0.66f);
			glVertex3f(1.0f, -1.25f, -0.66f);
			glVertex3f(1.0f, -1.25f, -0.3f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(3.06f, -1.25f, -0.66f);
			glVertex3f(3.06f, -1.25f, -0.76f);
			glVertex3f(1.0f, -1.25f, -0.76f);
			glVertex3f(1.0f, -1.25f, -0.66f);
		glEnd();
		
		glBegin(GL_POLYGON);
			glVertex3f(3.1f, -0.85f, -0.86f);
			glVertex3f(2.8f, -0.85f, -1.3f);
			glVertex3f(3.0f, -0.9f, -0.78f);
		glEnd();

		glBegin(GL_POLYGON);
			glVertex3f(3.0f, -1.07f, -0.76f);
			glVertex3f(3.06f, -1.25f, -0.76f);
			glVertex3f(2.76f, -1.25f, -1.3f);
			glVertex3f(2.7f, -1.07f, -1.36f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(3.0f, -0.9f, -0.78f);
			glVertex3f(3.0f, -1.07f, -0.76f);
			glVertex3f(2.7f, -1.07f, -1.36f);
			glVertex3f(2.8f, -0.85f, -1.3f);
		glEnd();

		glBegin(GL_POLYGON);
			glVertex3f(2.76f, -1.25f, -1.3f);
			glVertex3f(2.7f, -1.07f, -1.36f);
			glVertex3f(2.15f, -1.07f, -1.66f);
			glVertex3f(2.1f, -1.23f, -1.76f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(2.7f, -1.07f, -1.36f);
			glVertex3f(2.8f, -0.85f, -1.3f);
			glVertex3f(2.05f, -0.85f, -1.7f);
			glVertex3f(2.15f, -1.07f, -1.66f);
		glEnd();

		glBegin(GL_POLYGON);
			glVertex3f(3.06f, -1.25f, -0.76f);
			glVertex3f(2.76f, -1.25f, -1.3f);
			glVertex3f(1.0f, -1.25f, -1.3f);
			glVertex3f(1.0f, -1.25f, -0.76f);
		glEnd();
		glBegin(GL_POLYGON);
			glVertex3f(2.76f, -1.25f, -1.3f);
			glVertex3f(2.1f, -1.23f, -1.76f);
			glVertex3f(2.1f, -1.25f, -1.3f);
		glEnd();
		
		glTranslatef(-4.7f, 0.1f, 0.0f);

		glTranslatef(-0.5f, -0.6f, 0.0f);

		glTranslatef(3.784f, 0.0f, 0.0f);

		glDisable(GL_LIGHT2);
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: ���������� ��� �����.
	MSG msg;

	// ������������� ���������� �����
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MUSTANGMODEL, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ��������� ������������� ����������:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	// ���� ��������� ���������:
	while (1)
    {
            // ��������� ���� ���������
            while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
            {
                    if (GetMessage(&msg, NULL, 0, 0))
                    {
                            TranslateMessage(&msg);
                            DispatchMessage(&msg);
                    }
                    else
                    {
                            return TRUE;
                    }
            }
			DrawGLScene();                          // ���������� �����
			SwapBuffers(hDC);                               // ����������� ����� ������
			if (keys[VK_ESCAPE]) SendMessage(hWnd,WM_CLOSE,0,0);    // ���� ESC - �����
	}
	return (int) msg.wParam;
}

//
//  �������: MyRegisterClass()
//
//  ����������: ������������ ����� ����.
//
//  �����������:
//
//    ��� ������� � �� ������������� ���������� ������ � ������, ���� �����, ����� ������ ���
//    ��� ��������� � ��������� Win32, �� �������� ������� RegisterClassEx'
//    ������� ���� ��������� � Windows 95. ����� ���� ������� ����� ��� ����,
//    ����� ���������� �������� "������������" ������ ������ � ���������� �����
//    � ����.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MUSTANGMODEL));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MUSTANGMODEL);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   �������: InitInstance(HINSTANCE, int)
//
//   ����������: ��������� ��������� ���������� � ������� ������� ����.
//
//   �����������:
//
//        � ������ ������� ���������� ���������� ����������� � ���������� ����������, � �����
//        ��������� � ��������� �� ����� ������� ���� ���������.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{

   hInst = hInstance; // ��������� ���������� ���������� � ���������� ����������

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW/*WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS*/,
      120, 80, 800, 600, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }
   /*DEVMODE dmScreenSettings;                       // ����� ������
	memset(&dmScreenSettings, 0, sizeof(DEVMODE));          // ������� ��� �������� ���������
	dmScreenSettings.dmSize = sizeof(DEVMODE);              // ������ ��������� Devmode
	dmScreenSettings.dmPelsWidth    = 640;                  // ������ ������
	dmScreenSettings.dmPelsHeight   = 480;                  // ������ ������
	dmScreenSettings.dmFields       = DM_PELSWIDTH | DM_PELSHEIGHT; // ����� �������
	ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);*/
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  �������: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����������:  ������������ ��������� � ������� ����.
//
//  WM_COMMAND	- ��������� ���� ����������
//  WM_PAINT	-��������� ������� ����
//  WM_DESTROY	 - ������ ��������� � ������ � ���������.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	RECT    Screen;         // ������������ ������� ��� �������� ����
    GLuint  PixelFormat;
    static  PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),  // ������ ���� ���������
		1,                              // ����� ������ (?)
		PFD_DRAW_TO_WINDOW |            // ������ ��� ����
		PFD_SUPPORT_OPENGL |            // ������ ��� OpenGL
		PFD_DOUBLEBUFFER,               // ������ ��� �������� ������
		PFD_TYPE_RGBA,                  // ��������� RGBA ������
		16,                             // ����� 16 ��� ������� �����
		0, 0, 0, 0, 0, 0,                       // ������������� �������� ����� (?)
		0,                              // ��� ������ ������������
		0,                              // ��������� ��� ������������ (?)
		0,                              // ��� ������ �����������
		0, 0, 0, 0,                             // ���� ����������� ������������ (?)
		16,                             // 16 ������ Z-����� (����� �������) 
		0,                              // ��� ������ ����������
		0,                              // ��� ��������������� ������� (?)
		PFD_MAIN_PLANE,                 // ������� ���� ���������
		0,                              // ������ (?)
		0, 0, 0                         // ����� ���� ������������ (?)
    };

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// ��������� ����� � ����:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_CREATE:
        hDC = GetDC(hWnd);      // �������� �������� ���������� ��� ����
        PixelFormat = ChoosePixelFormat(hDC, &pfd);
                // ����� ��������� ���������� ��� ������ ������� ��������
		if (!PixelFormat)
        {
            MessageBox(0, (LPCWSTR)"Can't Find A SuitablePixelFormat.", (LPCWSTR)"Error", MB_OK|MB_ICONERROR);
            PostQuitMessage(0);
            // ��� ��������� �������, ��� ��������� ������ ����������
			break;  // �������������� ������� ����
        }
		if(!SetPixelFormat(hDC,PixelFormat,&pfd))
        {
            MessageBox(0, (LPCWSTR)"Can't Set The PixelFormat.", (LPCWSTR)"Error", MB_OK|MB_ICONERROR);
            PostQuitMessage(0);
            break;
        }
		hRC = wglCreateContext(hDC);
        if(!hRC)
        {
			MessageBox(0, (LPCWSTR)"Can't Create A GL Rendering Context.", (LPCWSTR)"Error", MB_OK|MB_ICONERROR);
			PostQuitMessage(0);
			break;
        }
		if(!wglMakeCurrent(hDC, hRC))
        {
			MessageBox(0, (LPCWSTR)"Can't activate GLRC.", (LPCWSTR)"Error", MB_OK|MB_ICONERROR);
			PostQuitMessage(0);
			break;
        }
		GetClientRect(hWnd, &Screen);
        InitGL(Screen.right, Screen.bottom);
		camXZ = 55.0f;
		camY = 5.0f;
		zoom = 12.0f;
		wheelsA = doorsA = hoodA = trunkA = 0.0f;
		wheels = doors = hood = trunk = 0;
        break;
	/*case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: �������� ����� ��� ���������...
		EndPaint(hWnd, &ps);
		break;*/
	case WM_DESTROY:
    case WM_CLOSE:
		ChangeDisplaySettings(NULL, 0);
		wglMakeCurrent(hDC,NULL);
		wglDeleteContext(hRC);
		ReleaseDC(hWnd,hDC);
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if ((wParam != 0x57)&&(wParam != 0x44)&&(wParam != 0x48)&&(wParam != 0x54))
			keys[wParam] = TRUE;
		break;
	case WM_KEYUP:
		if (wParam == 0x57)                // W (wheels) ������?
        {
			if (wheels < 3)
				wheels++;
			else wheels = 0;		// ������� ����� (���/��������/���������/����� ������)
        }
        if (wParam == 0x44)              // D (doors) ������?
        {
			if (doors < 1)
				doors++;
			else doors = 0;             // ������� ����� (���/��)
        }
		if (wParam == 0x48)                // H (hood) ������?
        {
			if (hood < 1)
				hood++;
			else hood = 0;             // ������� ����� (���/��)
        }
        if (wParam == 0x54)              // T (trunk) ������?
        {
			if (trunk < 1)
				trunk++;
			else trunk = 0;             // ������� �������� (���/��)
        }
		if ((wParam != 0x57)&&(wParam != 0x44)&&(wParam != 0x48)&&(wParam != 0x54))
			keys[wParam] = FALSE;
        break;
	case WM_SIZE:
        ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));
        break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// ���������� ��������� ��� ���� "� ���������".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
