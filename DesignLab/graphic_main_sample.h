#pragma once
#include "interface_graphic_main.h"


//! @class GraphicMainSample
//! @date 2023/08/09
//! @author ���J��
//! @brief �T���v���`��N���X�D�T���v�����b�Z�[�W��\������D
//! @details ���̃N���X�͎��ۂɎg�p����ړI�ō�������̂ł͂Ȃ��C�C���^�[�t�F�C�X��p�����N���X�̎����@�̉���̂��߂ɍ�������́D
//! @n �Œ���C���̂悤�ȗv�f������Όp���ł���D
//! @n [�\���ɂ���] 
//! @n �����Ⴒ���Ⴂ�낢����Ă��邪�C
//! @n final �c ����ȏ�p���͂ł��Ȃ��Ƃ����Ӗ��D���̃N���X���X�Ɍp�����悤�Ƃ���ƃG���[�ɂȂ�D���Ȃ݂ɁC�p�����Ȃ��N���X�ɂ͊�{�I�ɕt���Ă����Ƃ悢�D
//! @n public AbstractGraphicMain �c AbstractGraphicMain�N���X���p�������C�Ƃ����Ӗ��D
//! @n override �c �p�����̊֐����I�[�o�[���C�h�����Ƃ����Ӗ��C�t���Ȃ��Ƃ����Ȃ����t���Ă����ƃR���p�C���ɖ����I�ɃI�[�o�[���C�h��m�点����̂Ńo�O��h���₷���D
//! @n GraphicMainSample(const GraphicDataBroker* broker) : IGraphicMain(broker) {}; �c �R���X�g���N�^�ɂ����āC�e�N���X�̃R���X�g���N�^���Ăяo���C�Ƃ����Ӗ��D����������Y���ƃG���[�ɂȂ�̂Œ��ӁD
class GraphicMainSample final : public IGraphicMain
{
public:
	GraphicMainSample(const GraphicDataBroker* broker) : IGraphicMain(broker) {};
	~GraphicMainSample() = default;

	bool update() override;

	void draw() const override;

private:

	int m_counter = 0;
};


//! @file graphic_main_sample.h
//! @date 2023/08/09
//! @author ���J��
//! @brief IGraphicMain�̎g�������
//! @n �s�� : @lineinfo