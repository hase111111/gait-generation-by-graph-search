import os
import csv

BASE_DIR = "result"
TARGET_FILE = "simulation_result_detail1.csv"
OUTPUT_CSV = "final_position_comparison.csv"

# --------------------------------
# ディレクトリ名解析
# --------------------------------
def parse_folder_name(folder_name):
    parts = folder_name.split("_")
    if len(parts) < 6:
        return None, None

    option = parts[0]
    terrain = "_".join(parts[2:-3])  # 地形名（_ 含む）
    return option, terrain


# --------------------------------
# CSVから「最後の移動量」を読む
# 行6〜105・列15 → 行105のみ使用
# --------------------------------
def read_last_value(csv_path):
    last_value = None
    with open(csv_path, encoding="shift_jis") as f:
        reader = csv.reader(f)
        for i, row in enumerate(reader, start=1):
            if 6 <= i <= 105:
                if len(row) > 15:
                    try:
                        last_value = float(row[15])
                    except ValueError:
                        pass
    return last_value


# --------------------------------
# terrain ごとにデータ収集
# --------------------------------
terrain_dict = {}

for folder in os.listdir(BASE_DIR):
    folder_path = os.path.join(BASE_DIR, folder)
    if not os.path.isdir(folder_path):
        continue

    option, terrain = parse_folder_name(folder)
    if option is None:
        continue

    csv_path = os.path.join(folder_path, TARGET_FILE)
    if not os.path.exists(csv_path):
        continue

    last_value = read_last_value(csv_path)
    if last_value is None:
        continue

    terrain_dict.setdefault(terrain, {})[option] = last_value


# --------------------------------
# normal基準で比較 → CSV出力
# --------------------------------
with open(OUTPUT_CSV, "w", newline="", encoding="utf-8") as f:
    writer = csv.writer(f)
    writer.writerow([
        "terrain",
        "option",
        "normal_last",
        "option_last",
        "diff (option - normal)"
    ])

    for terrain, option_dict in terrain_dict.items():
        if "normal" not in option_dict:
            continue  # normalがない地形は評価不可

        normal_value = option_dict["normal"]

        for option, value in option_dict.items():
            if option == "normal":
                continue

            diff = value - normal_value
            writer.writerow([
                terrain,
                option,
                normal_value,
                value,
                diff
            ])

print(f"比較結果を {OUTPUT_CSV} に出力しました")
