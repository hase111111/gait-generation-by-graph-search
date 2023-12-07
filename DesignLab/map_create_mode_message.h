//! @file map_create_mode_message.h
//! @brief マップ生成時のモードとオプションを指定する構造体．

#ifndef DESIGNLAB_CREATE_MODE_MESSAGE_H_
#define DESIGNLAB_CREATE_MODE_MESSAGE_H_

#include <vector>

#include "cassert_define.h"


//! @enum MapCreateMode
//! @brief getMap関数のマップ生成のモードを指定する列挙体．
enum class MapCreateMode : int
{
	kFlat = 0,			//!< 普通の平らな面を生成する．
	kVerticalStripe,	//!< 縦じまの面を生成する．
	kHorizontalStripe,	//!< 横じまの面を生成する．
	kDiagonalStripe,	//!< 斜めじまの面を生成する．
	kMesh,				//!< 格子状の面を生成する．網目状の地形ともいっていい．
	kLatticePoint,		//!< 格子点の面を生成する．網目状の逆．
};

//! @enum MapCreateOption
//! @brief getMap関数のマップ生成のオプションを指定する列挙体．
//! @n bit演算を利用して複数指定できる．例えば穴あきかつ，階段状にしたいならば，MapCreateOption::kPerforated | MapCreateOption::kStep と指定する．
//! @n bit演算ができるようにunsigned int型にしている．
enum class MapCreateOption : unsigned int
{
	// 1 <<  x は 2^x を表す．

	kNone = 0,				//!< 特にオプションを指定しない．
	kPerforated = 1 << 0,	//!< 穴の空いたマップに変化させる．
	kStep = 1 << 1,			//!< 階段状の地形に変化させる．
	kSlope = 1 << 2,		//!< スロープ状の地形に変化させる．
	kTilt = 1 << 3,			//!< 縦軸を中心軸として回転させた地形に変化させる．
	kRough = 1 << 4,		//!< 凸凹の地形に変化させる．
};


//! @struct MapCreateModeMessage
//! @brief マップ生成時のモードとオプションを指定する構造体．
struct MapCreateModeMessage final
{
public:
	constexpr MapCreateModeMessage() :
		mode(MapCreateMode::kFlat),
		option(static_cast<unsigned int>(MapCreateOption::kNone))
	{
	}

	//! @brief マップ生成のモードを指定する．
	//! @param [in] mode マップ生成のモードを指定する列挙体．
	constexpr void SetMode(const MapCreateMode create_mode)
	{
		mode = create_mode;
	}

	//! @brief マップ生成のオプションを指定する．
	//! @n この関数を呼んだあと，その他のSet～関数を呼ぶと，段差の高さや，傾斜角を指定できる．
	//! @param [in] mode マップ生成のオプションを指定する列挙体をvectorで指定する．
	//! @n emptyであってはならない．
	void SetOption(const std::vector<MapCreateOption> create_options)
	{
		assert(!create_options.empty());

		for (const auto& i : create_options)
		{
			option |= static_cast<unsigned int>(i);
		}
	}

	//! @brief マップの大きさを指定する．
	//! @param [in] max_x マップのX座標の最大値．単位は[mm]．
	//! @param [in] min_x マップのX座標の最小値．単位は[mm]．
	//! @param [in] max_y マップのY座標の最大値．単位は[mm]．
	//! @param [in] min_y マップのY座標の最小値．単位は[mm]．
	//! @param [in] start_rough_x 不整地が始まるX座標．単位は[mm]．
	//! @param [in] base_z マップのZ座標．単位は[mm]．
	constexpr void SetMapSize(const float max_x, const float min_x, const float max_y,
		const float min_y, const float start_rough_x, const float map_base_z)
	{
		assert(min_x < max_x);
		assert(min_y < max_y);
		assert(min_x < start_rough_x);
		assert(start_rough_x < max_x);

		map_max_x = max_x;
		map_min_x = min_x;
		map_max_y = max_y;
		map_min_y = min_y;
		map_start_rough_x = start_rough_x;
		base_z = map_base_z;
	}

	constexpr void SetStripeInterval(const int interval)
	{
		assert(0 < interval);

		stripe_interval = interval;
	}

	constexpr void SetHoleRate(const int rate)
	{
		assert(0 <= rate);
		assert(rate <= 100);

		hole_rate = rate;
	}

	constexpr void SetStepValue(const float height, const float length)
	{
		assert(0 < length);

		step_height = height;
		step_length = length;
	}

	constexpr void SetTiltAngle(const float angle)
	{
		tilt_angle = angle;
	}

	constexpr void SetSlopeAngle(const float angle)
	{
		slope_angle = angle;
	}

	constexpr void SetRouthHeight(const float height_min, const float height_max)
	{
		assert(height_min < height_max);

		routh_min_height = height_min;
		routh_max_height = height_max;
	}


	MapCreateMode mode;		//!< マップ生成のモードを指定する列挙体．
	unsigned int option;	//!< マップ生成のオプションを指定するbit．

	float base_z{ 0.0f };		//!< マップの基準となるZ座標．
	float map_max_x{ 2600.f };	//!< マップのX座標の最大値．
	float map_min_x{ -400.f };	//!< マップのX座標の最小値．
	float map_max_y{ 2000.f };	//!< マップのY座標の最大値．
	float map_min_y{ -2000.f };	//!< マップのY座標の最小値．
	float map_start_rough_x{ 400.f };	//!< 不整地が始まるX座標．

	int stripe_interval{ 5 };	//!< 各種模様や穴を作成する際，これで指定したマス分の1辺を持つ正方形状にあなをあける．

	int hole_rate{ 0 };			//!< 不整地上の足場を除外する割合。ホール率[%]
	float step_height{ 0.f };	//!< 段差高さ[mm]．負の値にすると下りの階段になる．
	float step_length{ 0.f };	//!< 階段の奥行[mm]．
	float slope_angle{ 0.f };	//!< 斜面の傾斜角[deg]．
	float tilt_angle{ 0.f };	//!< 地形を傾ける角度[deg]．
	float routh_max_height{ 0.f };		//!< デコボコな地形の最大高さ[mm]
	float routh_min_height{ 0.f };		//!< デコボコな地形の最小高さ[mm]
};


#endif // DESIGNLAB_CREATE_MODE_MESSAGE_H_