import os
import csv

BASE_DIR = "result"
TARGET_FILE = "simulation_result_detail1.csv"
OUTPUT_CSV = "final_position_comparison_percent.csv"

def parse_folder_name(folder_name):
    parts = folder_name.split("_")
    if len(parts) < 6:
        return None, None
    option = parts[0]
    terrain = "_".join(parts[2:-3])
    return option, terrain

def read_last_value(csv_path):
    last_value = None
    with open(csv_path, encoding="shift_jis") as f:
        reader = csv.reader(f)
        for i, row in enumerate(reader, start=1):
            if 6 <= i <= 105 and len(row) > 15:
                try:
                    last_value = float(row[15])
                except ValueError:
                    pass
    return last_value

# terrain → option → value
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

# CSV 出力（％）
with open(OUTPUT_CSV, "w", newline="", encoding="utf-8") as f:
    writer = csv.writer(f)
    writer.writerow([
        "terrain",
        "option",
        "normal_last",
        "option_last",
        "percent_diff"
    ])

    for terrain, option_dict in terrain_dict.items():
        if "normal" not in option_dict:
            continue

        normal = option_dict["normal"]
        if normal == 0:
            continue  # 念のため

        for option, value in option_dict.items():
            if option == "normal":
                continue

            percent_diff = (value - normal) / normal * 100.0

            writer.writerow([
                terrain,
                option,
                normal,
                value,
                percent_diff
            ])

print(f"％比較結果を {OUTPUT_CSV} に出力しました")
