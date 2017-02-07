#pragma once

#include <string>
#include <GL/glut.h>

using namespace std;

class DataSet{
	friend class DataViewer;
public:
	DataSet(void);
	DataSet(string fileName);
	~DataSet(void);

	void readData();
	void readTexture(string textureFileName);
	void drawDataOnConsole();
protected:
	void activeTexture() const;
	const GLfloat* getDataAt(GLint i, GLint j);

	string fileName;
	// Model Data
	GLint rows;
	GLint columns;
	GLfloat xllCorner;
	GLfloat yllCorner;
	GLfloat cellSize;
	GLfloat noDataValue;
	GLdouble maxHeight;
	GLdouble minHeight;	
	GLdouble** data;
	// Texture Data
	GLuint tboID;
	GLint iWidth, iHeight, iComponents;
	GLenum eFormat;
};

