#include "DataSet.h"
#include "tga.h"
#include "utils.h"

#include <iostream>
#include <fstream>
#include <math.h>

DataSet::DataSet(void){
	fileName = "";
	// Model
	rows = 0.0f;
	columns = 0.0f;
	xllCorner = 0.0f;
	yllCorner = 0.0f;
	cellSize = 1.0f;
	noDataValue = -1.0f;
	maxHeight = 0.0f;
	minHeight = 0.0f;
	data = NULL;
	// Texture
	tboID = 0;
}

DataSet::DataSet(string fileName){
	this->fileName = fileName;
	DataSet();
}

DataSet::~DataSet(void){
	if(data != NULL){
		for(int i=0; i<rows; i++){
			if(data[i] != NULL);
			delete data[i];
		}
		delete data;
	}

	glDeleteTextures(1, &tboID);
}

void DataSet::readData(){

	if(fileName == ""){
		return;
	}

	fstream file;
	file.open(fileName, ios::in);

	if(!file){
		cerr << fileName <<"\tError file opening..." << endl;
		system("PAUSE");
		exit(EXIT_FAILURE);
	}

	string tmp;
	// Colonne della matrice
	file >> tmp;	file >> columns; 
	// Righe della matrice
	file >> tmp;	file >> rows; 
	// xll corner
	file >> tmp;	file >> xllCorner;	
	// yll corner
	file >> tmp;	file >> yllCorner;
	// Cell Size
	file >> tmp;	file >> cellSize;
	// NODATA_value
	file >> tmp;	file >> noDataValue;

	maxHeight = 0x0000;
	minHeight = 0xFFFF;

	data = new GLdouble* [rows];
	bool findNoDataValue = false;
	int k = 0;
	for(int i=0; i<rows; i++){
		data[i] = new GLdouble[columns];
		for(int j=0; j<columns; j++){
			k++;
			file >> data[i][j];
			if(data[i][j] == noDataValue){
				findNoDataValue = true;
				continue;
			}
			if(data[i][j] > maxHeight){
				maxHeight = data[i][j];
			}
			if(data[i][j] < minHeight){
				minHeight = data[i][j];
			}
		}
	}
	file.close();

	// Correction
	if(findNoDataValue) {
		for(int i=0; i<rows; i++){
			for(int j=0; j<columns; j++){
				if(data[i][j] == noDataValue){
					data[i][j] = minHeight;
				}
			}
		}
	}

	drawDataOnConsole();
}

void DataSet::readTexture(string textureFileName){
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &tboID);
	glBindTexture(GL_TEXTURE_2D, tboID);
	GLbyte *pBytes;
	pBytes = gltLoadTGA(textureFileName.data(), &iWidth, &iHeight, &iComponents, &eFormat);
	glTexImage2D(GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBytes);
	delete pBytes;	
	glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void DataSet::activeTexture() const {
	glBindTexture(GL_TEXTURE_2D, tboID);
}

const GLfloat* DataSet::getDataAt(GLint i, GLint j){
	if(i>=0 && i<rows && j>=0 && j<columns){
		const GLfloat result = data[i][j];
		return &result;
	}
	return NULL;
}

void DataSet::drawDataOnConsole(){
	cout << "+--------------------------------------------------+" << endl;
	cout << "DataSet: " << fileName << endl;
	cout << "Rows: " << rows << "\tColumns: " << columns << endl;
	cout << "xllCorner: " << xllCorner << "\tyllCorner: " << yllCorner << endl;
	cout << "Cell Size: " << cellSize << endl;
	cout << "Min: " << minHeight << endl;
	cout << "Max: " << maxHeight << endl;
	cout << "noDataValue: " << noDataValue << endl;
	cout << "+--------------------------------------------------+" << endl;
}