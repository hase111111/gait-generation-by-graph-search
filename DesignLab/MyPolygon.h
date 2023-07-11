#pragma once
#include <vector>
#include "MyVector2.h"

namespace my_vec
{
	//! @struct my_vec::SPolygon2
	//! @brief 2次元の多角形を表す構造体
	//! @details 2次元の多角形を表す構造体．
	struct SPolygon2 final
	{
		SPolygon2() = default;

		//! @brief 頂点を追加する関数
		//! @param[in] v 追加する頂点
		//! @note 他の頂点と重なっている場合でも追加する．
		inline void addVertex(const SVector2& v)
		{
			vertex.push_back(v);
		}

		//! @brief 頂点を追加する関数．他の頂点と重なっている場合は追加しない
		//! @param[in] v 追加する頂点
		//! @note 他の頂点と重なっている場合は追加しない．この処理の分だけ遅くなるので，重なることがない場合はaddVertexを使うこと
		//! @return 追加できたかどうか
		inline bool addVertexCheckForDuplicates(const SVector2& v)
		{
			for (const auto& i : vertex)
			{
				if (i == v)
				{
					return false;
				}
			}

			vertex.push_back(v);
			return true;
		}

		//! @brief 頂点を削除する関数
		//! @param[in] i 削除する頂点のインデックス
		//! @note 存在しない頂点を指定した場合は何もしない．
		//! @note 削除した頂点のインデックスは変わるので注意．
		inline void removeVertex(int i)
		{
			if (i < 0 || i >= getVertexNum())
			{
				return;
			}

			vertex.erase(vertex.begin() + i);
		}

		//! @brief 1番最後の頂点を削除する関数
		//! @note 頂点が1つもない場合は何もしない．
		inline void removeLastVertex()
		{
			if (getVertexNum() == 0)
			{
				return;
			}

			vertex.pop_back();
		}

		//! @brief 多角形の頂点数を返す関数
		//! @return 多角形の頂点数
		inline int getVertexNum() const { return static_cast<int>(vertex.size()); }

		//! @brief 頂点の座標を返す関数
		//! @param[in] i 頂点のインデックス
		//! @return 頂点の座標
		//! @note 存在しない頂点を指定した場合は(0,0)を返す．
		inline SVector2 getVertex(int i) const
		{
			if (i < 0 || i >= getVertexNum())
			{
				return SVector2(0, 0);
			}
			return vertex[i];
		}

		//! @brief 多角形が凸かどうか調べる関数
		//! @return 凸ならtrue，凹ならfalse
		bool isConvex() const
		{
			const int n = getVertexNum();

			//早期リターン.頂点数が3未満の場合は多角形ではない
			if (n < 3)
			{
				return false;
			}

			for (int i = 0; i < n; ++i)
			{
				const auto& v1 = vertex[(i + 1) % n] - vertex[i];
				const auto& v2 = vertex[(i + 2) % n] - vertex[(i + 1) % n];

				if (v1.cross(v2) < 0.0f)
				{
					return false;
				}
			}
			return true;
		}

		//! @brief 点が多角形の内部にあるかどうか調べる関数
		//! @param[in] p 調べる点
		//! @return 内部にあるならtrue，外部にあるならfalse
		//! @note 点が多角形の辺上にある場合は内部にあると判定する．
		//! @note 多角形が凸でない場合は正しく判定できない．
		bool isInside(const SVector2& _p) const
		{
			const int _num = getVertexNum();

			//早期リターン.頂点数が3未満の場合は多角形ではない
			if (_num < 3)
			{
				return false;
			}

			int _cnt = 0;

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

			return (_cnt % 2 == 1);
		}

	private:
		std::vector<SVector2> vertex;	//!< 頂点座標
	};
}