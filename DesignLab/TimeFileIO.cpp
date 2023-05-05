#include "TimeFileIO.h"

TimeFileIO::TimeFileIO()
{
	m_file_name = "time.csv";
}

bool TimeFileIO::outputTimeFile(const std::vector<double> _time)
{
	//ファイルを開く
	std::ofstream _time_file;
	_time_file.open(m_file_name);

	//開けなければ終了
	if (!_time_file) { return false; }

	for (auto &i : _time)
	{
		_time_file << i << std::endl;
	}

	//ファイルを閉じる
	_time_file.close();

    return true;
}
