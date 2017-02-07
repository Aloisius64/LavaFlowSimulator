#pragma once

#include "DataSet.h"
#include <string>

using namespace std;

class DataViewer
{
public:
	DataViewer(void);
	DataViewer(string terrainFile, string lavaFile, string temperatureFile, 
		string terrainTextureFile = "", bool displayListOn = true, GLfloat worldDimension = 1.0f, bool texellated = false);
	~DataViewer(void);

	inline GLuint getRows();
	inline GLuint getColumns();

	void drawDataSet();
	void drawHUDInfo(GLfloat windowWidth, GLfloat windowHeight);

private:
	inline GLfloat* normalize(GLfloat x, GLfloat y, GLfloat z);
	inline void printString(int x, int y,int z, const char *string);
	inline void drawSingleRows(GLuint i);
	inline void drawSingleRowsTexellated(GLuint i);
	inline void resetWorldSettings();

	// Fields
	// Models
	DataSet terrain;
	DataSet lava;
	DataSet temperature;
	bool lavaAndTemperatureSettted;
	//Texture
	bool textureOn;
	// Dispaly List
	bool displayListOn;
	GLuint baseList;
	GLuint* indexList;
	// Texellation
	GLboolean texellated;
	//
	GLfloat worldDimension;
	// Bounding Box Data
	GLfloat xData[8]; 
	GLfloat yData[8]; 
	GLfloat zData[8]; 
};

