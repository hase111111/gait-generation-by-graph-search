import pandas as pd
import numpy as np

# ===== 設定 =====
input_csv  = "filtered_by_count (1).csv"
output_csv = "filtered_snapped.csv"
step = 20  # 格子間隔(mm)
# =================

# 読み込み（x,y,z）
df = pd.read_csv(input_csv, header=None)
df.columns = ["x", "y", "z"]

# --- 基準点（最初のデータ） ---
x0, y0 = df.loc[0, ["x", "y"]]

# --- 格子点へスナップ ---
df["xg"] = x0 + np.round((df["x"] - x0) / step) * step
df["yg"] = y0 + np.round((df["y"] - y0) / step) * step

# (xg, yg) の集合
coords = set(zip(df["xg"], df["yg"]))

# --- 周囲24点チェック ---
def has_all_24_neighbors(x, y):
    for dx in (-step, 0, step):
        for dy in (-step, 0, step):
    # for dx in (-2*step, -step, 0, step, 2*step):
    #     for dy in (-2*step, -step, 0, step, 2*step):
            if dx == 0 and dy == 0:
                continue
            if (x + dx, y + dy) not in coords:
                return False
    return True

mask = np.array([
    has_all_24_neighbors(x, y)
    for x, y in zip(df["xg"], df["yg"])
])

# --- フィルタ ---
out_df = df.loc[mask, ["xg", "yg", "z"]].copy()

# 保存
out_df.to_csv(output_csv, index=False, header=False)

# --- 情報表示 ---
print("元データ点数:", len(df))
print("格子補正後の有効点数:", len(out_df))
print("削除点数:", len(df) - len(out_df))
print("基準点:", (x0, y0))
