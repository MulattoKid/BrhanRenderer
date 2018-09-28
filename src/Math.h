#ifndef MATH_H
#define MATH_H

struct Float
{
	float f;
	
	Float();
	Float(float f);
	
	Float& operator+=(const Float& F);
	Float& operator-=(const Float& F);
	Float& operator*=(const Float& F);
	Float& operator/=(const Float& F);
};
Float operator+(Float lhs, const Float& rhs);
Float operator-(Float lhs, const Float& rhs);
Float operator*(Float lhs, const Float& rhs);
Float operator/(Float lhs, const Float& rhs);

struct Vec3
{
	Float x, y, z;
	
	Vec3();
	Vec3(Float v);
	Vec3(Float x, Float y, Float z);
	
	
	Vec3& operator+=(const Vec3& v);
	Vec3& operator-=(const Vec3& v);
	Vec3& operator*=(const Vec3& v);
	Vec3& operator/=(const Vec3& v);
	
	Vec3& operator+=(const Float& v);
	Vec3& operator-=(const Float& v);
	Vec3& operator*=(const Float& v);
	Vec3& operator/=(const Float& v);
};
Vec3 operator+(Vec3 lhs, const Vec3& rhs);
Vec3 operator-(Vec3 lhs, const Vec3& rhs);
Vec3 operator*(Vec3 lhs, const Vec3& rhs);
Vec3 operator/(Vec3 lhs, const Vec3& rhs);
Vec3 operator+(Vec3 lhs, const Float& rhs);
Vec3 operator-(Vec3 lhs, const Float& rhs);
Vec3 operator*(Vec3 lhs, const Float& rhs);
Vec3 operator/(Vec3 lhs, const Float& rhs);

Float Pow(const Float& F);
Float Sqrt(const Float& F);
Float Sin(const Float& F);
Float ASin(const Float& F);
Float Cos(const Float& F);
Float ACos(const Float& F);
Float Abs(const Float& F);
Vec3 Abs(const Vec3& v);
Float Length(const Vec3& v);
Vec3 Normalize(const Vec3& v);
Vec3 Cross(const Vec3& v0, const Vec3& v1);
Float Dot(const Vec3& v0, const Vec3& v1);

#endif
