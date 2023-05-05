#pragma once
#include <string>
#include <fstream>
#include <vector>

class NodeFileIO final
{
public:
	NodeFileIO();
	~NodeFileIO() = default;

	//ノードの数の入ったvectorをファイルに出力する
	bool outputNodeFile(const std::vector<int> _node);

private:
	std::string m_file_name;
};
