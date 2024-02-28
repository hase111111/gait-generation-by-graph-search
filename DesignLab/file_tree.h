
//! @file      file_tree.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#ifndef DESIGNLAB_FILE_TREE_H_
#define DESIGNLAB_FILE_TREE_H_

#include <string>
#include <vector>


namespace designlab
{

//! @class FileTree
//! @brief ファイルツリーを作成するクラス．
class FileTree final
{
public:
    //! @brief ファイルツリーを表示する．
    //! @param[in] path ファイルツリーを作成するディレクトリのパス．
    //! @param[in] max_depth ファイルツリーの深さ，-1ならば無限に行う．
    void DisplayFileTree(const std::string& path, int max_depth) const;

    //! @brief ディレクトリの中から，ファイルを一つ選択する．
    //! @param[in] path ファイルツリーを作成するディレクトリのパス．
    //! @param[in] max_depth ファイルツリーの深さ，-1ならば無限に行う．
    //! @param[in] extension ファイルの拡張子( ".txt" ".csv" など)．
    //! 空文字ならば全てのファイルを対象とする．
    //! @param[in] keyword ファイル名に含まれるキーワード．
    //! 空文字ならば全てのファイルを対象とする．
    //! @param[out] output 選択されたファイルのパス．
    //! @retval true ファイルが選択された．
    //! @retval false ファイルが選択されなかった．
    bool SelectFile(const std::string& path, int max_depth,
                    const std::string& extension, const std::string keyword,
                    std::string* output) const;

private:
    //! @struct FileTreeData
    //! @brief ファイルツリーのデータ．
    //! @n ディレクトリを表現している．再帰的に処理をするため自分自身を所有する．
    struct FileTreeData final
    {
        FileTreeData() : path(""), directory({}), file({}) {}

        FileTreeData(const std::string& _path,
                     const std::vector<FileTreeData>& _directory,
                     const std::vector<std::string>& _file) :
            path(_path),
            directory(_directory),
            file(_file)
        {
        }

        std::string path;                       //!< 自分自身へのパス．
        std::vector<FileTreeData> directory;    //!< ディレクトリ．
        std::vector<std::string> file;          //!< ディレクトリの中のファイル．
    };

    //! @brief ファイルツリーを作成する．
    //! @param[in] path ファイルツリーを作成するディレクトリのパス．
    //! @param[in] max_depth ファイルツリーの深さ，-1ならば無限に行う．
    //! @param[in] extension ファイルの拡張子( ".txt" ".csv" など)
    //! 空文字ならば全てのファイルを対象とする．
    //! @param[in] keyword ファイル名に含まれるキーワード，
    //! 空文字ならば全てのファイルを対象とする．
    //! @return ファイルツリーのデータ．
    FileTreeData MakeFileTree(
        const std::string& path, int max_depth,
        const std::string& extension, const std::string keyword) const;

    //! @brief ファイルツリーをコマンドラインへ出力する．
    //! @n ファイルツリーのデータを再帰的に出力する．
    //! @param[in] tree ファイルツリーのデータ．
    //! @param[in] depth ディレクトリの階層の深さ．
    //! @param[in] not_display_empty 空のディレクトリを表示しないならば
    //!  true を指定する．
    //! @param[in] file_count ファイルの番号を割り当てるためのカウンタ．
    void OutputFileTree(const FileTreeData& tree, int depth, bool not_display_empty,
                        int* file_count) const;

    //! @brief ファイルツリーのデータから，ファイルのリストを作成する．
    //! @param[in] tree ファイルツリーのデータ．
    //! @return ファイルのリスト．
    std::vector<std::string> MakeFileList(const FileTreeData& tree) const;
};

}  // namespace designlab


#endif  // DESIGNLAB_FILE_TREE_H_
