
//! @file      com_selector.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_COM_SELECTOR_H_
#define DESIGNLAB_COM_SELECTOR_H_

#include <memory>
#include <utility>
#include <vector>

#include "com_type.h"
#include "math_polygon2.h"
#include "math_vector3.h"
#include "interface_hexapod_posture_validator.h"
#include "robot_state_node.h"


namespace designlab
{

//! @class ComSelector
//! @brief 波東さんのプログラムにおける CCC の処理と同様の処理を行い，重心を求めるクラス．
//! @details 重心位置の決め方は波東さんのプログラムに準拠している．
//! @n まずは，候補地点の多角形を囲む四角形を生成，その中に等間隔で候補点を打つ．
//! @n 次に多角形の中に入っていない点を除外する．
//! @n そして，絶対安全余裕を計算し，マージンを外れた点を除外する．
//! @n また，移動後の座標において，脚が他の脚と干渉する場合は除外する．
//! @n 以上の処理を行った後，残った点の重心を求める．
//! @note CCC では Target の値を持っているので，その値を利用して重心位置選択するが，
//! この実装ではこのクラスにその選択を任せたくない．
//! 同様の処理を行うために，Target の値を適当に決めている．
class ComSelector final
{
public:
    explicit ComSelector(const std::shared_ptr<const IHexapodPostureValidator>& checker_ptr) :
        checker_ptr_(checker_ptr)
    {
    };

    //! @brief 重心を求める．
    //! @param[in] polygon 重心を求める対象のポリゴン．この中に入る点を出力する．
    //! @param[in] current_node 現在のノード．
    //! @param[out] output_com 重心．
    //! @retval true 重心を求めることができた．
    //! @retval false 重心を求めることができなかった．
    bool GetComFromPolygon(const Polygon2& polygon,
                           const RobotStateNode& current_node, Vector3* output_com) const;

private:
    static constexpr int kDiscretizationNum = 10;  //!< 重心を求める際の分割数．

    //! @todo この値を PhantomXmk2 に移行する
    const float kStabilityMargin = 10.0f;  //!< 絶対安全余裕．

    //! @brief 候補地点を生成する．
    bool MakeComCandidatePoint(
        const Polygon2& polygon,
        std::pair<bool, Vector2> output_coms[kDiscretizationNum * kDiscretizationNum]) const;

    //! @brief 絶対安全余裕を計算し，マージンを外れていないか調べる．
    bool IsInMargin(const Polygon2& polygon,
                    const std::vector<Vector2>& edge_vec, const Vector2& candidate_point) const;

    const std::shared_ptr<const IHexapodPostureValidator> checker_ptr_;
};

}  // namespace designlab


#endif  // DESIGNLAB_COM_SELECTOR_H_
