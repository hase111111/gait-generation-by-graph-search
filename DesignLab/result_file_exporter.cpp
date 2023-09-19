#include "result_file_exporter.h"

#include <filesystem>

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


void ResultFileExporter::exportResult(const SSimulationResultRecorder& recoder)
{
	//初期化ができていない場合は，なにも出力しない．また，出力フラグがfalseの場合もなにも出力しない．
	if (!m_init_success || !m_do_export) { return; }


	//出力先ファイルを作成する．
	std::string output_file_name = RESULT_FOLDER_NAME + "/" + m_folder_name + "/" + FILE_NAME + std::to_string(m_export_count + 1) + ".csv";

	std::ofstream ofs(output_file_name);

	//ファイルが作成できなかった場合は，なにも出力しない．
	if (!ofs) { return; }


	//結果を出力する．
	ofs << "@SimuRes" << std::endl;

	ofs << recoder;


	//結果の詳細を出力する．
	ofs << std::endl << "@SimuResDetail" << std::endl;

	outputResultDetail(recoder, ofs);


	++m_export_count;	//出力した回数をカウントアップする．


	//ファイルを閉じる．
	ofs.close();
}

void ResultFileExporter::outputResultDetail(const SSimulationResultRecorder& recoder, std::ofstream& stream)
{
	if (!stream) { return; }

	//時間の統計を出力する．
	double max_time = recoder.computation_time[0];
	double min_time = recoder.computation_time[0];
	double sum_time = 0.0;

	for (const auto& i : recoder.computation_time)
	{
		if (i > max_time) { max_time = i; }
		if (i < min_time) { min_time = i; }

		sum_time += i;
	}

	double average_time = sum_time / static_cast<double>(recoder.computation_time.size());

	stream << "最大探索時間," << max_time << ",[msec]" << std::endl;
	stream << "最小探索時間," << min_time << ",[msec]" << std::endl;
	stream << "総合探索時間," << sum_time << ",[msec]" << std::endl;
	stream << "平均探索時間," << average_time << ",[msec]" << std::endl;


	//移動距離の統計を出力する
	if (recoder.result_nodes.size() >= 2)
	{
		float x_move_sum = 0.0f;
		float y_move_sum = 0.0f;
		float z_move_sum = 0.0f;

		for (size_t i = 0; i != recoder.result_nodes.size() - 1; ++i)
		{
			x_move_sum += recoder.result_nodes[i + 1].global_center_of_mass.x - recoder.result_nodes[i].global_center_of_mass.x;
			y_move_sum += recoder.result_nodes[i + 1].global_center_of_mass.y - recoder.result_nodes[i].global_center_of_mass.y;
			z_move_sum += recoder.result_nodes[i + 1].global_center_of_mass.z - recoder.result_nodes[i].global_center_of_mass.z;
		}

		double x_move_average = x_move_sum / static_cast<double>(recoder.result_nodes.size() - 1);
		double y_move_average = y_move_sum / static_cast<double>(recoder.result_nodes.size() - 1);
		double z_move_average = z_move_sum / static_cast<double>(recoder.result_nodes.size() - 1);

		stream << "X方向総移動距離," << x_move_sum << ",[mm]" << std::endl;
		stream << "Y方向総移動距離," << y_move_sum << ",[mm]" << std::endl;
		stream << "Z方向総移動距離," << z_move_sum << ",[mm]" << std::endl;
		stream << "X方向平均移動距離," << x_move_average << ",[mm/動作]" << std::endl;
		stream << "Y方向平均移動距離," << y_move_average << ",[mm/動作]" << std::endl;
		stream << "Z方向平均移動距離," << z_move_average << ",[mm/動作]" << std::endl;
	}

}
