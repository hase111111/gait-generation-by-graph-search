
//! @author    Hasegawa
//! @copyright © 埼玉大学 設計工学研究室 2023. All right reserved.

#include "math_polygon2.h"

#include "math_util.h"


namespace designlab
{

Polygon2::Polygon2(const std::vector<Vector2>& vertex)
{
    assert(vertex.size() <= kMaxVertexNum);  // 頂点数は最大値を超えてはいけない．

    for (int i = 0; i < static_cast<int>(vertex.size()); ++i)
    {
        AddVertex(vertex[i]);
    }
}

bool Polygon2::AddVertexCheckForDuplicates(const Vector2& v)
{
    for (int i = 0; i < vertex_num; i++)
    {
        if (vertex[static_cast<size_t>(i)] == v)
        {
            return false;
        }
    }

    vertex[static_cast<size_t>(vertex_num)] = v;
    ++vertex_num;

    assert(vertex_num <= kMaxVertexNum);  // 頂点数は最大値を超えてはいけない．

    return true;
}

void Polygon2::RemoveVertex(const int i)
{
    if (i < 0 || i >= GetVertexNum())
    {
        return;
    }

    // i番目の頂点を削除する．そして，i+1番目以降の頂点をi番目以降にコピーする．
    for (int j = i; j < GetVertexNum() - 1; ++j)
    {
        vertex[j] = vertex[j + 1];
    }

    vertex[vertex_num - 1] = Vector2(0.0f, 0.0f);

    --vertex_num;
}

bool Polygon2::IsConvex() const
{
    const int num = GetVertexNum();

    // 早期リターン．頂点数が3未満の場合は多角形ではない．
    if (num < 3)
    {
        return false;
    }

    // 右回りか左回りかを調べる．
    const auto v1 = vertex[1] - vertex[0];
    const auto v2 = vertex[2] - vertex[1];

    bool is_left_turn = v1.Cross(v2) > 0.0f;

    for (int i = 1; i < num; ++i)
    {
        const auto& v1_2 = vertex[(i + 1) % num] - vertex[i];
        const auto& v2_2 = vertex[(i + 2) % num] - vertex[(i + 1) % num];

        if (is_left_turn && v1_2.Cross(v2_2) < 0.0f)
        {
            return false;
        }
        else if (!is_left_turn && v1_2.Cross(v2_2) > 0.0f)
        {
            return false;
        }
    }

    return true;
}

bool Polygon2::IsInside(const Vector2& point) const
{
    const int num = GetVertexNum();

    // 早期リターン．頂点数が3未満の場合は多角形ではない．
    if (num < 3)
    {
        return false;
    }

    int cnt = 0;

    // 頂点が右回りか左回りかを調べる．
    bool is_left_turn = (vertex[1] - vertex[0]).Cross(vertex[2] - vertex[1]) > 0.0f;

    if (!is_left_turn)
    {
        for (int i = 0; i < num; ++i)
        {
            const auto& v1 = vertex[i] - point;
            const auto& v2 = vertex[(i + 1) % num] - point;

            if (v1.Cross(v2) == 0.0f && v1.Dot(v2) <= 0.0f)
            {
                return true;  // 点が辺上にある．
            }

            if (v1.y < v2.y)
            {
                if (v1.y < 0.0f && 0.0f <= v2.y && v1.Cross(v2) > 0.0f)
                {
                    --cnt;
                }
            }
            else
            {
                if (v2.y < 0.0f && 0.0f <= v1.y && v1.Cross(v2) < 0.0f)
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

            if (v1.Cross(v2) == 0.0f && v1.Dot(v2) <= 0.0f)
            {
                return true;  // 点が辺上にある．
            }

            if (v1.y > v2.y)
            {
                if (v2.y < 0.0f && 0.0f <= v1.y && v1.Cross(v2) < 0.0f)
                {
                    --cnt;
                }
            }
            else
            {
                if (v1.y < 0.0f && 0.0f <= v2.y && v1.Cross(v2) > 0.0f)
                {
                    ++cnt;
                }
            }
        }
    }

    return (cnt % 2 == 1);
}

std::string Polygon2::ToString() const
{
    std::string res;

    res += "Vertex Num : " + std::to_string(GetVertexNum()) + "\n";

    for (int i = 0; i < GetVertexNum(); ++i)
    {
        res += "Vertex " + std::to_string(i) + " : " + GetVertex(i).ToString() + "\n";
    }

    res += "Max X : " + math_util::FloatingPointNumToString(GetMaxX()) + "\n";
    res += "Min X : " + math_util::FloatingPointNumToString(GetMinX()) + "\n";
    res += "Max Y : " + math_util::FloatingPointNumToString(GetMaxY()) + "\n";
    res += "Min Y : " + math_util::FloatingPointNumToString(GetMinY()) + "\n";

    res += "Convex :";
    res += (IsConvex() ? "TRUE" : "FALSE");
    res += "\n";

    return res;
}

}  // namespace designlab
