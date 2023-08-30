#pragma once


//! @class WorldGridRenderer
//! @date 2023/08/23
//! @author 長谷川
//! @brief ワールドの格子線を描画するクラス
class WorldGridRenderer final
{
public:

	WorldGridRenderer();

	//! @brief ワールドの格子線を描画する
	void draw() const;


private:

	const unsigned int MAIN_GRID_X_COLOR;		//!< 格子線の色

	const unsigned int MAIN_GRID_Y_COLOR;		//!< 格子線の色

	const unsigned int SUB_GRID_X_COLOR;		//!< 細い格子線の色

	const unsigned int SUB_GRID_Y_COLOR;		//!< 細い格子線の色

	const float MAIN_GRID_INTERVAL;				//!< 格子線の間隔

	const int SUB_GRID_DEVIDE_NUM;				//!< メインの格子線を何分割してサブの格子線をいれるか

	const float GRID_LINE_Z;					//!< 格子線のZ座標
};


//! @file world_grid_renderer.h
//! @date 2023/08/23
//! @author 長谷川
//! @brief ワールドの格子線を描画するクラス
//! @n 行数 : @lineinfo