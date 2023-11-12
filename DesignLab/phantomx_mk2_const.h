//! @file phantomx_mk2_const.h
//! @brief PhantomX�̃X�e�[�^�X���܂Ƃ߂��萔�N���X�D

#ifndef DESIGNLAB_PHANTOMX_CONST_H_
#define DESIGNLAB_PHANTOMX_CONST_H_

#include <array>

#include "cassert_define.h"
#include "designlab_math_util.h"


//! @class PhantomXMkIIConst
//! @brief PhantomX mk-�U �̃p�����[�^��萔�ŕ\���������́D
//! @n �R���X�g���N�^���폜�����̂ŁC���̂͐����ł��Ȃ��D( PhantomXMkIIConst::kLegNum �݂����ɒl���Ăяo������ )
//! @details �ȒP�̂��ߒl�������ɂ܂Ƃ߂����C�ނ�݂ɂ��̒l���Q�Ƃ�����HexapodStateCalculator���g�����ƁD
//! @n �݌v�I�ɂ��܂萳�����Ȃ��C�����邪�C��������悭�J���ł��Ă���̂Ŗ�肪�N�����Ƃ��ɍl���邱�Ƃɂ���D
//! @n �܂��C���W�n�̓��{�b�g�O����x���C��������y���C�������z�����Ƃ�E����W�n�ł���D
class PhantomXMkIIConst final
{
private:
	constexpr static  int kPhantomXLegNum = 6;

public:

	// �R���X�g���N�^�ƃR�s�[�R���X�g���N�^���폜���C���̂𐶐��ł��Ȃ��悤�ɂ���D
	PhantomXMkIIConst() = delete;
	PhantomXMkIIConst(const PhantomXMkIIConst& other) = delete;
	PhantomXMkIIConst& operator=(const PhantomXMkIIConst& other) = delete;
	PhantomXMkIIConst(PhantomXMkIIConst&& other) = delete;

	//! ��1�֐߂̏����p�x[rad]
	constexpr static std::array<float, kPhantomXLegNum> kCoxaDefaultAngle = {
		::designlab::math_util::ConvertDegToRad(-45.0f), 
		::designlab::math_util::ConvertDegToRad(-90.0f), 
		::designlab::math_util::ConvertDegToRad(-135.0f),
		::designlab::math_util::ConvertDegToRad(135.0f), 
		::designlab::math_util::ConvertDegToRad(90.0f),  
		::designlab::math_util::ConvertDegToRad(45.0f) 
	};

	constexpr static float kCoxaAngleMin = ::designlab::math_util::ConvertDegToRad(-81.0f);	//!< ��1�֐߂̉��͈͂̍ŏ��l[rad]�D�ڂ�����reference���t�H���_�Q�ƁD
	constexpr static float kCoxaAngleMax = ::designlab::math_util::ConvertDegToRad(81.0f);	//!< ��1�֐߂̉��͈͂̍ő�l[rad]�D�ڂ�����reference���t�H���_�Q�ƁD

	constexpr static float kFemurAngleMin = ::designlab::math_util::ConvertDegToRad(-105.0f);	//!< ��2�֐߂̉��͈͂̍ŏ��l[rad]�D�ڂ�����reference���t�H���_�Q�ƁD
	constexpr static float kFemurAngleMax = ::designlab::math_util::ConvertDegToRad(99.0f);	//!< ��2�֐߂̉��͈͂̍ő�l[rad]�D�ڂ�����reference���t�H���_�Q�ƁD

	constexpr static float kTibiaAngleMin = ::designlab::math_util::ConvertDegToRad(-145.0f);	//!< ��2�֐߂̉��͈͂̍ŏ��l[rad]�D�ڂ�����reference���t�H���_�Q�ƁD
	constexpr static float kTibiaAngleMax = ::designlab::math_util::ConvertDegToRad(25.5f);	//!< ��2�֐߂̉��͈͂̍ő�l[rad]�D�ڂ�����reference���t�H���_�Q�ƁD

	constexpr static float kCoxaLength = 52.0f;		//!< ��1�֐ߕ��̒���[mm]�D�ڂ�����reference���t�H���_�Q�ƁD
	constexpr static float kFemurLength = 66.0f;	//!< ��2�֐ߕ��̒���[mm]�D�ڂ�����reference���t�H���_�Q�ƁD(���m�ȃX�e�[�^�X��66.061mm)
	constexpr static float kTibiaLength = 130.0f;	//!< ��3�֐ߕ��̒���[mm]�D�ڂ�����reference���t�H���_�Q�ƁD(���m�ȃX�e�[�^�X137mm)

	constexpr static float kCoxaBaseOffsetY = 61.64f;		//!< coxa link�̕t����(�O���E���)�܂ł̒���[mm]�D
	constexpr static float kCenterCoxaBaseOffsetY = 103.4f;	//!< coxa link�̕t����(����)�܂ł̒���[mm]�D
	constexpr static float kCoxaBaseOffsetX = 124.8f;		//!< coxa link�̕t����(�O���E���)�܂ł̒���[mm]�D
	constexpr static float kCoxaBaseOffsetZ = 1.116f;		//!< coxa link�̕t�����܂ł̒���(�����)[mm]�D
	constexpr static float kBodyHeight = 40.0f;			//!< ���̂̍���[mm]�D


	//! @brief ��1�֐߂̊p�x���L���Ȕ͈͓����ǂ����𔻒肷��D
	//! @param [in] leg_index �r�̔ԍ��D
	//! @param [in] angle ���肷��p�x�D
	//! @return bool �L���Ȕ͈͓��Ȃ�true�D
	constexpr static bool IsVaildCoxaAngle(const int leg_index, const float angle)
	{
		// 0 <= leg_index < kPhantomXLegNum �ł��邱�Ƃ�ۏ؂���D
		assert(0 <= leg_index);
		assert(leg_index < kPhantomXLegNum);

		return (
			kCoxaAngleMin + kCoxaDefaultAngle[leg_index] <= angle && 
			angle <= kCoxaAngleMax + kCoxaDefaultAngle[leg_index]
		);
	};

	//! @biref ��2�֐߂̊p�x���L���Ȕ͈͓����ǂ����𔻒肷��D
	//! @param [in] angle ���肷��p�x�D
	//! @return bool �L���Ȕ͈͓��Ȃ�true�D
	constexpr static bool IsVaildFemurAngle(const float angle)
	{
		return kFemurAngleMin <= angle && angle <= kFemurAngleMax;
	};

	//! @brief ��3�֐߂̊p�x���L���Ȕ͈͓����ǂ����𔻒肷��D
	//! @param [in] angle ���肷��p�x�D
	//! @return bool �L���Ȕ͈͓��Ȃ�true�D
	constexpr static bool IsVaildTibiaAngle(const float angle)
	{
		return kTibiaAngleMin <= angle && angle <= kTibiaAngleMax;
	};


	static_assert(kCoxaAngleMin < kCoxaAngleMax, "kCoxaAngleMax > Min�ł���K�v������܂��D");
	static_assert(kFemurAngleMin < kFemurAngleMax, "kFemurAngleMax > Min�ł���K�v������܂��D");
	static_assert(kTibiaAngleMin < kTibiaAngleMax, "kTibiaAngleMax > Min�ł���K�v������܂��D");
	static_assert(kCoxaLength > 0.f, "kCoxaLength�CCoxa Link�̒����͐��ł���K�v������܂��D");
	static_assert(kFemurLength > 0.f, "kFemurLength�CFemur Link�̒����͐��ł���K�v������܂��D");
	static_assert(kTibiaLength > 0.f, "kTibiaLength�CTibia Link�̒����͐��ł���K�v������܂��D");
};


#endif	// DESIGNLAB_PHANTOMX_CONST_H_