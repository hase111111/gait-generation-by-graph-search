
//! @author    hasegawa
//! @copyright 埼玉大学 設計工学研究室 2024. All right reserved.

#include "file_tree.h"

#include <filesystem>

#include "cassert_define.h"
#include "cmdio_util.h"


namespace designlab
{

void FileTree::DisplayFileTree(const std::string& path, int max_depth) const
{
    FileTreeData tree = MakeFileTree(path, max_depth, "", "sim");

    int count = 0;
    OutputFileTree(tree, 0, true, &count);
}

bool FileTree::SelectFile(const std::string& path, int max_depth,
                          const std::string& extension, const std::string keyword,
                          std::string* output) const
{
    assert(output != nullptr);

    // ファイルツリーを作成
    FileTreeData tree = MakeFileTree(path, max_depth, extension, keyword);

    // ファイルツリーを表示
    CmdIOUtil::OutputHorizontalLine("*", OutputDetail::kSystem);

    int count = 0;
    OutputFileTree(tree, 0, true, &count);
    CmdIOUtil::OutputNewLine(1, OutputDetail::kSystem);
    CmdIOUtil::OutputHorizontalLine("*", OutputDetail::kSystem);
    CmdIOUtil::OutputNewLine(1, OutputDetail::kSystem);

    // ファイルを選択
    std::vector<std::string> file_list = MakeFileList(tree);

    if (file_list.empty())
    {
        CmdIOUtil::Output("The file did not exist.", OutputDetail::kSystem);
        return false;
    }

    while (true)
    {
        int select_index =
            CmdIOUtil::InputInt(0, static_cast<int>(file_list.size()) - 1, 0,
                                "Please select a file. Enter an integer.");

        CmdIOUtil::OutputNewLine(1, OutputDetail::kSystem);
        CmdIOUtil::Output("The selected file is " + file_list[static_cast<size_t>(select_index)],
                          OutputDetail::kSystem);

        if (CmdIOUtil::InputYesNo("Are you sure?"))
        {
            *output = file_list[static_cast<size_t>(select_index)];
            break;
        }

        CmdIOUtil::OutputNewLine(1, OutputDetail::kSystem);
    }

    return true;
}

FileTree::FileTreeData FileTree::MakeFileTree(const std::string& path, int max_depth,
                                              const std::string& extension,
                                              const std::string keyword) const
{
    FileTreeData tree;

    tree.path = path;

    assert(std::filesystem::exists(tree.path));

    for (const auto& entry : std::filesystem::directory_iterator(path))
    {
        if (entry.is_directory())
        {
            // ディレクトリの場合、再帰的にファイルツリーを作成
            if (max_depth == 0)
            {
                tree.directory.push_back(
                    FileTreeData{ entry.path().string(), {}, {} });
            }
            else
            {
                tree.directory.push_back(
                    MakeFileTree(entry.path().string(), max_depth - 1, extension, keyword));
            }
        }
        else if (entry.is_regular_file())
        {
            // ファイルの場合、ファイル名を追加

            if (!extension.empty())
            {
                if (!(entry.path().extension().string() == extension ||
                    entry.path().extension().string() == "." + extension))
                {
                    continue;
                }
            }

            if (!keyword.empty())
            {
                if (entry.path().filename().string().find(keyword) == std::string::npos)
                {
                    continue;
                }
            }

            tree.file.push_back(entry.path().filename().string());
        }
    }

    return tree;
}

void FileTree::OutputFileTree(const FileTreeData& tree, int depth,
                              bool not_display_empty, int* file_count) const
{
    assert(file_count != nullptr);

    std::string indent = "";
    for (int i = 0; i < depth; ++i)
    {
        indent += "| ";
    }

    // 空 かつ 空のディレクトリを表示しない設定 でないならば表示する
    if (!(not_display_empty && tree.file.empty() && tree.directory.empty()))
    {
        // ディレクトリ名を出力する際に，パスの階層を削除する

        CmdIOUtil::Output(indent, OutputDetail::kSystem);

        std::string::size_type pos = tree.path.find_last_of("/\\");
        std::string dir_name = ((depth == 0) ? "" : "- ");
        dir_name += std::string("[ ") + tree.path.substr(pos + 1) + std::string(" ]");

        CmdIOUtil::Output(indent + dir_name, OutputDetail::kSystem);
    }

    for (const auto& directory : tree.directory)
    {
        OutputFileTree(directory, depth + 1, not_display_empty, file_count);
    }

    if (!tree.file.empty())
    {
        CmdIOUtil::Output(indent + "|", OutputDetail::kSystem);

        for (const auto& file : tree.file)
        {
            CmdIOUtil::Output(indent + "|- " + file + " [-" + std::to_string(*file_count) + "-]",
                              OutputDetail::kSystem);

            (*file_count)++;
        }
    }
}

std::vector<std::string> FileTree::MakeFileList(const FileTreeData& tree) const
{
    std::vector<std::string> file_list;

    for (const auto& directory : tree.directory)
    {
        std::vector<std::string> tmp = MakeFileList(directory);

        file_list.insert(file_list.end(), tmp.begin(), tmp.end());
    }

    for (const auto& file : tree.file)
    {
        file_list.push_back(tree.path + "\\" + file);
    }

    return file_list;
}

}  // namespace designlab
