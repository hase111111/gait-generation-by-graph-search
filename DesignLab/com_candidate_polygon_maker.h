
//! @file      com_candidate_polygon_maker.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_COM_CANDIDATE_POLYGON_MAKER_H_
#define DESIGNLAB_COM_CANDIDATE_POLYGON_MAKER_H_

#include <array>
#include <memory>
#include <vector>

#include "math_polygon2.h"
#include "discrete_com_pos.h"
#include "interface_hexapod_coordinate_converter.h"
#include "robot_state_node.h"


namespace designlab
{

//! @struct ComPosAndPolygon
//! @brief 離散化された重心位置とその重心位置を含む多角形の組み合わせを表す構造体．
//! @see ComCandidatePolygonMaker
struct ComPosAndPolygon
{
    ComPosAndPolygon() : com_pos(enums::DiscreteComPos::kFront), polygon(), is_able(false) {}

    enums::DiscreteComPos com_pos;  //!< 離散化された重心位置．
    Polygon2 polygon;  //!< 重心位置を含む多角形．
    bool is_able;  //!< 重心位置を含む多角形が正しいかどうか．
};


//! @class ComCandidatePolygonMaker
//! @brief 重心位置の候補地点を示す多角形を作成するクラス．
//! @details 現在のロボットの状態を表すノードから，重心位置の候補地点を示す多角形を作成する．
//! @n 手法は波東さんの卒業研究を参考にしているため，詳細はそちらを参照のこと．
//! 当然だが，その手法ではロボットの姿勢変更を考慮していないのでこのクラスを使用する場合は，
//! ロボットの回転・旋回は行うことができない
class ComCandidatePolygonMaker final
{
public:
    static constexpr int kMakePolygonNum = 7;  //!< 作成する多角形の数．


    explicit ComCandidatePolygonMaker(
        const std::shared_ptr<const IHexapodCoordinateConverter>& converter_ptr);


    //! @brief 現在のロボットの状態を表すノードから，重心位置の候補地点を示す多角形を作成する．
    //! @param [in] node 現在のロボットの状態を表すノード．
    //! @param [out] output_poly 重心位置の候補地点を示す多角形．
    void MakeCandidatePolygon(const RobotStateNode& node,
                              std::array<ComPosAndPolygon, kMakePolygonNum>* output_poly) const;


private:
    //! 多角形のチェックを行う場合は trueにする．
    //! 重いので falseにしたいが，深さ5までなら問題なし．
    static constexpr bool kDoCheckPolygon = true;


    //! @brief 重心位置の候補地点を示す多角形を作成する．
    //! 中心周りの図形は4角形か5角形を用いて表現する．
    void MakeCandidateBox(const std::array<Vector2, HexapodConst::kLegNum>& leg_pos,
                          const int start_leg_num, Polygon2* output_poly) const;

    //! @brief 重心位置の候補地点を示す多角形を作成する．
    //! 中心周りの図形は3角形を用いて表現する．
    void MakeCandidateTriangle(const std::array<Vector2, HexapodConst::kLegNum>& leg_pos,
                               ComPosAndPolygon* output) const;

    //! @brief 正しい多角形が生成されているかを確認する．
    //! @param [in] _poly 確認する多角形．
    //! @return 正しい多角形が生成されているか．
    bool IsAblePolygon(const Polygon2& poly) const;

    const std::shared_ptr<const IHexapodCoordinateConverter> converter_ptr_;
};

}  // namespace designlab


#endif  // DESIGNLAB_COM_CANDIDATE_POLYGON_MAKER_H_
