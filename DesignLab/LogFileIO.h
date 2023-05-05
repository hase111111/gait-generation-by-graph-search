#pragma once
#include <string>
#include <fstream>
#include "listFunc.h"
#include "SimulateResult.h"

// File Input Output つまりファイル入出力のこと

class LogFileIO final
{
public:
	LogFileIO();
	~LogFileIO() = default;

	//Logファイルを作成する．失敗するとfalseを返す
	bool openLogFile();

	//Logファイルを閉じる
	void closeLogFile();

	//Logファイルに数値を書き込む
	void addLogStringWithInt(const int _data);

	//Logファイルにノードの情報を書き込む
	void addLogStringWithNode(const int num, const LNODE& node_log);

	//Logファイルに文字列を書き込む
	void addLogString(const std::string _str);

	//Logファイルにシミュレーションの結果を書き込む
	void addLogStringSimulation(const int _loop_num, const SimulateResult& _res);

private:
	const std::string LOG_NAME = "WalkingPatternLog";	//出力するファイル名
	const std::string LOG_EXTENSION = ".csv";			//拡張子

	int m_log_num = 1;
	std::string m_file_name;
	std::ofstream m_all_log;
};