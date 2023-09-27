//! @file designlab_rotation_matrix.h
//! @brief 回転行列を表す構造体

#ifndef DESIGNLAB_ROTATION_MATRIX_H_
#define DESIGNLAB_ROTATION_MATRIX_H_


#include <array>

#include "designlab_euler.h"
#include "designlab_vector3.h"


namespace designlab 
{
	//! @struct designlab::RotationMatrix
	//! @brief 3次元の回転行列を表す構造体
	struct RotationMatrix3x3 final
	{
		//! @brief 単位行列を生成する
		RotationMatrix3x3() : element({ {
			{ 1.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f }
		} }) {};

		//! @brief 任意の回転行列を生成する
		RotationMatrix3x3(
			const float r11, const float r12, const float r13,
			const float r21, const float r22, const float r23,
			const float r31, const float r32, const float r33
		) : element({ {
			{ r11, r12, r13 },
			{ r21, r22, r23 },
			{ r31, r32, r33 }
		} }) {};

		//! @brief XYZオイラー角から回転行列を生成する．X→Y→Zの順に回転する．
		RotationMatrix3x3(const EulerXYZ& euler_xyz);

		RotationMatrix3x3(const RotationMatrix3x3& other) = default;
		RotationMatrix3x3(RotationMatrix3x3&& other) noexcept = default;
		RotationMatrix3x3& operator =(const RotationMatrix3x3& other) = default;
		~RotationMatrix3x3() = default;


		RotationMatrix3x3& operator* (const RotationMatrix3x3& other);


		//! @brief x軸周りに回転する回転行列を生成する
		//! @param [in] angle 回転角 [rad]
		//! @return x軸周りに回転する回転行列
		static RotationMatrix3x3 CreateRotationMatrixX(float angle);

		//! @brief y軸周りに回転する回転行列を生成する
		//! @param [in] angle 回転角 [rad]
		//! @return y軸周りに回転する回転行列
		static RotationMatrix3x3 CreateRotationMatrixY(float angle);

		//! @brief z軸周りに回転する回転行列を生成する
		//! @param [in] angle 回転角 [rad]
		//! @return z軸周りに回転する回転行列
		static RotationMatrix3x3 CreateRotationMatrixZ(float angle);
		

		//! データの並びについて
		//! @n ____|_R11_R12_R13_|
		//! @n R_=_|_R21_R22_R23_|   
		//! @n ____|_R31_R32_R33_|
		//! @n
		//! @n R11はelement[0][0]，R12はelement[0][1]，R32はelement[2][1]となる．
		//! @n つまり，element[ 行 - 1 ][ 列 - 1 ]となる．
		std::array<std::array<float,3>, 3> element;
	};


	//! @brief 回転させたベクトルを返す
	//! @param [in] vec 回転させるベクトル
	//! @param [in] rot 回転行列
	Vector3 rotVector(const Vector3& vec, const RotationMatrix3x3& rot);

}


#endif	// !DESIGNLAB_ROTATION_MATRIX_H_