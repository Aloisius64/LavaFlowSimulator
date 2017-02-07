#ifndef MYCAMERA_H
#define MYCAMERA_H

#include <cstdlib>
#include <cmath>

#define PI 3.1415926535897932384626433832795

typedef struct MyVector3f_Struct {
	GLfloat x;
	GLfloat y;
	GLfloat z;
} MyVector3f;

MyVector3f VectorNormalized(MyVector3f axis){
	MyVector3f normalized;
	GLfloat sum = axis.x*axis.x+axis.y*axis.y+axis.z*axis.z;
	GLfloat norma = sqrt(sum);
	normalized.x = axis.x/norma;
	normalized.y = axis.y/norma;
	normalized.z = axis.z/norma; 
	return normalized;
}

GLfloat ScalarProduct(MyVector3f input, MyVector3f normalizedAxis){
	return input.x*normalizedAxis.x+input.y*normalizedAxis.y+input.z*normalizedAxis.z;
}

MyVector3f VectorProduct(MyVector3f a, MyVector3f b){
	MyVector3f c;
	c.x = a.y*b.z - a.z*b.y;
	c.y = -(a.x*b.z - a.z*b.x);
	c.z = a.x*b.y-a.y*b.x;
	return VectorNormalized(c);
	//return c;
}

class MyCamera {
public:
	MyCamera();

	// Translate
	void moveRight(GLfloat delta);
	void moveLeft(GLfloat delta);
	void moveUp(GLfloat delta);
	void moveDown(GLfloat delta);
	void moveForward(GLfloat delta);
	void moveBackward(GLfloat delta);
	// Rotate
	void rotateX(GLfloat angle);
	void rotateY(GLfloat angle);
	void rotateZ(GLfloat angle);
	MyVector3f RotateAroundAnyAxis(MyVector3f input, MyVector3f axis, GLfloat degrees);
//private:
	MyVector3f position;
	MyVector3f upVector;
	MyVector3f direction;
	MyVector3f rightVector;
};

MyCamera::MyCamera(){
	position.x = 0.0f;
	position.y = 0.0f;
	position.z = 0.0f;

	direction.x = 0.0f;
	direction.y = 0.0f;
	direction.z = -1.0f;

	upVector.x = 0.0f;
	upVector.y = 1.0f;
	upVector.z = 0.0f;

	rightVector = VectorProduct(direction, upVector);
}

void MyCamera::moveRight(GLfloat delta){
	position.x += delta*rightVector.x;
	//position.y += delta*rightVector.y;
	position.z += delta*rightVector.z;
}

void MyCamera::moveLeft(GLfloat delta){
	position.x -= delta*rightVector.x;
	//position.y -= delta*rightVector.y;
	position.z -= delta*rightVector.z;
}

void MyCamera::moveUp(GLfloat delta){
	position.x += delta*upVector.x;
	position.y += delta*upVector.y;
	position.z += delta*upVector.z;
}

void MyCamera::moveDown(GLfloat delta){
	position.x -= delta*upVector.x;
	position.y -= delta*upVector.y;
	position.z -= delta*upVector.z;
}

void MyCamera::moveForward(GLfloat delta){
	position.x += delta*direction.x;
	position.y += delta*direction.y;
	position.z += delta*direction.z;
}

void MyCamera::moveBackward(GLfloat delta){
	position.x -= delta*direction.x;
	position.y -= delta*direction.y;
	position.z -= delta*direction.z;
}

void MyCamera::rotateX(GLfloat angle){
	direction = RotateAroundAnyAxis(direction, rightVector, angle);
	upVector = RotateAroundAnyAxis(upVector, rightVector, angle);
	//upVector = VectorProduct(direction, rightVector);
}

void MyCamera::rotateY(GLfloat angle){
	direction = RotateAroundAnyAxis(direction, upVector, angle);
	rightVector = RotateAroundAnyAxis(rightVector, upVector, angle);
	//rightVector = VectorProduct(direction, upVector);
}

void MyCamera::rotateZ(GLfloat angle){
	upVector = RotateAroundAnyAxis(upVector, direction, angle);
	rightVector = RotateAroundAnyAxis(rightVector, direction, angle);
	//rightVector = VectorProduct(upVector, direction);
}

MyVector3f MyCamera::RotateAroundAnyAxis(MyVector3f input, MyVector3f axis, GLfloat degrees){
	MyVector3f output;
	GLfloat radians = degrees*PI/180;
	MyVector3f normalizedAxis = VectorNormalized(axis);
	const GLfloat C = cosf(radians);
	const GLfloat S = sinf(radians);
	const GLfloat DOT = ScalarProduct(input, normalizedAxis);
	output.x = normalizedAxis.x*DOT*(1-C) + C*input.x + ((-normalizedAxis.z)*(input.y) + (normalizedAxis.y)*(input.z))*S;
    output.y = normalizedAxis.y*DOT*(1-C) + C*input.y + ((normalizedAxis.z)*(input.x) - (normalizedAxis.x)*(input.z))*S;
    output.z = normalizedAxis.z*DOT*(1-C) + C*input.z + ((-normalizedAxis.y)*(input.x) + (normalizedAxis.x)*(input.y))*S;
	return VectorNormalized(output);
	//return output;
}

#endif