
#include"stdafx.h"
#include<Windows.h>
#define VECTOR_DLL
#include "CVector3_Dll.h"
#include"MathHelper.h"

CVector3_Dll::CVector3_Dll() :
	X(0.0f),
	Y(0.0f),
	Z(0.0f)
{
}

CVector3_Dll::CVector3_Dll(float _x, float _y, float _z) :
	X(_x), Y(_y), Z(_z)
{
}

CVector3_Dll::CVector3_Dll(const CVector3_Dll & other)
{
	X = other.X;
	Y = other.Y;
	Z = other.Z;
}

CVector3_Dll::~CVector3_Dll()
{
}

CVector3_Dll & CVector3_Dll::operator=(const CVector3_Dll & other)
{
	X = other.X;
	Y = other.Y;
	Z = other.Z;

	return *this;
}

CVector3_Dll CVector3_Dll::operator+(const CVector3_Dll & other)
{
	CVector3_Dll res;
	res.setValues(
		this->X + other.X,
		this->Y + other.Y,
		this->Z + other.Z
	);
	return res;
}

CVector3_Dll & CVector3_Dll::operator+=(const CVector3_Dll & other)
{
	X += other.X;
	Y += other.Y;
	Z += other.Z;

	return *this;
}

CVector3_Dll CVector3_Dll::operator-(const CVector3_Dll & other)
{
	CVector3_Dll res;
	res.setValues(
		this->X + other.X,
		this->Y + other.Y,
		this->Z + other.Z
	);
	return res;
}

CVector3_Dll & CVector3_Dll::operator-=(const CVector3_Dll & other)
{
	X -= other.X;
	Y -= other.Y;
	Z -= other.Z;

	return *this;
}

CVector3_Dll CVector3_Dll::cross(const CVector3_Dll & other)
{
	CVector3_Dll res;
	res.setValues(
		this->Y*other.Z - this->Z*other.Y,
		this->Z*other.X - this->X*other.Z,
		this->X*other.Y - this->Y*other.X
	);
	return res;
}

float CVector3_Dll::dot(const CVector3_Dll & other)
{
	return ((this->X * other.X) + (this->Y * other.Y) + (this->Z * other.Z));
}

void CVector3_Dll::setValues(float _x, float _y, float _z)
{
	X = _x;
	Y = _y;
	Z = _z;
}

void CVector3_Dll::setValues(float * values)
{
	X = *(values + 0);
	Y = *(values + 1);
	Z = *(values + 2);
}

void CVector3_Dll::normalize()
{
	float mag = magnitude();
	X /= mag;
	Y /= mag;
	Z /= mag;
}

float CVector3_Dll::magnitude()
{
	float len = (X * X) + (Y * Y) + (Z * Z);
	float invSqRoot = quickReverseSqrt2(len);
	float sqRoot = 1 / invSqRoot;

	return sqRoot;
}

float CVector3_Dll::quickReverseSqrt(float number)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y = number;
	i = *(long *)&y;                       // evil floating point bit level hacking
	i = 0x5f3759df - (i >> 1);             // what the fuck? 
	y = *(float *)&i;
	y = y * (threehalfs - (x2 * y * y));   // 1st iteration
//	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

	return y;
}

float CVector3_Dll::quickReverseSqrt2(float number)
{
	union {
		float f;
		long i;
	} conv;

	float x2;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	conv.f = number;
	conv.i = 0x5f3759df - (conv.i >> 1);	// what the fuck? 
	conv.f = conv.f * (threehalfs - (x2 * conv.f * conv.f));
	return conv.f;
}
