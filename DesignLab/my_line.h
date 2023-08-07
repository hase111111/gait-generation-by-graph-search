#pragma once

#include "my_vector2.h"


namespace my_vec
{
	//! @struct SLine2
	//! @date 2023/08/06
	//! @auther 長谷川
	//! @brief 2次元の線分を表す構造体
	struct SLine2 final
	{
		SLine2() = default;
		constexpr SLine2(const SVector2& start, const SVector2& end) : start(start), end(end) {}


		bool operator==(const SLine2& other) const
		{
			return start == other.start && end == other.end;
		}


		//! @brief 線分の長さを求める関数
		//! @return float 線分の長さ
		inline float getLength() const
		{
			return (end - start).length();
		}

		//! @brief 線分が平行かどうか調べる関数
		//! @param[in] other 他の線分
		//! @return bool 平行ならtrue，そうでなければfalse
		constexpr bool isParallel(const SLine2& other) const
		{
			//外積を用いて計算する
			return my_math::isEqual((end - start).cross(other.end - other.start), 0.0f);
		}

		//! @brief 他の線分との交点を求める．
		//! @param [in] other 他の線分
		//! @return my_vec::SVector2 交点．交点がないor平行な場合は(0, 0)を返す．
		//! @note 参考：http://marupeke296.com/COL_main.html
		SVector2 getIntersection(const SLine2& other) const;

		//! @brief 交点が存在しているかどうか調べる関数
		//! @param [in] other 他の線分
		//!	@return 交点があるならtrue．ないor平行ならfalse
		bool hasIntersection(const SLine2& other) const;


		SVector2 start;	//!< 線分の始点
		SVector2 end;	//!< 線分の終点
	};

} //namespace my_vec


//! @file my_line.h
//! @date 2023/08/06
//! @auther 長谷川
//! @brief 2次元の線分を表す構造体
//! @n 行数 : @lineinfo
