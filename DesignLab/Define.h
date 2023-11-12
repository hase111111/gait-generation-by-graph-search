//! @file define.h
//! @brief プロジェクト全体で使用するDefineをまとめたもの

#ifndef DESIGNLAB_DEFINE_H_
#define DESIGNLAB_DEFINE_H_

//#define DESIGNLAB_DONOT_USE_DXLIB	//!< DXライブラリを使用しない場合はコメントアウトを外す

//#define DESIGNLAB_USE_ROS			//!< ROSを使用する場合はコメントアウトを外す


// DESIGNLAB_USE_ROSがdefineされているかつ，DESIGNLAB_DONOT_USE_DXLIBがdefineされていない場合，DESIGNLAB_DONOT_USE_DXLIBをdefineする
#ifdef DESIGNLAB_USE_ROS
#ifndef DESIGNLAB_DONOT_USE_DXLIB
#define DESIGNLAB_DONOT_USE_DXLIB
#endif
#endif

#endif // DESIGNLAB_DEFINE_H_