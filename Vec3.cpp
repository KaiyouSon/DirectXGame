#include "Vec3.h"

// ��������̃x�N�g���Ƃ̑����Z
Vec3 Vec3::operator+(const Vec3& other) const { return {x + other.x, y + other.y, z + other.z}; }

// ��������̃x�N�g���Ƃ̈����Z
Vec3 Vec3::operator-(const Vec3& other) const { return {x - other.x, y - other.y, z - other.z}; }

// ��������̃x�N�g���Ƃ̊|���Z
Vec3 Vec3::operator*(const Vec3& other) const { return {x * other.x, y * other.y, z * other.z}; }

// ��������̃x�N�g���Ƃ̊���Z
Vec3 Vec3::operator/(const Vec3& other) const { return {x / other.x, y / other.y, z / other.z}; }

// ��̒l�Ƃ̊|���Z
Vec3 Vec3::operator*(float num) const { return {x * num, y * num, z * num}; }

// ��̒l�Ƃ̊���Z
Vec3 Vec3::operator/(float num) const { return {x / num, y / num, z / num}; }

// ����������Z +=
Vec3& Vec3::operator+=(const Vec3& other) {
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

// ����������Z -=
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

// ����������Z *=
Vec3& Vec3::operator*=(float num) {
	x *= num;
	y *= num;
	z *= num;
	return *this;
}

// ����������Z /=
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