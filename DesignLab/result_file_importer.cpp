#include "result_file_importer.h"

#include <filesystem>
#include <fstream>
#include <sstream>

#include "cassert_define.h"
#include "cmdio_util.h"

namespace dle = ::designlab::enums;
namespace dlio = ::designlab::cmdio;
namespace fs = std::filesystem;


bool ResultFileImporter::ImportNodeListAndMapState(const std::string& file_path, std::vector<RobotStateNode>* node_list, MapState* map_state) const
{
	// 引数の確認
	assert(node_list != nullptr);
	assert(node_list->empty());
	assert(map_state != nullptr);
	assert(map_state->GetMapPointSize() == 0);


	// ファイルが存在するかどうかを確認．ないならばfalseを返す．
	if (!fs::exists(file_path))
	{
		dlio::Output("NodeListファイルが存在しませんでした．", dle::OutputDetail::kError);
		return false;
	}

	// node_list1.csv ならば map_state1.csvも読み込む
	std::string map_file_path = file_path;
	map_file_path.replace(map_file_path.find(ResultFileConst::kNodeListName), ResultFileConst::kNodeListName.size(), ResultFileConst::kMapStateName);

	if (!fs::exists(map_file_path))
	{
		dlio::Output("MapStateファイルが存在しませんでした．", dle::OutputDetail::kError);
		return false;
	}


	if (!ImportNodeList(file_path, node_list) || !ImportMapState(map_file_path, map_state))
	{
		dlio::Output("ファイル読み込み中にエラーが発生しました．", dle::OutputDetail::kError);
	}

	return true;
}

bool ResultFileImporter::ImportNodeList(const std::string& file_path, [[maybe_unused]] std::vector<RobotStateNode>* node_list) const
{
	// ファイルを開く
	std::ifstream ifs(file_path);

	// ファイルが開けないならばfalseを返す．
	if (not ifs.is_open())
	{
		dlio::Output("ファイルを開けませんでした．", dle::OutputDetail::kSystem);

		return false;
	}


	// ファイルを読み込む
	std::string str;
	std::vector<std::string> str_list;

	while (std::getline(ifs, str))
	{
		str_list.push_back(str);
	}

	// ファイルを閉じる
	ifs.close();

	// ファイルの内容を解析する
	// ノードリストの読み込み
	for (const auto& i : str_list)
	{
		RobotStateNode node;
		std::stringstream ss(i);

		node = RobotStateNode::FromString(ss.str());

		(*node_list).push_back(node);
	}

	return true;
}

bool ResultFileImporter::ImportMapState(const std::string& file_path, MapState* map_state) const
{
	assert(map_state != nullptr);

	// ファイルを開く
	std::ifstream ifs(file_path);

	// ファイルが開けないならばfalseを返す．
	if (not ifs.is_open())
	{
		dlio::Output("ファイルを開けませんでした．", dle::OutputDetail::kSystem);

		return false;
	}


	// ファイルを読み込む
	std::string str;
	std::vector<std::string> str_list;

	while (std::getline(ifs, str))
	{
		str_list.push_back(str);
	}

	// ファイルを閉じる
	ifs.close();

	// ファイルの内容を解析する
	(*map_state).ClearMapPoint();

	for (const auto& i : str_list)
	{
		designlab::Vector3 point;
		std::stringstream ss(i);

		try
		{
			ss >> point;

			(*map_state).AddMapPoint(point);
		}
		catch (...)
		{
			// 何もしない
		}
	}

	return true;
}