#include "MyPolygon.h"

bool my_vec::SPolygon2::isConvex() const
{
	const int n = getVertexNum();

	//早期リターン.頂点数が3未満の場合は多角形ではない
	if (n < 3)
	{
		return false;
	}

	//右回りか左回りかを調べる
	const auto _v1 = vertex[1] - vertex[0];
	const auto _v2 = vertex[2] - vertex[1];

	bool _left_turn = _v1.cross(_v2) > 0.0f;

	for (int i = 1; i < n; ++i)
	{
		const auto& v1 = vertex[(i + 1) % n] - vertex[i];
		const auto& v2 = vertex[(i + 2) % n] - vertex[(i + 1) % n];

		if (_left_turn && v1.cross(v2) < 0.0f)
		{
			return false;
		}
		else if (!_left_turn && v1.cross(v2) > 0.0f)
		{
			return false;
		}
	}
	return true;
}

bool my_vec::SPolygon2::isInside(const SVector2& _p) const
{
	const int _num = getVertexNum();

	//早期リターン.頂点数が3未満の場合は多角形ではない
	if (_num < 3)
	{
		return false;
	}

	int _cnt = 0;

	//頂点が右回りか左回りかを調べる
	bool _left_turn = (vertex[1] - vertex[0]).cross(vertex[2] - vertex[1]) > 0.0f;

	if (!_left_turn)
	{
		for (int i = 0; i < _num; ++i)
		{
			const auto& v1 = vertex[i] - _p;
			const auto& v2 = vertex[(i + 1) % _num] - _p;

			if (v1.cross(v2) == 0.0f && v1.dot(v2) <= 0.0f)
			{
				return true;	//点が辺上にある
			}

			if (v1.y < v2.y)
			{
				if (v1.y < 0.0f && 0.0f <= v2.y && v1.cross(v2) > 0.0f)
				{
					--_cnt;
				}
			}
			else
			{
				if (v2.y < 0.0f && 0.0f <= v1.y && v1.cross(v2) < 0.0f)
				{
					++_cnt;
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < _num; ++i)
		{
			const auto& v1 = vertex[i] - _p;
			const auto& v2 = vertex[(i + 1) % _num] - _p;

			if (v1.cross(v2) == 0.0f && v1.dot(v2) <= 0.0f)
			{
				return true;	//点が辺上にある
			}

			if (v1.y > v2.y)
			{
				if (v2.y < 0.0f && 0.0f <= v1.y && v1.cross(v2) < 0.0f)
				{
					--_cnt;
				}
			}
			else
			{
				if (v1.y < 0.0f && 0.0f <= v2.y && v1.cross(v2) > 0.0f)
				{
					++_cnt;
				}
			}
		}
	}

	return (_cnt % 2 == 1);
}
