//! @file designlab_rotation_matrix.h
//! @brief ‰ñ“]s—ñ‚ğ•\‚·\‘¢‘Ì

#ifndef DESIGNLAB_ROTATION_MATRIX_H_
#define DESIGNLAB_ROTATION_MATRIX_H_


#include <array>

#include "designlab_euler.h"


namespace designlab 
{
	//! @struct designlab::RotationMatrix
	//! @brief 3ŸŒ³‚Ì‰ñ“]s—ñ‚ğ•\‚·\‘¢‘Ì
	struct RotationMatrix3x3 final
	{
		//! @brief ’PˆÊs—ñ‚ğ¶¬‚·‚é
		RotationMatrix3x3() : element({ {
			{ 1.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f }
		} }) {};

		//! @brief ”CˆÓ‚Ì‰ñ“]s—ñ‚ğ¶¬‚·‚é
		RotationMatrix3x3(
			const float r11, const float r12, const float r13,
			const float r21, const float r22, const float r23,
			const float r31, const float r32, const float r33
		) : element({ {
			{ r11, r12, r13 },
			{ r21, r22, r23 },
			{ r31, r32, r33 }
		} }) {};

		//! @brief XYZƒIƒCƒ‰[Šp‚©‚ç‰ñ“]s—ñ‚ğ¶¬‚·‚éDX¨Y¨Z‚Ì‡‚É‰ñ“]‚·‚éD
		RotationMatrix3x3(const EulerXYZ& euler_xyz);

		RotationMatrix3x3(const RotationMatrix3x3& other) = default;
		RotationMatrix3x3(RotationMatrix3x3&& other) noexcept = default;
		RotationMatrix3x3& operator =(const RotationMatrix3x3& other) = default;
		~RotationMatrix3x3() = default;

		//! @brief x²ü‚è‚É‰ñ“]‚·‚é‰ñ“]s—ñ‚ğ¶¬‚·‚é
		//! @param [in] angle ‰ñ“]Šp [rad]
		//! @return x²ü‚è‚É‰ñ“]‚·‚é‰ñ“]s—ñ
		static RotationMatrix3x3 CreateRotationMatrixX(float angle);

		//! @brief y²ü‚è‚É‰ñ“]‚·‚é‰ñ“]s—ñ‚ğ¶¬‚·‚é
		//! @param [in] angle ‰ñ“]Šp [rad]
		//! @return y²ü‚è‚É‰ñ“]‚·‚é‰ñ“]s—ñ
		static RotationMatrix3x3 CreateRotationMatrixY(float angle);

		//! @brief z²ü‚è‚É‰ñ“]‚·‚é‰ñ“]s—ñ‚ğ¶¬‚·‚é
		//! @param [in] angle ‰ñ“]Šp [rad]
		//! @return z²ü‚è‚É‰ñ“]‚·‚é‰ñ“]s—ñ
		static RotationMatrix3x3 CreateRotationMatrixZ(float angle);
		

		//! ƒf[ƒ^‚Ì•À‚Ñ‚É‚Â‚¢‚Ä
		//! @n     | R11 R12 R13 |
		//! @n R = | R21 R22 R23 |   
		//! @n     | R31 R32 R33 |
		//! @n
		//! @n R11‚Íelement[0][0]CR12‚Íelement[0][1]CR32‚Íelement[2][1]‚Æ‚È‚éD
		//! @n ‚Â‚Ü‚èCelement[ s - 1 ][ —ñ - 1 ]‚Æ‚È‚éD
		std::array<std::array<float,3>, 3> element;
	};
}


#endif	// !DESIGNLAB_ROTATION_MATRIX_H_