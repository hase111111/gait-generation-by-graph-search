
//! @file      map_creator_by_csv.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#ifndef DESIGNLAB_MAP_CREATOR_BY_CSV_H_
#define DESIGNLAB_MAP_CREATOR_BY_CSV_H_

#include <string>

#include "interface_map_creator.h"


namespace designlab
{

//! @class MapCreatorByCsv
//! @brief CSV ファイルからマップを生成するクラス．
class MapCreatorByCsv final : public IMapCreator
{
public:
    //! @brief コンストラクタで作成するマップ情報を与える
    //! @param[in] map_file_path マップのCSVファイルのパス．
    explicit MapCreatorByCsv(const std::string& map_file_path);


    MapState InitMap() override;

    void UpdateMap(MapState* current_map) override;

private:
    std::string map_file_path_;  //!< マップの CSV ファイルのパス．
};

}  // namespace designlab


#endif  // DESIGNLAB_MAP_CREATOR_BY_CSV_H_
