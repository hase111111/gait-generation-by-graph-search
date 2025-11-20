import numpy as np

def leg_ground_map(str):
    if str =="010101":
        return 0
    elif str =="101010":
        return 1
    elif str =="111111":
        return 2
    elif str =="011111":
        return 3
    elif str =="101111":
        return 4
    elif str =="110111":
        return 5
    elif str =="111011":
        return 6
    elif str =="111101":
        return 7
    elif str =="111110":
        return 8
    elif str =="001111":
        return 9
    elif str =="010111":
        return 10
    elif str =="011011":
        return 11
    elif str =="011101":
        return 12
    elif str =="011110":
        return 13
    elif str =="100111":
        return 14
    elif str =="101011":
        return 15
    elif str =="101101":
        return 16
    elif str =="101110":
        return 17
    elif str =="110011":
        return 18
    elif str =="110101":
        return 19
    elif str =="110110":
        return 20
    elif str =="111001":
        return 21
    elif str =="111010":
        return 22
    elif str =="111100":
        return 23
    elif str =="001011":
        return 24
    elif str =="001101":
        return 25
    elif str =="010011":
        return 26
    elif str =="010110":
        return 27
    elif str =="011001":
        return 28
    elif str =="011010":
        return 29
    elif str =="100101":
        return 30
    elif str =="100110":
        return 31
    elif str =="101001":
        return 32
    elif str =="101100":
        return 33
    elif str =="110010":
        return 34
    elif str =="110100":
        return 35
    else:
        raise ValueError(f"脚接地パターンが不正です: {str}")
  
def get_color_by_leg_ground(leg_ground_int):
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

def hierarcy_data_from_csv(data): 
    # for文で，各行のデータを処理
    hierarcy_data = []
    past_move = "none"

    for idx, row in data.iterrows():
        # A列目とAS列目のデータを取得
        bit = row[0]
        move = row[44]

        if past_move == "kLegHierarchyChange":
            hierarcy_data.append(bit)
        elif past_move == "kLegUpDown":
            hierarcy_data.append(bit)

        past_move = move.replace(" ", "")

    return hierarcy_data

def bitstr_to_bool_int_list(bitstr):
    # 上位4bitを無視し，下位24bitを6組の(bool, int)に変換
    bits = bitstr[4:]
    return [(bool(int(bits[i*4])), int(bits[i*4+1:i*4+4], 2)) for i in range(6)]

def bool_list_to_2bit_int(bools):
    # boolのリストを2進数として整数に変換
    res = 0
    for ind, b in enumerate(bools):
        res += int(b) << (5 - ind)
    return res

def bool_list_to_leg_ground_int(bools):
    # boolのリストを文字列化して，脚接地パターン整数に変換
    bitstr = ''.join(['1' if b else '0' for b in bools])
    return leg_ground_map(bitstr)

def convert_base7(arr):
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

def convert_hierarchy_str(arr):
    """
    arr: 長さ6の配列（要素は1～7）
    戻り値: "n1-n2-n3-n4-n5-n6" の形式の文字列
    """

    if len(arr) != 6:
        raise ValueError("配列の長さが6ではありません")

    return '-'.join(str(x) for x in arr)

def command_to_vec(cmd, index=0):
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

def state_to_vector(commands):
    """
    commands: 長さ6のリスト [int, int, ...]
    戻り値：意味に基づいた 12 次元のリスト
    """
    if len(commands) != 6:
        raise ValueError(f"引数の配列は長さ6である必要があります。{len(commands)}")

    vec = []
    for cmd in commands:
        x, y = command_to_vec(cmd)
        vec.extend([x, y])

    return vec

def state_to_scalar(commands, weight_y=0.1):
    """
    commands: 長さ6のリスト
    戻り値：意味を保つ1次元特徴量
    
    1次元値 = sum(x_i) + weight_y * sum(y_i)
    """

    if len(commands) != 6:
        raise ValueError("引数の配列は長さ6である必要があります。")

    xs = []
    ys = []
    cnt = 0
    for cmd in commands:
        x, y = command_to_vec(cmd, cnt)
        xs.append(x)
        ys.append(y)
        cnt += 1

    # 重み付き1次元特徴量
    return sum(xs) + weight_y * sum(ys)

def bool_int_list_to_int(bit_list, raw=True):
    # (bool, int)のリストをmapする2つの数字に変換
    bools = [int(b) for b, _ in bit_list]
    ints = [n for _, n in bit_list]
    if raw:
        return (bool_list_to_leg_ground_int(bools), convert_base7(ints))
    else:
        return (bool_list_to_leg_ground_int(bools), state_to_scalar(ints))

def tuple_list_to_simple_str(tpl_list):
    # (bool, int)のタプルリストを簡易文字列に変換
    str_parts = []
    for b, n in tpl_list:
        b_char = 'T' if b else 'F'
        str_parts.append(f"{b_char}{n}")
    return "[" + ",".join(str_parts) + "]"

def tuple_list_reverse(tpl_list):
    # (bool, int)のタプルリストを逆順にする
    return list(reversed(tpl_list))

if __name__ == "__main__":
    print("ここでは実行されません")
