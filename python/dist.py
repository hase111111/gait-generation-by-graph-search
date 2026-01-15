"""
移動量を計算するモジュール
"""

import os
import statistics

BASE_DIR = "result"
TARGET_FILE = "simulation_result_detail1.csv"

all_values = []
val_dict = {}

for folder in os.listdir(BASE_DIR):
    folder_path = os.path.join(BASE_DIR, folder)

    if not folder.startswith("normal_"):
        continue

    if not os.path.isdir(folder_path):
        continue

    csv_path = os.path.join(folder_path, TARGET_FILE)

    if not os.path.exists(csv_path):
        continue

    # CSV 読み込み
    with open(csv_path, encoding="shift_jis") as f:
        for i, line in enumerate(f, start=1):
            if 6 <= i <= 105:
                cols = line.strip().split(",")
                if len(cols) >= 60:
                    try:
                        all_values.append(float(cols[15]))
                        val_dict[folder[:-16]] = float(cols[15])
                    except ValueError:
                        pass  # 数値でない行は無視

# ===== 集計 =====
if len(all_values) == 0:
    print("データが見つかりませんでした")
else:
    print("データ数:", len(all_values))
    print("平均:", statistics.mean(all_values))
    print("分散:", statistics.variance(all_values))
    print("標準偏差:", statistics.stdev(all_values))
    print("最小値:", min(all_values))
    print("最大値:", max(all_values))

# 全データについて，横軸をステップ数，縦軸を距離としたグラフをプロット
# 各データごとの最大値のみをプロット
import matplotlib.pyplot as plt
plt.figure()
x = list(val_dict.keys())
y = list(val_dict.values())
plt.bar(x, y)
plt.xticks(rotation=90)
plt.xlabel("Folder")
plt.ylabel("Distance")
plt.title("Distance by Folder")
plt.tight_layout()

# 縦軸を1600までに制限
plt.ylim(0, 1600)
plt.show()
