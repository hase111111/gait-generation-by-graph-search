#include "result_file_exporter.h"

#include <filesystem>

#include "cmdio_util.h"
#include "stopwatch.h"


namespace dlio = designlab::cmdio;
namespace sf = std::filesystem;	//長すぎるので，filesystemの名前空間を短縮する．


const std::string ResultFileConst::kDirectoryName = sf::current_path().string() + "/result";

const std::string ResultFileConst::kFileName = "sim_result";

const std::string ResultFileConst::kNodeListName = "node_list";

const std::string ResultFileConst::kMapStateName = "map_state";


ResultFileExporter::ResultFileExporter() :
	export_count_(0),
	init_success_(false),
	do_export_(true)
{
}

void ResultFileExporter::Init()
{
	//resultフォルダがなければ作成する．
	if (not sf::exists(ResultFileConst::kDirectoryName))
	{
		dlio::Output("結果出力先フォルダ " + ResultFileConst::kDirectoryName + "が存在しないので作成します．", OutputDetail::kInfo);
		sf::create_directory(ResultFileConst::kDirectoryName);
	}

	//フォルダ名を指定する．現在時刻を取得し，それをフォルダ名にする．
	Stopwatch timer;
	folder_name_ = timer.GetNowTimeString();


	//出力先フォルダを作成する．
	std::string output_folder_name = ResultFileConst::kDirectoryName + "/" + folder_name_;

	if (sf::exists(output_folder_name))
	{
		//すでに同名のフォルダが存在する場合は，初期化失敗フラグを立てる．
		init_success_ = false;

		dlio::Output("結果出力先のフォルダ " + output_folder_name + "はすでに存在します．", OutputDetail::kError);

		return;
	}

	sf::create_directory(output_folder_name);	//フォルダを作成する．

	if (not sf::exists(output_folder_name))
	{
		//今度は逆に，フォルダが作成できなかった場合は，初期化失敗フラグを立てる．
		init_success_ = false;

		dlio::Output("結果出力先のフォルダ " + output_folder_name + "を作成できませんでした．", OutputDetail::kError);

		return;
	}

	init_success_ = true;
}


void ResultFileExporter::PushSimulationResult(const SimulationResultRecorder& simu_result)
{
	//結果をセットする
	result_list_.push_back(simu_result);
}


void ResultFileExporter::ExportLatestNodeList() const 
{
	//初期化ができていない場合は，なにも出力しない．また，出力フラグがfalseの場合もなにも出力しない．
	if (not init_success_)
	{
		dlio::Output("結果出力先のフォルダの初期化に失敗しているため，結果を出力できません", OutputDetail::kError);
		return;
	}

	if (not do_export_)
	{
		dlio::Output("結果出力フラグがfalseのため，結果を出力しません", OutputDetail::kInfo);
		return;
	}

	//出力先ファイルを作成する．
	std::string output_file_name = ResultFileConst::kDirectoryName + "/" + folder_name_ + "/" + ResultFileConst::kNodeListName + std::to_string(result_list_.size()) + ".csv";

	std::ofstream ofs(output_file_name);

	//ファイルが作成できなかった場合は，なにも出力しない．
	if (not ofs)
	{
		dlio::Output("ファイル " + output_file_name + "を作成できませんでした．", OutputDetail::kError);
		return;
	}

	for (const auto& i : result_list_.back().graph_search_result_recoder)
	{
		ofs << i.result_node << "\n";
	}

	ofs.close();
}


void ResultFileExporter::ExportLatestMapState() const 
{
	//初期化ができていない場合は，なにも出力しない．また，出力フラグがfalseの場合もなにも出力しない．
	if (not init_success_)
	{
		dlio::Output("結果出力先のフォルダの初期化に失敗しているため，結果を出力できません", OutputDetail::kError);
		return;
	}

	if (not do_export_)
	{
		dlio::Output("結果出力フラグがfalseのため，結果を出力しません", OutputDetail::kInfo);
		return;
	}

	//出力先ファイルを作成する．
	std::string output_file_name = ResultFileConst::kDirectoryName + "/" + folder_name_ + "/" + ResultFileConst::kMapStateName + std::to_string(result_list_.size()) + ".csv";

	std::ofstream ofs(output_file_name);

	//ファイルが作成できなかった場合は，なにも出力しない．
	if (not ofs)
	{
		dlio::Output("ファイル " + output_file_name + " を作成できませんでした．", OutputDetail::kError);
		return;
	}

	ofs << result_list_.back().map_state << "\n";

	ofs.close();
}


void ResultFileExporter::ExportResult(const SimulationResultRecorder& recoder)
{
	//初期化ができていない場合は，なにも出力しない．また，出力フラグがfalseの場合もなにも出力しない．
	if (not init_success_) 
	{
		dlio::Output("結果出力先のフォルダの初期化に失敗しているため，結果を出力できません", OutputDetail::kError);
		return; 
	}

	if (not do_export_) 
	{
		dlio::Output("結果出力フラグがfalseのため，結果を出力しません", OutputDetail::kInfo);
		return; 
	}


	//出力先ファイルを作成する．
	std::string output_file_name = ResultFileConst::kDirectoryName + "/" + folder_name_ + "/" + ResultFileConst::kFileName + std::to_string(export_count_ + 1) + ".csv";

	std::ofstream ofs(output_file_name);

	//ファイルが作成できなかった場合は，なにも出力しない．
	if (not ofs) { return; }


	//結果を出力する．
	ofs << "@SimuRes" << std::endl;

	ofs << recoder;


	//結果の詳細を出力する．
	ofs << std::endl << "@SimuResDetail" << std::endl;

	OutputResultDetail(recoder, ofs);


	++export_count_;	//出力した回数をカウントアップする．


	//ファイルを閉じる．
	ofs.close();
}

void ResultFileExporter::OutputResultDetail(const SimulationResultRecorder& recoder, std::ofstream& stream)
{
	if (!stream) { return; }

	//時間の統計を出力する．
	double max_time = recoder.graph_search_result_recoder[0].computation_time;
	double min_time = max_time;
	double sum_time = 0.0;

	for (const auto& i : recoder.graph_search_result_recoder)
	{
		if (i.computation_time > max_time) { max_time = i.computation_time; }
		if (i.computation_time < min_time) { min_time = i.computation_time; }

		sum_time += i.computation_time;
	}

	double average_time = sum_time / static_cast<double>(recoder.graph_search_result_recoder.size());

	stream << "最大探索時間," << max_time << ",[msec]" << std::endl;
	stream << "最小探索時間," << min_time << ",[msec]" << std::endl;
	stream << "総合探索時間," << sum_time << ",[msec]" << std::endl;
	stream << "平均探索時間," << average_time << ",[msec]" << std::endl;


	//移動距離の統計を出力する
	if (recoder.graph_search_result_recoder.size() > 1)
	{
		float x_move_sum = 0.0f;
		float y_move_sum = 0.0f;
		float z_move_sum = 0.0f;

		for (size_t i = 0; i != recoder.graph_search_result_recoder.size() - 1; ++i)
		{
			RobotStateNode current_node = recoder.graph_search_result_recoder[i].result_node;
			RobotStateNode next_node = recoder.graph_search_result_recoder[i + 1].result_node;
			designlab::Vector3 com_dif = next_node.global_center_of_mass - current_node.global_center_of_mass;

			x_move_sum += com_dif.x;
			y_move_sum += com_dif.y;
			z_move_sum += com_dif.z;
		}

		double x_move_average = x_move_sum / static_cast<double>(recoder.graph_search_result_recoder.size() - 1);
		double y_move_average = y_move_sum / static_cast<double>(recoder.graph_search_result_recoder.size() - 1);
		double z_move_average = z_move_sum / static_cast<double>(recoder.graph_search_result_recoder.size() - 1);

		stream << "X方向総移動距離," << x_move_sum << ",[mm]" << std::endl;
		stream << "Y方向総移動距離," << y_move_sum << ",[mm]" << std::endl;
		stream << "Z方向総移動距離," << z_move_sum << ",[mm]" << std::endl;
		stream << "X方向平均移動距離," << x_move_average << ",[mm/動作]" << std::endl;
		stream << "Y方向平均移動距離," << y_move_average << ",[mm/動作]" << std::endl;
		stream << "Z方向平均移動距離," << z_move_average << ",[mm/動作]" << std::endl;
	}

}
