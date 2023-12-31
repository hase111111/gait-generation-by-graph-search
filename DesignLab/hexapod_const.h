
//! @file      hexapod_const.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_HEXAPOD_CONST_H_
#define DESIGNLAB_HEXAPOD_CONST_H_


namespace designlab
{

//! @class HexapodConst
//! @brief Hexapodの定数をまとめたクラス．
//! @details コンストラクタを削除したので，実体は生成できない．
//! @code
//! int a = HexapodConst::kLegNum;  // OK
//!
//! HexapodConst h;  // コンパイルエラー
//! @endcode
//! @n 先行研究のマクロをまとめたもの，徐々にここから値を消していく．
class HexapodConst final
{
public:
    //! コンストラクタとコピーコンストラクタを削除し，実体を生成できないようにする．
    HexapodConst() = delete;
    HexapodConst(const HexapodConst& other) = delete;               //!< 削除する．
    HexapodConst& operator=(const HexapodConst& other) = delete;    //!< 削除する．
    HexapodConst(HexapodConst&& other) = delete;                    //!< 削除する．

    //! Hexapodの脚の本数を表す．これを変更しても脚の本数が変更できるわけではない．
    //! マジックナンバーをなくすことが目的．
    constexpr static int kLegNum = 6;
};

}  // namespace designlab


#endif  // DESIGNLAB_HEXAPOD_CONST_H_
