//! @file designlab_quaternion.h
//! @brief クォータニオンを表す構造体


#ifndef DESIGNLAB_QUATERNION_H_
#define DESIGNLAB_QUATERNION_H_


#include <cmath>

#include "cassert_define.h"
#include "designlab_euler.h"
#include "designlab_vector3.h"


//このdefineを有効にすると，ノルムが1でないクォータニオンを許さない
#define DESIGNLAB_QUATERNION_ASSERT_NORM_1


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
		//! @brief 1 + {0,0,0}で初期化する，
		constexpr Quaternion() : w(1.0f), v(0.0f, 0.0f, 0.0f) {}

		//! @brief スカラー成分とベクトル成分を指定して初期化する．ノルムが1になるように代入すること，
		//! @n 使用は非推奨，MakeByAngleAxisを使うこと
		constexpr Quaternion(const float w_, const float x_, const float y_, const float z_) : w(w_), v(x_, y_, z_) {}

		//! @brief スカラー成分とベクトル成分を指定して初期化する．ノルムが1になるように代入すること，
		//! @n 使用は非推奨，MakeByAngleAxisを使うこと
		constexpr Quaternion(const float w_, const ::designlab::Vector3& v_) : w(w_), v(v_)	{}

		constexpr Quaternion(const Quaternion& q) = default;
		constexpr Quaternion(Quaternion&& q) noexcept = default;
		constexpr Quaternion& operator =(const Quaternion& q) = default;

		constexpr Quaternion operator + () const { return *this; }
		constexpr Quaternion operator - () const { return { -w, -v }; }
		constexpr Quaternion operator + (const Quaternion& q) const { return { w + q.w, v + q.v }; }
		constexpr Quaternion operator - (const Quaternion& q) const { return { w - q.w, v - q.v }; }
		constexpr Quaternion operator * (const Quaternion& q) const { return { w * q.w - v.Dot(q.v), w * q.v + q.w * v + v.Cross(q.v) }; }
		constexpr Quaternion operator * (const float s) const { return { w * s, v * s }; }
		constexpr Quaternion operator / (const float s) const { return { w / s, v / s }; }

		bool operator == (const Quaternion& q) const { return (w == q.w) && (v == q.v); }
		bool operator != (const Quaternion& q) const { return !(*this == q); }

		bool operator < (const Quaternion& q) const { return (w < q.w) || (w == q.w && v < q.v); }
		bool operator > (const Quaternion& other) const { return other < *this; }
		bool operator <= (const Quaternion& other) const { return !(*this > other); }
		bool operator >= (const Quaternion& other) const { return !(*this < other); }


		//! @brief クォータニオンの内積を返す．クォータニオンを4次元のベクトルとみなし，ベクトルの内積を求める
		//! @param [in] other 内積を求めるクォータニオン
		//! @return float 内積
		constexpr float Dot(Quaternion other) const { return w * other.w + v.Dot(other.v); }

		//! @brief クォータニオンの共役を返す．共役なクォータニオンとは，ベクトル成分の符号を反転させたもの
		//! @n q = w + xi + yj + zk とすると，qの共役は w - xi - yj - zk となる．回転は逆方向になる
		//! @return designlab::Quaternion 共役クォータニオン
		constexpr Quaternion Conjugate() const { return { w, -v }; }

		//! @brief クォータニオンの長さの2乗を返す(ノルムの2乗)．
		//! @n クォータニオンの長さの2乗は，w^2 + x^2 + y^2 + z^2 で求められる
		//! @return float 長さの2乗
		constexpr float LengthSquared() const { return w * w + v.Dot(v); }

		//! @brief クォータニオンのノルムを返す
		//! @n ノルムとは，ベクトルの大きさのこと．クォータニオンのノルムは，w^2 + x^2 + y^2 + z^2 で求められる
		//! @return float ノルム
		inline float Norm() const { return std::sqrt(LengthSquared()); }

		//! @brief クォータニオンの逆数を返す
		//! @n クォータニオンqの逆数q^-1は，qの共役をノルムで割ったもの
		constexpr Quaternion Inverse() const { return Conjugate() * (1 / Norm()); }

		//! @brief 正規化したクォータニオンを返す
		//! @n クォータニオンの正規化とは，ノルムを1にすること．
		//! @n クォータニオンqの正規化は，q / |q| で求められる
		//! @return designlab::Quaternion 正規化されたクォータニオン
		inline Quaternion Normalize() const { return *this * (1 / Norm()); }

		//! @brief クォータニオンをXYZオイラー角に変換する
		//! @return designlab::EulerXYZ XYZオイラー角
		constexpr EulerXYZ toRotator() const;

		//! @brief 他のクォータニオンとの距離の2乗を返す．クォータニオンを4次元ベクトルとみなし，ベクトルの距離の2乗を求める
		//! @return float 距離の2乗
		constexpr float DistanceSquared(const Quaternion& q) const { return (*this - q).LengthSquared(); }

		//! @brief 回転軸と回転角からクォータニオンを作成する
		//! @n q = cos(θ/2) * w + sin(θ/2) * { v.x  + v.y  + v.z } となる
		//! @param [in] rad_angle 回転角θ [rad]
		//! @param [in] axis 回転軸
		static Quaternion MakeByAngleAxis(float rad_angle, const Vector3& axis);


		float w;	//!< スカラー成分
		::designlab::Vector3 v;	//!< ベクトル成分

	};


	constexpr Quaternion operator * (float s, const Quaternion& q) { return q * s; }

	//! @brief 3次元の位置ベクトルを回転させる．
	//! @param [in] vec 回転させるベクトル．
	//! @param [in] q 回転させるクォータニオン．
	//! @param [in] use_normalized_quaternions 正規化されたクォータニオンを使うかどうか．
	//! @n 正規化クォータニオンならば，共役と逆数が等しいので，計算量を減らすことができる．
	//! @return designlab::Vector3 回転後のベクトル．
	Vector3 RotateVector3(const Vector3& vec, const Quaternion& q, bool use_normalized_quaternions);

	//! @brief 球面線形補間を行う．
	//! @param [in] q1 クォータニオン1
	//! @param [in] q2 クォータニオン2
	//! @param [in] t 補間係数．0～1の値を取る．
	//! @return designlab::Quaternion 補間されたクォータニオン
	Quaternion SlerpQuaternion(const Quaternion& q1, const Quaternion& q2, float t);

}	// namespace designlab	


// defineはundefするまで有効なので，undefする(他のファイルに影響しないようにするため，)
#undef DESIGNLAB_QUATERNION_ASSERT_NORM_1


#endif	// DESIGNLAB_QUATERNION_H_