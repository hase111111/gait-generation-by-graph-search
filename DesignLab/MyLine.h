#pragma once
#include "MyVector2.h"

namespace my_vec
{
	//! @struct SLine2
	//! @brief 2次元の線分を表す構造体
	struct SLine2 final
	{
		SLine2() = default;
		constexpr SLine2(const SVector2& start, const SVector2& end) : start(start), end(end) {}

		SVector2 start;
		SVector2 end;

		//! @brief 他の線分との交点を求める．
		//! @param[in] other 他の線分
		//! @return 交点．交点がないor平行な場合は(0, 0)を返す．
		//! @note 参考：http://marupeke296.com/COL_main.html
		SVector2 getIntersection(const SLine2& other) const;

		//! @brief 交点が存在しているかどうか調べる関数
		//! @param[in] other 他の線分
		//!	@return 交点があるならtrue．ないor平行ならfalse
		bool hasIntersection(const SLine2& other) const;

		//! @brief 線分の長さを求める関数
		//! @return 線分の長さ
		inline float getLength() const
		{
			return (end - start).length();
		}

		//! @brief 線分が平行かどうか調べる関数
		//! @param[in] other 他の線分
		constexpr bool isParallel(const SLine2& other) const
		{
			//外積を用いて計算する
			return my_math::isEqual((end - start).cross(other.end - other.start), 0.0f);
		}

		bool operator==(const SLine2& other) const
		{
			return start == other.start && end == other.end;
		}
	};
}