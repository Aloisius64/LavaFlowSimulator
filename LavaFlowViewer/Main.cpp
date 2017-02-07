#include <iostream>

#include <GL/glut.h>

#include "MyCamera.h"
#include "DataViewer.h"

using namespace std;

// DataSet
DataViewer* dataViewer;

// Camera
MyCamera camera;
const GLfloat defaultDelta = 10.0f;
static GLint sensibility = 0.1f;
static GLfloat defaultRotation = 1.0f;
const float sensitivity = 0.5;
static int old_x, old_y;

// Window Properties
static GLint defaultWidth = 1280;
static GLint defaultHeight = 720;

// Clipping Planes
static int zNear = 1;
static int zFar = 500;

// World Properties
int xRot = 0, yRot = 0, zRot = 0;
static GLfloat dimensionWorld = 150.0f;

// Lights
const GLfloat lightIntesity = 0.8f;
GLfloat ambientLight[] = { 0.2, 0.2, 0.2 };
GLfloat diffuseLight[] = { lightIntesity, lightIntesity, lightIntesity };
GLfloat specularLight[] = { 0.1, 0.1, 0.1 };
GLfloat lightPosition[] = { 0.0f, dimensionWorld, 0.0f, 1.0f };
static GLint lightXRot = 0.0f;
static GLint lightYRot = 0.0f;

// Prototype Functions
void init(void);
void drawVirtualSpace(GLfloat dimension);
void display(void);
void reshape(int w, int h);
void timeF(int value);
void manageKeyBoardEvent(unsigned char key, int x, int y);
void updateCameraAndView();
void mouse(int button, int state, int x, int y);
void motion(int x, int y);

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(defaultWidth, defaultHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("DataSet");

	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutKeyboardFunc(manageKeyBoardEvent);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	glutMainLoop();
	// Clean Memory
	delete dataViewer;
	return 0;
}

void init(void) {
	glClearColor(0.2431372f, 0.2431372f, 0.2588235f, 1.0f);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	camera.position.y = 10.0f;
	camera.position.z = dimensionWorld;

	//dataViewer = new DataViewer("topografia.dat", "lava.dat", "temperature.dat", "textureMedium.tga", true, dimensionWorld, true);
	//dataViewer = new DataViewer("topografia.dat", "lava.dat", "temperature.dat", "", true, dimensionWorld, false);
	dataViewer = new DataViewer("topografia3.dat", "", "", "", true, dimensionWorld, true);

	// Lights
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Normals
	glEnable(GL_NORMALIZE);
}

void drawVirtualSpace(GLfloat dimension) {
	// X,Z Plane
	glLineWidth(0.5f);
	GLfloat limit = -(dimension / 2);
	GLfloat space = dimension / 10.0f;
	GLint numLines = dimension / space;
	GLint next = dimension / numLines;
	glColor3f(1.0f, 1.0f, 1.0f);
	for (int i = 0; i <= numLines; i++) {
		if (i == (numLines + 1) / 2) {
			continue;
		}
		glBegin(GL_LINES);
		glVertex3f(limit + (i * next), 0.0f, -(dimension / 2));
		glVertex3f(limit + (i * next), 0.0f, (dimension / 2));
		glEnd();

		glBegin(GL_LINES);
		glVertex3f(-(dimension / 2), 0.0f, limit + (i * next));
		glVertex3f((dimension / 2), 0.0f, limit + (i * next));
		glEnd();
	}

	glLineWidth(1.5f);
	// X Axis
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-(dimension / 2), 0.0f, 0.0f);
	glVertex3f((dimension / 2), 0.0f, 0.0f);
	glEnd();

	// Y Axis
	glBegin(GL_LINES);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -(dimension / 2), 0.0f);
	glVertex3f(0.0f, (dimension / 2), 0.0f);
	glEnd();

	// Z Axis
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, -(dimension / 2));
	glVertex3f(0.0f, 0.0f, (dimension / 2));
	glEnd();
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Virtual Axis
	//glPushAttrib(GL_TEXTURE_BIT);{
	//	glDisable(GL_TEXTURE_2D);
	//	glPushAttrib(GL_LIGHTING_BIT); {
	//		glDisable(GL_LIGHTING);
	//		drawVirtualSpace(dimensionWorld);
	//	} glPopAttrib();
	//	glEnable(GL_TEXTURE_2D);
	//}glPopAttrib();

	// Lights
	glPushMatrix();
	{
		glPushAttrib(GL_TEXTURE_BIT);
		{
			glDisable(GL_TEXTURE_2D);
			glRotatef(lightYRot, 0.0f, 1.0f, 0.0f);
			glRotatef(lightXRot, 1.0f, 0.0f, 0.0f);
			glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
			glTranslatef(lightPosition[0], lightPosition[1], lightPosition[2]);
			glColor3f(1.0f, 0.0f, 0.0f);
			glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
			glutSolidCone(4.0f, 6.0f, 15, 15);

			glPushAttrib(GL_LIGHTING_BIT);
			{
				glDisable(GL_LIGHTING);
				glColor3f(1.0f, 1.0f, 0.0f);
				glutSolidSphere(3.0f, 10, 10);
			}glPopAttrib();
		}glPopAttrib();
	}glPopMatrix();

	// Terrain
	glPushMatrix();
	{
		glRotatef((GLfloat)xRot, 1.0f, 0.0f, 0.0f);
		glRotatef((GLfloat)yRot, 0.0f, 1.0f, 0.0f);
		glRotatef((GLfloat)zRot, 0.0f, 0.0f, 1.0f);

		dataViewer->drawDataSet();
	}
	glPopMatrix();

	// HUD Display
	dataViewer->drawHUDInfo(defaultWidth, defaultHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLdouble)defaultWidth / (GLdouble)defaultHeight, zNear, zFar);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camera.position.x, camera.position.y, camera.position.z,
			  camera.position.x + camera.direction.x,
			  camera.position.y + camera.direction.y,
			  camera.position.z + camera.direction.z,
			  0.0,
			  1.0,
			  0.0);

	glutSwapBuffers();
}

void reshape(int w, int h) {
	defaultWidth = w;
	defaultHeight = h;

	glViewport(0, 0, w, h);

	if (defaultHeight <= 0) {
		defaultHeight = 1;
	}

	updateCameraAndView();
}

void manageKeyBoardEvent(unsigned char key, int x, int y) {
	//if (GetAsyncKeyState('W')) {
	//	camera.moveForward(defaultDelta);
	//}
	//if (GetAsyncKeyState('A')) {
	//	camera.moveLeft(defaultDelta);
	//}
	//if (GetAsyncKeyState('S')) {
	//	camera.moveBackward(defaultDelta);
	//}
	//if (GetAsyncKeyState('D')) {
	//	camera.moveRight(defaultDelta);
	//}
	//if (GetAsyncKeyState('Q')) {
	//	camera.moveUp(defaultDelta);
	//}
	//if (GetAsyncKeyState('E')) {
	//	camera.moveDown(defaultDelta);
	//}

	/*if(key == 'i'){
	camera.rotateX(1);
	} else if (key == 'j'){
	camera.rotateY(1);
	} else if (key == 'k'){
	camera.rotateX(359);
	} else if (key == 'l'){
	camera.rotateY(-1);
	} else if (key == 'u'){
	camera.rotateZ(1);
	} else if (key == 'o'){
	camera.rotateZ(-1);
	} */

	switch (key) {
		case 'w':
		camera.moveForward(defaultDelta);
		break;
		case 'a':
		camera.moveLeft(defaultDelta);
		break;
		case 's':
		camera.moveBackward(defaultDelta);
		break;
		case 'd':
		camera.moveRight(defaultDelta);
		break;
		case 'i':
		xRot = (xRot + 10) % 360;
		glutPostRedisplay();
		break;
		case 'k':
		xRot = (xRot - 10) % 360;
		glutPostRedisplay();
		break;
		case 'j':
		yRot = (yRot + 10) % 360;
		glutPostRedisplay();
		break;
		case 'l':
		yRot = (yRot - 10) % 360;
		glutPostRedisplay();
		break;
		case 'u':
		zRot = (zRot + 10) % 360;
		glutPostRedisplay();
		break;
		case 'o':
		zRot = (zRot - 10) % 360;
		glutPostRedisplay();
		break;
		case 'b':
		lightXRot = (lightXRot + 10) % 360;
		glutPostRedisplay();
		break;
		case 'n':
		lightXRot = (lightXRot - 10) % 360;
		glutPostRedisplay();
		break;
		case 'v':
		lightYRot = (lightYRot + 10) % 360;
		glutPostRedisplay();
		break;
		case 'm':
		lightYRot = (lightYRot - 10) % 360;
		glutPostRedisplay();
		break;
	}

	updateCameraAndView();
}

void updateCameraAndView() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLdouble)defaultWidth / (GLdouble)defaultHeight, zNear, zFar);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camera.position.x, camera.position.y, camera.position.z,
			  camera.position.x + camera.direction.x,
			  camera.position.y + camera.direction.y,
			  camera.position.z + camera.direction.z,
			  0.0,
			  1.0,
			  0.0);

	// Refresh the Window
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
	if ((button == 3) || (button == 4)) { // It's a wheel event
		if (button == 3) { // Scroll Wheel Up
			camera.moveForward(defaultDelta);
		} else { // Scroll Wheel Down
			camera.moveBackward(defaultDelta);
		}
	} else {  // normal button event
		old_x = x - defaultWidth / 2;
		old_y = y - defaultHeight / 2;
	}

	updateCameraAndView();
	//glutPostRedisplay();
}

void motion(int x, int y) {
	float rot_x = 0.0f, rot_y = 0.0f;
	x -= defaultWidth / 2;
	y -= defaultHeight / 2;

	rot_x = (float)(x - old_x) * sensitivity;
	rot_y = -(float)(y - old_y) * sensitivity;
	old_x = x;
	old_y = y;

	xRot += rot_y;
	yRot += rot_x;

	/*
	*	FPS Camera
	*/
	{
		//MyVector3f axis;
		//axis.x = 0.0f;
		//axis.y = 1.0f;
		//axis.z = 0.0f;
		//camera.upVector = axis;
		//camera.direction = camera.RotateAroundAnyAxis(camera.direction, axis, rot_x);
		//camera.rightVector = camera.RotateAroundAnyAxis(camera.rightVector, axis, rot_x);
		//axis.x = -camera.direction.z;
		//axis.y = 0.0f;
		//axis.z = camera.direction.x;
		////camera.upVector = axis;
		//axis = VectorNormalized(axis);
		//camera.direction = camera.RotateAroundAnyAxis(camera.direction, axis, rot_y);
		//camera.rightVector = camera.RotateAroundAnyAxis(camera.rightVector, axis, rot_y);
	}

	updateCameraAndView();
}