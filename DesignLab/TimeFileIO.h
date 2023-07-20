#pragma once
#include <string>
#include <fstream>
#include <vector>

class TimeFileIO final 
{
public:
	TimeFileIO();
	~TimeFileIO() = default;

	//! @brief 計測にかかった時間の入ったvectorをファイルに出力する
	//! @param [in] _time 計測にかかった時間の入ったvector
	//! @return bool true:出力成功 false:出力失敗
	bool outputTimeFile(const std::vector<double> _time);

private:
	std::string m_file_name;
};


//! @file TimeFileIO.h
//! @brief 計測にかかった時間をファイルに出力するクラスのヘッダファイル
//! @date 2022/06/17
//! @auther 長谷川

//! @class TimeFileIO
//! @brief 計測にかかった時間をファイルに出力するクラス
//! @date 2022/06/17
//! @auther 長谷川
