
//! @file      result_file_importer.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//! Saitama University All right reserved.

#ifndef DESIGNLAB_RESULT_FILE_IMPORTER_H_
#define DESIGNLAB_RESULT_FILE_IMPORTER_H_

#include <string>
#include <vector>

#include "result_file_exporter.h"


namespace designlab
{

//! @class ResultFileImporter
//! @brief 結果をファイルから読み込むクラス．
class ResultFileImporter final
{
public:
    //! @brief ノードリストとマップの状態をファイルから読み込む．
    //! @param[in] file_path ノードリストのパス．
    //! @param[out] node_list ノードリスト．
    //! @param[out] map_state マップの状態．
    //! @retval true 成功．
    //! @retval false 失敗．
    bool ImportNodeListAndMapState(
      const std::string& file_path,
      std::vector<RobotStateNode>* node_list,
      MapState* map_state) const;


private:
    bool ImportNodeList(const std::string& file_path,
                        std::vector<RobotStateNode>* node_list) const;

    bool ImportMapState(const std::string& file_path, MapState* map_state) const;
};

}  // namespace designlab


#endif  // DESIGNLAB_RESULT_FILE_IMPORTER_H_
