#include "application_setting_importer.h"

#include <fstream>
#include <filesystem>
#include <map>

#include <magic_enum.hpp>

#include "application_setting_exporter.h"
#include "application_setting_record_vaildator.h"
#include "cmdio_util.h"


namespace dlio = ::designlab::cmdio;


ApplicationSettingRecord ApplicationSettingImporter::ImportOrUseDefault() const
{
	dlio::Output("[" + static_cast<std::string>(typeid(ApplicationSettingImporter).name()) + "]", OutputDetail::kSystem);
	dlio::Output("設定ファイルを読み込みます．file_path : " + kSettingFileName, OutputDetail::kSystem);

	//ファイルを探す，存在しなかったらデフォルトの設定を出力して終了，fsystemはC++17から，実行できない場合は設定を見直してみてください
	if (!std::filesystem::is_regular_file(kSettingFileName))
	{
		dlio::Output("設定ファイルが見つかりませんでした．", OutputDetail::kSystem);

		OutputDefaultSettingFile();

		dlio::Output("デフォルトの設定を使用します．", OutputDetail::kSystem);

		return ApplicationSettingRecord();
	}

	dlio::Output("設定ファイルが見つかりました．パースを開始します．", OutputDetail::kSystem);

	//ファイルを読み込む
	toml::value data;

	try
	{
		std::ifstream ifs(kSettingFileName, std::ios::binary);		//バイナリモードで読み込む

		data = toml::parse(ifs, kSettingFileName);					//ファイルをパース(読みこみ&解析)する
	}
	catch (toml::syntax_error err)
	{
		dlio::Output("設定ファイルのパースに失敗しました．", OutputDetail::kSystem);
		dlio::OutputNewLine(1, OutputDetail::kSystem);
		dlio::Output("<パースに失敗した箇所>", OutputDetail::kSystem);
		dlio::Output(err.what(), OutputDetail::kSystem);
		dlio::OutputNewLine(1, OutputDetail::kSystem);

		OutputDefaultSettingFile();

		dlio::Output("デフォルトの設定を使用します．", OutputDetail::kSystem);

		return ApplicationSettingRecord();
	}

	dlio::Output("設定ファイルのパースに成功しました．データをシリアライズします．", OutputDetail::kSystem);

	ApplicationSettingRecord setting;

	try
	{
		setting = toml::from<ApplicationSettingRecord>::from_toml(data);	//dataをApplicationSettingRecord型に変換する
	}
	catch (...)
	{
		dlio::Output("データのシリアライズに失敗しました．", OutputDetail::kSystem);

		OutputDefaultSettingFile();

		dlio::Output("デフォルトの設定を使用します．", OutputDetail::kSystem);

		return ApplicationSettingRecord();
	}

	dlio::Output("データのシリアライズに成功しました．データのバリデートを開始します．", OutputDetail::kSystem);

	ApplicationSettingRecordVaildator vaildator;

	const auto [is_vaild, err_str] = vaildator.Validate(setting);

	if (!is_vaild)
	{
		dlio::Output("データのバリデートに失敗しました．", OutputDetail::kSystem);
		dlio::OutputNewLine(1, OutputDetail::kSystem);
		dlio::Output("<エラー内容>", OutputDetail::kSystem);
		dlio::Output(err_str, OutputDetail::kSystem);
		dlio::OutputNewLine(1, OutputDetail::kSystem);

		OutputDefaultSettingFile();

		dlio::Output("デフォルトの設定を使用します．", OutputDetail::kSystem);

		return ApplicationSettingRecord();
	}

	dlio::Output("データのバリデートに成功しました．", OutputDetail::kSystem);
	dlio::Output("読み込みは正常に完了しました．", OutputDetail::kSystem);

	return setting;
}

void ApplicationSettingImporter::OutputDefaultSettingFile() const
{
	if (!dlio::InputYesNo("デフォルトの設定ファイルを出力しますか?"))
	{
		return;
	}

	const ApplicationSettingRecord default_setting;

	const ApplicationSettingExporter exporter;

	exporter.Export(kSettingFileName, default_setting);

	return;
}