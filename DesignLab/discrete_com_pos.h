
//! @file      discrete_com_pos.h
//! @author    Hasegawa
//! @copyright 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_DISCRETE_COM_POS_H_
#define DESIGNLAB_DISCRETE_COM_POS_H_


namespace designlab::enums
{

//! @enum 離散化された重心位置を表す列挙型．
//! @brief 重心がどこにあるかを表す．
enum class DiscreteComPos
{
    kFront = 1,     //!< 重心が前方にある．
    kFrontLeft,     //!< 重心が左前方にある．
    kBackLeft,      //!< 重心が左後方にある．
    kBack,          //!< 重心が後方にある．
    kBackRight,     //!< 重心が右後方にある．
    kFrontRight,    //!< 重心が右前方にある．
    kCenterFront,   //!< 重心が中央前方にある．三角形．
    kCenterBack,    //!< 重心が中央後方にある．逆三角径．
};

}  // namespace designlab::enums


#endif  // DESIGNLAB_DISCRETE_COM_POS_H_
