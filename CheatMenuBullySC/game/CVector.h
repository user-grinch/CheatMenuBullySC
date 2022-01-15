#pragma once
class CVector
{
public:
	float x, y, z;

	CVector(float x = 0.0f, float y = 0.0f, float z = 0.0f)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	CVector(const CVector& vec)
	{
		this->x = vec.x;
		this->y = vec.y;
		this->z = vec.z;
	}

	// Basic math operations
	CVector operator=(const CVector& vec)
	{
		this->x = vec.x;
		this->y = vec.y;
		this->z = vec.z;

		return *this;
	}

	CVector operator+(const CVector& vec)
	{
		this->x += vec.x;
		this->y += vec.y;
		this->z += vec.z;

		return *this;
	}

	CVector operator-(const CVector& vec)
	{
		this->x -= vec.x;
		this->y -= vec.y;
		this->z -= vec.z;

		return *this;
	}

	CVector operator*(const CVector& vec)
	{
		this->x *= vec.x;
		this->y *= vec.y;
		this->z *= vec.z;

		return *this;
	}

	CVector operator/(const CVector& vec)
	{
		this->x /= vec.x;
		this->y /= vec.y;
		this->z /= vec.z;

		return *this;
	}

	bool operator==(const CVector& vec)
	{
		return (this->x == vec.x && this->y == vec.y && this->z == vec.z);
	}
};

