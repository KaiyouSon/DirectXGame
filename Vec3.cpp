#include "Vec3.h"

// ‚à‚¤ˆê•û‚ÌƒxƒNƒgƒ‹‚Æ‚Ì‘«‚µŽZ
Vec3 Vec3::operator+(const Vec3& other) const { return {x + other.x, y + other.y, z + other.z}; }

// ‚à‚¤ˆê•û‚ÌƒxƒNƒgƒ‹‚Æ‚Ìˆø‚«ŽZ
Vec3 Vec3::operator-(const Vec3& other) const { return {x - other.x, y - other.y, z - other.z}; }

// ‚à‚¤ˆê•û‚ÌƒxƒNƒgƒ‹‚Æ‚ÌŠ|‚¯ŽZ
Vec3 Vec3::operator*(const Vec3& other) const { return {x * other.x, y * other.y, z * other.z}; }

// ‚à‚¤ˆê•û‚ÌƒxƒNƒgƒ‹‚Æ‚ÌŠ„‚èŽZ
Vec3 Vec3::operator/(const Vec3& other) const { return {x / other.x, y / other.y, z / other.z}; }

// ˆê‚Â‚Ì’l‚Æ‚ÌŠ|‚¯ŽZ
Vec3 Vec3::operator*(float num) const { return {x * num, y * num, z * num}; }

// ˆê‚Â‚Ì’l‚Æ‚ÌŠ„‚èŽZ
Vec3 Vec3::operator/(float num) const { return {x / num, y / num, z / num}; }

// •¡‡‘ã“ü‰‰ŽZ +=
Vec3& Vec3::operator+=(const Vec3& other) {
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

// •¡‡‘ã“ü‰‰ŽZ -=
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

// •¡‡‘ã“ü‰‰ŽZ *=
Vec3& Vec3::operator*=(float num) {
	x *= num;
	y *= num;
	z *= num;
	return *this;
}

// •¡‡‘ã“ü‰‰ŽZ /=
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