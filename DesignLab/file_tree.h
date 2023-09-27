//! @file file_tree.h
//! @brief ファイルツリーを作成するクラス．

#ifndef DESIGNLAB_FILE_TREE_H_
#define DESIGNLAB_FILE_TREE_H_

#include <string>
#include <vector>

class FileTree
{
public:

	//! @brief ファイルツリーを作成する
	//! @param [in] path ファイルツリーを作成するディレクトリのパス
	//! @param [in] max_depth ファイルツリーの深さ，-1ならば無限に行う
	void DisplayFileTree(const std::string& path, int max_depth) const;

private:

	struct FileTreeData
	{
		FileTreeData() : path(""), directory({}), file({}) {}
		FileTreeData(const std::string& _path, const std::vector<FileTreeData>& _directory, const std::vector<std::string>& _file) : 
			path(_path), 
			directory(_directory), 
			file(_file) 
		{
		}

		std::string path;					//!< パス
		std::vector<FileTreeData> directory; //!< ディレクトリ
		std::vector<std::string> file;		//!< ファイル
	};

	//! @brief ファイルツリーを作成する
	//! @param [in] path ファイルツリーを作成するディレクトリのパス
	//! @param [in] max_depth ファイルツリーの深さ，-1ならば無限に行う
	//! @return FileTreeData ファイルツリーのデータ
	FileTreeData MakeFileTree(const std::string& path, int max_depth) const;

	void OutputFileTree(const FileTreeData& tree,int depth) const;
};


#endif	// DESIGNLAB_FILE_TREE_H_