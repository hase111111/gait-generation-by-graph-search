#include "result_file_exporter.h"

#include <filesystem>
#include <fstream>

#include "designlab_timer.h"


namespace sf = std::filesystem;	//長すぎるので，filesystemの名前空間を短縮する．


void ResultFileExporter::init()
{
	//resultフォルダがなければ作成する．
	if (!sf::exists(RESULT_FOLDER_NAME))
	{
		sf::create_directory(RESULT_FOLDER_NAME);
	}

	//フォルダ名を指定する．現在時刻を取得し，それをフォルダ名にする．
	DesignlabTimer timer;
	m_folder_name = timer.getNowTime();


	//出力先フォルダを作成する．
	std::string output_folder_name = RESULT_FOLDER_NAME + "/" + m_folder_name;

	if (sf::exists(output_folder_name))
	{
		//すでに同名のフォルダが存在する場合は，初期化失敗フラグを立てる．
		m_init_success = false;
		return;
	}

	sf::create_directory(output_folder_name);	//フォルダを作成する．

	if (!sf::exists(output_folder_name))
	{
		//今度は逆に，フォルダが作成できなかった場合は，初期化失敗フラグを立てる．
		m_init_success = false;
		return;
	}

	m_init_success = true;
}


void ResultFileExporter::exportResult(const SSimulationRecord& recoder)
{
	//初期化ができていない場合は，なにも出力しない．また，出力フラグがfalseの場合もなにも出力しない．
	if (!m_init_success || !m_do_export) { return; }


	//出力先ファイルを作成する．
	std::string output_file_name = RESULT_FOLDER_NAME + "/" + m_folder_name + "/" + FILE_NAME + std::to_string(m_export_count + 1) + ".csv";

	std::ofstream ofs(output_file_name);

	//ファイルが作成できなかった場合は，なにも出力しない．
	if (!ofs) { return; }


	//ノードの情報を出力する．


	++m_export_count;	//出力した回数をカウントする．
}