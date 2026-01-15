import csv
import matplotlib.pyplot as plt
from collections import defaultdict

INPUT_CSV = "final_position_comparison.csv"

labels = []
diffs = []
options = []

# --------------------------------
# CSV 読み込み
# --------------------------------
with open(INPUT_CSV, encoding="utf-8") as f:
    reader = csv.DictReader(f)
    for row in reader:
        terrain = row["terrain"]
        option = row["option"]
        diff = float(row["diff (option - normal)"])

        labels.append(f"{terrain} | {option}")
        diffs.append(diff)
        options.append(option)

# --------------------------------
# option → 色 を割り当て
# --------------------------------
unique_options = sorted(set(options))
color_map = {
    opt: plt.rcParams["axes.prop_cycle"].by_key()["color"][i % 10]
    for i, opt in enumerate(unique_options)
}
colors = [color_map[opt] for opt in options]

# --------------------------------
# プロット
# --------------------------------
plt.figure(figsize=(max(10, len(labels) * 0.4), 6))
plt.bar(labels, diffs, color=colors)
plt.axhline(0)

plt.xlabel("Terrain | Option")
plt.ylabel("Difference (option - normal)")
plt.title("Final Position Difference vs Normal (All Terrains & Options)")
plt.xticks(rotation=90)
plt.gca().margins(x=0.01)
plt.tight_layout()

# 凡例（option別）
handles = [
    plt.Rectangle((0, 0), 1, 1, color=color_map[opt])
    for opt in unique_options
]
plt.legend(handles, unique_options, title="Option")

plt.tight_layout()

plt.show()
