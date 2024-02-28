
//! @file      discrete_leg_pos.h
//! @author    Hasegawa
//! @copyright 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_DISCRETE_LEG_POS_H_
#define DESIGNLAB_DISCRETE_LEG_POS_H_


namespace designlab::enums
{

//! @enum DiscreteLegPos
//! @brief 離散化された脚位置を表す列挙体．
//! @n 先行研究では 1～7の int型の数値で表現されているが，
//! 可読性を上げるために列挙体にした．
//! @n 離散化された脚位置は 3bit (0 ～ 7)の範囲で表現されるため，これを拡張する場合，
//! leg state を表す変数の型を変更する必要がある．
enum class DiscreteLegPos : int
{
    kLowerBack = 1,     //!< 現在の位置より後方かつ下方にある．
    kBack,              //!< 現在の位置より後方にある．
    kUpperBack,         //!< 現在の位置より後方かつ上方にある．
    kCenter,            //!< 現在の位置にある．
    kLowerFront,        //!< 現在の位置より前方かつ下方にある．
    kFront,             //!< 現在の位置より前方にある．
    kUpperFront,        //!< 現在の位置より前方かつ上方にある．
};

}  // namespace designlab::enums


#endif  // DESIGNLAB_DISCRETE_LEG_POS_H_
