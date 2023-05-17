#pragma once

//MapCreator.h内で大量に宣言ざれていた #defineを移したもの
//MapCreator.hは様々なファイルでインクルードされているせいで影響が凄い．
//cppでのみ使用するようにここに非難させる．

//#define MAKE_SLOPE_MAP	//斜面を不整地とするときdefine

//#define ISOSELES_TRIANGLE	//2等辺三角形の不整地にするときはdefine

#define MAKE_STEP_MAP	//階段を吸整地とするときはdefine

#define HOLE_RANDOM

//#define RANDOM_ADD_Z 3		// ±MAGNIFICATION(0~n)←乱数×RANDOM_ADD_Z分だけ高さ方向に加える。高さ[mm] 高さ変えないときは、CO

#define READMAPDATA_FROM_CSV //csvファイル（excelファイル）からマップデータを読み取るときはdefine