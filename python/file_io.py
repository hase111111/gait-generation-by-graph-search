import os
import pandas as pd
from typing import List

# ------------------------------------------------------------------
# ファイル入出力系
# ------------------------------------------------------------------

def get_file_paths(file_name : str) -> List[str]:
    """
    カレントディレクトリ以下のすべての file_name を探し，そのパスのリストを返す関数.
    """
    file_paths : List[str] = []
    for root, _, files in os.walk('.'):
        if file_name in files:
            file_paths.append(os.path.join(root, file_name))
    return file_paths

def read_all_csv_files(file_name : str) -> List[pd.DataFrame]:
    """
    カレントディレクトリ以下の file_name.csv をすべて読み込み、CSVの配列を返す関数
    """
    file_paths = get_file_paths(file_name)
    data_frames : List[pd.DataFrame] = []
    for path in file_paths:
        df = pd.read_csv(path, header=None)  # type: ignore
        data_frames.append(df)
    return data_frames

def read_csv_file(file_path : str) -> pd.DataFrame:
    """
    指定されたパスのCSVファイルを読み込み、DataFrameとして返す関数
    """
    return pd.read_csv(file_path, header=None)  # type: ignore

# ------------------------------------------------------------------
# CSVデータ操作系
# ------------------------------------------------------------------

def combine_csv_data(data_frames: List[pd.DataFrame]) -> pd.DataFrame:
    """
    CSVのデータの配列を受け取り，結合して1つのDataFrameとして返す関数.
    """
    combined_df = pd.concat(data_frames, ignore_index=True)
    return combined_df


# ------------------------------------------------------------------
# ファイル名・ディレクトリ名操作系
# ------------------------------------------------------------------

def get_upper_directory_name(path: str) -> str:
    """
    指定されたパスの一つ上のディレクトリ名を返す関数. \n
    ex) /home/user/documents/file.txt -> documents
    """
    normalized_path = os.path.normpath(path)
    parent_dir = os.path.dirname(normalized_path)
    upper_dir = os.path.basename(parent_dir)
    return upper_dir

def get_prefix_before_first_underscore(name: str) -> str:
    """
    ファイル名の最初のアンダースコアまでの部分を返す関数. \n
    ex) diagonal_40_40_20251116_1935_43 -> diagonal
    """
    parts = name.split('_')
    return parts[0] if parts else name

def generate_color_from_filename(file_name: str) -> str:
    """
    ハッシュを用いて，ファイル名からユニークな色を生成する関数.
    """
    hash_value = hash(file_name)
    r = (hash_value & 0xFF0000) >> 16
    g = (hash_value & 0x00FF00) >> 8
    b = hash_value & 0x0000FF
    return f"#{r:02x}{g:02x}{b:02x}"

if __name__ == "__main__":
    print("ここでは実行されません.")
