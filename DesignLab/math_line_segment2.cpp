
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#include "math_line_segment2.h"

#include "cassert_define.h"
#include "math_util.h"


namespace designlab
{

Vector2 LineSegment2::GetIntersection(const LineSegment2& other) const
{
    if (IsParallel(other))
    {
        return Vector2{ 0, 0 };  // 平行ならば交点は存在しない．
    }

    const Vector2 v1 = end - start;
    const Vector2 v2 = other.end - other.start;
    const Vector2 v3 = other.start - start;
    const float d = v1.Cross(v2);

    const float t1 = v3.Cross(v2) / d;
    const float t2 = v3.Cross(v1) / d;

    // t1, t2が0~1の範囲内にあるかならば，交点は線分上に存在する．

    if (t1 < 0.0f - MathConst<float>::kAllowableError ||
        t1 > 1.0f + MathConst<float>::kAllowableError ||
        t2 < 0.0f - MathConst<float>::kAllowableError ||
        t2 > 1.0f + MathConst<float>::kAllowableError)
    {
        return Vector2{ 0, 0 };  // 交点は線分の外．
    }

    return start + v1 * t1;
}

bool LineSegment2::HasIntersection(const LineSegment2& other) const
{
    if (IsParallel(other))
    {
        return false;  // 平行ならば交点は存在しない．
    }

    const Vector2 v1 = end - start;
    const Vector2 v2 = other.end - other.start;
    const Vector2 v3 = other.start - start;
    const float d = v1.Cross(v2);

    const float t1 = v3.Cross(v2) / d;
    const float t2 = v3.Cross(v1) / d;

    // t1, t2が0~1の範囲内にあるかならば，交点は線分上に存在する．

    if (t1 < 0.0f - MathConst<float>::kAllowableError ||
        t1 > 1.0f + MathConst<float>::kAllowableError ||
        t2 < 0.0f - MathConst<float>::kAllowableError ||
        t2 > 1.0f + MathConst<float>::kAllowableError)
    {
        return false;  // 交点は線分の外．
    }

    return true;
}

bool LineSegment2::CheckAndGetIntersection(const LineSegment2& other, Vector2* intersection) const
{
    // 交点を代入するポインタは nullptr であってはならない．
    assert(intersection != nullptr);

    // 端点一致の場合，その点を返す
    if ((start == other.start && end != other.end) ||
        (start == other.end && end != other.start))
    {
        (*intersection) = start;
        return true;
    }
    else if ((end == other.start && start != other.end) ||
             (end == other.end && start != other.start))
    {
        (*intersection) = end;
        return true;
    }

    if (IsParallel(other))
    {
        return false;  // 平行ならば交点は存在しない．
    }

    const Vector2 v1 = end - start;
    const Vector2 v2 = other.end - other.start;
    const Vector2 v3 = other.start - start;
    const float d = v1.Cross(v2);

    const float t1 = v3.Cross(v2) / d;
    const float t2 = v3.Cross(v1) / d;

    // t1, t2が0~1の範囲内にあるかならば，交点は線分上に存在する．

    if (t1 < 0.0f - MathConst<float>::kAllowableError ||
        t1 > 1.0f + MathConst<float>::kAllowableError ||
        t2 < 0.0f - MathConst<float>::kAllowableError ||
        t2 > 1.0f + MathConst<float>::kAllowableError)
    {
        return false;  // 交点は線分の外．
    }

    *intersection = start + v1 * t1;
    return true;
}

}  // namespace designlab
