#include<Windows.h>
#include <GL/glut.h> 
#include<iostream>
#include<fstream>
#include<vector>

using namespace std;

struct Point
{
	float x, y, z;
};

vector<Point> v;
vector<vector<int>> faces;
int point_count, face_count;

void reshape(int w, int h);
void display();
void processNormalKeys(unsigned char key, int x, int y);
void processSpecialKeys(int key, int x, int y);

void init(void)
{


	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);   //двухсторонний расчет освещения
	glEnable(GL_NORMALIZE);   //нормализация векторов (установление единичной длины)

	//направленный источник света
	GLfloat material_diffuse[] = { 1.0, 0.0, 0.0, 0.0 };
	GLfloat light0_diffuse[] = { 1.0, 1.0, 1.0 };   //белый свет
	GLfloat light0_direction[] = { -0.2, -0.5, -1.0, 0.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_diffuse);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);   //цвет рассеянного излучения источника света
	glLightfv(GL_LIGHT0, GL_POSITION, light0_direction);

	//точечный источник света
	GLfloat light1_diffuse[] = { 1.0, 1.0, 1.0 };
	GLfloat light1_position[] = { 0.0, 0.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

	//прожектор
	GLfloat light3_diffuse[] = { 1.0, 1.0, 1.0 };
	GLfloat light3_position[] = { 0.0, 0.0, 1.0, 1.0 };
	GLfloat light3_spot_direction[] = { 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT3, GL_DIFFUSE, light3_diffuse);
	glLightfv(GL_LIGHT3, GL_POSITION, light3_position);
	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 30);
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, light3_spot_direction);

	glEnable(GL_LIGHTING);   //включение освещения
	glEnable(GL_LIGHT0);   //включение направленного источника света
	//glEnable(GL_LIGHT1);   //включение точечного источника света
	//glEnable(GL_LIGHT3);   //включение прожектора
}

void readfromfile()
{
	float x, y, z;
	Point p;
	ifstream f("points.txt", ios::in);
	f >> point_count >> face_count;
	for (int i = 0; i < point_count; i++)
	{
		f >> p.x >> p.y >> p.z;
		v.push_back(p);
	}
	faces.resize(face_count);
	for (int i = 0; i < face_count; i++)
	{
		int n;
		f >> n;
		for (int j = 0; j < n; j++)
		{
			int p;
			f >> p;
			faces[i].push_back(p);
		}
	}
	f.close();
}

int main(int argc, char * argv[])
{
	readfromfile();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(800, 600);
	glutCreateWindow("LW6_1");
	init();

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);

	glEnable(GL_DEPTH_TEST);

	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	glutMainLoop();

	return 0;
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(0, 150, 0, 70, -150, 70);
	gluPerspective(50, 2, 0, 20);
	gluLookAt(0, 0, 250, 0, 0, 0, 0, 1, 0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void display()
{
	glClearColor(0.6, 0.6, 0.6, 0.0);
	//glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < face_count; i++)
	{
		if (faces[i].size() == 3)
		{
			glColor3f(1.0, 0.0, 0.0);
		}
		else 
		{
			glColor3f(0.0, 1.0, 0.0);
		}
		glBegin(GL_POLYGON);
		for (int j = 0; j < faces[i].size(); j++)
		{
			if (i < 11 || (i > 21 && i < 38))   //передние грани
			{
				glNormal3f(0.0, 0.0, 1.0);   //установление нормали
			}
			if (i == 16 || i == 17)   //задние грани
			{
				glNormal3f(0.0, 0.0, -1.0);
			}
			if (i == 46 || i == 47)   //верхние грани
			{
				glNormal3f(0.0, 1.0, 0.0);
			}
			if (i == 12 || i == 15 || i == 41 || i == 42)   //нижние грани
			{
				glNormal3f(0.0, -1.0, 0.0);
			}
			if (i == 14 || i == 20 || i == 21 || i == 40 || i == 44 || i == 48)   //правые грани
			{
				glNormal3f(1.0, 0.0, 0.0);
			}
			if (i == 13 || i == 18 || i == 19 || i == 39 || i == 43 || i == 45)   //левые грани
			{
				glNormal3f(-1.0, 0.0, 0.0);
			}
			glVertex3f(v[faces[i][j]].x, v[faces[i][j]].y, v[faces[i][j]].z);
		}
		glEnd();
	}
	glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int x, int y)
{
	if (key == 27)
		exit(0);
	if (key == 65)
	{
		glMatrixMode(GL_MODELVIEW);
		glTranslated(2, 2, 0);
		display();
	}
	if (key == '+') {
		glMatrixMode(GL_MODELVIEW);
		glScaled(1.3, 1.3, 1.3);
		display();
	}
	if (key == '-') {
		glMatrixMode(GL_MODELVIEW);
		glScaled(0.9, 0.9, 0.9);
		display();
	}
	if (key == 127) {
		glMatrixMode(GL_MODELVIEW);
		glRotated(-2, 0, 0, 1);
		display();
	}
}

void processSpecialKeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		glMatrixMode(GL_MODELVIEW);
		glTranslated(0, 2, 0);
		display();
		break;
	case GLUT_KEY_DOWN:
		glMatrixMode(GL_MODELVIEW);
		glTranslated(0, -2, 0);
		display();
		break;
	case GLUT_KEY_LEFT:
		glMatrixMode(GL_MODELVIEW);
		glTranslated(-2, 0, 0);
		display();
		break;
	case GLUT_KEY_RIGHT:
		glMatrixMode(GL_MODELVIEW);
		glTranslated(2, 0, 0);
		display();
		break;
	case GLUT_KEY_HOME:
		glMatrixMode(GL_MODELVIEW);
		glRotated(-2, 1, 0, 0);
		display();
		break;
	case GLUT_KEY_END:
		glMatrixMode(GL_MODELVIEW);
		glRotated(-2, 0, 1, 0);
		display();
		break;
	case GLUT_KEY_PAGE_UP:
		glMatrixMode(GL_MODELVIEW);
		glRotated(-2, 1, 1, 1);
		display();
		break;
	case GLUT_KEY_PAGE_DOWN:
		glMatrixMode(GL_MODELVIEW);
		glRotated(2, 1, 1, 1);
		display();
		break;
	}
}
