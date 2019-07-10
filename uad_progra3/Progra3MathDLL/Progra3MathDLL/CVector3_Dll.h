#pragma once
#ifdef VECTOR_DLL
#define VECTOR_CLASS __declspec(dllexport)
#else
#define VECTOR_CLASS __declspec(dllexport)
#endif // VECTOR_DLL

class VECTOR_DLL CVector3_Dll {
public:
	float X, Y, Z;

public:
	CVector3_Dll();
	CVector3_Dll(float _x, float _y, float _z);
	CVector3_Dll(const CVector3_Dll &other);
	~CVector3_Dll();

	CVector3_Dll& operator=(const CVector3_Dll &other);

	CVector3_Dll operator+(const CVector3_Dll & other);
	CVector3_Dll& operator+=(const CVector3_Dll &other);

	CVector3_Dll operator-(const CVector3_Dll & other);
	CVector3_Dll& operator-=(const CVector3_Dll &other);

	CVector3_Dll cross(const CVector3_Dll & other);
	float dot(const CVector3_Dll & other);

	void setValues(float _x, float _y, float _z);
	void setValues(float *values);
	float getX() const { return X; }
	float getY() const { return Y; }
	float getZ() const { return Z; }
	void getValues(float *values) const { values[0] = X; values[1] = Y; values[2] = Z; }
	void normalize();
	float magnitude();

	inline static CVector3_Dll ZeroVector()
	{
		return CVector3_Dll(0.0f, 0.0f, 0.0f);
	}

private:
	float quickReverseSqrt(float number);
	float quickReverseSqrt2(float number);
};
