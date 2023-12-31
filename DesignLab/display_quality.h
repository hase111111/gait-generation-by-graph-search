
//! @file      display_quality.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_DISPLAY_QUALITY_H_
#define DESIGNLAB_DISPLAY_QUALITY_H_


namespace designlab::enums
{

//! enum DisplayQuality
//! @brief 描画の品質設定を示す列挙体．
enum class DisplayQuality : int
{
    kLow = 0,  //!< 低品質．
    kMedium,   //!< 中品質．
    kHigh,     //!< 高品質．
};

}  // namespace designlab::enums


#endif  // DESIGNLAB_DISPLAY_QUALITY_H_
