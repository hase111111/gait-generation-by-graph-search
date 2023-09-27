#include "file_tree.h"

#include <filesystem>

#include "cmdio_util.h"


namespace dlio = designlab::cmdio;


void FileTree::DisplayFileTree(const std::string& path, int max_depth) const
{
	FileTreeData tree = MakeFileTree(path, max_depth);

    OutputFileTree(tree, 0);
}

FileTree::FileTreeData FileTree::MakeFileTree(const std::string& path, int max_depth) const
{
    FileTreeData tree;

    tree.path = path;

    for (const auto& entry : std::filesystem::directory_iterator(path)) 
    {
        if (entry.is_directory()) 
        {
            // ディレクトリの場合、再帰的にファイルツリーを作成
            if (max_depth == 0) 
            {
                tree.directory.push_back(FileTreeData{ entry.path().string(), {}, {} });
            }
            else
            {
                tree.directory.push_back(MakeFileTree(entry.path().string(), max_depth - 1));
            }
        }
        else if (entry.is_regular_file()) 
        {
            // ファイルの場合、ファイル名を追加
            tree.file.push_back(entry.path().filename().string());
        }
    }

    return tree;
}

void FileTree::OutputFileTree(const FileTreeData& tree, int depth) const
{
    std::string indent = "";
    for (int i = 0; i < depth; ++i)
    {
		indent += "| ";
	}

    // ディレクトリ名を出力する際に，パスの階層を削除する
    std::string::size_type pos = tree.path.find_last_of("/\\");
    std::string dir_name = ((depth == 0) ? "" : "- ");
    dir_name += std::string("[ ") + tree.path.substr(pos + 1) + std::string(" ]");

	dlio::Output(indent + dir_name, OutputDetail::kSystem);

    for (const auto& directory : tree.directory)
    {
		OutputFileTree(directory, depth + 1);
	}

    for (const auto& file : tree.file)
    {
		dlio::Output(indent + "|- " + file, OutputDetail::kSystem);
	}

    dlio::Output(indent, OutputDetail::kSystem);
}
