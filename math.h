#pragma once

#include <imgui.h>
#include <iostream>
#include <string>

#define M_PI 3.14159265358979323846f

const float Deg2Rad = M_PI / 180.0f;

struct Vector2
{
	float x, y;

	Vector2(float x = 0, float y = 0) : x(x), y(y) {}

	float magnitude() const
	{
		return std::sqrt(x * x + y * y);
	}

	Vector2 normalize() const
	{
		float mag = magnitude();
		return Vector2(x / mag, y / mag);
	}

	static float Distance(const Vector2& v1, const Vector2& v2)
	{
		float dx = v1.x - v2.x;
		float dy = v1.y - v2.y;
		return std::sqrt(dx * dx + dy * dy);
	}

	Vector2 operator+(const Vector2& other) const
	{
		return Vector2(x + other.x, y + other.y);
	}

	Vector2 operator-(const Vector2& other) const
	{
		return Vector2(x - other.x, y - other.y);
	}

	Vector2 operator*(float scalar) const
	{
		return Vector2(x * scalar, y * scalar);
	}

	friend std::ostream& operator<<(std::ostream& os, const Vector2& v)
	{
		os << "(" << v.x << ", " << v.y << ")";
		return os;
	}

	std::string to_string() {
		return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
	}


	operator ImVec2() const {
		return ImVec2(x, y);
	}
};

struct Vector3
{
	float x, y, z;

	Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

	float& operator[](int index) {
		switch (index) {
		case 0: return x;
		case 1: return y;
		case 2: return z;
		default:
			throw std::out_of_range("Index out of range in Vector3");
		}
	}

	const float& operator[](int index) const {
		switch (index) {
		case 0: return x;
		case 1: return y;
		case 2: return z;
		default:
			throw std::out_of_range("Index out of range in Vector3");
		}
	}

	friend std::ostream& operator<<(std::ostream& os, const Vector3& v)
	{
		os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
		return os;
	}

	std::string to_string() {
		return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
	}

	bool operator!=(const Vector3& rhs) const
	{
		return (x != rhs.x) || (y != rhs.y) || (z != rhs.z);
	}

	bool operator==(const Vector3& rhs) const
	{
		return !((x != rhs.x) || (y != rhs.y) || (z != rhs.z));
	}

	Vector3 operator+(const Vector3& other) const
	{
		return Vector3(x + other.x, y + other.y, z + other.z);
	}

	Vector3 operator-(const Vector3& other) const
	{
		return Vector3(x - other.x, y - other.y, z - other.z);
	}

	Vector3 operator*(float scalar) const
	{
		return Vector3(x * scalar, y * scalar, z * scalar);
	}

	static float distance(const Vector3& v1, const Vector3& v2)
	{
		float dx = v1.x - v2.x;
		float dy = v1.y - v2.y;
		float dz = v1.z - v2.z;
		return std::sqrt(dx * dx + dy * dy + dz * dz);
	}

	operator Vector2() const
	{
		return Vector2(x, y);
	}

	operator ImVec2() const {
		return ImVec2(x, y);
	}

	void normalize() {
		float n = std::sqrt(x * x + y * y + z * z);
		x /= n;
		y /= n;
		z /= n;
	}

	static Vector3 cross(const Vector3& v1, const Vector3& v2) {
		return Vector3(
			v1.y * v2.z - v1.z * v2.y,
			v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x
		);
	}

	float length() const
	{
		return sqrt(x * x + y * y + z * z);
	}
};

struct Vector4 {
	float x, y, z, w;

	Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}

	Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}


	friend std::ostream& operator<<(std::ostream& os, const Vector4& v)
	{
		os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
		return os;
	}

	operator Vector3() const {
		return Vector3(x, y, z);
	}
};

struct Matrix4x4 {
	float* operator[ ](int index) {
		return matrix[index];
	}

	float matrix[4][4];

	void transpose() {
		for (int i = 0; i < 4; ++i) {
			for (int j = i + 1; j < 4; ++j) {
				std::swap(matrix[i][j], matrix[j][i]);
			}
		}
	}

	Vector4 GetColumn(int index) const {
		return Vector4(matrix[0][index], matrix[1][index], matrix[2][index], matrix[3][index]);
	}
};


struct Quaternion {
	//https://gist.github.com/HelloKitty/91b7af87aac6796c3da9
	//https://github.com/ihaiucom/learn.MathLibraryForUnity/blob/master/DestMath/Unikon/Quaternion.cs

	float x, y, z, w;

	Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
	Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}


	friend std::ostream& operator<<(std::ostream& os, const Quaternion& q)
	{
		os << "(" << q.x << ", " << q.y << ", " << q.z << ", " << q.w << ")";
		return os;
	}

	static Quaternion LookRotation(const Vector3& forward, const Vector3& up) {
		Vector3 vector = forward;
		vector.normalize();

		Vector3 vector2 = Vector3::cross(up, vector);
		vector2.normalize();

		Vector3 vector3 = Vector3::cross(vector, vector2);
		vector3.normalize();

		float m00 = vector2.x;
		float m01 = vector2.y;
		float m02 = vector2.z;
		float m10 = vector3.x;
		float m11 = vector3.y;
		float m12 = vector3.z;
		float m20 = vector.x;
		float m21 = vector.y;
		float m22 = vector.z;

		float num8 = (m00 + m11) + m22;
		Quaternion quaternion;

		if (num8 > 0.0f) {
			float num = std::sqrt(num8 + 1.0f);
			quaternion.w = num * 0.5f;
			num = 0.5f / num;
			quaternion.x = (m12 - m21) * num;
			quaternion.y = (m20 - m02) * num;
			quaternion.z = (m01 - m10) * num;
			return quaternion;
		}

		if ((m00 >= m11) && (m00 >= m22)) {
			float num7 = std::sqrt(((1.0f + m00) - m11) - m22);
			float num4 = 0.5f / num7;
			quaternion.x = 0.5f * num7;
			quaternion.y = (m01 + m10) * num4;
			quaternion.z = (m02 + m20) * num4;
			quaternion.w = (m12 - m21) * num4;
			return quaternion;
		}

		if (m11 > m22) {
			float num6 = std::sqrt(((1.0f + m11) - m00) - m22);
			float num3 = 0.5f / num6;
			quaternion.x = (m10 + m01) * num3;
			quaternion.y = 0.5f * num6;
			quaternion.z = (m21 + m12) * num3;
			quaternion.w = (m20 - m02) * num3;
			return quaternion;
		}

		float num5 = std::sqrt(((1.0f + m22) - m00) - m11);
		float num2 = 0.5f / num5;
		quaternion.x = (m20 + m02) * num2;
		quaternion.y = (m21 + m12) * num2;
		quaternion.z = 0.5f * num5;
		quaternion.w = (m01 - m10) * num2;
		return quaternion;
	}

	Vector3 getEulerAngles() const {
		//https://stackoverflow.com/questions/12088610/conversion-between-euler-quaternion-like-in-unity3d-engine
		Vector3 euler;


		// if the input quaternion is normalized, this is exactly one. Otherwise, this acts as a correction factor for the quaternion's not-normalizedness
		float unit = (x * x) + (y * y) + (z * z) + (w * w);

		// this will have a magnitude of 0.5 or greater if and only if this is a singularity case
		float test = x * w - y * z;

		if (test > 0.4995f * unit) { // singularity at north pole
			euler.x = M_PI / 2;
			euler.y = 2.0f * atan2(y, x);
			euler.z = 0;
		}
		else if (test < -0.4995f * unit) { // singularity at south pole
			euler.x = -M_PI / 2;
			euler.y = -2.0f * atan2(y, x);
			euler.z = 0;
		}
		else { // no singularity - this is the majority of cases
			euler.x = asin(2.0f * (w * x - y * z));
			euler.y = atan2(2.0f * w * y + 2.0f * z * x, 1 - 2.0f * (x * x + y * y));
			euler.z = atan2(2.0f * w * z + 2.0f * x * y, 1 - 2.0f * (z * z + x * x));
		}

		// all the math so far has been done in radians. Before returning, we convert to degrees...
		euler.x *= 180.0f / M_PI;
		euler.y *= 180.0f / M_PI;
		euler.z *= 180.0f / M_PI;

		//...and then ensure the degree values are between 0 and 360
		euler.x = fmod(euler.x, 360.0f);
		euler.y = fmod(euler.y, 360.0f);
		euler.z = fmod(euler.z, 360.0f);

		return euler;
	}

	void setEulerAngles(const Vector3& euler) {
		float halfDegToRad = 0.5f * Deg2Rad;
		float x = euler.x * halfDegToRad;
		float y = euler.y * halfDegToRad;
		float z = euler.z * halfDegToRad;


		float sinX = sin(x);
		float cosX = cos(x);
		float sinY = sin(y);
		float cosY = cos(y);
		float sinZ = sin(z);
		float cosZ = cos(z);

		this->w = cosY * cosX * cosZ + sinY * sinX * sinZ;
		this->x = cosY * sinX * cosZ + sinY * cosX * sinZ;
		this->y = sinY * cosX * cosZ - cosY * sinX * sinZ;
		this->z = cosY * cosX * sinZ - sinY * sinX * cosZ;
	}

};

