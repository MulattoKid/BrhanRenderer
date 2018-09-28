#include <cmath>
#include <math.h>
#include "Math.h"

///////////////////////////
///////////////////////////
//////////FLOAT////////////
///////////////////////////
///////////////////////////
Float::Float()
{
	this->f = 0.0f;
}

Float::Float(float f)
{
	this->f = f;
}

inline Float operator+(Float lhs, const Float& rhs)
{
	lhs += rhs;
	return lhs;
}

inline Float& Float::operator+=(const Float& F)
{
	f += F.f;
	return *this;
}

inline Float operator-(Float lhs, const Float& rhs)
{
	lhs -= rhs;
	return lhs;
}

inline Float& Float::operator-=(const Float& F)
{
	f -= F.f;
	return *this;
}

inline Float operator*(Float lhs, const Float& rhs)
{
	lhs *= rhs;
	return lhs;
}

inline Float& Float::operator*=(const Float& F)
{
	f *= F.f;
	return *this;
}

inline Float operator/(Float lhs, const Float& rhs)
{
	lhs /= rhs;
	return lhs;
}

inline Float& Float::operator/=(const Float& F)
{
	f /= F.f;
	return *this;
}

///////////////////////////
///////////////////////////
//////////VEC3/////////////
///////////////////////////
///////////////////////////
Vec3::Vec3()
{
	this->x = Float(0.0f);
	this->y = Float(0.0f);
	this->z = Float(0.0f);
}

Vec3::Vec3(Float v)
{
	this->x = v;
	this->y = v;
	this->z = v;
}

Vec3::Vec3(Float x, Float y, Float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vec3 operator+(Vec3 lhs, const Vec3& rhs)
{
	lhs += rhs;
	return lhs;
}

Vec3& Vec3::operator+=(const Vec3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vec3 operator-(Vec3 lhs, const Vec3& rhs)
{
	lhs -= rhs;
	return lhs;
}

Vec3& Vec3::operator-=(const Vec3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vec3 operator*(Vec3 lhs, const Vec3& rhs)
{
	lhs *= rhs;
	return lhs;
}

Vec3& Vec3::operator*=(const Vec3& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

Vec3 operator/(Vec3 lhs, const Vec3& rhs)
{
	lhs /= rhs;
	return lhs;
}

Vec3& Vec3::operator/=(const Vec3& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}
	
Vec3 operator+(Vec3 lhs, const Float& rhs)
{
	lhs += rhs;
	return lhs;
}

Vec3& Vec3::operator+=(const Float& v)
{
	x += v;
	y += v;
	z += v;
	return *this;
}

Vec3 operator-(Vec3 lhs, const Float& rhs)
{
	lhs -= rhs;
	return lhs;
}

Vec3& Vec3::operator-=(const Float& v)
{
	x -= v;
	y -= v;
	z -= v;
	return *this;
}

Vec3 operator*(Vec3 lhs, const Float& rhs)
{
	lhs += rhs;
	return lhs;
}

Vec3& Vec3::operator*=(const Float& v)
{
	x *= v;
	y *= v;
	z *= v;
	return *this;
}

Vec3 operator/(Vec3 lhs, const Float& rhs)
{
	lhs /= rhs;
	return lhs;
}

Vec3& Vec3::operator/=(const Float& v)
{
	x /= v;
	y /= v;
	z /= v;
	return *this;
}

///////////////////////////
///////////////////////////
////////FUCNTIONS//////////
///////////////////////////
///////////////////////////
Float Pow(const Float& F, const float p)
{
	return Float(std::pow(F.f, p));
}

Float Sqrt(const Float& F)
{
	return Float(std::sqrt(F.f));
}

Float Sin(const Float& F)
{
	return Float(std::sin(F.f));
}

Float ASin(const Float& F)
{
	return Float(std::asin(F.f));
}

Float Cos(const Float& F)
{
	return Float(std::cos(F.f));
}

Float ACos(const Float& F)
{
	return Float(std::acos(F.f));
}

Float Abs(const Float& F)
{
	return Float(std::abs(F.f));
}

Vec3 Abs(const Vec3& v)
{
	return Vec3(Abs(v.x), Abs(v.y), Abs(v.z));
}

Float Length(const Vec3& v)
{
	return Float(Sqrt(Pow(v.x, 2.0f) + Pow(v.y, 2.0f) + Pow(v.z, 2.0f)));
}

Vec3 Normalize(const Vec3& v)
{
	Float length = Length(v);
	return v / length;
}

//https://www.mathsisfun.com/algebra/vectors-cross-product.html
Vec3 Cross(const Vec3& v0, const Vec3& v1)
{
	Float x = (v0.y * v1.z) - (v0.z * v1.y);
	Float y = (v0.z * v1.x) - (v0.x * v1.z);
	Float z = (v0.x * v1.y) - (v0.y * v1.x);
	return Vec3(x, y, z);
}

//https://www.mathsisfun.com/algebra/vectors-dot-product.html
Float Dot(const Vec3& v0, const Vec3& v1)
{
	return Float((v0.x * v1.x) + (v0.y * v1.y) + (v0.z * v1.z));
}
