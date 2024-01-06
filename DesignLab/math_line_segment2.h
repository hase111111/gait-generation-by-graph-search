
//! @file      math_line_segment2.h
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#ifndef DESIGNLAB_MATH_LINE_SEGMENT2_H_
#define DESIGNLAB_MATH_LINE_SEGMENT2_H_

#include "math_vector2.h"
#include "math_util.h"


namespace designlab
{

//! @struct LineSegment2
//! @brief 2次元の線分を表す構造体．
struct LineSegment2 final
{
    LineSegment2() = default;
    constexpr LineSegment2(const Vector2& start, const Vector2& end) :
        start(start), end(end) {}
    constexpr LineSegment2(float start_x, float start_y, float end_x, float end_y) :
        start(start_x, start_y), end(end_x, end_y) {}
    constexpr LineSegment2(const LineSegment2& other) = default;
    constexpr LineSegment2(LineSegment2&& other) noexcept = default;
    constexpr LineSegment2& operator=(const LineSegment2& other) = default;

    constexpr bool operator==(const LineSegment2& other) const
    {
        return start == other.start && end == other.end;
    }

    constexpr bool operator!=(const LineSegment2& other) const { return !(*this == other); }


    //! @brief 線分の長さを求める関数．
    //! @return 線分の長さ．
    inline float GetLength() const
    {
        return (end - start).GetLength();
    }

    //! @brief 引数の線分と自身が平行かどうか調べる関数．
    //! 全て constexpr 関数で処理できるため非常に高速．
    //! @param[in] other 他の線分．
    //! @retval true 平行である．
    //! @retval false 平行でない時．
    constexpr bool IsParallel(const LineSegment2& other) const
    {
        // 外積が0なら平行．
        return math_util::IsEqual((end - start).Cross(other.end - other.start), 0.0f);
    }

    //! @brief 他の線分との交点を求める．
    //! @param[in] other 他の線分．
    //! @return 交点の座標．交点がない，または平行な場合は(0, 0)を返す．
    //! @n 端点一致，かつ平行の場合を考慮していないので注意．
    //! @note 参考
    //! @li http://marupeke296.com/COL_main.html (アクセス日 2023/12/24)
    Vector2 GetIntersection(const LineSegment2& other) const;

    //! @brief 他の線分と交点が存在しているかどうか調べる関数．
    //! @param[in] other 他の線分．
    //! @retval true 交点がある．
    //! @retval false 交点がない．あるいは平行である時．
    bool HasIntersection(const LineSegment2& other) const;

    //! @brief 他の線分と交点が存在しているかどうか調べ，交点を返す関数．
    //! @param[in] other 他の線分．
    //! @param[out] intersection 交点．
    //! @retval true 交点が1つだけある．
    //! @retval false 交点がない，または線分が重なっていて交点が無限にある．
    bool CheckAndGetIntersection(const LineSegment2& other, Vector2* intersection) const;


    Vector2 start;  //!< 線分の始点．
    Vector2 end;    //!< 線分の終点．
};

}  // namespace designlab


#endif  // DESIGNLAB_MATH_LINE_SEGMENT2_H_
