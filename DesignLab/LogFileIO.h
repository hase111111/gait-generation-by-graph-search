#pragma once
#include <string>
#include <fstream>
#include "Node.h"
#include "SimulateResult.h"

class LogFileIO final
{
public:
	LogFileIO();
	~LogFileIO() = default;

	//! @brief Logファイルを作成する．失敗するとfalseを返す
	//! @return bool ファイル作成に成功したらtrue，失敗したらfalse
	bool openLogFile();

	//! @brief Logファイルを閉じる
	void closeLogFile();

	//! @brief Logファイルに数値を書き込む
	//! @param [in] _data 書き込む数値
	void addLogStringWithInt(const int _data);

	//! @brief Logファイルにノードの情報を書き込む
	//! @param [in] num ノードの番号
	//! @param [in] node_log 書き込むノードの情報
	void addLogStringWithNode(const int num, const SNode& node_log);

	//! @brief Logファイルに文字列を書き込む
	//! @param [in] _str 書き込む文字列
	void addLogString(const std::string _str);

	////! @brief Logファイルにシミュレーションの結果を書き込む
	////! @param [in] _loop_num ループ回数
	////! @param [in] _res シミュレーションの結果
	//void addLogStringSimulation(const int _loop_num, const SimulateResult& _res);

private:
	const std::string LOG_NAME = "WalkingPatternLog";	//出力するファイル名
	const std::string LOG_EXTENSION = ".csv";			//拡張子

	int m_log_num = 1;
	std::string m_file_name;
	std::ofstream m_all_log;
};

//! @file LogFileIO.h
//! @brief ログファイルの入出力を行うクラス．
//! @date 2023/06/17
//! @author 長谷川

//! @class LogFileIO
//! @brief ログファイルの入出力を行うクラス．
//! @date 2023/06/17
//! @author 長谷川
