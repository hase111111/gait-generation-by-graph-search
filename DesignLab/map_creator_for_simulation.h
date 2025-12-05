
//! @file map_creator_for_simulation.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#pragma once

#include <vector>

#include "interface_map_creator.h"
#include "map_state.h"
#include "simulation_map_parameter.h"
#include "vector3.h"

namespace gaitgen {

//! @class MapCreatorForSimulation
//! @brief シミュレーション用のマップを生成するクラス.
//! @details
//! MapState クラスの初期化処理の中で呼ばれ,マップを生成するクラス.
class MapCreatorForSimulation final : public IMapCreator {
 public:
  //! @brief コンストラクタで作成するマップ情報を与える.
  //! @param[in] param マップ生成のモードとオプションを指定する構造体.
  explicit MapCreatorForSimulation(const SimulationMapParameter& param);

  MapState InitMap() const override;

  void UpdateMap(MapState* current_map) const override;

 private:
  //! フラットなマップを生成する.
  void CreateFlatMap(std::vector<Vector3>* map) const;

  //! 縦じま模様の穴が開いたマップを生成する.
  void CreateVerticalStripeMap(std::vector<Vector3>* map) const;

  //! 横じま模様の穴が開いたマップを生成する.
  void CreateHorizontalStripeMap(std::vector<Vector3>* map) const;

  //! 斜めじま模様の穴が開いたマップを生成する.
  void CreateDiagonalStripeMap(std::vector<Vector3>* map) const;

  //! 斜め線模様の穴が開いたマップを生成する.
  void CreateSlantedStripesMap(std::vector<Vector3>* map) const;

  //! 網目模様の穴が開いたマップを生成する.
  void CreateMeshMap(std::vector<Vector3>* map) const;

  //! 網目模様の穴が開いたマップを生成する.
  void CreateLatticePointMap(std::vector<Vector3>* map) const;

  //! 円形のマップを生成する.
  void CreateCircleMap(std::vector<Vector3>* map) const;

  //! ドーナツ型のマップを生成する.
  void CreateDonutMap(std::vector<Vector3>* map) const;

  //! 壁のマップを生成する.
  void CreateWallMap(std::vector<Vector3>* map) const;

  //! 生成されたマップにランダムな穴をあける.
  void ChangeMapToPerforated(std::vector<Vector3>* map) const;

  //! 生成されたマップを階段状にする.
  void ChangeMapToStep(std::vector<Vector3>* map) const;

  //! 生成されたマップを坂道にする.
  void ChangeMapToSlope(std::vector<Vector3>* map) const;

  //! 生成されたマップを傾ける.
  void ChangeMapToTilt(std::vector<Vector3>* map) const;

  //! 生成されたマップをデコボコにする.
  void ChangeMapToRough(std::vector<Vector3>* map) const;

  //! 生成されたマップに放射状の穴をあける.
  void ChangeMapToRadial(std::vector<Vector3>* map) const;

  //! マップ生成のモードを指定する列挙体.
  const SimulationMapParameter parameter_;
};

}  // namespace gaitgen
