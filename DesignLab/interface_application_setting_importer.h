//! @file interface_application_setting_importer.h
//! @brief 設定ファイルを読み込むクラスのインターフェース．


#ifndef DESIGNLAB_INTERFACE_APPLICATION_SETTING_IMPORTER_H_
#define DESIGNLAB_INTERFACE_APPLICATION_SETTING_IMPORTER_H_


#include <memory>

#include "application_setting_record.h"


//! @class IApplicationSettingImporter
//! @brief 設定ファイルを読み込むクラスのインターフェース．
//! @n 現在思い付きでtomlファイルで読み込みをしているが，後々変更する可能性があるので，このインターフェースを用意している．
//! @n jsonとかに変えたらこのクラスを継承して，jsonファイルを読み込むクラスを作成してね．
class IApplicationSettingImporter
{
public:

	virtual ~IApplicationSettingImporter() = default;

	//! @brief 設定ファイルを読み込む．
	//! @n 設定ファイルが存在しない場合はデフォルトの設定ファイルを出力する．
	//! @return std::shared_ptr<ApplicationSettingRecord> 設定ファイルの内容．
	virtual std::shared_ptr<ApplicationSettingRecord> ReadFileOrUseAndOutputDefault() = 0;
};


#endif	// DESIGNLAB_INTERFACE_APPLICATION_SETTING_IMPORTER_H_