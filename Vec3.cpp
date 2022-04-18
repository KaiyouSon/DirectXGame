#include "Vec3.h"

// もう一方のベクトルとの足し算
Vec3 Vec3::operator+(const Vec3& other) const { return {x + other.x, y + other.y, z + other.z}; }

// もう一方のベクトルとの引き算
Vec3 Vec3::operator-(const Vec3& other) const { return {x - other.x, y - other.y, z - other.z}; }

// もう一方のベクトルとの掛け算
Vec3 Vec3::operator*(const Vec3& other) const { return {x * other.x, y * other.y, z * other.z}; }

// もう一方のベクトルとの割り算
Vec3 Vec3::operator/(const Vec3& other) const { return {x / other.x, y / other.y, z / other.z}; }

// 一つの値との掛け算
Vec3 Vec3::operator*(float num) const { return {x * num, y * num, z * num}; }

// 一つの値との割り算
Vec3 Vec3::operator/(float num) const { return {x / num, y / num, z / num}; }

// 複合代入演算 +=
Vec3& Vec3::operator+=(const Vec3& other) {
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

// 複合代入演算 -=
Vec3& Vec3::operator-=(const Vec3& other) {
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

Vec3& Vec3::operator+=(float num) {
	x += num;
	y += num;
	z += num;
	return *this;
}

Vec3& Vec3::operator-=(float num) {
	x -= num;
	y -= num;
	z -= num;
	return *this;
}

// 複合代入演算 *=
Vec3& Vec3::operator*=(float num) {
	x *= num;
	y *= num;
	z *= num;
	return *this;
}

// 複合代入演算 /=
Vec3& Vec3::operator/=(float num) {
	x += num;
	y += num;
	z += num;
	return *this;
}

Vec3& Vec3::operator=(float num) {
	x = num;
	y = num;
	z = num;
	return *this;
}

Vec3& Vec3::operator++() {
	x++;
	y++;
	z++;
	return *this;
}

Vec3 Vec3::operator++(int) {
	Vec3 tmp = *this;
	++*this;
	return tmp;
}

Vec3& Vec3::operator--() {
	x--;
	y--;
	z--;
	return *this;
}

Vec3 Vec3::operator--(int) {
	Vec3 tmp = *this;
	--*this;
	return tmp;
}

bool Vec3::operator==(const Vec3& other) { return x == other.x && y == other.y && z == other.z; }

bool Vec3::operator!=(const Vec3& other) { return x != other.x || y != other.y || z != other.z; }

bool Vec3::operator>=(const Vec3& other) { return x >= other.x && y >= other.y && z >= other.z; }

bool Vec3::operator<=(const Vec3& other) { return x <= other.x && y <= other.y && z <= other.z; }

bool Vec3::operator==(float num) { return x == num && y == num && z == num; }

bool Vec3::operator!=(float num) { return x != num || y != num || z != num; }

bool Vec3::operator>=(float num) { return x >= num && y >= num && z >= num; }

bool Vec3::operator<=(float num) { return x <= num && y <= num && z <= num; }