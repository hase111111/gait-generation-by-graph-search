
# continuous_simulation_map以下の点群データに対して
# 行数をカウントし，504500行と比較して，何%になるかを計算する
import csv
from collections import defaultdict
GRID = 20  # mm
TARGET_COUNT = 504500
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
def count_points(points):
    return len(points)
def calculate_percentage(count, target=TARGET_COUNT):
    return (count / target) * 100

# continuous_simulation_map以下のすべてのCSVファイルに対して処理を行う
import os
def process_directory(directory):
    results = {}
    for filename in os.listdir(directory):
        if filename.endswith(".csv"):
            path = os.path.join(directory, filename)
            points = load_points(path)
            count = count_points(points)
            percentage = calculate_percentage(count)
            results[filename] = (count, percentage)
    return results  

if __name__ == "__main__":
    directory = "continuous_simulation_map"
    results = process_directory(directory)
    for filename, (count, percentage) in results.items():
        print(f"{filename}: {count} points, {percentage:.2f}% of target")   
        