#pragma once

#include "designlab_vector.h"
#include "designlab_rotator.h"


namespace dl_vec
{
	// https://zenn.dev/mebiusbox/books/132b654aa02124/viewer/2966c7
	// https://www.sports-sensing.com/brands/labss/motionmeasurement/motion_biomechanics/quaternion04.html#:~:text=%E3%82%AF%E3%82%A9%E3%83%BC%E3%82%BF%E3%83%8B%E3%82%AA%E3%83%B3%EF%BC%88quaternion%EF%BC%89%E3%81%AF%E5%9B%9B%E5%85%83,%E5%9B%9E%E8%BB%A2%E3%82%92%E8%A1%A8%E7%8F%BE%E3%81%97%E3%81%BE%E3%81%99%EF%BC%8E
	// https://www.f-sp.com/entry/2017/06/30/221124
	struct SQuaternion
	{
		constexpr SQuaternion() : w(1.0f), v(0.0f, 0.0f, 0.0f) {}
		constexpr SQuaternion(float w_, float x_, float y_, float z_) : w(w_), v(x_, y_, z_) {}
		constexpr SQuaternion(float w_, const SVector& v_) : w(w_), v(v_) {}
		constexpr SQuaternion(const SQuaternion& q) : w(q.w), v(q.v) {}

		constexpr SQuaternion operator + (const SQuaternion& q) const { return { w + q.w, v + q.v }; }
		constexpr SQuaternion operator - (const SQuaternion& q) const { return { w - q.w, v - q.v }; }
		constexpr SQuaternion operator * (const SQuaternion& q) const { return { w * q.w - v.dot(q.v), w * q.v + q.w * v + v.cross(q.v) }; }
		constexpr SQuaternion operator * (float s) const { return { w * s, v * s }; }

		constexpr SQuaternion conjugate() const { return { w, -v }; }
		constexpr float norm() const { return w * w + v.dot(v); }
		constexpr SQuaternion inverse() const { return conjugate() * (1 / norm()); }
		constexpr SRotator toRotator() const;
		void setRotAngleAndAxis(float rad_angle, const SVector& axis);

		float w;	//!< スカラー成分
		SVector v;	//!< ベクトル成分

	};


	constexpr SQuaternion operator * (float s, const SQuaternion& q) { return q * s; }

	SVector rotVecByQuat(const SVector& vec, const SQuaternion& q);

}	// namespace dl_vec	


//! @file designlab_quaternion.h
//! @date 2023/08/21
//! @author 長谷川
//! @brief クォータニオンを表現する構造体
//! @n 行数 : @lineinfo
