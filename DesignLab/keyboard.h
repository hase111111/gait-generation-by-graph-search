
//! @file      keyboard.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//!  Saitama University All right reserved.

#ifndef DESIGNLAB_KEYBOARD_H_
#define DESIGNLAB_KEYBOARD_H_

#include <array>


namespace designlab
{

//! @class Keyboard
//! @brief Dxlibのキーボード入力を取得するクラス．
class Keyboard final
{
public:
    Keyboard();


    //! @brief キー入力を更新する．
    //! これを毎フレーム実行しないと，キー入力を取得できない．
    void Update();

    //! @brief keyCodeのキーが押されているフレーム数を取得する．
    //! @param[in] keyCode どのキーを調べたいか，
    //! 例えばZキーだったら，KEY_INPUT_Z となる．
    //! @return 押されているフレーム数．異常な key_code が渡された場合は-1を返す．
    int GetPressingCount(const int key_code) const;

    //! @brief keyCodeのキーが離されているフレーム数を取得する．
    //! @param[in] keyCode どのキーを調べたいか，
    //! 例えばZキーだったら，KEY_INPUT_Z となる．
    //! @return 離されているフレーム数．異常な key_code が渡された場合は-1を返す．
    int GetReleasingCount(const int key_code) const;

private:
    static const int kKeyNum = 256;  //!< Dxlibにおけるキー総数．

    //! @brief keyCodeが有効なキー番号か返す．
    //! @param[in] keyCode どのキーを調べたいか，
    //! 例えばZキーだったら，KEY_INPUT_Z となる．
    //! @retval true 有効なキー番号である．
    //! @retval false 無効なキー番号である．
    bool IsAvailableCode(const int key_code) const;

    std::array<int, kKeyNum> key_pressing_counter_;   //!< 押されカウンタ．
    std::array<int, kKeyNum> key_releasing_counter_;  //!< 離されカウンタ．
};

}  // namespace designlab


#endif  // DESIGNLAB_KEYBOARD_H_
