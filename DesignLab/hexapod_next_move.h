
//! @file      hexapod_next_move.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory, Saitama University All right reserved.

#ifndef DESIGNLAB_HEXAPOD_NEXT_MOVE_H_
#define DESIGNLAB_HEXAPOD_NEXT_MOVE_H_


namespace designlab::enums
{

//! @enum HexapodMove
//! @brief ロボットが次にどの動作をするのかを表す列挙体．
//! @details 先行研究では int型の変数 debug を用いていたが，可読性が低いので列挙体を用いる．
enum class HexapodMove : int
{
    kNone,                  //!< 何も動作をしない．
    kLegUpDown,             //!< 脚の上下移動.
    kLegHierarchyChange,    //!< 脚の平行移動．脚の階層を変更する．
    kComMove,               //!< 重心の平行移動．Center Of Massで重心のこと．
    kComUpDown,             //!< 重心の上下移動．
    kBodyPitchRot,          //!< ピッチ軸周りの回転．
    kBodyYawRot,            //!< ヨー軸周りの回転．
};

}  // namespace designlab::enums


#endif  // DESIGNLAB_HEXAPOD_NEXT_MOVE_H_
