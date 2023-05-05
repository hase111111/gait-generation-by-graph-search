#include "NodeFileIO.h"

NodeFileIO::NodeFileIO()
{
	m_file_name = "last_node_num.csv";
}

bool NodeFileIO::outputNodeFile(const std::vector<int> _node)
{
	//ファイルを開く
	std::ofstream _node_file;
	_node_file.open(m_file_name);

	//開けなければfalse
	if (!_node_file) { return false; }

	//出力する
	for (auto &i : _node) 
	{
		_node_file << i << std::endl;
	}

	//ファイルを閉じる
	_node_file.close();	

	return true;
}
