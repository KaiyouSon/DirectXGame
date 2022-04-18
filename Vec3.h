#pragma once
class Vec3 {
  public:
	float x, y, z;

  public:
	Vec3() : x(0), y(0), z(0){};
	Vec3(float x, float y, float z) : x(x), y(y), z(z){};

	// 算術演算子のオーバーロード
	Vec3 operator+(const Vec3& other) const; // もう一方のベクトルとの足し算
	Vec3 operator-(const Vec3& other) const; // もう一方のベクトルとの引き算
	Vec3 operator*(const Vec3& other) const; // もう一方のベクトルとの掛け算
	Vec3 operator/(const Vec3& other) const; // もう一方のベクトルとの割り算
	Vec3 operator*(float num) const;         // 一つの値との掛け算
	Vec3 operator/(float num) const;         // 一つの値との割り算

	// 複合代入演算子のオーバーロード
	Vec3& operator+=(const Vec3& other); // 複合代入演算 +=
	Vec3& operator-=(const Vec3& other); // 複合代入演算 -=
	Vec3& operator+=(float num);         // 複合代入演算 +=
	Vec3& operator-=(float num);         // 複合代入演算 -=
	Vec3& operator*=(float num);         // 複合代入演算 *=
	Vec3& operator/=(float num);         // 複合代入演算 /=

	// 代入演算子のオーバーロード
	Vec3& operator=(float num); // 一つの値を代入

	// インクリメント/デクリメント演算子のオーバーロード
	Vec3& operator++();   // 前置インクリメント
	Vec3 operator++(int); // 後置インクリメント
	Vec3& operator--();   // 前置デクリメント
	Vec3 operator--(int); // 後置デクリメント

	// 比較演算子のオーバーロード
	bool operator==(const Vec3& other);
	bool operator!=(const Vec3& other);
	bool operator>=(const Vec3& other);
	bool operator<=(const Vec3& other);
	bool operator==(float num);
	bool operator!=(float num);
	bool operator>=(float num);
	bool operator<=(float num);
};
