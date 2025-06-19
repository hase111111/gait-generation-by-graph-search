
//! @file display_quality.h

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php

#ifndef DESIGNLAB_DISPLAY_QUALITY_H_
#define DESIGNLAB_DISPLAY_QUALITY_H_

namespace designlab {

//! enum DisplayQuality
//! @brief 描画の品質設定を示す列挙体.
enum class DisplayQuality : int {
  kLow = 0,  //!< 低品質.
  kMedium,   //!< 中品質.
  kHigh,     //!< 高品質.
};

}  // namespace designlab

#endif  // DESIGNLAB_DISPLAY_QUALITY_H_
