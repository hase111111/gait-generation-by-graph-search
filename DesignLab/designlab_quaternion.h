#pragma once

#include <cmath>

#include "designlab_vector3.h"
#include "designlab_rotator.h"


namespace designlab
{
	// https://zenn.dev/mebiusbox/books/132b654aa02124/viewer/2966c7
	// https://www.sports-sensing.com/brands/labss/motionmeasurement/motion_biomechanics/quaternion04.html#:~:text=%E3%82%AF%E3%82%A9%E3%83%BC%E3%82%BF%E3%83%8B%E3%82%AA%E3%83%B3%EF%BC%88quaternion%EF%BC%89%E3%81%AF%E5%9B%9B%E5%85%83,%E5%9B%9E%E8%BB%A2%E3%82%92%E8%A1%A8%E7%8F%BE%E3%81%97%E3%81%BE%E3%81%99%EF%BC%8E
	// https://www.f-sp.com/entry/2017/06/30/221124
	struct SQuaternion
	{
		constexpr SQuaternion() : w(1.0f), v(0.0f, 0.0f, 0.0f) {}
		constexpr SQuaternion(float w_, float x_, float y_, float z_) : w(w_), v(x_, y_, z_) {}
		constexpr SQuaternion(float w_, const Vector3& v_) : w(w_), v(v_) {}
		constexpr SQuaternion(const SQuaternion& q) : w(q.w), v(q.v) {}

		constexpr SQuaternion operator + () const { return *this; }
		constexpr SQuaternion operator - () const { return { -w, -v }; }
		constexpr SQuaternion operator + (const SQuaternion& q) const { return { w + q.w, v + q.v }; }
		constexpr SQuaternion operator - (const SQuaternion& q) const { return { w - q.w, v - q.v }; }
		constexpr SQuaternion operator * (const SQuaternion& q) const { return { w * q.w - v.Dot(q.v), w * q.v + q.w * v + v.Cross(q.v) }; }
		constexpr SQuaternion operator * (float s) const { return { w * s, v * s }; }
		constexpr SQuaternion operator / (float s) const { return { w / s, v / s }; }


		//! @brief クォータニオンの共役を返す．共役なクォータニオンとは，ベクトル成分の符号を反転させたもの
		//! @n q = w + xi + yj + zk とすると，qの共役は w - xi - yj - zk となる．回転は逆方向になる
		//! @return designlab::SQuaternion 共役クォータニオン
		constexpr SQuaternion conjugate() const { return { w, -v }; }

		//! @brief クォータニオンのノルムを返す
		//! @n ノルムとは，ベクトルの大きさのこと．クォータニオンのノルムは，w^2 + x^2 + y^2 + z^2 で求められる
		//! @return float ノルム
		constexpr float norm() const { return w * w + v.Dot(v); }

		//! @brief クォータニオンの逆数を返す
		//! @n クォータニオンqの逆数q^-1は，qの共役をノルムで割ったもの
		constexpr SQuaternion inverse() const { return conjugate() * (1 / norm()); }

		//! @brief 正規化したクォータニオンを返す
		//! @n クォータニオンの正規化とは，ノルムを1にすること．
		//! @n クォータニオンqの正規化は，q / |q| で求められる
		//! @return designlab::SQuaternion 正規化されたクォータニオン
		inline SQuaternion normalize() const { return *this * (1 / sqrt(norm())); }

		//! @brief クォータニオンをXYZオイラー角に変換する
		//! @return designlab::SRotator XYZオイラー角
		constexpr SRotator toRotator() const;

		//! @brief 他のクォータニオンとの距離の2乗を返す．クォータニオンを4次元ベクトルとみなし，ベクトルの距離の2乗を求める
		//! @return float 距離の2乗
		constexpr float distanceSquared(const SQuaternion& q) const { return (*this - q).norm(); }

		//! @brief 回転軸と回転角からこのクォータニオンを変更し，設定する
		//! @n q = cos(θ/2) * w + sin(θ/2) * { x  + y  + z } となる
		//! @param [in] rad_angle 回転角（ラジアン）
		//! @param [in] axis 回転軸
		void setRotAngleAndAxis(float rad_angle, const Vector3& axis);


		float w;	//!< スカラー成分
		Vector3 v;	//!< ベクトル成分

	};


	constexpr SQuaternion operator * (float s, const SQuaternion& q) { return q * s; }

	Vector3 rotVecByQuat(const Vector3& vec, const SQuaternion& q);

}	// namespace designlab	


//! @file designlab_quaternion.h
//! @date 2023/08/21
//! @author 長谷川
//! @brief クォータニオンを表現する構造体
//! @n 行数 : @lineinfo
