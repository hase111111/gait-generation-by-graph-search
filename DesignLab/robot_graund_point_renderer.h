#pragma once


//! @class RobotGraundPointRenderer
//! @date 2033/08/29
//! @author 長谷川
//! @brief ロボットの脚接地点の座標を描画するクラス
class RobotGraundPointRenderer final
{
public:

	RobotGraundPointRenderer();


	//! ロボットの脚接地点の描画を行う．
	//! @param [in] draw_simu_num 描画を行うシミュレーションの番号( 0, 1, 2, ...)
	//! @param [in] draw_all_simulation 上のパラメータを無視して，すべてのシミュレーションについて描画する
	void draw(const int draw_simu_num, const bool draw_all_simulation = false) const;

private:

	const unsigned int GRAUND_POINT_COLOR;			//!< 脚接地点の色

	const unsigned int GRAUND_POINT_COLOR_BLACK;	//!< すべてのシミュレーションについて描画する場合，現在のシミュレーション以外の色

};



//! @file robot_graund_point_renderer.h
//! @date 2033/08/29
//! @author 長谷川
//! @brief ロボットの脚接地点の座標を描画するクラス
//! @n 行数 : @lineinfo