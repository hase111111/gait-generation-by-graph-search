
//! @file      graphic_const.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_GRAPHIC_CONST_H_
#define DESIGNLAB_GRAPHIC_CONST_H_

#include <string>

#include "cassert_define.h"


namespace designlab
{

//! @class GraphicConst
//! @brief 画像表示処理用の定数クラス．
class GraphicConst final
{
public:
    //! コンストラクタを削除して，実体を生成できないようにする．
    GraphicConst() = delete;
    GraphicConst(const GraphicConst& other) = delete;
    GraphicConst(GraphicConst&& other) = delete;
    GraphicConst& operator=(const GraphicConst& other) = delete;

    static const char kWindowName[];  //!< ウィンドウの名前．
    static const int kColorBit;  //!< 色を表現するビット数．通常32で良いが軽くするなら16にする．

    static const int kBackColorRed;     //!< ウィンドウ背景色． 赤色成分．0～255の範囲で指定．
    static const int kBackColorGreen;   //!< ウィンドウ背景色． 緑色成分．0～255の範囲で指定．
    static const int kBackColorBlue;    //!< ウィンドウ背景色． 青色成分．0～255の範囲で指定．

    static const float kCameraToTargetMax;  //!< カメラと注視目標の最大距離．
    static const float kCameraToTargetMin;  //!< カメラと注視目標の最小距離．
};

}  // namespace designlab


#endif  // DESIGNLAB_GRAPHIC_CONST_H_
