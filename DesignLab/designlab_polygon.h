#pragma once

#include <vector>

#include "designlab_vector2.h"


namespace designlab
{

	static constexpr int MAX_VERTEX_NUM = 6;	//!< 速度を早くするためにあらかじめ最大サイズを決定しておく．


	//! @struct designlab::SPolygon2
	//! @brief 2次元の多角形を表す構造体
	//! @details 2次元の多角形を表す構造体．
	//! @n 頂点は反時計回り，或いは時計回りに並んでいる必要がある．
	//! @n そうでない場合はいくつかの関数が正常に動作しない．@n
	//! @n また，頂点の数は最大でMAX_VERTEX_NUM個までしか登録できない．
	//! @n これは動作の高速化のために確保した配列のサイズを固定しているためである．
	//! @n この値を変更する場合は，コード中のMAX_VERTEX_NUMの値を変更する必要がある．@n
	//! @n なお，コード中のmax関数，min関数については以下を参照．
	//! @n 参照 : https://cpprefjp.github.io/reference/algorithm/max.html
	struct SPolygon2 final
	{

		SPolygon2() { vertex.resize(MAX_VERTEX_NUM); };

		//! @brief 頂点を追加する関数
		//! @param [in] v 追加する頂点
		//! @note 他の頂点と重なっている場合でも追加する．
		inline void addVertex(const SVector2& v)
		{
			vertex[vertex_num] = v;
			++vertex_num;
		}

		//! @brief 頂点を追加する関数．他の頂点と重なっている場合は追加しない
		//! @param[in] v 追加する頂点
		//! @note 他の頂点と重なっている場合は追加しない．この処理の分だけ遅くなるので，重なることがない場合はaddVertexを使うこと
		//! @return bool 追加できたかどうか，追加できた場合はtrue，追加できなかった場合はfalse
		inline bool addVertexCheckForDuplicates(const SVector2& v)
		{
			for (int i = 0; i < vertex_num; i++)
			{
				if (vertex[i] == v)
				{
					return false;
				}
			}

			vertex[vertex_num] = v;
			++vertex_num;
			return true;
		}

		//! @brief 頂点を削除する関数．遅いので使用するべきではない
		//! @param [in] i 削除する頂点のインデックス
		//! @note 存在しない頂点を指定した場合は何もしない．
		//! @note 削除した頂点のインデックスは変わるので注意．
		inline void removeVertex(const int i)
		{
			if (i < 0 || i >= getVertexNum())
			{
				return;
			}

			vertex.erase(vertex.begin() + i);
			vertex.push_back({ 0,0 });
			--vertex_num;
		}

		//! @brief 1番最後の頂点を削除する関数
		//! @note 頂点が1つもない場合は何もしない．
		inline void removeLastVertex()
		{
			if (getVertexNum() == 0)
			{
				return;
			}

			--vertex_num;
		}

		//! @brief 多角形をリセットする関数
		inline void reset()
		{
			vertex_num = 0;
		}

		//! @brief 多角形の頂点数を返す関数
		//! @return 多角形の頂点数
		constexpr int getVertexNum() const { return vertex_num; }

		//! @brief 頂点の座標を返す関数
		//! @param [in] i 頂点のインデックス
		//! @return SVector2 頂点の座標
		//! @note 存在しない頂点を指定した場合は(0,0)を返す．
		inline SVector2 getVertex(int i) const
		{
			if (i < 0 || i >= getVertexNum())
			{
				return SVector2{ 0, 0 };
			}
			return vertex[i];
		}

		//! @brief 頂点の中で最大のx座標を返す関数
		//! @return float 頂点の中で最大のx座標
		inline float getMaxX() const
		{
			float max_x = vertex[0].x;

			for (int i = 1; i < getVertexNum(); ++i)
			{
				max_x = (std::max)(max_x, vertex[i].x);
			}

			return max_x;
		}

		//! @brief 頂点の中で最小のx座標を返す関数
		//! @return float 頂点の中で最小のx座標
		inline float getMinX() const
		{
			float min_x = vertex[0].x;

			for (int i = 1; i < getVertexNum(); ++i)
			{
				min_x = (std::min)(min_x, vertex[i].x);
			}

			return min_x;
		}

		//! @brief 頂点の中で最大のy座標を返す関数
		//! @return float 頂点の中で最大のy座標
		inline float getMaxY() const
		{
			float max_y = vertex[0].y;

			for (int i = 1; i < getVertexNum(); ++i)
			{
				max_y = (std::max)(max_y, vertex[i].y);
			}

			return max_y;
		}

		//! @brief 頂点の中で最小のy座標を返す関数
		//! @return 頂点の中で最小のy座標
		inline float getMinY() const
		{
			float min_y = vertex[0].y;

			for (int i = 1; i < getVertexNum(); ++i)
			{
				min_y = (std::min)(min_y, vertex[i].y);
			}

			return min_y;
		}

		//! @brief 多角形が凸かどうか調べる関数
		//! @return bool 凸ならtrue，凹ならfalse
		bool isConvex() const;

		//! @brief 点が多角形の内部にあるかどうか調べる関数．多角形が凸でない場合は正しく判定できない．
		//! @param [in] point 調べる点
		//! @return bool 内部にあるならtrue，外部にあるならfalse
		//! @note 点が時計回り，反時計回りのいずれかの順番で頂点が並んでいる必要がある．
		//! @note 点が多角形の辺上にある場合は内部にあると判定する．
		//! @note 多角形が凸でない場合は正しく判定できない．
		bool isInside(const SVector2& point) const;


	private:

		std::vector<SVector2> vertex;	//!< 頂点座標
		int vertex_num = 0;				//!< 頂点数

	};


	// 出力ストリーム
	template <class Char>
	inline std::basic_ostream<Char>& operator <<(std::basic_ostream<Char>& os, const SPolygon2& v)
	{
		os << "Vertex Num : " << v.getVertexNum() << "\n";

		for (int i = 0; i < v.getVertexNum(); ++i)
		{
			os << "Vertex " << i << " : " << v.getVertex(i) << "\n";
		}

		os << "Max X : " << v.getMaxX() << "\n";
		os << "Min X : " << v.getMinX() << "\n";
		os << "Max Y : " << v.getMaxY() << "\n";
		os << "Min Y : " << v.getMinY() << "\n";

		os << "Convex :" << (v.isConvex() ? "True" : "False") << "\n";

		return os;
	}

}	// namespace designlab


//! @file designlab_polygon.h
//! @date 2023/08/06
//! @author 長谷川
//! @brief 多角形を表す構造体
//! @n 行数 : @lineinfo
