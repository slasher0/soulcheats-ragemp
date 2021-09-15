#pragma once
#include "imgui/imgui.h"
#include "Config.h"
#include <cmath>
#pragma warning(disable: 4996)


struct Vector2
{
	float x{};
	float y{};
};
struct _Vector3
{
	float x{};
	float y{};
	float z{};
};
struct Vector4
{
	float x{};
	float y{};
	float z{};
	float w{};
};

class Vector3
{
public:
	float x, y, z;

	Vector3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vector3()
	{
		x = y = z = 0;
	}
	float length()
	{
		return (float)sqrt(x * x + y * y + z * z);
	}
	float dotproduct(Vector3 dot)
	{
		return (x * dot.x + y * dot.y + z * dot.z);
	}

	float Distance(Vector3& from, Vector3& to)
	{
		return Vector3(from.x - to.x, from.y - to.y, from.z - to.z).length();
	}
};


#pragma pack(push, 1)
class scrVector
{
public:
	scrVector() = default;
	scrVector(float x, float y, float z) : x(x), y(y), z(z) {}
public:
	float x{};
private:
	char m_padding1[0x04];
public:
	float y{};
private:
	char m_padding2[0x04];
public:
	float z{};
private:
	char m_padding3[0x04];
};
#pragma pack(pop)