//! @file file_tree.h
//! @brief ファイルツリーを作成するクラス．

#ifndef DESIGNLAB_FILE_TREE_H_
#define DESIGNLAB_FILE_TREE_H_


#include <string>
#include <vector>


//! @class FileTree
//! @brief ファイルツリーを作成するクラス．
class FileTree
{
public:

	//! @brief ファイルツリーを表示する
	//! @param [in] path ファイルツリーを作成するディレクトリのパス
	//! @param [in] max_depth ファイルツリーの深さ，-1ならば無限に行う
	void DisplayFileTree(const std::string& path, int max_depth) const;

	//! @brief ディレクトリの中から，ファイルを一つ選択する
	//! @param [in] path ファイルツリーを作成するディレクトリのパス
	//! @param [in] max_depth ファイルツリーの深さ，-1ならば無限に行う
	//! @param [in] extension ファイルの拡張子( ".txt" ".csv" など)，空文字ならば全てのファイルを対象とする
	//! @param [in] keyword ファイル名に含まれるキーワード，空文字ならば全てのファイルを対象とする
	//! @param [out] output 選択されたファイルのパス
	//! @return bool ファイルが選択されたかどうか
	bool SelectFile(const std::string& path, int max_depth, const std::string& extension, const std::string keyword,std::string *output);

private:

	//! @struct FileTreeData
	//! @brief ファイルツリーのデータ．再帰的に自分自身を所有する．
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
		std::vector<FileTreeData> directory;//!< ディレクトリ
		std::vector<std::string> file;		//!< ファイル
	};

	//! @brief ファイルツリーを作成する
	//! @param [in] path ファイルツリーを作成するディレクトリのパス
	//! @param [in] max_depth ファイルツリーの深さ，-1ならば無限に行う
	//! @param [in] extension ファイルの拡張子( ".txt" ".csv" など)，空文字ならば全てのファイルを対象とする
	//! @param [in] keyword ファイル名に含まれるキーワード，空文字ならば全てのファイルを対象とする
	//! @return FileTreeData ファイルツリーのデータ
	FileTreeData MakeFileTree(const std::string& path, int max_depth,const std::string& extension,const std::string keyword) const;

	//! @brief ファイルツリーを出力する
	//! @n ファイルツリーのデータを再帰的に出力する
	//! @param [in] tree ファイルツリーのデータ
	//! @param [in] depth 描画の深さ
	//! @param [in] not_display_empty 空のディレクトリを表示しないかどうか
	//! @param [in] file_count ファイルの番号を割り当てるためのカウンタ
	void OutputFileTree(const FileTreeData& tree,int depth, bool not_display_empty, int* file_count) const;

	//! @brief ファイルツリーのデータから，ファイルのリストを作成する
	//! @param [in] tree ファイルツリーのデータ
	std::vector<std::string> MakeFileList(const FileTreeData& tree) const;
};


#endif	// DESIGNLAB_FILE_TREE_H_