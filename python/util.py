from typing import List, Tuple
import pandas as pd

# ------------------------------------------------------------------
# 1000100010001000100010001000 -> bitdata
# [(true,4),(true,4),(true,4),(true,4),(true,4),(true,4)] -> tupledata
# [true,true,true,true,true,true], [4,4,4,4,4,4] -> boollist, intlist
# ------------------------------------------------------------------

def leg_ground_map(bit_str: str) -> int:
    if bit_str =="010101":
        return 0
    elif bit_str =="101010":
        return 1
    elif bit_str =="111111":
        return 2
    elif bit_str =="011111":
        return 3
    elif bit_str =="101111":
        return 4
    elif bit_str =="110111":
        return 5
    elif bit_str =="111011":
        return 6
    elif bit_str =="111101":
        return 7
    elif bit_str =="111110":
        return 8
    elif bit_str =="001111":
        return 9
    elif bit_str =="010111":
        return 10
    elif bit_str =="011011":
        return 11
    elif bit_str =="011101":
        return 12
    elif bit_str =="011110":
        return 13
    elif bit_str =="100111":
        return 14
    elif bit_str =="101011":
        return 15
    elif bit_str =="101101":
        return 16
    elif bit_str =="101110":
        return 17
    elif bit_str =="110011":
        return 18
    elif bit_str =="110101":
        return 19
    elif bit_str =="110110":
        return 20
    elif bit_str =="111001":
        return 21
    elif bit_str =="111010":
        return 22
    elif bit_str =="111100":
        return 23
    elif bit_str =="001011":
        return 24
    elif bit_str =="001101":
        return 25
    elif bit_str =="010011":
        return 26
    elif bit_str =="010110":
        return 27
    elif bit_str =="011001":
        return 28
    elif bit_str =="011010":
        return 29
    elif bit_str =="100101":
        return 30
    elif bit_str =="100110":
        return 31
    elif bit_str =="101001":
        return 32
    elif bit_str =="101100":
        return 33
    elif bit_str =="110010":
        return 34
    elif bit_str =="110100":
        return 35
    else:
        raise ValueError(f"脚接地パターンが不正です: {bit_str}")
  
def get_color_by_leg_ground(leg_ground_int: int) -> str:
    if leg_ground_int == 0:
        return "red"
    elif leg_ground_int == 1:
        return "orange"
    elif leg_ground_int == 2:
        return "yellow"
    elif leg_ground_int in range(3, 9):
        diff = 255 - (leg_ground_int - 3) * 20
        return f"#{diff:02x}{diff:02x}00"  # yellow to light green
    elif leg_ground_int in range(9, 24):
        temp = (leg_ground_int - 9) * 10
        diff = 255 - temp
        return f"#{temp:02x}{diff:02x}00"
    else:
        temp = (leg_ground_int - 24) * 10
        diff = 255 - temp
        return f"#{temp:02x}00{diff:02x}"

# ------------------------------------------------------------------
# CSVデータ操作系.
# ------------------------------------------------------------------

def make_bitdata_list_from_csv(data: pd.DataFrame) -> List[str]: 
    """
    CSVデータフレームから，先頭のビットデータを取得して, 
    そのリストを作成して返す関数.
    """
    # for文で，各行のデータを処理.
    bitdata: List[str] = []
    past_move = "none"

    for _, row in data.iterrows():
        # A列目とAS列目のデータを取得.
        bit = row[0]
        move = row[44]

        if past_move == "kLegHierarchyChange":
            bitdata.append(bit)
        elif past_move == "kLegUpDown":
            bitdata.append(bit)

        past_move = move.replace(" ", "")

    return bitdata

def make_all_bitdata_list_from_csv(data_frames: pd.DataFrame) -> List[str]:
    """
    複数のCSVデータフレームから，先頭のビットデータを取得して, 
    そのリストを作成して返す関数. \n
    すべてのbitデータをまとめて返します.
    """
    all_bitdata: List[str] = []

    for _, row in data_frames.iterrows():
        bit_str : str= str(row[0])
        
        if len(bit_str) != 28:
            raise ValueError(f"ビットデータの長さが28ではありません.{bit_str}")
        
        all_bitdata.append(bit_str)

    return all_bitdata

def make_all_bitdata_list_from_csv_with_move(data_frames: pd.DataFrame) -> List[Tuple[str, str]]:
    """
    複数のCSVデータフレームから，先頭のビットデータを取得して, 
    そのリストを作成して返す関数. \n
    すべてのbitデータをまとめて返します.
    """
    all_bitdata: List[Tuple[str, str]] = []

    for _, row in data_frames.iterrows():
        bit_str : str= str(row[0])
        
        if len(bit_str) != 28:
            raise ValueError(f"ビットデータの長さが28ではありません.{bit_str}")
        
        all_bitdata.append((bit_str, row[44]))

    return all_bitdata

def make_tupledata_list_from_csv(data: pd.DataFrame) -> List[List[Tuple[bool, int]]]:
    """
    CSVデータフレームから，先頭のビットデータを取得して, 
    そのタプルデータリストを作成して返す関数.
    """
    bitdata_list = make_all_bitdata_list_from_csv_with_move(data)
    tupledata = [bitdata_to_tupledata(bitstr) for bitstr, _ in bitdata_list]
    move = [move for _, move in bitdata_list]
    fixed_tupledata_list = fix_tupledata(tupledata, move)

    # move が kLegupDown, kComUpDown となる index のみを抽出した tupledata を返す
    result_tupledata_list: List[List[Tuple[bool, int]]] = []
    for i in range(len(bitdata_list)):
        _, mv = bitdata_list[i]
        if mv.replace(" ", "") in ("kLegUpDown", "kComUpDown"):
            result_tupledata_list.append(fixed_tupledata_list[i])
    return result_tupledata_list

# ------------------------------------------------------------------
# ビットデータ変換系.
# ------------------------------------------------------------------

def bitdata_to_tupledata(bitstr: str) -> List[Tuple[bool, int]]:
    """
    28bitのビットデータ文字列を受け取り，\n
    上位4bitを無視し，下位24bitをTuple(bool, int)のList(要素数6)に変換.
    """
    if len(bitstr) != 28:
        raise ValueError("ビットデータの長さが28ではありません.")
    
    bits = bitstr[4:]
    return [(bool(int(bits[i*4])), int(bits[i*4+1:i*4+4], 2)) for i in range(6)]

def fix_tupledata(tpl_list: List[List[Tuple[bool, int]]], 
                  move: List[str]) -> List[List[Tuple[bool, int]]]:
    """
    タプルデータリストを受け取り，move情報に基づいて修正を行う関数.\n
    例えば，階層を変更したが，接地しない場合に対応する.\n
    """
    if len(tpl_list) != len(move):
        raise ValueError("タプルデータリストとmove情報の長さが一致しません." +
                         f"{len(tpl_list)=}, {len(move)=}")
    
    fixed_list = [list(tpl) for tpl in tpl_list]

    for i in range(len(tpl_list)):
        current_tpl = tpl_list[i]
        current_move = move[i].replace(" ", "")
        if len(current_tpl) != 6:
            raise ValueError(f"要素数が6ではありません. index={i}, len={len(current_tpl)}")
        if current_move == "kComUpDown":
            # tupleのboolがfalseかつ，intが4出ない場合, 4に修正.
            # さらに，次とその前の状態も同様に修正.
            for j in range(6):
                leg_bool, leg_int = current_tpl[j]
                if not leg_bool and leg_int != 4:
                    fixed_list[i-1][j] = (leg_bool, 4)
                    fixed_list[i][j] = (leg_bool, 4)
                    fixed_list[i+1][j] = (leg_bool, 4)

    return fixed_list

# def bool_list_to_2bit_int(bools: List[bool]) -> int:
#     # boolのリストを2進数として整数に変換
#     res = 0
#     for ind, b in enumerate(bools):
#         res += int(b) << (5 - ind)
#     return res

def bool_list_to_leg_ground_int(bools: List[int]) -> int:
    # boolのリストを文字列化して，脚接地パターン整数に変換
    bitstr = ''.join(['1' if b else '0' for b in bools])
    return leg_ground_map(bitstr)

def convert_base7(arr: List[int]) -> int:
    """
    arr: 長さ6の配列（要素は1～7）
    1. 全要素を -1 する
    2. 6桁の7進数として解釈
    3. 10進数整数に変換して返す
    """

    if len(arr) != 6:
        raise ValueError("配列の長さが6ではありません")

    # -1 した値を7進の桁に並べる
    digits = [x - 1 for x in arr]  # 0〜6 になる

    if any(d < 0 or d > 6 for d in digits):
        raise ValueError("配列の要素は1から7の範囲である必要があります")

    # 7進 → 10進変換
    num = 0
    for d in digits:
        num = num * 7 + d

    return num

def convert_hierarchy_str(arr: List[int]) -> str:
    """
    arr: 長さ6の配列（要素は1～7）
    戻り値: "n1-n2-n3-n4-n5-n6" の形式の文字列
    """

    if len(arr) != 6:
        raise ValueError("配列の長さが6ではありません")

    return '-'.join(str(x) for x in arr)

def command_to_vec(cmd: int, index: int = 0) -> Tuple[int, int]:
    """
    単一コマンドを (前後, 上下) の2次元ベクトルへ変換
    """
    base_val = 1000

    # --- 前後方向 ---
    if 1 <= cmd <= 3:
        x = -base_val - index  # 後ろへ
    elif cmd == 4:
        x = 0    # その場
    else:
        x = base_val + index    # 前へ

    # --- 上下方向 ---
    if cmd in (1, 5):
        y = -base_val - index   # 下げる
    elif cmd in (3, 7):
        y = base_val + index    # 上げる
    else:
        y = 0    # 変化なし

    return (x, y)

def state_to_vector(commands: List[int]) -> List[int]:
    """
    commands: 長さ6のリスト [int, int, ...]
    戻り値：意味に基づいた 12 次元のリスト
    """
    if len(commands) != 6:
        raise ValueError(f"引数の配列は長さ6である必要があります。{len(commands)}")

    vec : List[int] = []
    for cmd in commands:
        x, y = command_to_vec(cmd)
        vec.extend([x, y])

    return vec

def state_to_scalar(commands: List[int], weight_y: float =0.1) -> int:
    """
    commands: 長さ6のリスト
    戻り値：意味を保つ1次元特徴量
    
    1次元値 = sum(x_i) + weight_y * sum(y_i)
    """

    if len(commands) != 6:
        raise ValueError("引数の配列は長さ6である必要があります。")

    xs: List[int] = []
    ys: List[int] = []
    cnt = 0
    for cmd in commands:
        x, y = command_to_vec(cmd, cnt)
        xs.append(x)
        ys.append(y)
        cnt += 1

    # 重み付き1次元特徴量
    return int(sum(xs) + weight_y * sum(ys))

def bool_int_list_to_int(bit_list: List[Tuple[bool, int]], raw: bool = True) -> Tuple[int, int]:
    """
    boolとintのタプルリストを (遊脚の組み合わせ, 階層) の2つの整数に変換 \n
    [(T,4),(T,4), ...] -> (遊脚の組み合わせ:int, 階層:int)
    """
    bools : List[int] = [int(b) for b, _ in bit_list]
    ints : List[int] = [n for _, n in bit_list]
    if raw:
        return (bool_list_to_leg_ground_int(bools), convert_base7(ints))
    else:
        return (bool_list_to_leg_ground_int(bools), state_to_scalar(ints))

def tupledata_to_simple_str(tpl_list: List[Tuple[bool, int]]) -> str:
    """
    (bool, int)のタプルリストを簡易文字列に変換 \n
    [(true,4),(false,3), ...] -> "[TF..., 43...]"
    """
    if len(tpl_list) != 6:
        raise ValueError("タプルリストの長さが6ではありません.")

    bool_str = ''.join(['T' if b else 'F' for b, _ in tpl_list])
    int_str = ''.join([str(n) for _, n in tpl_list])
    return f"[{bool_str}, {int_str}]"

def tuple_list_reverse(tpl_list: List[Tuple[bool, int]]) -> List[Tuple[bool, int]]:
    # (bool, int)のタプルリストを逆順にする
    return list(reversed(tpl_list))

#10進数のint -> 7進数の文字列
def int_to_base7_str(num: str) -> str:
    num_ = int(num)
    if num_ < 0:
        raise ValueError("負の数は変換できません")

    if num_ == 0:
        return "0"

    digits : List[str] = []
    while num_:
        digits.append(str(num_ % 7 + 1))
        num_ //= 7

    return ''.join(reversed(digits))

if __name__ == "__main__":
    print("ここでは実行されません")
