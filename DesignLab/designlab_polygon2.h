//! @file designlab_polygon2.h
//! @brief 多角形を表す構造体


#ifndef DESIGNLAB_POLYGON2_H
#define DESIGNLAB_POLYGON2_H


#include <array>
#include <string>
#include <optional>
#include <vector>

#include "cassert_define.h"
#include "designlab_vector2.h"


namespace designlab
{

	static constexpr int kMaxVertexNum = 6;	//!< 速度を早くするためにあらかじめ最大サイズを決定しておく．


	//! @struct designlab::Polygon2
	//! @brief 2次元の多角形を表す構造体
	//! @details 2次元の多角形を表す構造体．
	//! @n 頂点は反時計回り，或いは時計回りに並んでいる必要がある．
	//! @n そうでない場合はいくつかの関数が正常に動作しない．
	//! @n
	//! @n また，頂点の数は最大でkMaxVertexNum個までしか登録できない．
	//! @n これは動作の高速化のために確保した配列のサイズを固定しているためである．
	//! @n この値を変更する場合は，コード中のkMaxVertexNumの値を変更する必要がある．
	//! @n
	//! @n なお，コード中のmax関数，min関数については以下を参照．
	//! @n 参照 : https://cpprefjp.github.io/reference/algorithm/max.html
	struct Polygon2 final
	{

		constexpr Polygon2() : vertex_num(0) {};

		Polygon2(std::vector<Vector2> vertex);


		//! @brief 頂点を追加する関数
		//! @param [in] v 追加する頂点
		//! @note 他の頂点と重なっている場合でも追加する．
		constexpr void AddVertex(const Vector2& v)
		{
			vertex[vertex_num] = v;
			++vertex_num;

			assert(vertex_num <= kMaxVertexNum);	// 頂点数は最大値を超えてはいけない
		}

		//! @brief 頂点を追加する関数．他の頂点と重なっている場合は追加しない．
		//! @param[in] v 追加する頂点の座標
		//! @note 他の頂点と重なっている場合は追加しない．この処理の分だけ遅くなるので，重なることがない場合はaddVertexを使うこと
		//! @return bool 追加できたかどうか，追加できた場合はtrue，追加できなかった場合はfalse
		bool AddVertexCheckForDuplicates(const Vector2& v);


		//! @brief 頂点を削除する関数．遅いので多用するべきではない
		//! @param [in] index 削除する頂点のインデックス
		//! @note 存在しない頂点を指定した場合は何もしない．
		//! @note 削除した頂点のインデックスは変わるので注意．
		void RemoveVertex(const int index);

		//! @brief 1番最後の頂点を削除する関数
		//! @note 頂点が1つもない場合は何もしない．
		constexpr void RemoveLastVertex()
		{
			if (GetVertexNum() == 0)
			{
				return;
			}

			--vertex_num;
		}

		//! @brief 多角形をリセットする関数
		//! @n 頂点を削除して，頂点数を0にする．
		constexpr void Reset() { vertex_num = 0; }

		//! @brief 多角形の頂点数を返す関数
		//! @return int 多角形の頂点数
		constexpr int GetVertexNum() const { return vertex_num; }

		//! @brief 頂点の座標を返す関数
		//! @param [in] i 頂点のインデックス
		//! @return Vector2 頂点の座標
		//! @n 存在しない頂点を指定した場合は(0,0)を返す．
		constexpr Vector2 GetVertex(const int i) const
		{
			if (i < 0 || i >= GetVertexNum())
			{
				return Vector2{ 0, 0 };
			}

			return vertex[i];
		}

		//! @brief 頂点の座標を返す関数．std::optionalを使っているので，存在しない頂点を指定した場合はstd::nulloptを返す．
		//! @param [in] i 頂点のインデックス
		//! @return std::optional<Vector2> 頂点の座標
		//! @n 存在しない頂点を指定した場合はstd::nulloptを返す．
		constexpr std::optional<Vector2> GetVertexOpt(const int i) const
		{
			if (i < 0 || i >= GetVertexNum())
			{
				return std::nullopt;
			}

			return vertex[i];
		}

		//! @brief 頂点の中で最大のx座標を返す関数
		//! @return float 頂点の中で最大のx座標
		constexpr float GetMaxX() const 
		{
			float max_x = vertex[0].x;

			for (int i = 1; i < GetVertexNum(); ++i)
			{
				max_x = (std::max)(max_x, vertex[i].x);
			}

			return max_x;
		}

		//! @brief 頂点の中で最小のx座標を返す関数
		//! @return float 頂点の中で最小のx座標
		constexpr float GetMinX() const 
		{
			float min_x = vertex[0].x;

			for (int i = 1; i < GetVertexNum(); ++i)
			{
				min_x = (std::min)(min_x, vertex[i].x);
			}

			return min_x;
		}

		//! @brief 頂点の中で最大のy座標を返す関数
		//! @return float 頂点の中で最大のy座標
		constexpr float GetMaxY() const
		{
			float max_y = vertex[0].y;

			for (int i = 1; i < GetVertexNum(); ++i)
			{
				max_y = (std::max)(max_y, vertex[i].y);
			}

			return max_y;
		}

		//! @brief 頂点の中で最小のy座標を返す関数
		//! @return float 頂点の中で最小のy座標
		constexpr float GetMinY() const
		{
			float min_y = vertex[0].y;

			for (int i = 1; i < GetVertexNum(); ++i)
			{
				min_y = (std::min)(min_y, vertex[i].y);
			}

			return min_y;
		}

		//! @brief 多角形が凸かどうか調べる関数
		//! @return bool 凸ならtrue，凹ならfalse
		bool IsConvex() const;

		//! @brief 点が多角形の内部にあるかどうか調べる関数．多角形が凸でない場合は正しく判定できない．
		//! @param [in] point 調べる点
		//! @return bool 内部にあるならtrue，外部にあるならfalse
		//! @note 点が時計回り，反時計回りのいずれかの順番で頂点が並んでいる必要がある．
		//! @note 点が多角形の辺上にある場合は内部にあると判定する．
		//! @note 多角形が凸でない場合は正しく判定できない．
		bool IsInside(const Vector2& point) const;


		//! @brief 多角形のデータを文字列で出力する
		//! @return std::string 多角形のデータを文字列で出力したもの
		std::string ToString() const;

	private:

		std::array<Vector2, kMaxVertexNum> vertex;	//!< 頂点座標

		int vertex_num;					//!< 頂点数
	};


	// 出力ストリーム
	template <class Char>
	inline std::basic_ostream<Char>& operator <<(std::basic_ostream<Char>& os, const Polygon2& poly)
	{
		os << poly.ToString();

		return os;
	}

}	// namespace designlab


#endif // DESIGNLAB_POLYGON2_H