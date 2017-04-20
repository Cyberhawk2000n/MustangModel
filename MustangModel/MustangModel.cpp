// MustangModel.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "MustangModel.h"
#include <windows.h>                      // Заголовочный файл для Windows
#include <gl\gl.h>                        // Заголовочный файл для OpenGL32 библиотеки
#include <gl\glu.h>                       // Заголовочный файл для GLu32 библиотеки
#include <gl\glaux.h>                     // Заголовочный файл для GLaux библиотеки
#include <math.h>

#define MAX_LOADSTRING 100
#define M_PI 3.1415926535897932384626433832795

// Глобальные переменные:
HINSTANCE hInst;								// текущий экземпляр
TCHAR szTitle[MAX_LOADSTRING];					// Текст строки заголовка
TCHAR szWindowClass[MAX_LOADSTRING];			// имя класса главного окна
static HGLRC hRC;               // Постоянный контекст рендеринга
static HDC hDC;                 // Приватный контекст устройства GDI
BOOL    keys[256];              // Массив для процедуры обработки клавиатуры
HWND hWnd;
GLfloat rtri;           // Угол для треугольник
GLfloat rquad;          // Угол для четырехугольника
GLfloat camXZ;
GLfloat camY;
GLfloat zoom;
GLfloat wheelsA, doorsA, hoodA, trunkA;
static int wheels, doors, hood, trunk;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

GLvoid InitGL(GLsizei Width, GLsizei Height)    // Вызвать после создания окна GL
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);   // Очистка экрана в черный цвет
	glClearDepth(1.0);                      // Разрешить очистку буфера глубины
    glDepthFunc(GL_LESS);                   // Тип теста глубины
    glEnable(GL_DEPTH_TEST);                // разрешить тест глубины
	glShadeModel(GL_SMOOTH);        // разрешить плавное цветовое сглаживание
    glMatrixMode(GL_PROJECTION);    // Выбор матрицы проекции
    glLoadIdentity();               // Сброс матрицы проекции
    gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);
                            // Вычислить соотношение геометрических размеров для окна
    glMatrixMode(GL_MODELVIEW);     // Выбор матрицы просмотра модели
}

GLvoid ReSizeGLScene(GLsizei Width, GLsizei Height)
{
        if (Height==0)          // Предотвращение деления на ноль, если окно слишком мало
                Height=1;

        glViewport(0, 0, Width, Height);
                // Сброс текущей области вывода и перспективных преобразований

        glMatrixMode(GL_PROJECTION);            // Выбор матрицы проекций
        glLoadIdentity();                       // Сброс матрицы проекции

        gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);
                // Вычисление соотношения геометрических размеров для окна
        glMatrixMode(GL_MODELVIEW);     // Выбор матрицы просмотра модели
}

GLvoid DrawGLScene(GLvoid)
{
		int i;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                // очистка Экрана и буфера глубины
        //glLoadIdentity();
		if (keys[VK_LEFT])                // Стрелка влево была нажата?
        {
			if (camXZ > 0.05f)
				camXZ -= 0.5f;             // Вращаем экран влево
			else camXZ = 360.0f;
        }
        if (keys[VK_RIGHT])              // Стрелка вправо нажата?
        {
			if (camXZ < 359.95f)
                camXZ += 0.5f;             // Вращаем экран вправо
			else camXZ = 0.0f;
        }
		if (keys[VK_DOWN])                // Стрелка вниз была нажата?
        {
			if (camY >= -89.95f)
				camY -= 0.5f;             // Вращаем экран вверх
        }
        if (keys[VK_UP])              // Стрелка вверх нажата?
        {
			if (camY <= 89.95f)
                camY += 0.5f;             // Вращаем экран вниз
        }
		if (keys[0x58])                // X нажата?
        {
			if (zoom >= 5.005f)
				zoom -= 0.05f;             // Приближаем экран
        }
        if (keys[0x5A])              // Z нажата?
        {
			if (zoom <= 17.995f)
                zoom += 0.05f;             // Отдаляем экран
        }
		switch (wheels)                // W (wheels) нажата?
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
        if (doors)              // D (doors) нажата?
		{
			if (doorsA < 35.0f)
				doorsA += 1.0f;
		}
		else
		{
			if (doorsA > 0.0f)
				doorsA -= 1.0f;
		}
		if (hood)                // H (hood) нажата?
        {
			if (hoodA < 45.0f)
				hoodA += 1.0f;
        }
		else
		{
			if (hoodA > 0.0f)
				hoodA -= 1.0f;
		}
        if (trunk)              // T (trunk) нажата?
        {
			if (trunkA < 45.0f)
				trunkA += 1.0f;
        }
		else
		{
			if (trunkA > 0.0f)
				trunkA -= 1.0f;
		}
		// рассчет освещения
        glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glShadeModel(GL_SMOOTH); 
		glEnable(GL_NORMALIZE); //делам нормали одинаковой величины во избежание артефактов
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
		gluLookAt(zoom*cos(M_PI*camY/180.0)*sin(M_PI*camXZ/180.0), zoom*sin(M_PI*camY/180.0), zoom*cos(M_PI*camY/180.0)*cos(M_PI*camXZ/180.0), // Положение глаз, взгляд "из"
			0.0f, 0.0f, 0.0f, // Цель, взгляд "на"
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
		
		//Основа
		glDisable(GL_LIGHTING);
		glColor3f(0xff, 0xd7, 0x00);
		glTranslatef(0.0f,-1.381f,0.0f);
		glRotatef(90,1.0f,0.0f,0.0f);
		gluDisk(gluNewQuadric(), 0.0, 5.0, 30, 5); // диск
		glRotatef(-90,1.0f,0.0f,0.0f);
		glTranslatef(0.0f,1.381f,0.0f);
		//Колесо переднее правое
		glColor3f(0.02f, 0.02f, 0.02f);
		glTranslatef(2.84f, -0.6905f, 1.420f);
		glRotatef(-wheelsA, 0.0f, 0.0f, 1.0f);
		gluCylinder(gluNewQuadric(), 0.6905f, 0.6905f, 0.3f, 20, 20); // цилиндр
		glRotatef(wheelsA, 0.0f, 0.0f, 1.0f);
		glColor3f(0.8f, 0.8f, 0.8f);
		glTranslatef(0.0f, 0.0f, 0.18f);
		glRotatef(-wheelsA, 0.0f, 0.0f, 1.0f);
		gluDisk(gluNewQuadric(), 0.0f, 0.45f, 30, 5); // диск
		glColor3f(0.4f, 0.4f, 0.4f);
		gluCylinder(gluNewQuadric(), 0.45f, 0.45f, 0.12f, 20, 20); // цилиндр
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
		gluDisk(gluNewQuadric(), 0.45f, 0.6905f, 30, 5); // диск
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
		//Колесо переднее левое
		glColor3f(0.02f, 0.02f, 0.02f);
		glTranslatef(2.84f, -0.6905f, -1.420f);
		glRotatef(180.0, 0.0f, 1.0f, 0.0f);
		glRotatef(wheelsA, 0.0f, 0.0f, 1.0f);
		gluCylinder(gluNewQuadric(), 0.6905f, 0.6905f, 0.3f, 20, 20); // цилиндр
		glRotatef(-wheelsA, 0.0f, 0.0f, 1.0f);
		glColor3f(0.8f, 0.8f, 0.8f);
		glTranslatef(0.0f, 0.0f, 0.18f);
		glRotatef(wheelsA, 0.0f, 0.0f, 1.0f);
		gluDisk(gluNewQuadric(), 0.0f, 0.45f, 30, 5); // диск
		glColor3f(0.4f, 0.4f, 0.4f);
		gluCylinder(gluNewQuadric(), 0.45f, 0.45f, 0.12f, 20, 20); // цилиндр
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
		gluDisk(gluNewQuadric(), 0.45f, 0.6905f, 30, 5); // диск
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
		//Колесо заднее правое
		glColor3f(0.02f, 0.02f, 0.02f);
		glTranslatef(-2.6f, -0.6905f, 1.447f);
		glRotatef(-wheelsA, 0.0f, 0.0f, 1.0f);
		gluCylinder(gluNewQuadric(), 0.6905f, 0.6905f, 0.34f, 20, 20); // цилиндр
		glRotatef(wheelsA, 0.0f, 0.0f, 1.0f);
		glColor3f(0.8f, 0.8f, 0.8f);
		glTranslatef(0.0f, 0.0f, 0.22f);
		glRotatef(-wheelsA, 0.0f, 0.0f, 1.0f);
		gluDisk(gluNewQuadric(), 0.0f, 0.45f, 30, 5); // диск
		glColor3f(0.4f, 0.4f, 0.4f);
		gluCylinder(gluNewQuadric(), 0.45f, 0.45f, 0.12f, 20, 20); // цилиндр
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
		gluDisk(gluNewQuadric(), 0.45f, 0.6905f, 30, 5); // диск
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

		//Колесо заднее левое
		glColor3f(0.02f, 0.02f, 0.02f);
		glTranslatef(-2.6f, -0.6905f, -1.447f);
		glRotatef(180.0, 0.0f, 1.0f, 0.0f);
		glRotatef(wheelsA, 0.0f, 0.0f, 1.0f);
		gluCylinder(gluNewQuadric(), 0.6905f, 0.6905f, 0.34f, 20, 20); // цилиндр
		glRotatef(-wheelsA, 0.0f, 0.0f, 1.0f);
		glColor3f(0.8f, 0.8f, 0.8f);
		glTranslatef(0.0f, 0.0f, 0.22f);
		glRotatef(wheelsA, 0.0f, 0.0f, 1.0f);
		gluDisk(gluNewQuadric(), 0.0f, 0.45f, 30, 5); // диск
		glColor3f(0.4f, 0.4f, 0.4f);
		gluCylinder(gluNewQuadric(), 0.45f, 0.45f, 0.12f, 20, 20); // цилиндр
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
		gluDisk(gluNewQuadric(), 0.45f, 0.6905f, 30, 5); // диск
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

		//правый бок
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
		//дверь
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
		//зеркало
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
		//зеркало
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(2.83f - 3.3f, 1.32f, -0.165f);
			glVertex3f(2.63f - 3.3f, 1.32f, 0.2f);
			glVertex3f(2.63f - 3.3f, 1.5f, 0.15f);
			glVertex3f(2.83f - 3.3f, 1.55f, -0.165f);
		glEnd();
		//ручка
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
		//правое крыло
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
			glVertex3f(1.5f, 1.2f, -0.2f);//правое место касания!!!!!!!!
		glEnd();

		//правый кузов
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

		//левый бок
		glColor3f(0.0f, 0.0f, 0.65f);
		glTranslatef(-1.87f, -0.92f, -1.87f);
		//glRotatef(-180.0, 0.0f, 1.0f, 0.0f);
		//низ
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
		//левый бок (продолжение)
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
		//дверь
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
		//зеркало
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
		//зеркало
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_POLYGON);
			glNormal3f(0, 0, 1);
			glVertex3f(2.83f - 3.3f, 1.32f, 0.165f);
			glVertex3f(2.63f - 3.3f, 1.32f, -0.2f);
			glVertex3f(2.63f - 3.3f, 1.5f, -0.15f);
			glVertex3f(2.83f - 3.3f, 1.55f, 0.165f);
		glEnd();
		//ручка
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
		//левое крыло
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

		//левый кузов
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
		//кузов
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

		//здоровые выхлопные трубы
		glColor3f(0.02f, 0.02f, 0.02f);
		glTranslatef(-0.7f, -0.7f, 1.0f);
		glRotatef(270.0, 0.0f, 1.0f, 0.0f);
		gluCylinder(gluNewQuadric(), 0.09f, 0.09f, 0.25f, 20, 20); // цилиндр
		glRotatef(-270.0, 0.0f, 1.0f, 0.0f);
		glTranslatef(-0.2f, 0.0f, 0.0f);
		glRotatef(270.0, 0.0f, 1.0f, 0.0f);
		gluDisk(gluNewQuadric(), 0.0f, 0.09, 30, 5); // диск
		glRotatef(-270.0, 0.0f, 1.0f, 0.0f);
		glTranslatef(0.2f, 0.0f, 0.0f);
		glTranslatef(0.7f, 0.7f, -1.0f);
		//glColor3f(0.0f, 0.0f, 0.8f);

		glTranslatef(-0.7f, -0.7f, -1.0f);
		glRotatef(270.0, 0.0f, 1.0f, 0.0f);
		gluCylinder(gluNewQuadric(), 0.09f, 0.09f, 0.25f, 20, 20); // цилиндр
		glRotatef(-270.0, 0.0f, 1.0f, 0.0f);
		glTranslatef(-0.2f, 0.0f, 0.0f);
		glRotatef(270.0, 0.0f, 1.0f, 0.0f);
		gluDisk(gluNewQuadric(), 0.0f, 0.09, 30, 5); // диск
		glRotatef(-270.0, 0.0f, 1.0f, 0.0f);
		glTranslatef(0.2f, 0.0f, 0.0f);
		glTranslatef(0.7f, 0.7f, 1.0f);
		
		glColor3f(0.0f, 0.0f, 0.65f);

		//низ
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

		//задние габаритные фонари
		
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
		
		//правый фонарь
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
		//левый фонарь
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

		//багажник
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

		//верх
		//задний верх
		
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

		//верх верх
		
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

		//front правое стекло
		/*glBegin(GL_POLYGON);
			glVertex3f(3.6f, 0.55f, 0.8f);
			glVertex3f(4.7f, -0.1f, 0.8f);
			glVertex3f(4.6f, -0.1f, 1.35f);
			glVertex3f(4.5f, -0.05f, 1.5f);
			glVertex3f(3.6f, 0.5f, 1.35f);
		glEnd();*/

		//заднее стекло
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

		//заднее правое стекло
		glBegin(GL_POLYGON);
			glVertex3f(1.5f, 0.3f, 1.5f);
			glVertex3f(0.5f, -0.1f, 1.55f);
			glVertex3f(1.6f, -0.1f, 1.65f);
		glEnd();

		//заднее левоe стекло
		glBegin(GL_POLYGON);
			glVertex3f(1.5f, 0.3f, -1.5f);
			glVertex3f(0.5f, -0.1f, -1.55f);
			glVertex3f(1.6f, -0.1f, -1.65f);
		glEnd();

		//переднее стекло
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

		//капот
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
		//боковины капота...
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
		//правая ^
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
		//левая ^
		glRotatef(-hoodA, 0.0f, 0.0f, 1.0f);

		//перед
		//верхняя решётка
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

		//нижняя решётка
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

		//окантовка верхней решётки
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

		//самый дальний полигон (передний бампер)
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

		//правая фара
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
		//левая фара
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

		//нижняя окантовка
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

 	// TODO: разместите код здесь.
	MSG msg;

	// Инициализация глобальных строк
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MUSTANGMODEL, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Выполнить инициализацию приложения:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	// Цикл основного сообщения:
	while (1)
    {
            // Обработка всех сообщений
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
			DrawGLScene();                          // Нарисовать сцену
			SwapBuffers(hDC);                               // Переключить буфер экрана
			if (keys[VK_ESCAPE]) SendMessage(hWnd,WM_CLOSE,0,0);    // Если ESC - выйти
	}
	return (int) msg.wParam;
}

//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  НАЗНАЧЕНИЕ: регистрирует класс окна.
//
//  КОММЕНТАРИИ:
//
//    Эта функция и ее использование необходимы только в случае, если нужно, чтобы данный код
//    был совместим с системами Win32, не имеющими функции RegisterClassEx'
//    которая была добавлена в Windows 95. Вызов этой функции важен для того,
//    чтобы приложение получило "качественные" мелкие значки и установило связь
//    с ними.
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
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//
//   КОММЕНТАРИИ:
//
//        В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится на экран главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{

   hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW/*WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS*/,
      120, 80, 800, 600, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }
   /*DEVMODE dmScreenSettings;                       // Режим работы
	memset(&dmScreenSettings, 0, sizeof(DEVMODE));          // Очистка для хранения установок
	dmScreenSettings.dmSize = sizeof(DEVMODE);              // Размер структуры Devmode
	dmScreenSettings.dmPelsWidth    = 640;                  // Ширина экрана
	dmScreenSettings.dmPelsHeight   = 480;                  // Высота экрана
	dmScreenSettings.dmFields       = DM_PELSWIDTH | DM_PELSHEIGHT; // Режим Пиксела
	ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);*/
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//
//  WM_COMMAND	- обработка меню приложения
//  WM_PAINT	-Закрасить главное окно
//  WM_DESTROY	 - ввести сообщение о выходе и вернуться.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	RECT    Screen;         // используется позднее для размеров окна
    GLuint  PixelFormat;
    static  PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),  // Размер этой структуры
		1,                              // Номер версии (?)
		PFD_DRAW_TO_WINDOW |            // Формат для Окна
		PFD_SUPPORT_OPENGL |            // Формат для OpenGL
		PFD_DOUBLEBUFFER,               // Формат для двойного буфера
		PFD_TYPE_RGBA,                  // Требуется RGBA формат
		16,                             // Выбор 16 бит глубины цвета
		0, 0, 0, 0, 0, 0,                       // Игнорирование цветовых битов (?)
		0,                              // нет буфера прозрачности
		0,                              // Сдвиговый бит игнорируется (?)
		0,                              // Нет буфера аккумуляции
		0, 0, 0, 0,                             // Биты аккумуляции игнорируются (?)
		16,                             // 16 битный Z-буфер (буфер глубины) 
		0,                              // Нет буфера траффарета
		0,                              // Нет вспомогательных буферов (?)
		PFD_MAIN_PLANE,                 // Главный слой рисования
		0,                              // Резерв (?)
		0, 0, 0                         // Маски слоя игнорируются (?)
    };

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Разобрать выбор в меню:
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
        hDC = GetDC(hWnd);      // Получить контекст устройства для окна
        PixelFormat = ChoosePixelFormat(hDC, &pfd);
                // Найти ближайшее совпадение для нашего формата пикселов
		if (!PixelFormat)
        {
            MessageBox(0, (LPCWSTR)"Can't Find A SuitablePixelFormat.", (LPCWSTR)"Error", MB_OK|MB_ICONERROR);
            PostQuitMessage(0);
            // Это сообщение говорит, что программа должна завершится
			break;  // Предтовращение повтора кода
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
		// TODO: добавьте любой код отрисовки...
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
		if (wParam == 0x57)                // W (wheels) нажата?
        {
			if (wheels < 3)
				wheels++;
			else wheels = 0;		// Вращать колёса (нет/медленно/нормально/очень быстро)
        }
        if (wParam == 0x44)              // D (doors) нажата?
        {
			if (doors < 1)
				doors++;
			else doors = 0;             // Открыть двери (нет/да)
        }
		if (wParam == 0x48)                // H (hood) нажата?
        {
			if (hood < 1)
				hood++;
			else hood = 0;             // Открыть капот (нет/да)
        }
        if (wParam == 0x54)              // T (trunk) нажата?
        {
			if (trunk < 1)
				trunk++;
			else trunk = 0;             // Открыть багажник (нет/да)
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

// Обработчик сообщений для окна "О программе".
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
