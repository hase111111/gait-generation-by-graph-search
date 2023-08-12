#pragma once
#include <string>
#include <fstream>
#include <vector>

class NodeFileIO final
{
public:
	NodeFileIO();
	~NodeFileIO() = default;

	//! @brief ノードの数の入ったvectorをファイルに出力する
	//! @param [in] _node ノードの数の入ったvector
	//! @return bool true:出力成功 false:出力失敗
	bool outputNodeFile(const std::vector<int> _node);

private:
	std::string m_file_name;
};


//! @file NodeFileIO.h
//! @brief ノードの数をファイルに出力するクラスのヘッダファイル
//! @date 2022/06/17
//! @author 長谷川

//! @class NodeFileIO
//! @brief ノードの数をファイルに出力するクラス
//! @date 2022/06/17
//! @author 長谷川
