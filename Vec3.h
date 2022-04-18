#pragma once
class Vec3 {
  public:
	float x, y, z;

  public:
	Vec3() : x(0), y(0), z(0){};
	Vec3(float x, float y, float z) : x(x), y(y), z(z){};

	// �Z�p���Z�q�̃I�[�o�[���[�h
	Vec3 operator+(const Vec3& other) const; // ��������̃x�N�g���Ƃ̑����Z
	Vec3 operator-(const Vec3& other) const; // ��������̃x�N�g���Ƃ̈����Z
	Vec3 operator*(const Vec3& other) const; // ��������̃x�N�g���Ƃ̊|���Z
	Vec3 operator/(const Vec3& other) const; // ��������̃x�N�g���Ƃ̊���Z
	Vec3 operator*(float num) const;         // ��̒l�Ƃ̊|���Z
	Vec3 operator/(float num) const;         // ��̒l�Ƃ̊���Z

	// ����������Z�q�̃I�[�o�[���[�h
	Vec3& operator+=(const Vec3& other); // ����������Z +=
	Vec3& operator-=(const Vec3& other); // ����������Z -=
	Vec3& operator+=(float num);         // ����������Z +=
	Vec3& operator-=(float num);         // ����������Z -=
	Vec3& operator*=(float num);         // ����������Z *=
	Vec3& operator/=(float num);         // ����������Z /=

	// ������Z�q�̃I�[�o�[���[�h
	Vec3& operator=(float num); // ��̒l����

	// �C���N�������g/�f�N�������g���Z�q�̃I�[�o�[���[�h
	Vec3& operator++();   // �O�u�C���N�������g
	Vec3 operator++(int); // ��u�C���N�������g
	Vec3& operator--();   // �O�u�f�N�������g
	Vec3 operator--(int); // ��u�f�N�������g

	// ��r���Z�q�̃I�[�o�[���[�h
	bool operator==(const Vec3& other);
	bool operator!=(const Vec3& other);
	bool operator>=(const Vec3& other);
	bool operator<=(const Vec3& other);
	bool operator==(float num);
	bool operator!=(float num);
	bool operator>=(float num);
	bool operator<=(float num);
};
