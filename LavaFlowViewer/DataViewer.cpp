#include "DataViewer.h"
#include "tools.h"

DataViewer::DataViewer(void) : terrain(), lava(), temperature()
{
	textureOn = false;
	displayListOn = false;
	worldDimension = 1.0f;
}

DataViewer::DataViewer(string terrainFile, string lavaFile, string temperatureFile, 
					   string terrainTextureFile, bool displayListOn, GLfloat worldDimension, bool texellated) 
					   : terrain(terrainFile), lava(lavaFile), temperature(temperatureFile){
						   terrain.readData();
						   lavaAndTemperatureSettted = false;
						   if(lavaFile != ""){
							   lava.readData();
							   temperature.readData();
							   lavaAndTemperatureSettted = true;
						   }						 

						   textureOn = false;

						   // Texture
						   if(terrainTextureFile != ""){
							   terrain.readTexture(terrainTextureFile);
							   textureOn = true;
						   }

						   // Display Lists
						   baseList = 0;
						   indexList = NULL;
						   this->displayListOn = displayListOn;

						   // Texellation
						   this->texellated = texellated;

						   if(this->displayListOn){
							   GLint finalRows = getRows()-1;
							   if(texellated){
								   finalRows = getRows();
							   }
							   baseList = glGenLists(finalRows);
							   glListBase(baseList);
							   indexList = new GLuint [finalRows];
							   for(GLuint i=0; i<finalRows; i++){
								   indexList[i] = baseList * i;
								   glNewList(indexList[i], GL_COMPILE);
								   {
									   if(!texellated)
										   drawSingleRows(i);
									   else
										   drawSingleRowsTexellated(i);
								   }
								   glEndList();
							   }
						   }

						   this->worldDimension = worldDimension;

						   // Set Bounding Box
						   xData[0] = 0.0f;								yData[0] = terrain.minHeight;	zData[0] = 0.0f;
						   xData[1] = getColumns()*terrain.cellSize;	yData[1] = terrain.minHeight;	zData[1] = 0.0f;
						   xData[2] = 0.0f;								yData[2] = terrain.maxHeight;	zData[2] = 0.0f;
						   xData[3] = getColumns()*terrain.cellSize;	yData[3] = terrain.maxHeight;	zData[3] = 0.0f;
						   xData[4] = 0.0f;								yData[4] = terrain.maxHeight;	zData[4] = getRows()*terrain.cellSize;
						   xData[5] = getColumns()*terrain.cellSize;	yData[5] = terrain.maxHeight;	zData[5] = getRows()*terrain.cellSize;
						   xData[6] = 0.0f;								yData[6] = terrain.minHeight;	zData[6] = getRows()*terrain.cellSize;
						   xData[7] = getColumns()*terrain.cellSize;	yData[7] = terrain.minHeight;	zData[7] = getRows()*terrain.cellSize;
}

DataViewer::~DataViewer(void){
	if(displayListOn){
		delete indexList;
	}
}

GLuint DataViewer::getRows(){
	return terrain.rows;
}

GLuint DataViewer::getColumns(){
	return terrain.columns;
}

void DataViewer::drawDataSet(){
	glPushMatrix();	
	{
		resetWorldSettings();

		GLint finalRows = getRows()-1;
		if(texellated){
			finalRows = getRows();
		}

		if(displayListOn){
			glCallLists(finalRows, GL_UNSIGNED_INT, indexList);
		} else {
			for(GLuint i=0; i<finalRows; i++){
				if(!texellated){
					drawSingleRows(i);				
				} else {
					drawSingleRowsTexellated(i);
				}
			}
		}
	}
	glPopMatrix();
}

void DataViewer::drawHUDInfo(GLfloat windowWidth, GLfloat windowHeight){
	if(temperature.fileName == "") {
		return;
	}
	GLfloat temperatureDiff = temperature.maxHeight - temperature.minHeight;
	GLfloat color = 0.0f;

	glPushAttrib(GL_TEXTURE_BIT);{
		glDisable(GL_TEXTURE_2D);

		glPushAttrib(GL_LIGHTING_BIT);{
			glDisable(GL_LIGHTING);

			glPushMatrix();{
				glDepthMask(GL_FALSE);
				glDisable(GL_DEPTH_TEST);

				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				gluOrtho2D(0, windowWidth, 0, windowHeight); //left,right,bottom,top

				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();

				GLfloat distanceX = 75.0f;
				GLfloat distanceY = 50.0f;

				glBegin(GL_QUADS);{
					color =	(temperature.minHeight - temperature.minHeight)/temperatureDiff;
					glColor3f(1.0f, color, 0.0f);
					glVertex2f(windowWidth-distanceX*2.0f,	distanceY);
					glVertex2f(windowWidth-distanceX,	distanceY);

					color =	(temperature.maxHeight - temperature.minHeight)/temperatureDiff;
					glColor3f(1.0f, color, 0.0f);
					glVertex2f(windowWidth-distanceX,	windowHeight-distanceY);
					glVertex2f(windowWidth-distanceX*2.0f,	windowHeight-distanceY);
				}glEnd();

				glColor3f(1.0f, 1.0f, 1.0f);
				printString(windowWidth-distanceX,	distanceY, 0.0f, to_string((int)temperature.minHeight).c_str());
				printString(windowWidth-distanceX,	windowHeight-distanceY-15.0f, 0.0f, to_string((int)temperature.maxHeight).c_str());

				color = 0.5f;
				glColor3f(color, color, color);
				glBegin(GL_QUADS);{
					glVertex2f(windowWidth-distanceX*2.0f,	distanceY);
					glVertex2f(windowWidth-distanceX,	distanceY);

					glVertex2f(windowWidth-distanceX,	distanceY + 10.0f);
					glVertex2f(windowWidth-distanceX*2.0f,	distanceY + 10.0f);
				}glEnd();


				glDepthMask(GL_TRUE);
				glEnable(GL_DEPTH_TEST);
			}glPopMatrix();

			glEnable(GL_LIGHTING);
		}glPopAttrib();
		glEnable(GL_TEXTURE_2D);
	}glPopAttrib();
}

void DataViewer::printString(int x, int y,int z, const char *string){
	//set the position of the text in the window using the x and y coordinates
	glRasterPos2f(x,y);
	//get the length of the string to display
	int len = (int) strlen(string);
	//loop to display character by character
	for (int i = 0; i < len; i++){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
	}
}

void DataViewer::drawSingleRows(GLuint i){
	GLdouble heightDiff = terrain.maxHeight - terrain.minHeight;
	GLfloat temperatureDiff = temperature.maxHeight - temperature.minHeight;
	// Color Material
	GLfloat color = 0.5f;
	GLfloat colorTemperature = 0.0f;
	// Texture Coordinates
	GLfloat s = 0.0f; // j
	GLfloat t = 0.0f; // i
	// Normal
	GLTVector3 vNormal;

	// Terrain
	glPushMatrix();	{
		glBegin(GL_TRIANGLE_STRIP);{
			for(int j=0; j<getColumns(); j++){
				if(j%2==0){ // 1 Normal per Cell
					GLTVector3 vPoints[3] = {
						{j*terrain.cellSize, terrain.data[i][j], i*terrain.cellSize},
						{j*terrain.cellSize, terrain.data[i+1][j], (i+1)*terrain.cellSize},
						{(j+1)*terrain.cellSize, terrain.data[i][j+1], i*terrain.cellSize}};
					gltGetNormalVector(vPoints[0], vPoints[1], vPoints[2], vNormal);
					glNormal3fv(vNormal);
				}
				// Color
				glColor3f(color, color, color);
				// 1° Vertex
				// Texture
				t = ((getRows()-1) - i) / (GLfloat)(getRows()-1);
				s = j / (GLfloat)(getColumns()-1);
				glTexCoord2f(s, t);
				// Vertex
				glVertex3f(j*terrain.cellSize, terrain.data[i][j], i*terrain.cellSize);
				// --------------------------------------------------------------------
				// 2° Vertex
				// Texture
				t = ((getRows()-1) - (i+1)) / (GLfloat)((getRows()-1)-1);
				s = j / (GLfloat)(getColumns()-1);
				glTexCoord2f(s, t);
				// Vertex
				glVertex3f(j*terrain.cellSize, terrain.data[i+1][j], (i+1)*terrain.cellSize);
			}
		}glEnd();
	}glPopMatrix();
	// Lava And Temperature
	if(lavaAndTemperatureSettted){
		glPushMatrix();{
			glPushAttrib(GL_TEXTURE_BIT);{
				glDisable(GL_TEXTURE_2D);
				glBegin(GL_TRIANGLE_STRIP);{
					for(int j=0; j<getColumns(); j++){
						if(j%2==0){
							GLTVector3 vPoints[3] = {{j*lava.cellSize, lava.data[i][j], i*lava.cellSize},
							{j*lava.cellSize, lava.data[i+1][j], (i+1)*lava.cellSize},
							{(j+1)*lava.cellSize, lava.data[i][j+1], i*lava.cellSize}};
							gltGetNormalVector(vPoints[0], vPoints[1], vPoints[2], vNormal);
							glNormal3fv(vNormal);
						}

						// Color
						color =	(temperature.data[i][j]-temperature.minHeight)/temperatureDiff;
						glColor3f(1.0f, color, 0.0f);
						if(color==0.0f){
							glColor3f(0.5f, 0.5f, 0.5f);
						}
						// Vertex
						glVertex3f(j*terrain.cellSize, lava.data[i][j]+terrain.data[i][j], i*terrain.cellSize);

						// Color
						color =	(temperature.data[i+1][j]-temperature.minHeight)/temperatureDiff;
						glColor3f(1.0f, color, 0.0f);
						if(color==0.0f){
							glColor3f(0.5f, 0.5f, 0.5f);
						}
						// Vertex
						glVertex3f(j*terrain.cellSize,  lava.data[i+1][j]+terrain.data[i+1][j], (i+1)*terrain.cellSize);
					}
				}glEnd();
			}glPopAttrib();
		}glPopMatrix();
	}
}

void DataViewer::drawSingleRowsTexellated(GLuint i){
	GLdouble heightDiff = terrain.maxHeight - terrain.minHeight;
	GLfloat temperatureDiff = temperature.maxHeight - temperature.minHeight;
	// Color Material
	GLfloat color = 0.5f;
	GLfloat colorTemperature = 0.0f;
	// Texture Coordinates
	GLfloat s = 0.0f; // j
	GLfloat t = 0.0f; // i
	// Normal
	GLTVector3 vNormal;

	static const GLfloat halfCellSize = terrain.cellSize/2.0f; 
	static const GLint NOi[4] = {-1,0,-1,0};
	static const GLint NOj[4] = {-1,-1,0,0};
	static const GLint NEi[4] = {-1,0,-1,0};
	static const GLint NEj[4] = {0,0,1,1};
	static const GLint SOi[4] = {0,1,0,1};
	static const GLint SOj[4] = {-1,-1,0,0};
	static const GLint SEi[4] = {0,1,0,1};
	static const GLint SEj[4] = {0,0,1,1};
	static const GLint sign_i[4] = {-1,-1,1,1};
	static const GLint sign_j[4] = {-1,1,-1,1};

	glPushMatrix();{
		glBegin(GL_TRIANGLE_STRIP);{
			for(int j=0; j<getColumns(); j++){
				GLfloat heights[4] = {0.0f};
				GLfloat x[4] = {0.0f};
				GLfloat z[4] = {0.0f};
				GLint tmpCount = 0;
				GLfloat avg = 0.0f;

				for(GLint k=0; k<4; k++){
					if(terrain.getDataAt(i+NOi[k], j+NOj[k]) != NULL){
						tmpCount ++;
						avg += *terrain.getDataAt(i+NOi[k], j+NOj[k]);
					}
				}
				heights[0] = avg/tmpCount;
				x[0] = j*terrain.cellSize - halfCellSize;
				z[0] = i*terrain.cellSize - halfCellSize;

				tmpCount = 0;
				avg = 0.0f;
				for(GLint k=0; k<4; k++){
					if(terrain.getDataAt(i+SOi[k], j+SOj[k]) != NULL){
						tmpCount ++;
						avg += *terrain.getDataAt(i+SOi[k], j+SOj[k]);
					}
				}
				heights[1] = avg/tmpCount;
				x[1] = j*terrain.cellSize - halfCellSize;
				z[1] = i*terrain.cellSize + halfCellSize;

				tmpCount = 0;
				avg = 0.0f;
				for(GLint k=0; k<4; k++){
					if(terrain.getDataAt(i+NEi[k], j+NEj[k]) != NULL){
						tmpCount ++;
						avg += *terrain.getDataAt(i+NEi[k], j+NEj[k]);
					}
				}
				heights[2] = avg/tmpCount;
				x[2] = j*terrain.cellSize + halfCellSize;
				z[2] = i*terrain.cellSize - halfCellSize;

				tmpCount = 0;
				avg = 0.0f;
				for(GLint k=0; k<4; k++){
					if(terrain.getDataAt(i+SEi[k], j+SEj[k]) != NULL){
						tmpCount ++;
						avg += *terrain.getDataAt(i+SEi[k], j+SEj[k]);
					}
				}
				heights[3] = avg/tmpCount;
				x[3] = j*terrain.cellSize + halfCellSize;
				z[3] = i*terrain.cellSize + halfCellSize;


				// Normal
				{
					GLTVector3 vPoints[3] = {
						{x[0], heights[0], z[0]},
						{x[1], heights[1], z[1]},
						{x[2], heights[2], z[2]}
					};
					gltGetNormalVector(vPoints[0], vPoints[1], vPoints[2], vNormal);
					glNormal3fv(vNormal);
				}

				// Color
				glColor3f(color, color, color);

				// Vertex
				for(GLint k=0; k<4; k++){
					// Texture
					t = ((getRows()-1) - i) / (GLfloat)(getRows()-1);
					s = j / (GLfloat)(getColumns()-1);
					glTexCoord2f(s, t);
					// Vertex
					glVertex3f(x[k], heights[k], z[k]);
				}
			}
		}glEnd();
	}glPopMatrix();

	if(lavaAndTemperatureSettted){
		glPushMatrix();{
			glPushAttrib(GL_TEXTURE_BIT);{
				glDisable(GL_TEXTURE_2D);
				glBegin(GL_TRIANGLE_STRIP);{
					for(int j=0; j<getColumns(); j++){
						GLfloat heights[4];
						GLfloat terrainHeights[4];
						terrainHeights[0] = terrainHeights[1] = terrainHeights[2] = terrainHeights[3] = 0.0f;
						GLfloat x[4];
						GLfloat z[4];

						int tmpCount = 0;
						GLfloat avg = 0.0f;
						for(int k=0; k<4; k++){
							if(lava.getDataAt(i+NOi[k], j+NOj[k]) != NULL){
								tmpCount ++;
								avg += *lava.getDataAt(i+NOi[k], j+NOj[k]);
								terrainHeights[0] += *terrain.getDataAt(i+NOi[k], j+NOj[k]);
							}
						}
						heights[0] = avg/tmpCount;
						terrainHeights[0] /= tmpCount;
						x[0] = j*terrain.cellSize - halfCellSize;
						z[0] = i*terrain.cellSize - halfCellSize;

						tmpCount = 0;
						avg = 0.0f;
						for(int k=0; k<4; k++){
							if(lava.getDataAt(i+SOi[k], j+SOj[k]) != NULL){
								tmpCount ++;
								avg += *lava.getDataAt(i+SOi[k], j+SOj[k]);
								terrainHeights[1] += *terrain.getDataAt(i+SOi[k], j+SOj[k]);
							}
						}
						heights[1] = avg/tmpCount;
						terrainHeights[1] /= tmpCount;
						x[1] = j*terrain.cellSize - halfCellSize;
						z[1] = i*terrain.cellSize + halfCellSize;

						tmpCount = 0;
						avg = 0.0f;
						for(int k=0; k<4; k++){
							if(lava.getDataAt(i+NEi[k], j+NEj[k]) != NULL){
								tmpCount ++;
								avg += *lava.getDataAt(i+NEi[k], j+NEj[k]);
								terrainHeights[2] += *terrain.getDataAt(i+NEi[k], j+NEj[k]);
							}
						}
						heights[2] = avg/tmpCount;
						terrainHeights[2] /= tmpCount;
						x[2] = j*terrain.cellSize + halfCellSize;
						z[2] = i*terrain.cellSize - halfCellSize;

						tmpCount = 0;
						avg = 0.0f;
						for(int k=0; k<4; k++){
							if(terrain.getDataAt(i+SEi[k], j+SEj[k]) != NULL){
								tmpCount ++;
								avg += *lava.getDataAt(i+SEi[k], j+SEj[k]);
								terrainHeights[3] += *terrain.getDataAt(i+SEi[k], j+SEj[k]);
							}
						}
						heights[3] = avg/tmpCount;
						terrainHeights[3] /= tmpCount;
						x[3] = j*terrain.cellSize + halfCellSize;
						z[3] = i*terrain.cellSize + halfCellSize;


						// Normal
						{
							GLTVector3 vPoints[3] = {{x[0], terrainHeights[0]+heights[0], z[0]},
							{x[1], terrainHeights[1]+heights[1], z[1]},
							{x[2], terrainHeights[2]+heights[2], z[2]}};
							gltGetNormalVector(vPoints[0], vPoints[1], vPoints[2], vNormal);
							glNormal3fv(vNormal);
						}

						// Color
						color =	(temperature.data[i][j]-temperature.minHeight)/temperatureDiff;
						glColor3f(1.0f, color, 0.0f);
						if(color==0.0f){
							glColor3f(0.5f, 0.5f, 0.5f);
						}

						// Vertex
						for(int k=0; k<4; k++){
							// Normal
							glNormal3f(x[k], terrainHeights[k]+heights[k], z[k]);
							// Vertex
							glVertex3f(x[k], terrainHeights[k]+heights[k], z[k]);
						}
					}
				}glEnd();
			}glPopAttrib();
		}glPopMatrix();
	} // if(lavaAndTemperatureSettted)
}

void DataViewer::resetWorldSettings(){
	GLint xTranslate = getRows() / 2;
	GLint zTranslate = getColumns() / 2;
	GLdouble heightDiff = terrain.maxHeight - terrain.minHeight;
	GLfloat scale = worldDimension / (getRows()*terrain.cellSize);
	GLfloat scaleZ = worldDimension / (getColumns()*terrain.cellSize);

	//glPushAttrib(GL_LIGHTING_BIT);{
	//	glDisable(GL_LIGHTING);
	//	glPushAttrib(GL_TEXTURE_BIT);{
	//		glDisable(GL_TEXTURE_2D);
	//		glColor3f(0.5f, 0.5f, 0.5f);
	//		glutWireCube(worldDimension*1.0f);
	//	}glPopAttrib();
	//}glPopAttrib();

	glScalef(scale, scale, scale);

	if(terrain.minHeight >= 0){
		glTranslatef(0.0f, -terrain.minHeight-(heightDiff/2), 0.0f);
	} else if(terrain.maxHeight <= 0){
		glTranslatef(0.0f, terrain.maxHeight+(heightDiff/2), 0.0f);
	} else {
		GLfloat absMinHeight = abs(terrain.minHeight);
		if(absMinHeight > terrain.maxHeight){
			glTranslatef(0.0f, (absMinHeight-terrain.maxHeight)/2.0f, 0.0f);
		} else if(absMinHeight < terrain.maxHeight){
			glTranslatef(0.0f, -(terrain.maxHeight-absMinHeight)/2.0f, 0.0f);
		}
	}

	glTranslatef(-(getColumns()*terrain.cellSize / 2.0f), 0.0f, -(getRows()*terrain.cellSize / 2.0f));

	glPushAttrib(GL_LIGHTING_BIT);{
		glDisable(GL_LIGHTING);
		glPushAttrib(GL_TEXTURE_BIT);{
			glDisable(GL_TEXTURE_2D);
			glColor3f(1.0f, 1.0f, 1.0f);

			glBegin(GL_LINE_LOOP);{
				glVertex3f(xData[0], yData[0], zData[0]);
				glVertex3f(xData[1], yData[1], zData[1]);
				glVertex3f(xData[3], yData[3], zData[3]);
				glVertex3f(xData[2], yData[2], zData[2]);
			}glEnd();

			glBegin(GL_LINE_LOOP);{
				glVertex3f(xData[4], yData[4], zData[4]);
				glVertex3f(xData[5], yData[5], zData[5]);
				glVertex3f(xData[7], yData[7], zData[7]);
				glVertex3f(xData[6], yData[6], zData[6]);
			}glEnd();

			glBegin(GL_LINES);{
				glVertex3f(xData[2], yData[2], zData[2]);
				glVertex3f(xData[4], yData[4], zData[4]);
			}glEnd();

			glBegin(GL_LINES);{
				glVertex3f(xData[3], yData[3], zData[3]);
				glVertex3f(xData[5], yData[5], zData[5]);
			}glEnd();

			glBegin(GL_LINES);{
				glVertex3f(xData[0], yData[0], zData[0]);
				glVertex3f(xData[6], yData[6], zData[6]);
			}glEnd();

			glBegin(GL_LINES);{
				glVertex3f(xData[1], yData[1], zData[1]);
				glVertex3f(xData[7], yData[7], zData[7]);
			}glEnd();
		}glPopAttrib();
	}glPopAttrib();
}

GLfloat* DataViewer::normalize(GLfloat x, GLfloat y, GLfloat z){
	GLfloat normalized[3] = {0.0f};
	GLfloat sum = x*x+y*y+z*z;
	GLfloat norma = sqrt(sum);
	normalized[0] = x/norma;
	normalized[1] = y/norma;
	normalized[2] = z/norma; 
	return normalized;
}