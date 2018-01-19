#include "Transform.h"
#include <iostream>

using namespace std;

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#include <windows.h> // Header File For Windows
#include <gl\gl.h> // Header File For The OpenGL32 Library
#include <gl\glu.h> // Header File For The GLu32 Library

CTransform::CTransform(void)
	: theUpdateTransformation(NULL)
{
	Mtx.SetToIdentity();
	Update_Mtx.SetToIdentity();
}

void CTransform::reset(void){
	Mtx.SetToIdentity();
	Update_Mtx.SetToIdentity();
}

CTransform::CTransform( const float dx, const float dy, const float dz )
{
	Mtx.SetToTranslation( dx, dy, dz );
}

CTransform::~CTransform(void)
{
	if (theUpdateTransformation)
	{
		delete theUpdateTransformation;
		theUpdateTransformation = NULL;
	}
}

void CTransform::PreRendering()
{
	glPushMatrix();
		glMultMatrixf(Mtx.a);
}

void CTransform::Draw(void)
{
}

void CTransform::PostRendering()
{
	glPopMatrix();
}

void CTransform::ApplyTranslate(const float dx, const float dy, const float dz)
{
	Mtx44 TempMtx;
	TempMtx.SetToTranslation(dx, dy, dz);
	Mtx = Mtx*TempMtx;
}

void CTransform::GetTranslate(float & x, float & y, float & z)
{
	x = Mtx.a[12];
	y = Mtx.a[13];
	z = Mtx.a[14];

}

void CTransform::ApplyRotate(const float angle, const float rx, const float ry, const float rz)
{
	Mtx44 TempMtx;
	TempMtx.SetToRotation(angle, rx, ry, rz);
	Mtx = Mtx*TempMtx;

}

float CTransform::GetRotate(const AXIS theAxis)
{
	if (theAxis == X_AXIS)
	{
		float y = acos(Mtx.a[5]); // Equals to cos a
		float z = asin(-Mtx.a[9]); // Equals to sin a
		if (abs(y - z) < Math::EPSILON)
			return y;
	}
	else if (theAxis == Y_AXIS)
	{
		float x = acos(Mtx.a[0]); // Equals to cos a
		float z = asin(Mtx.a[8]); // Equals to -sin a
		if (abs(x - z) < Math::EPSILON)
			return x;
	}
	else // Z_AXIS
	{
		float x = acos(Mtx.a[0]); // Equals to cos a
		float y = asin(-Mtx.a[4]); // Equals to -sin a
		if (abs(x - y) < Math::EPSILON)
			return x;
	}
	return 0.0f;

}

void CTransform::GetScale(float & x, float & y, float & z)
{
	x = Mtx.a[0];
	y = Mtx.a[5];
	z = Mtx.a[10];
}

void CTransform::SetTranslate( const float dx, const float dy, const float dz  )
{
	Mtx44 TempMtx;
	TempMtx.SetToTranslation( dx, dy, dz );
	Mtx = Mtx*TempMtx;
}

//void CTransform::SetRotate( const float angle, const float rx, const float ry, const float rz  )
//{
//	Mtx44 TempMtx;
//	TempMtx.Rotatef( angle, rx, ry, rz );
//	
//	Mtx44 MtxBackToOrigin;
//
//	MtxBackToOrigin.matrix[ MtxBackToOrigin.INDEX4( 0, 3 ) ] = -Mtx.matrix[ Mtx.INDEX4( 0, 3 ) ];
//	MtxBackToOrigin.matrix[ MtxBackToOrigin.INDEX4( 1, 3 ) ] = -Mtx.matrix[ Mtx.INDEX4( 1, 3 ) ];
//	MtxBackToOrigin.matrix[ MtxBackToOrigin.INDEX4( 2, 3 ) ] = -Mtx.matrix[ Mtx.INDEX4( 2, 3 ) ];
//
//	Mtx44 MtxBackToPosition;
//	MtxBackToPosition.matrix[ MtxBackToPosition.INDEX4( 0, 3 ) ] = Mtx.matrix[ Mtx.INDEX4( 0, 3 ) ];
//	MtxBackToPosition.matrix[ MtxBackToPosition.INDEX4( 1, 3 ) ] = Mtx.matrix[ Mtx.INDEX4( 1, 3 ) ];
//	MtxBackToPosition.matrix[ MtxBackToPosition.INDEX4( 2, 3 ) ] = Mtx.matrix[ Mtx.INDEX4( 2, 3 ) ];
//
//	Mtx = Mtx * MtxBackToOrigin;
//	Mtx = Mtx * TempMtx;
//	Mtx = Mtx * MtxBackToPosition;
//}
void CTransform::SetRotate( const float angle, const float rx, const float ry, const float rz  ){
	Mtx44 TempMtx;
	TempMtx.SetToRotation( angle, rx, ry, rz );
	Mtx = Mtx*TempMtx;
}


//void CTransform::SetRotate2( const float angle, const float rx, const float ry, const float rz, 
//							 const float offset_x, const float offset_y, const float offset_z )
//{
//	Mtx44 TempMtx;
//	TempMtx.Rotatef( angle, rx, ry, rz );
//	
//	Mtx44 MtxBackToOrigin;
//
//	MtxBackToOrigin.matrix[ MtxBackToOrigin.INDEX4( 0, 3 ) ] = -offset_x;
//	MtxBackToOrigin.matrix[ MtxBackToOrigin.INDEX4( 1, 3 ) ] = -offset_y;
//	MtxBackToOrigin.matrix[ MtxBackToOrigin.INDEX4( 2, 3 ) ] = -offset_z;
//
//	Mtx44 MtxBackToPosition;
//	MtxBackToPosition.matrix[ MtxBackToPosition.INDEX4( 0, 3 ) ] = offset_x;
//	MtxBackToPosition.matrix[ MtxBackToPosition.INDEX4( 1, 3 ) ] = offset_y;
//	MtxBackToPosition.matrix[ MtxBackToPosition.INDEX4( 2, 3 ) ] = offset_z;
//
//	Mtx = Mtx * MtxBackToOrigin;
//	Mtx = Mtx * TempMtx;
//	Mtx = Mtx * MtxBackToPosition;
//}

void CTransform::SetScale( const float sx, const float sy, const float sz  )
{
	//Mtx.SetToScale( sx, sy, sz );

	float scaleX = sx, scaleY = sy, scaleZ = sz;
	if (scaleX == 0.0f)
		scaleX = 1.0f;
	if (scaleY == 0.0f)
		scaleY = 1.0f;
	if (scaleZ == 0.0f)
		scaleZ = 1.0f;
	Mtx.SetToScale(scaleX, scaleY, scaleZ);

}

void CTransform::ApplyTransform( Mtx44 newMTX )
{
	Mtx = Mtx * newMTX;
}

void CTransform::GetOffset( float& x, float& y, float& z )
{
	/*
	x = Mtx.a[ Mtx.INDEX4( 0, 3 ) ];
	y = Mtx.a[ Mtx.INDEX4( 1, 3 ) ];
	z = Mtx.a[ Mtx.INDEX4( 2, 3 ) ];
	*/
}

void CTransform::Reset(void)
{
	Mtx.SetToIdentity();
}

// Get the transformation matrix
Mtx44 CTransform::GetTransform(void)
{
	return Mtx;
}

void CTransform::PrintSelf(void) const
{
	cout << "======================================================================"
		<< endl;
	cout << "CTransform::PrintSelf" << endl;
	cout << "----------------------------------------------------------------------"
		<< endl;
	cout << "[\t" << Mtx.a[0] << "\t" << Mtx.a[4] << "\t" << Mtx.a[8] << "\t" <<
		Mtx.a[12] << "\t]" << endl;
	cout << "[\t" << Mtx.a[1] << "\t" << Mtx.a[5] << "\t" << Mtx.a[9] << "\t" <<
		Mtx.a[13] << "\t]" << endl;
	cout << "[\t" << Mtx.a[2] << "\t" << Mtx.a[6] << "\t" << Mtx.a[10] << "\t" <<
		Mtx.a[14] << "\t]" << endl;
	cout << "[\t" << Mtx.a[3] << "\t" << Mtx.a[7] << "\t" << Mtx.a[11] << "\t" <<
		Mtx.a[15] << "\t]" << endl;
	cout << "======================================================================"
		<< endl;
}

void CTransform::SetUpdateTransformation(CUpdateTransformation * theUpdateTransformation)
{
	this->theUpdateTransformation = theUpdateTransformation;
}

Mtx44 CTransform::GetUpdateTransform(void)
{
	if (theUpdateTransformation == NULL)
		return DefaultMtx;

	//Update theUpdateTransformation
	theUpdateTransformation->Update();

	return theUpdateTransformation->GetUpdateTransformation();
}
