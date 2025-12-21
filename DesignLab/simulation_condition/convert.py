import pandas as pd

# ===== 設定 =====
INPUT_CSV = "filtered_by_count (1).csv"
OUTPUT_CSV = "filtered_grid_neighbors8.csv"
GRID_STEP = 20

# ===== CSV読み込み =====
df = pd.read_csv(INPUT_CSV, header=None, names=["x", "y", "z"])

# (x, y) を集合にして高速判定
points = set(zip(df["x"], df["y"]))

# 8近傍の相対座標
neighbors = [
    (-GRID_STEP, -GRID_STEP),
    (-GRID_STEP,  0),
    (-GRID_STEP,  GRID_STEP),
    (0, -GRID_STEP),
    (0,  GRID_STEP),
    (GRID_STEP, -GRID_STEP),
    (GRID_STEP,  0),
    (GRID_STEP,  GRID_STEP),
]

# ===== 判定処理 =====
keep_mask = []

for x, y in zip(df["x"], df["y"]):
    count = 0
    for dx, dy in neighbors:
        if (x + dx, y + dy) in points:
            count += 1

    # 周囲8点すべて存在する場合のみ残す
    keep_mask.append(count == 8)

# ===== フィルタ & 出力 =====
filtered_df = df[keep_mask]
filtered_df.to_csv(OUTPUT_CSV, index=False, header=False)

print(f"入力点数: {len(df)}")
print(f"残った点数: {len(filtered_df)}")
print(f"出力ファイル: {OUTPUT_CSV}")
