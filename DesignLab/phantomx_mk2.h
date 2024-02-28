
//! @file      phantomx_mk2.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#ifndef DESIGNLAB_PHANTOMX_MK2_H_
#define DESIGNLAB_PHANTOMX_MK2_H_

#include "interface_hexapod_coordinate_converter.h"
#include "interface_hexapod_joint_calculator.h"
#include "interface_hexapod_state_presenter.h"
#include "interface_hexapod_posture_validator.h"
#include "phantomx_mk2_parameter_record.h"


namespace designlab
{

//! @class PhantomXMkII
//! @brief PhantomX mk-2 の状態を計算するクラス．
class PhantomXMkII :
    public IHexapodCoordinateConverter,
    public IHexapodJointCalculator,
    public IHexapodStatePresenter,
    public IHexapodPostureValidator
{
public:
    explicit PhantomXMkII(const PhantomXMkIIParameterRecord& parameter_record);


    HexapodJointState CalculateJointState(const int leg_index,
                                          const Vector3& leg_pos) const noexcept override;

    bool IsValidJointState(const int leg_index, const Vector3& leg_pos,
                           const HexapodJointState& joint_state) const noexcept override;


    Vector3 ConvertGlobalToLegCoordinate(
        const Vector3& converted_position, int leg_index,
        const Vector3& center_of_mass_global, const Quaternion& robot_quat,
        const bool consider_rot) const override;

    Vector3 ConvertLegToGlobalCoordinate(
        const Vector3& converted_position, int leg_index,
        const Vector3& center_of_mass_global, const Quaternion& robot_quat,
        const bool consider_rot) const override;

    Vector3 ConvertRobotToGlobalCoordinate(
        const Vector3& converted_position,
        const Vector3& center_of_mass_global, const Quaternion& robot_quat,
        const bool consider_rot) const override;

    Vector3 ConvertRobotToLegCoordinate(const Vector3& converted_position, int leg_index) const;

    Vector3 ConvertLegToRobotCoordinate(const Vector3& converted_position, int leg_index) const;


    Vector3 GetFreeLegPosLegCoordinate(int leg_index) const noexcept override;

    Vector3 GetLegBasePosRobotCoordinate(int leg_index) const noexcept override;

    float GetGroundHeightMarginMin() const noexcept override;

    float GetGroundHeightMarginMax() const noexcept override;


    bool IsLegInRange(int leg_index, const Vector3& leg_pos) const override;

    bool IsLegInterfering(const std::array<Vector3, HexapodConst::kLegNum>& leg_pos) const override;

    float CalculateStabilityMargin(const leg_func::LegStateBit& leg_state,
      const std::array<Vector3, HexapodConst::kLegNum>& leg_pos) const override;

    bool IsStable(const leg_func::LegStateBit& leg_state,
      const std::array<Vector3, HexapodConst::kLegNum>& leg_pos) const override;

    bool IsBodyInterferingWithGround(const RobotStateNode& node,
                                     const DividedMapState& devide_map) const override;

private:
    static constexpr int kMaxLegRSize{ 200 };   //!< kMaxLegRの配列のサイズ．


    std::array<float, kMaxLegRSize> InitMaxLegR() const;
    std::array<Vector2, HexapodConst::kLegNum> InitMinLegPosXY() const;
    std::array<Vector2, HexapodConst::kLegNum> InitMaxLegPosXY() const;

    //! 地面から胴体を持ち上げる高さ[mm]．最小ここまで下げられる．
    const float kBodyLiftingHeightMin;

    //! 地面から胴体を持ち上げる高さ[mm]．最大ここまで上げられる．
    const float kBodyLiftingHeightMax;

    const float kMovableCoxaAngleMin;   //!< 脚の可動範囲の最小値[rad]
    const float kMovableCoxaAngleMax;   //!< 脚の可動範囲の最大値[rad]

    const float kMinLegR;   //!< 脚の付け根から脚先までの最小距離[mm]
    const float kMaxLegR;   //!< 脚の付け根から脚先までの最大距離[mm]

    const float kFreeLegHeight;  //!< 重心から見た遊脚高さ[mm]．
    const float kStableMargin;   //!< 静的安全余裕．

    const float kMinLegDistance;  //! 脚先どうしの最小距離[mm]．

    //! 脚の付け根から脚先までの最大距離[mm]．
    //! 脚の付け根と重心のz方向の距離の差をインデックスにする．
    std::array<float, kMaxLegRSize> kMaxLegRArray;

    //! coxa jointの最小位置まで回した時の脚先座標．脚座標系のxyからみた座標．
    std::array<Vector2, HexapodConst::kLegNum> kMinLegPosXY;

    //! coxa jointの最大位置まで回した時の脚先座標．脚座標系のxyからみた座標．
    std::array<Vector2, HexapodConst::kLegNum> kMaxLegPosXY;


    //!< 脚の付け根の座標( leg base position )．ロボット座標系．
    const std::array<Vector3, HexapodConst::kLegNum> leg_base_pos_robot_coordinate_;

    //!< 遊脚する位置．脚座標系．
    const std::array<Vector3, HexapodConst::kLegNum> free_leg_pos_leg_coordinate_;
};

}  // namespace designlab


#endif  // DESIGNLAB_PHANTOMX_MK2_H_
