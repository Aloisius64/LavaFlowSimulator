#include "DataSet2.h"

#include <iostream>
#include <fstream>

DataSet2::DataSet2(void)
{
	//DataSet::DataSet();
}

DataSet2::DataSet2(string fileName){
	//DataSet::DataSet(fileName);
	this->fileName = fileName;
}

DataSet2::~DataSet2(void)
{
	//DataSet::~DataSet();
}

void DataSet2::readData(){

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
	columns++;
	// Righe della matrice
	file >> tmp;	file >> rows; 
	rows++;
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

	GLdouble** dataTmp;
	dataTmp = new GLdouble* [rows-1];
	bool findNoDataValue = false;
	int k = 0;
	for(int i=0; i<rows-1; i++){
		dataTmp[i] = new GLdouble[columns-1];
		for(int j=0; j<columns-1; j++){
			k++;
			file >> dataTmp[i][j];
			if(dataTmp[i][j] == noDataValue){
				findNoDataValue = true;
				continue;
			}
			if(dataTmp[i][j] > maxHeight){
				maxHeight = dataTmp[i][j];
			}
			if(dataTmp[i][j] < minHeight){
				minHeight = dataTmp[i][j];
			}
		}
	}
	file.close();

	// Correction
	if(findNoDataValue) {
		for(int i=0; i<rows; i++){
			for(int j=0; j<columns; j++){
				if(dataTmp[i][j] == noDataValue){
					dataTmp[i][j] = minHeight;
				}
			}
		}
	}

	//
	data = new GLdouble* [rows];
	for(int i=0; i<rows; i++){
		data[i] = new GLdouble[columns];
		for(int j=0; j<columns; j++){
			// Centrale
			if((i>0 && i<rows-1) && (j>0 && j<columns-1)){
				data[i][j] = (dataTmp[i-1][j-1]+
					dataTmp[i-1][j]+
					dataTmp[i][j-1]+
					dataTmp[i][j])/4.0f;
			} else if((i>0 && i<rows-1) && !(j>0 && j<columns-1)){ // colonne sx dx
				data[i][j] = (dataTmp[i][j-1]+dataTmp[i][j])/2.0f;
			} else if(!(i>0 && i<rows-1) && (j>0 && j<columns-1)){ // righe top bottom
				
				data[i][j] = (dataTmp[i-1][j]+dataTmp[i][j])/2.0f;
			} else {
				data[i][j] = 0.0f;
			}  
		}
	}

	for(int i=0; i<rows-1; i++){
			delete dataTmp[i];
	}
	delete dataTmp;

	drawDataOnConsole();
}
