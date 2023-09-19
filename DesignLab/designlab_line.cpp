#include "designlab_line.h"

#include "designlab_math.h"


designlab::SVector2 designlab::SLine2::getIntersection(const SLine2& other) const
{
	if (isParallel(other))
	{
		return SVector2{ 0, 0 };	//平行ならば交点は存在しない．
	}

	const SVector2 v1 = end - start;
	const SVector2 v2 = other.end - other.start;
	const SVector2 v3 = other.start - start;
	const float d = v1.cross(v2);

	const float t1 = v3.cross(v2) / d;
	const float t2 = v3.cross(v1) / d;

	if (t1 < 0.0f - dl_math::ALLOWABLE_ERROR || t1 > 1.0f + dl_math::ALLOWABLE_ERROR || t2 < 0.0f - dl_math::ALLOWABLE_ERROR || t2 > 1.0f + dl_math::ALLOWABLE_ERROR)
	{
		return SVector2{ 0, 0 };	//交点は線分の外
	}

	return start + v1 * t1;
}


bool designlab::SLine2::hasIntersection(const SLine2& other) const
{
	if (isParallel(other))
	{
		return false;	//平行ならば交点は存在しない．
	}

	const auto v1 = end - start;
	const auto v2 = other.end - other.start;
	const auto v3 = other.start - start;
	const auto d = v1.cross(v2);

	const auto t1 = v3.cross(v2) / d;
	const auto t2 = v3.cross(v1) / d;

	if (t1 < 0.0f - dl_math::ALLOWABLE_ERROR || t1 > 1.0f + dl_math::ALLOWABLE_ERROR || t2 < 0.0f - dl_math::ALLOWABLE_ERROR || t2 > 1.0f + dl_math::ALLOWABLE_ERROR)
	{
		return false;	//交点は線分の外
	}

	return true;
}
