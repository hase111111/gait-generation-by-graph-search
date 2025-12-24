import os
import re
import statistics
from collections import defaultdict

BASE_DIR = "result"
TARGET_FILE = "simulation_result_detail1.csv"
OUTPUT_TEX = "terrain_statistics.tex"

FOLDER_PATTERN = re.compile(
    r"^(normal|legacy|pruning)_success_(.+?)_\d{8}_\d{4}_\d{2}$"
)

# data[terrain][format] = [values...]
data = defaultdict(lambda: defaultdict(list))

# ===== データ収集 =====
for folder in os.listdir(BASE_DIR):
    m = FOLDER_PATTERN.match(folder)
    if not m:
        continue

    fmt = m.group(1)
    terrain = m.group(2)

    csv_path = os.path.join(BASE_DIR, folder, TARGET_FILE)
    if not os.path.exists(csv_path):
        continue

    with open(csv_path, encoding="shift_jis") as f:
        for i, line in enumerate(f, start=1):
            if 6 <= i <= 105:
                cols = line.strip().split(",")
                if len(cols) >= 60:
                    try:
                        data[terrain][fmt].append(float(cols[59]))
                    except ValueError:
                        pass

# ===== 補助関数 =====
def latex_escape(s):
    return (
        s.replace("\\", r"\textbackslash ")
         .replace("%", r"_percent")
         .replace("_", r"\_")
    )

def camel_case_with_space(s):
    # snake_case → "Camel Case With Space"
    words = s.split("_")
    return " ".join(w.capitalize() for w in words).replace("%", r" Percent")


# ===== LaTeX 出力 =====
with open(OUTPUT_TEX, "w", encoding="utf-8") as out:
    out.write("% Auto-generated tables for Chapter 4\n\n")

    cnt = 0
    for terrain in sorted(data.keys()):
        terrain_cc = camel_case_with_space(terrain)

        # normal 平均（比率計算用）
        if "normal" not in data[terrain]:
            continue
        normal_mean = statistics.mean(data[terrain]["normal"])

        out.write(r"\begin{table}[!htbp]" + "\n")
        out.write(r"\centering" + "\n")
        out.write(
            r"\caption{Statistics for terrain: "
            + terrain_cc + r"}" + "\n"
        )
        out.write(
            r"\label{table_ch4_"
            + terrain.replace("%", r"percent") + r"}  % chktex 24" + "\n"
        )

        # 縦線 + 比率列の前を二重線
        out.write(
            r"\begin{tabular}{|l|r|r|r|r|r||r|}  % chktex 44" + "\n"
        )
        out.write(r"\hline  % chktex 44" + "\n")

        # ヘッダ
        out.write(
            r"Method & Mean[ms] & Variance[ms$^2$] & Std.Dev[ms] & Min[ms] & Max[ms] & Ratio [\%] \\"
            + "\n"
        )
        out.write(r"\hline\hline  % chktex 44" + "\n")

        for fmt in ["normal", "legacy", "pruning"]:
            values = data[terrain].get(fmt, [])
            if not values:
                continue

            mean = statistics.mean(values)
            var = statistics.variance(values) if len(values) >= 2 else 0.0
            std = statistics.stdev(values) if len(values) >= 2 else 0.0
            min_v = min(values)
            max_v = max(values)

            ratio = (mean / normal_mean) * 100.0

            out.write(
                f"{fmt} & "
                f"{mean:.3f} & {var:.3f} & {std:.3f} & "
                f"{min_v:.3f} & {max_v:.3f} & "
                f"{ratio:.3f} \\\\" + "\n"
            )

        out.write(r"\hline  % chktex 44" + "\n")
        out.write(r"\end{tabular}" + "\n")
        out.write(r"\end{table}" + "\n\n")

        # 4つごとに改ページ
        cnt += 1
        if cnt % 5 == 0:
            out.write(r"\newpage" + "\n\n")
