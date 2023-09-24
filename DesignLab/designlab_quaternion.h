//! @file designlab_quaternion.h
//! @brief クォータニオンを表す構造体


#ifndef DESIGNLAB_QUATERNION_H_
#define DESIGNLAB_QUATERNION_H_


#include <cmath>

#include "designlab_euler.h"
#include "designlab_vector3.h"


namespace designlab
{
	//! @struct designlab::Quaternion
	//! @brief クォータニオンを表す構造体
	//! @details クォータニオンは，スカラー成分とベクトル成分からなる．
	//! @n 四元数とも呼ばれる．
	//! @n 参考
	//! @n https://zenn.dev/mebiusbox/books/132b654aa02124/viewer/2966c7
	//! @n https://www.sports-sensing.com/brands/labss/motionmeasurement/motion_biomechanics/quaternion04.html#:~:text=%E3%82%AF%E3%82%A9%E3%83%BC%E3%82%BF%E3%83%8B%E3%82%AA%E3%83%B3%EF%BC%88quaternion%EF%BC%89%E3%81%AF%E5%9B%9B%E5%85%83,%E5%9B%9E%E8%BB%A2%E3%82%92%E8%A1%A8%E7%8F%BE%E3%81%97%E3%81%BE%E3%81%99%EF%BC%8E
	//! @n https://www.f-sp.com/entry/2017/06/30/221124
	struct Quaternion
	{
		constexpr Quaternion() : w(1.0f), v(0.0f, 0.0f, 0.0f) {}
		constexpr Quaternion(float w_, float x_, float y_, float z_) : w(w_), v(x_, y_, z_) {}
		constexpr Quaternion(float w_, const Vector3& v_) : w(w_), v(v_) {}

		constexpr Quaternion(const Quaternion& q) = default;
		constexpr Quaternion(Quaternion&& q) noexcept = default;
		constexpr Quaternion& operator =(const Quaternion& q) = default;

		constexpr Quaternion operator + () const { return *this; }
		constexpr Quaternion operator - () const { return { -w, -v }; }
		constexpr Quaternion operator + (const Quaternion& q) const { return { w + q.w, v + q.v }; }
		constexpr Quaternion operator - (const Quaternion& q) const { return { w - q.w, v - q.v }; }
		constexpr Quaternion operator * (const Quaternion& q) const { return { w * q.w - v.Dot(q.v), w * q.v + q.w * v + v.Cross(q.v) }; }
		constexpr Quaternion operator * (float s) const { return { w * s, v * s }; }
		constexpr Quaternion operator / (float s) const { return { w / s, v / s }; }


		//! @brief クォータニオンの共役を返す．共役なクォータニオンとは，ベクトル成分の符号を反転させたもの
		//! @n q = w + xi + yj + zk とすると，qの共役は w - xi - yj - zk となる．回転は逆方向になる
		//! @return designlab::Quaternion 共役クォータニオン
		constexpr Quaternion Conjugate() const { return { w, -v }; }

		//! @brief クォータニオンのノルムを返す
		//! @n ノルムとは，ベクトルの大きさのこと．クォータニオンのノルムは，w^2 + x^2 + y^2 + z^2 で求められる
		//! @return float ノルム
		constexpr float Norm() const { return w * w + v.Dot(v); }

		//! @brief クォータニオンの逆数を返す
		//! @n クォータニオンqの逆数q^-1は，qの共役をノルムで割ったもの
		constexpr Quaternion Inverse() const { return Conjugate() * (1 / Norm()); }

		//! @brief 正規化したクォータニオンを返す
		//! @n クォータニオンの正規化とは，ノルムを1にすること．
		//! @n クォータニオンqの正規化は，q / |q| で求められる
		//! @return designlab::Quaternion 正規化されたクォータニオン
		inline Quaternion Normalize() const { return *this * (1 / sqrt(Norm())); }

		//! @brief クォータニオンをXYZオイラー角に変換する
		//! @return designlab::EulerXYZ XYZオイラー角
		constexpr EulerXYZ toRotator() const;

		//! @brief 他のクォータニオンとの距離の2乗を返す．クォータニオンを4次元ベクトルとみなし，ベクトルの距離の2乗を求める
		//! @return float 距離の2乗
		constexpr float DistanceSquared(const Quaternion& q) const { return (*this - q).Norm(); }

		//! @brief 回転軸と回転角からクォータニオンを作成する
		//! @n q = cos(θ/2) * w + sin(θ/2) * { x  + y  + z } となる
		//! @param [in] rad_angle 回転角（ラジアン）
		//! @param [in] axis 回転軸
		static Quaternion MakeByRotAngleAndAxis(float rad_angle, const Vector3& axis);


		float w;	//!< スカラー成分
		Vector3 v;	//!< ベクトル成分

	};


	constexpr Quaternion operator * (float s, const Quaternion& q) { return q * s; }

	Vector3 rotVecByQuat(const Vector3& vec, const Quaternion& q);

}	// namespace designlab	


#endif	// DESIGNLAB_QUATERNION_H_