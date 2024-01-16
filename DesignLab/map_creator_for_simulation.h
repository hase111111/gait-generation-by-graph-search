
//! @file      map_creator_for_simulation.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_MAP_CREATOR_FOR_SIMULATION_H_
#define DESIGNLAB_MAP_CREATOR_FOR_SIMULATION_H_

#include <vector>

#include "math_vector3.h"
#include "interface_map_creator.h"
#include "map_state.h"
#include "simulation_map_parameter.h"


namespace designlab
{

//! @class MapCreatorForSimulation
//! @brief シミュレーション用のマップを生成するクラス．
//! @details MapStateクラスの初期化処理のなかで呼んで，マップを生成してくれるクラス．
//! @n 先行研究のプログラムでは mainfunction.h で実装されていた処理である．
class MapCreatorForSimulation final : public IMapCreator
{
public:
    //! @brief コンストラクタで作成するマップ情報を与える
    //! @param[in] param マップ生成のモードとオプションを指定する構造体．
    explicit MapCreatorForSimulation(const SimulationMapParameter& param);


    MapState InitMap() override;

    void UpdateMap(MapState* current_map) override;


private:
    //! フラットなマップを生成する．
    void CreateFlatMap(std::vector<Vector3>* map) const;

    //! 縦じま模様の穴が開いたマップを生成する．
    void CreateVerticalStripeMap(std::vector<Vector3>* map) const;

    //! 横じま模様の穴が開いたマップを生成する．
    void CreateHorizontalStripeMap(std::vector<Vector3>* map) const;

    //! 斜めじま模様の穴が開いたマップを生成する．
    void CreateDiagonalStripeMap(std::vector<Vector3>* map) const;

    //! 網目模様の穴が開いたマップを生成する．
    void CreateMeshMap(std::vector<Vector3>* map)  const;

    //! 網目模様の穴が開いたマップを生成する．
    void CreateLatticePointMap(std::vector<Vector3>* map) const;

    //! 円形のマップを生成する．
    void CreateCircleMap(std::vector<Vector3>* map) const;

    //! ドーナツ型のマップを生成する．
    void CreateDonutMap(std::vector<Vector3>* map) const;

    //! 生成されたマップにランダムな穴をあける．
    void ChangeMapToPerforated(std::vector<Vector3>* map) const;

    //! 生成されたマップを階段状にする．
    void ChangeMapToStep(std::vector<Vector3>* map) const;

    //! 生成されたマップを坂道にする．
    void ChangeMapToSlope(std::vector<Vector3>* map) const;

    //! 生成されたマップを傾ける．
    void ChangeMapToTilt(std::vector<Vector3>* map) const;

    //! 生成されたマップをデコボコにする．
    void ChangeMapToRough(std::vector<Vector3>* map) const;

    //! 生成されたマップに放射状の穴をあける．
    void ChangeMapToRadial(std::vector<Vector3>* map) const;


    const SimulationMapParameter messenger_;  //!< マップ生成のモードを指定する列挙体．
};

}  // namespace designlab


#endif  // DESIGNLAB_MAP_CREATOR_FOR_SIMULATION_H_
