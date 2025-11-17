import os
import pandas as pd
import hashlib

def get_file_paths(file_name):
    file_paths = []
    for root, dirs, files in os.walk('.'):
        if file_name in files:
            file_paths.append(os.path.join(root, file_name))
    return file_paths

# カレントディレクトリ以下の file_name.csv をすべて読み込み、CSVの配列を返す関数
def read_all_csv_files(file_name):
    file_paths = get_file_paths(file_name)
    data_frames = []
    for path in file_paths:
        df = pd.read_csv(path, header=None)
        data_frames.append(df)
    return data_frames

def read_csv_file(file_path):
    return pd.read_csv(file_path, header=None)

# CSVのデータの配列を受け取り，結合して1つのDataFrameとして返す関数
def combine_csv_data(data_frames):
    combined_df = pd.concat(data_frames, ignore_index=True)
    return combined_df

# ex) /home/user/documents/file.txt -> documents
def get_upper_directory_name(path):
    normalized_path = os.path.normpath(path)
    parent_dir = os.path.dirname(normalized_path)
    upper_dir = os.path.basename(parent_dir)
    return upper_dir

# ex) diagonal_40_40_20251116_1935_43 -> diagonal
def get_prefix_before_first_underscore(name):
    parts = name.split('_')
    return parts[0] if parts else name

# ハッシュ的に，ファイル名からユニークな色を生成する関数
def generate_color_from_filename(file_name):
    hash_value = hash(file_name)
    r = (hash_value & 0xFF0000) >> 16
    g = (hash_value & 0x00FF00) >> 8
    b = hash_value & 0x0000FF
    return f"#{r:02x}{g:02x}{b:02x}"

if __name__ == "__main__":
    print("ここでは実行されません")
