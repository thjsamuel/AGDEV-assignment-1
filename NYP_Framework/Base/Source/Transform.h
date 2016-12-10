#pragma once
#include "Mtx44.h"
#include "UpdateTransform.h"

class CTransform
{
protected:
	Mtx44 Mtx, DefaultMtx;
	CUpdateTransformation* theUpdatedTransform;
public:
	enum AXIS
	{
		X_AXIS = 0,
		Y_AXIS,
		Z_AXIS,
		NUM_AXIS
	};

	// Default Constructor
	CTransform(void);
	// Overloaded Constructor
	CTransform(const float dx, const float dy, const float dz);
	// Destructor
	~CTransform(void);

	void ApplyTranslate(const float dx, const float dy, const float dz);
	void GetTranslate(float& x, float& y, float& z);
	void ApplyRotate(const float angle, const float rx, const float ry, const float rz);
	float GetRotate(const AXIS theAxis);
	void SetScale(const float sx, const float sy, const float sz);
	void GetScale(float& x, float& y, float& z);
	// Apply a Transformation Matrix to the Transformation Matrix here
	void ApplyTransform(Mtx44 newMTX);
	// Reset the transformation matrix to identity matrix
	void Reset(void); //reset to identity
	// Get the transformation matrix
	Mtx44 GetTransform(void);
	void SetUpdateTransformation(CUpdateTransformation* theUpdateTransformation = NULL);
	Mtx44 GetUpdateTransform(void);																																																
	// Print Self
	void PrintSelf(void) const;
};