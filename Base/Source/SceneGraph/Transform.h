#pragma once
#include "Mtx44.h"
#include "UpdateTransformation.h"
class CSceneNode;

class CTransform
{
protected:
	Mtx44 Mtx, Update_Mtx, DefaultMtx;
	CUpdateTransformation*theUpdateTransformation;

public:
	CTransform(void);
	CTransform(const float dx, const float dy, const float dz);
	~CTransform(void);

	void PreRendering();
	void Draw(void);
	void PostRendering();

	enum AXIS
	{
		X_AXIS = 0,
		Y_AXIS,
		Z_AXIS,
		NUM_AXIS
	};

	// Apply a translation to the Transformation Matrix
	void ApplyTranslate(const float dx, const float dy, const float dz);
	// Get the translation from the Transformation Matrix
	void GetTranslate(float& x, float& y, float& z);
	// Apply a rotation to the Transformation Matrix
	void ApplyRotate(const float angle, const float rx, const float ry, const float rz);
	// Get the rotation from the Transformation Matrix
	float GetRotate(const AXIS theAxis);
	// Get the scale from the Transformation Matrix
	void GetScale(float& x, float& y, float& z);
	//Apply a Transformation Matrix to the Transformation Matrix here
	void ApplyTransform(Mtx44 newMTX);
	// Reset the transformation matrix to identity matrix
	void Reset(void); //reset to identity
	//Get the transformation matrix
	Mtx44 GetTransform(void);

	//Set the update Transformation
	void SetUpdateTransformation(CUpdateTransformation* theUpdateTransformation = NULL);
	//Get the update transformation matrix
	Mtx44 GetUpdateTransform(void);

	//print self
	void PrintSelf(void)const;

	void SetTranslate(const float dx, const float dy, const float dz);
	void SetRotate(const float angle, const float rx, const float ry, const float rz);
	void SetRotate2(const float angle, const float rx, const float ry, const float rz, const float offset_x, const float offset_y, const float offset_z);
	void SetScale(const float sx, const float sy, const float sz);
	void GetOffset(float& x, float& y, float& z);

	void reset(void); //reset to identity
};
