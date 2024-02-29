
//! @file      math_polygon2.h
//! @author    Hasegawa
//! @copyright (C) 2023 Design Engineering Laboratory,
//! Saitama University All right reserved.

#ifndef DESIGNLAB_MATH_POLYGON2_H_
#define DESIGNLAB_MATH_POLYGON2_H_

#include <array>
#include <optional>
#include <string>
#include <vector>

#include "cassert_define.h"
#include "math_vector2.h"


namespace designlab
{

//! @struct designlab::Polygon2
//! @brief 2次元の多角形を表す構造体．
//! @details
//! @n 頂点は反時計回り，或いは時計回りに並んでいる必要がある．
//! そうでない場合はいくつかの関数が正常に動作しない．
//! @n
//! @n また，頂点の数は最大でkMaxVertexNum個までしか登録できない．
//! これは動作の高速化のために確保した配列のサイズを固定しているためである．
//! この値を変更する場合は，コード中のkMaxVertexNumの値を変更する必要がある．
//! @n
//! @n なお，コード中の max 関数，min関数については以下を参照．
//! @subsubsection 参照
//! @li cpprefjp std::max
//!     https://cpprefjp.github.io/reference/algorithm/max.html
//!     ( アクセス日 2024/2/29 )
struct Polygon2 final
{
    constexpr Polygon2() : vertex_num(0)
    {
    }

    explicit Polygon2(const std::vector<Vector2>& vertex);


    //! @brief 頂点を追加する関数．
    //! @param[in] v 追加する頂点．
    //! @note 他の頂点と重なっている場合でも追加する．
    constexpr void AddVertex(const Vector2& v)
    {
        vertex[static_cast<size_t>(vertex_num)] = v;
        ++vertex_num;

        assert(vertex_num <= kMaxVertexNum);  // 頂点数は最大値を超えてはいけない．
    }

    //! @brief 頂点を追加する関数．他の頂点と重なっている場合は追加しない．
    //! @param[in] v 追加する頂点の座標．
    //! @note 他の頂点と重なっている場合は追加しない．
    //! この処理の分だけ遅くなるので，重なることがない場合は AddVertex を使うこと．
    //! @retval true 追加できた，
    //! @retval false 追加できなかった．
    bool AddVertexCheckForDuplicates(const Vector2& v);


    //! @brief 頂点を削除する関数．遅いので多用するべきではない．
    //! @param[in] index 削除する頂点のインデックス．
    //! @note 存在しない頂点を指定した場合は何もしない．
    //! @note 削除した頂点のインデックスは変わるので注意．
    void RemoveVertex(const int index);

    //! @brief 1番最後の頂点を削除する関数．
    //! @note 頂点が1つもない場合は何もしない．
    constexpr void RemoveLastVertex()
    {
        if (GetVertexNum() == 0)
        {
            return;
        }

        --vertex_num;
    }

    //! @brief 多角形をリセットする関数．
    //! @n 頂点を削除して，頂点数を0にする．
    constexpr void Reset()
    {
        vertex_num = 0;
    }

    //! @brief 多角形の頂点数を返す関数．
    //! @return 多角形の頂点数．
    constexpr int GetVertexNum() const
    {
        return vertex_num;
    }

    //! @brief 頂点の座標を返す関数．
    //! @param[in] i 頂点のインデックス．
    //! @return 頂点の座標．
    //! @n 存在しない頂点を指定した場合は(0,0)を返す．
    constexpr Vector2 GetVertex(const int i) const
    {
        if (i < 0 || i >= GetVertexNum())
        {
            return Vector2{ 0, 0 };
        }

        return vertex[static_cast<size_t>(i)];
    }

    //! @brief 頂点の座標を返す関数．std::optional を使っているので，
    //! 存在しない頂点を指定した場合は std::nullopt を返す．
    //! @param[in] i 頂点のインデックス．
    //! @return 頂点の座標．存在しない頂点を指定した場合は std::nullopt を返す．
    constexpr std::optional<Vector2> GetVertexOpt(const int i) const
    {
        if (i < 0 || i >= GetVertexNum())
        {
            return std::nullopt;
        }

        return vertex[static_cast<size_t>(i)];
    }

    //! @brief 頂点の中で最大のx座標を返す関数．
    //! @return 頂点の中で最大のx座標．
    constexpr float GetMaxX() const
    {
        float max_x = vertex[0].x;

        for (int i = 1; i < GetVertexNum(); ++i)
        {
            max_x = (std::max)(max_x, vertex[static_cast<size_t>(i)].x);
        }

        return max_x;
    }

    //! @brief 頂点の中で最小のx座標を返す関数．
    //! @return 頂点の中で最小のx座標．
    constexpr float GetMinX() const
    {
        float min_x = vertex[0].x;

        for (int i = 1; i < GetVertexNum(); ++i)
        {
            min_x = (std::min)(min_x, vertex[static_cast<size_t>(i)].x);
        }

        return min_x;
    }

    //! @brief 頂点の中で最大のy座標を返す関数．
    //! @return 頂点の中で最大のy座標．
    constexpr float GetMaxY() const
    {
        float max_y = vertex[0].y;

        for (int i = 1; i < GetVertexNum(); ++i)
        {
            max_y = (std::max)(max_y, vertex[static_cast<size_t>(i)].y);
        }

        return max_y;
    }

    //! @brief 頂点の中で最小のy座標を返す関数．
    //! @return 頂点の中で最小のy座標．
    constexpr float GetMinY() const
    {
        float min_y = vertex[0].y;

        for (int i = 1; i < GetVertexNum(); ++i)
        {
            min_y = (std::min)(min_y, vertex[static_cast<size_t>(i)].y);
        }

        return min_y;
    }

    //! @brief 多角形が凸かどうか調べる関数．
    //! @retval true 凸．
    //! @retval false 凹．
    bool IsConvex() const;

    //! @brief 点が多角形の内部にあるかどうか調べる関数．
    //! 多角形が凸でない場合は正しく判定できない．
    //! @param[in] point 調べる点
    //! @return bool 内部にあるなら true，外部にあるなら false
    //! @note 点が時計回り，反時計回りのいずれかの順番で頂点が並んでいる必要がある．
    //! @note 点が多角形の辺上にある場合は内部にあると判定する．
    //! @note 多角形が凸でない場合は正しく判定できない．
    bool IsInside(const Vector2& point) const;


    //! @brief 多角形のデータを文字列で出力する
    //! @return 多角形のデータを文字列で出力したもの
    std::string ToString() const;

private:
    //! 速度を早くするためにあらかじめ最大サイズを決定しておく．
    static constexpr int kMaxVertexNum = 6;

    std::array<Vector2, kMaxVertexNum> vertex;  //!< 頂点座標．

    int vertex_num;  //!< 頂点数．
};


//! 出力ストリーム．
template <class Char>
inline std::basic_ostream<Char>& operator <<(
    std::basic_ostream<Char>& os, const Polygon2& poly)
{
    os << poly.ToString();

    return os;
}

}  // namespace designlab


#endif  // DESIGNLAB_MATH_POLYGON2_H_
