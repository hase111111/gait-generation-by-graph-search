import csv
import matplotlib.pyplot as plt

INPUT_CSV = "final_position_comparison_percent.csv"

labels = []
percents = []
options = []

with open(INPUT_CSV, encoding="utf-8") as f:
    reader = csv.DictReader(f)
    for row in reader:
        terrain = row["terrain"]
        option = row["option"]
        percent = float(row["percent_diff"])

        labels.append(f"{terrain} | {option}")
        percents.append(percent)
        options.append(option)

# option → 色
unique_options = sorted(set(options))
base_colors = plt.rcParams["axes.prop_cycle"].by_key()["color"]
color_map = {
    opt: base_colors[i % len(base_colors)]
    for i, opt in enumerate(unique_options)
}
colors = [color_map[o] for o in options]

# プロット
plt.figure(figsize=(max(12, len(labels) * 0.45), 6))
plt.bar(labels, percents, color=colors)
plt.axhline(0)

plt.xlabel("Terrain | Option")
plt.ylabel("Difference vs Normal (%)")
plt.title("Final Position Difference vs Normal (%)")

# 横軸対策
plt.xticks(rotation=90)
plt.gca().margins(x=0.01)
plt.tight_layout()

# 凡例
handles = [
    plt.Rectangle((0, 0), 1, 1, color=color_map[opt])
    for opt in unique_options
]
plt.legend(handles, unique_options, title="Option")

plt.show()
