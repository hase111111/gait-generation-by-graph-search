
# このスクリプトがおかれているディレクトリを取得
import os

current_dir = os.path.dirname(os.path.abspath(__file__))
print(f"current_dir: {current_dir}")

# カレントディレクトリ内のC++ファイルを取得
cpp_ext = ".cpp"
header_ext = ".h"

cpp_files = [f for f in os.listdir(current_dir) if f.endswith(cpp_ext)]
header_files = [f for f in os.listdir(current_dir) if f.endswith(header_ext)]

print(f"cpp_files num : {len(cpp_files)}")
print(f"header_files num : {len(header_files)}")


# ライセンスのテンプレート
file_name_comment = "//! @file "
license_template = """

// Copyright(c) 2023-2025 Design Engineering Laboratory, Saitama University
// Released under the MIT license
// https://opensource.org/licenses/mit-license.php
"""

print(file_name_comment + license_template)


# 全ファイルについてライセンスを更新する
# まず，ファイルの先頭にあるコメントを削除する
# その後，file_name_comment, license_templateを追加する
def update_license(filepath, new_license):
    with open(filepath, "r", encoding="utf-8") as file:
        lines = file.readlines()

    # `//` で始まらない最初の行を探す
    start_index = 0
    for i, line in enumerate(lines):
        if not line.strip().startswith("//"):  # `//` で始まらない行を見つける
            start_index = i
            break

    # 新しいライセンスを挿入し、先頭のコメントを削除
    new_content = new_license + "\n" + "".join(lines[start_index:])

    # 上書き保存
    with open(filepath, "w", encoding="utf-8") as file:
        file.write(new_content)


# ファイルごとにライセンスを更新
for cpp_file in cpp_files:
    update_license(cpp_file, file_name_comment + cpp_file + license_template)