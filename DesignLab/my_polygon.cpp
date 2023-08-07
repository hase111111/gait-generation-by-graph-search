#include "my_polygon.h"


bool my_vec::SPolygon2::isConvex() const
{
	const int num = getVertexNum();

	//早期リターン.頂点数が3未満の場合は多角形ではない
	if (num < 3)
	{
		return false;
	}

	//右回りか左回りかを調べる
	const auto v1 = vertex[1] - vertex[0];
	const auto v2 = vertex[2] - vertex[1];

	bool is_left_turn = v1.cross(v2) > 0.0f;

	for (int i = 1; i < num; ++i)
	{
		const auto& v1 = vertex[(i + 1) % num] - vertex[i];
		const auto& v2 = vertex[(i + 2) % num] - vertex[(i + 1) % num];

		if (is_left_turn && v1.cross(v2) < 0.0f)
		{
			return false;
		}
		else if (!is_left_turn && v1.cross(v2) > 0.0f)
		{
			return false;
		}
	}

	return true;

	//ちなみに処理はすべてgithun copilotが書いてくれたので，処理の中身はよくわかっていない...
}


bool my_vec::SPolygon2::isInside(const SVector2& point) const
{
	const int num = getVertexNum();

	//早期リターン.頂点数が3未満の場合は多角形ではない
	if (num < 3)
	{
		return false;
	}

	int cnt = 0;

	//頂点が右回りか左回りかを調べる
	bool is_left_turn = (vertex[1] - vertex[0]).cross(vertex[2] - vertex[1]) > 0.0f;

	if (!is_left_turn)
	{
		for (int i = 0; i < num; ++i)
		{
			const auto& v1 = vertex[i] - point;
			const auto& v2 = vertex[(i + 1) % num] - point;

			if (v1.cross(v2) == 0.0f && v1.dot(v2) <= 0.0f)
			{
				return true;	//点が辺上にある
			}

			if (v1.y < v2.y)
			{
				if (v1.y < 0.0f && 0.0f <= v2.y && v1.cross(v2) > 0.0f)
				{
					--cnt;
				}
			}
			else
			{
				if (v2.y < 0.0f && 0.0f <= v1.y && v1.cross(v2) < 0.0f)
				{
					++cnt;
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < num; ++i)
		{
			const auto& v1 = vertex[i] - point;
			const auto& v2 = vertex[(i + 1) % num] - point;

			if (v1.cross(v2) == 0.0f && v1.dot(v2) <= 0.0f)
			{
				return true;	//点が辺上にある
			}

			if (v1.y > v2.y)
			{
				if (v2.y < 0.0f && 0.0f <= v1.y && v1.cross(v2) < 0.0f)
				{
					--cnt;
				}
			}
			else
			{
				if (v1.y < 0.0f && 0.0f <= v2.y && v1.cross(v2) > 0.0f)
				{
					++cnt;
				}
			}
		}
	}

	return (cnt % 2 == 1);

	//ちなみに処理はすべてgithun copilotが書いてくれたので，処理の中身はよくわかっていない...
}
