#pragma once

#include <memory>

#include "interface_graphic_main.h"
#include "graphic_data_broker.h"
#include "abstract_hexapod_state_calculator.h"
#include "application_setting_recorder.h"


class IGraphicMainBuilder
{
public:
	IGraphicMainBuilder() = default;
	virtual ~IGraphicMainBuilder() = default;

	//! @brief GraphicMainのインスタンスを生成する．
	//! @param[in] broker GraphicMainに渡すデータを仲介するクラスのポインタ．
	//! @param[in] calc ロボットの状態を計算するクラスのシェアードポインタ．
	//! @param[in] setting アプリケーションの設定を保持するクラスのポインタ．
	virtual std::unique_ptr<IGraphicMain> build(const GraphicDataBroker* const broker, std::shared_ptr<AbstractHexapodStateCalculator> calc,
		const SApplicationSettingRecorder* const setting) = 0;

private:

};
