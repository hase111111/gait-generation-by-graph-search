import csv
from collections import defaultdict

GRID = 20  # mm


def load_points(path):
    points = []
    with open(path, newline='', encoding='utf-8') as f:
        reader = csv.reader(f)
        rows = list(reader)

        start = 0
        try:
            float(rows[0][0])
        except ValueError:
            start = 1  # ヘッダあり

        for r in rows[start:]:
            x, y, z = map(float, r[:3])
            points.append((x, y, z))
    return points


def save_points(points, path):
    with open(path, "w", newline="", encoding="utf-8") as f:
        w = csv.writer(f)
        w.writerow(["x", "y", "z"])
        for p in points:
            w.writerow(p)


def filter_along_axis(points, fixed_axis="x"):
    """
    fixed_axis = "x" → x固定でy連続を見る（y方向チェック）
    fixed_axis = "y" → y固定でx連続を見る（x方向チェック）
    """

    groups = defaultdict(list)

    for x, y, z in points:
        key = x if fixed_axis == "x" else y
        groups[key].append((x, y, z))

    result = []

    for _, pts in groups.items():
        # 連続判定用の座標
        idx = 1 if fixed_axis == "x" else 0
        pts.sort(key=lambda p: p[idx])

        i = 0
        n = len(pts)

        while i < n:
            j = i
            while j + 1 < n and pts[j + 1][idx] - pts[j][idx] == GRID:
                j += 1

            length = j - i + 1
            seg = pts[i:j + 1]

            if length == 1:
                result.append(seg[0])

            elif length == 2:
                # 座標が大きい方
                result.append(seg[1])

            elif length == 3:
                # 真ん中
                result.append(seg[1])

            else:
                # 4点以上
                result.extend(seg)

            i = j + 1

    return result


# =========================
# 実行
# =========================
input_csv = "filtered_snapped.csv"
output_csv = "filtered_xy_result.csv"

points = load_points(input_csv)

# ① y方向チェック（x固定）
points_y = filter_along_axis(points, fixed_axis="x")

# ② x方向チェック（y固定）
points_xy = filter_along_axis(points_y, fixed_axis="y")

save_points(points_xy, output_csv)

print("入力点数:", len(points))
print("y方向処理後:", len(points_y))
print("x方向処理後:", len(points_xy))
print("保存先:", output_csv)
