#ifndef MATH_H
#define MATH_H

#include <cmath>
#include "glm/vec3.hpp"
#include <limits>
#include <math.h>

//p.214
static const float machine_epsilon = std::numeric_limits<float>::epsilon() * 0.5f;
//p.217
inline float Gamma(int n)
{
	return (n * machine_epsilon) / (1.0f - n * machine_epsilon);
}

//https://github.com/mmp/pbrt-v3/blob/5c81f476136d7ccb94feb300cb6aa34532746bca/src/core/efloat.h
struct EFloat
{
	float f;
	float err;
	
	EFloat() { this->f = 0.0f; this->err = 0.0f; }
	EFloat(float f) { this->f = f;  this->err = 0.0f; }
	EFloat& operator+=(const EFloat& F)
	{
		err = err + F.err + Gamma(1) * (std::abs(f + F.f) + err + F.err);
		f += F.f;
		return *this;
	}
	EFloat& operator-=(const EFloat& F)
	{
		err = err + F.err + Gamma(1) * (std::abs(f - F.f) + err + F.err);
		f -= F.f;
		return *this;
	}
	EFloat& operator*=(const EFloat& F)
	{
		err = std::abs(F.f * err) + std::abs(f * F.err) + (err * F.err) + (Gamma(1) * std::abs(f * F.f));
		f *= F.f;
		return *this;
	}
	EFloat& operator/=(const EFloat& F)
	{
		err = (std::abs(f) + err) / (std::abs(F.f) - F.err) - std::abs(f / F.f) + (Gamma(1) * (std::abs(f) + err) / (std::abs(F.f) - F.err));
		f /= F.f;
		return *this;
	}
};
inline EFloat operator+(EFloat lhs, const EFloat& rhs)
{
	lhs += rhs;
	return lhs;
}
inline EFloat operator-(EFloat F)
{
	F.f = -F.f;
	return F;
}
inline EFloat operator-(EFloat lhs, const EFloat& rhs)
{
	lhs -= rhs;
	return lhs;
}
inline EFloat operator*(EFloat lhs, const EFloat& rhs)
{
	lhs *= rhs;
	return lhs;
}
inline EFloat operator/(EFloat lhs, const EFloat& rhs)
{
	lhs /= rhs;
	return lhs;
}
inline bool operator<(const EFloat& lhs, const EFloat& rhs)
{
	return lhs.f < rhs.f ? true : false;
}
inline bool operator<=(const EFloat& lhs, const EFloat& rhs)
{
	return lhs.f <= rhs.f ? true : false;
}
inline bool operator>(const EFloat& lhs, const EFloat& rhs)
{
	return lhs.f > rhs.f ? true : false;
}
inline bool operator>=(const EFloat& lhs, const EFloat& rhs)
{
	return lhs.f >= rhs.f ? true : false;
}
inline bool operator==(const EFloat& lhs, const EFloat& rhs)
{
	return lhs.f == rhs.f ? true : false;
}
inline bool operator!=(const EFloat& lhs, const EFloat& rhs)
{
	return lhs.f != rhs.f ? true : false;
}

struct Vec3
{
	EFloat x, y, z;
	
	Vec3()
	{
		this->x = EFloat(0.0f);
		this->y = EFloat(0.0f);
		this->z = EFloat(0.0f);
	}
	Vec3(EFloat v)
	{
		this->x = v;
		this->y = v;
		this->z = v;
	}
	Vec3(EFloat x, EFloat y, EFloat z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	Vec3(const glm::vec3& v)
	{
		this->x = v.x;
		this->y = v.y;
		this->z = v.z;
	}
	
	Vec3& operator+=(const Vec3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	Vec3& operator-=(const Vec3& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}
	Vec3& operator*=(const Vec3& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}
	Vec3& operator/=(const Vec3& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}
	
	Vec3& operator+=(const EFloat& v)
	{
		x += v;
		y += v;
		z += v;
		return *this;
	}
	Vec3& operator-=(const EFloat& v)
	{
		x -= v;
		y -= v;
		z -= v;
		return *this;
	}
	Vec3& operator*=(const EFloat& v)
	{
		x *= v;
		y *= v;
		z *= v;
		return *this;
	}
	Vec3& operator/=(const EFloat& v)
	{
		x /= v;
		y /= v;
		z /= v;
		return *this;
	}
};
inline Vec3 operator+(Vec3 lhs, const Vec3& rhs)
{
	lhs += rhs;
	return lhs;
}
inline Vec3 operator-(Vec3 v)
{
	v.x = -v.x;
	v.y = -v.y;
	v.z = -v.z;
	return v;
}
inline Vec3 operator-(Vec3 lhs, const Vec3& rhs)
{
	lhs -= rhs;
	return lhs;
}
inline Vec3 operator*(Vec3 lhs, const Vec3& rhs)
{
	lhs *= rhs;
	return lhs;
}
inline Vec3 operator/(Vec3 lhs, const Vec3& rhs)
{
	lhs /= rhs;
	return lhs;
}
inline Vec3 operator+(Vec3 lhs, const EFloat& rhs)
{
	lhs += rhs;
	return lhs;
}
inline Vec3 operator-(Vec3 lhs, const EFloat& rhs)
{
	lhs -= rhs;
	return lhs;
}
inline Vec3 operator*(Vec3 lhs, const EFloat& rhs)
{
	lhs *= rhs;
	return lhs;
}
inline Vec3 operator/(Vec3 lhs, const EFloat& rhs)
{
	lhs /= rhs;
	return lhs;
}

inline EFloat Pow(const EFloat& F, const float p)
{
	return EFloat(std::pow(F.f, p));
}
inline EFloat Sqrt(const EFloat& F)
{
	return EFloat(std::sqrt(F.f));
}
inline EFloat Sin(const EFloat& F)
{
	return EFloat(std::sin(F.f));
}
inline EFloat ASin(const EFloat& F)
{
	return EFloat(std::asin(F.f));
}
inline EFloat Cos(const EFloat& F)
{
	return EFloat(std::cos(F.f));
}
inline EFloat ACos(const EFloat& F)
{
	return EFloat(std::acos(F.f));
}
inline EFloat Abs(const EFloat& F)
{
	return EFloat(std::abs(F.f));
}
inline Vec3 Abs(const Vec3& v)
{
	return Vec3(Abs(v.x), Abs(v.y), Abs(v.z));
}
inline EFloat Length(const Vec3& v)
{
	return EFloat(Sqrt(Pow(v.x, 2.0f) + Pow(v.y, 2.0f) + Pow(v.z, 2.0f)));
}
inline Vec3 Normalize(const Vec3& v)
{
	return v / Length(v);
}
//https://www.mathsisfun.com/algebra/vectors-cross-product.html
inline Vec3 Cross(const Vec3& v0, const Vec3& v1)
{
	EFloat x = (v0.y * v1.z) - (v0.z * v1.y);
	EFloat y = (v0.z * v1.x) - (v0.x * v1.z);
	EFloat z = (v0.x * v1.y) - (v0.y * v1.x);
	return Vec3(x, y, z);
}
//https://www.mathsisfun.com/algebra/vectors-dot-product.html
inline EFloat Dot(const Vec3& v0, const Vec3& v1)
{
	return EFloat((v0.x * v1.x) + (v0.y * v1.y) + (v0.z * v1.z));
}

#endif
