#pragma once
#include "MyVector.h"
#include "Node.h"

//���{�b�g�̍��W���ԂȂǂ̒l���v�Z����N���X�D�� Hexapod�N���X���y���������́D
class HexapodStateCalclator
{
public:
	HexapodStateCalclator();

	//�r���W�͋r�̕t���������_�Ƃ������W�n�Ȃ̂ŁC������O���[�o�����W�ɕϊ�����D�܂��C���{�b�g�̉�]���l������D
	my_vec::SVector getGlobalLegPos(const SNode& _node, const int _leg_num) const;

	//��2�����̍��W�����݂̏d�S���W�Ɖ�]����C�r�̕t���������_�Ƃ������[�J�����W�ɕϊ�����D
	my_vec::SVector getLocalLegPos(const SNode& _node, my_vec::SVector _global_pos, const int _leg_num) const;

	//�r�ʒu�͗��U������Đ��䂳��邪�C���̎���4�̈ʒu���O���[�o�����W�ŏo�͂���D�܂��C���{�b�g�̉�]���l������D
	my_vec::SVector getGlobalLeg2Pos(const SNode& _node, const int _leg_num) const;

	// coxa joint (�r�̕t���� : ��1�֐�) �̍��W��Ԃ��D��]���l�������O���[�o�����W.
	my_vec::SVector getGlobalCoxaJointPos(const SNode& _node, const int _leg_num) const;

	//�m�[�h�̏��͌��݂̋r�ʒu�Əd�S�ʒu���������Ȃ��̂ŁC�W���C���g���ǂ��ɂ��邩��������Ȃ��D����Ă��̊֐��Ōv�Z����D
	void calclateJointPos(const SNode& _node);

	//�ycalclateJointPos�֐����g�p���Ă���g������!!�zfemur joint (��2�֐�) �̍��W��Ԃ��D��]���l�������O���[�o�����W.
	my_vec::SVector getGlobalFemurJointPos(const SNode& _node, const int _leg_num) const;

	//�ycalclateJointPos�֐����g�p���Ă���g������!!�ztibia joint (��3�֐�) �̍��W��Ԃ��D��]���l�������O���[�o�����W.
	my_vec::SVector getGlobalTibiaJointPos(const SNode& _node, const int _leg_num) const;

	//�ÓI�����o�ϐ��� m_leg_max_r , m_leg_min_r �̒l���v�Z���ď���������D���̊֐����̂��ÓI�Ȃ̂�SystemMain�ň�x�������s����΂悢�D
	static void initLegR();

	//�yinitLegR�֐����g�p���Ă���g������!!�z�t��������r��܂ł�Z���W�̍��𗘗p���āC�t��������r��܂ł̍ő唼�a���擾����D�t�����̕�����ɂ���O��œ����܂��D
	float getMaxLegR(const float _coxa_z_to_leg_z) const;

	//�yinitLegR�֐����g�p���Ă���g������!!�z�t��������r��܂ł�Z���W�̍��𗘗p���āC�t��������r��܂ł̍ŏ����a���擾����D�t�����̕�����ɂ���O��œ����܂��D
	float getMinLegR(const float _coxa_z_to_leg_z) const;

	//! @brief �r�̊����`�F�b�N����D
	//! @param _node �m�[�h���
	//! @return �����Ă���ꍇ��true��Ԃ��D
	bool isLegInterfering(const SNode& _node) const;

	//! @brief �r�����͈͓����`�F�b�N����D���x�d���̂��߁C��������Ƃ����v�Z���s���D
	//! @param _node �m�[�h���
	//! @param _leg_num �r�ԍ�
	//! @return ���͈͓��Ȃ�true��Ԃ��D
	bool isLegInRange(const SNode& _node, const int _leg_num) const;

	//! @brief �S�Ă̋r�����͈͓����`�F�b�N����D���x�d���̂��߁C��������Ƃ����v�Z���s���D
	//! @param _node �m�[�h���
	//! @return ���͈͓��Ȃ�true��Ԃ��D
	bool isAllLegInRange(const SNode& _node) const;

	//! @brief �]�΂Ȃ��p�����ǂ������ׂ�D
	//! @param _node �m�[�h���
	//! @return �]�΂Ȃ��p���Ȃ�true��Ԃ��D
	bool isAblePause(const SNode& _node) const;

private:
	my_vec::SVector getLocalCoxaJointPos(const int _leg_num) const;	// coxa joint (�r�̕t����)�̍��W��Ԃ��D�d�S�����_�Ƃ��郍�[�J�����W�D

	my_vec::SVector m_local_femurjoint_pos[HexapodConst::LEG_NUM];	//FemurJoint(��2�֐�)�̈ʒu�D�r�̕t���������_�Ƃ��郍�[�J�����W�DcalclateJointPos�֐��Œl���Z�b�g����D
	my_vec::SVector m_local_tibiajoint_pos[HexapodConst::LEG_NUM];	//TibiaJoint(��3�֐�)�̈ʒu�D�r�̕t���������_�Ƃ��郍�[�J�����W�DcalclateJointPos�֐��Œl���Z�b�g����D

	constexpr static int MAX_DIF_Z = 200;	//�t��������r��܂ł�Z���W�̍��̍ő�l

	static float m_leg_max_r[MAX_DIF_Z];	//�d�S��������r�ʒu�����������́C�r�̎�肤��ő唼�a���L�^�������́D���� Leg_ROM_R
	static float m_leg_min_r[MAX_DIF_Z];	//�d�S��������r�ʒu�����������́C�r�̎�肤��ŏ����a���L�^�������́D���� Leg_ROM_R
};