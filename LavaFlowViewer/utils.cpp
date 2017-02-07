//#include "utils.h"
//    
//// Calculate the cross product of two vectors
//void gltVectorCrossProduct(const GLTVector3 vU, const GLTVector3 vV, GLTVector3 vResult)
//{
//	vResult[0] = vU[1]*vV[2] - vV[1]*vU[2];
//	vResult[1] = -vU[0]*vV[2] + vV[0]*vU[2];
//	vResult[2] = vU[0]*vV[1] - vV[0]*vU[1];
//}
//
//// Subtract one vector from another
//void gltSubtractVectors(const GLTVector3 vFirst, const GLTVector3 vSecond, GLTVector3 vResult) 
//{
//	vResult[0] = vFirst[0] - vSecond[0];
//	vResult[1] = vFirst[1] - vSecond[1];
//	vResult[2] = vFirst[2] - vSecond[2];
//}
//
//// Given three points on a plane in counter clockwise order, calculate the unit normal
//void gltGetNormalVector(const GLTVector3 vP1, const GLTVector3 vP2, const GLTVector3 vP3, GLTVector3 vNormal)
//{
//	GLTVector3 vV1, vV2;
//
//	gltSubtractVectors(vP2, vP1, vV1);
//	gltSubtractVectors(vP3, vP1, vV2);
//
//	gltVectorCrossProduct(vV1, vV2, vNormal);
//}
//
//void gltAverageNormals(const GLTVector3 N24, const GLTVector3 N46, const GLTVector3 N68, const GLTVector3 N82, GLTVector3 N)
//{
//	int i;
//	for (i=0; i<3; i++)
//		N[i] = N24[i] + N46[i] + N68[i] + N82[i];
//}
