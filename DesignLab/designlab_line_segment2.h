//! @file designlab_line_segment2.h
//! @brief 2次元の線分を表す構造体

#ifndef DESIGNLAB_LINE_SEGMENT2_H_
#define DESIGNLAB_LINE_SEGMENT2_H_


#include "designlab_vector2.h"
#include "designlab_math_util.h"


namespace designlab
{
	//! @struct LineSegment2
	//! @brief 2次元の線分を表す構造体
	struct LineSegment2 final
	{
		LineSegment2() = default;
		constexpr LineSegment2(const Vector2& start, const Vector2& end) : start(start), end(end) {}
		constexpr LineSegment2(float startx, float starty, float endx, float endy) : start(startx, starty), end(endx, endy) {}
		constexpr LineSegment2(const LineSegment2& other) = default;
		constexpr LineSegment2(LineSegment2&& other) noexcept = default;
		constexpr LineSegment2& operator=(const LineSegment2& other) = default;


		bool operator==(const LineSegment2& other) const
		{
			return start == other.start && end == other.end;
		}


		//! @brief 線分の長さを求める関数
		//! @return float 線分の長さ
		inline float GetLength() const
		{
			return (end - start).Length();
		}

		//! @brief 線分が平行かどうか調べる関数
		//! @param[in] other 他の線分
		//! @return bool 平行ならtrue，そうでなければfalse
		constexpr bool IsParallel(const LineSegment2& other) const
		{
			//外積が0なら平行
			return ::designlab::math_util::IsEqual(
				(end - start).Cross(other.end - other.start), 
				0.0f
			);
		}

		//! @brief 他の線分との交点を求める．
		//! @param [in] other 他の線分
		//! @return designlab::Vector2 交点．交点がないor平行な場合は(0, 0)を返す．
		//! @note 参考：http://marupeke296.com/COL_main.html
		//! @n 端点一致，かつ平行の場合を考慮していないので注意
		Vector2 GetIntersection(const LineSegment2& other) const;

		//! @brief 他の線分と交点が存在しているかどうか調べる関数
		//! @param [in] other 他の線分
		//!	@return bool 交点があるならtrue．ないor平行ならfalse
		bool HasIntersection(const LineSegment2& other) const;

		//! @brief 他の線分と交点が存在しているかどうか調べ，交点を返す関数
		//! @param [in] other 他の線分
		//! @param [out] intersection 交点
		//! @return bool 交点があるならtrue．ないor平行ならfalse
		bool CheckAndGetIntersection(const LineSegment2& other, Vector2* intersection) const;


		Vector2 start;	//!< 線分の始点
		Vector2 end;	//!< 線分の終点
	};

} //namespace designlab


#endif // !DESIGNLAB_LINE_SEGMENT2_H_