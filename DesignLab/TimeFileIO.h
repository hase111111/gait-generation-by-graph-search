#pragma once
#include <string>
#include <fstream>
#include <vector>

class TimeFileIO final 
{
public:
	TimeFileIO();
	~TimeFileIO() = default;

	//計測にかかった時間の入ったvectorをファイルに出力する
	bool outputTimeFile(const std::vector<double> _time);

private:
	std::string m_file_name;
};
