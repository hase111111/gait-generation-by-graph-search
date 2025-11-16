import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# === 1. データ読み込み ===
# CSVファイルのパスを指定
file_path1 = "long_flat_20251016_1308_53/node_list1.csv"
file_path2 = "long_diagonal_20251016_1032_51/node_list1.csv"
file_path3 = "long_horizontal_20251016_1026_57/node_list1.csv"
file_path4 = "long_mesh_20251016_1036_31/node_list1.csv"   # あなたのCSVファイルへのパス
file_path5 = "long_random_60_20251016_1052_25/node_list1.csv"   # あなたのCSVファイルへのパス
file_path6 = "long_vertical_20251016_1024_08/node_list1.csv"   # あなたのCSVファイルへの
file_path7 = "long_step30_20251023_0944_01/node_list1.csv"   # あなたのCSVファイルへのパス
file_path = "node_list1.csv"  # 必要に応じて変更
df = pd.read_csv(file_path1, header=None)

# === 2. 28bitデータのデコード ===
def decode_28bit(bitstr):
    """
    28bitのbit列から6組の(bool, int)を復元する関数
    - 先頭4bitを無視
    - 残りを6つの4bitブロックに分ける
      -> 先頭1bit: bool
      -> 残り3bit: int(0~7)
    """
    bits = bitstr[4:]
    return [(bool(int(bits[i*4])), int(bits[i*4+1:i*4+4], 2)) for i in range(6)]

# === 3. 階層番号と階層内位置の定義 ===
def layer_index(bools):
    """
    6脚のbool(接地/遊脚)構成を基に階層番号(0~35)を定義
    """
    return int(''.join(str(int(b)) for b in bools), 2) % 36

def int_to_base7(ints):
    """
    6脚の位置(0~6)を7進数として連結し、階層内の一意な位置値に変換
    """
    value = 0
    for n in ints:
        value = value * 7 + n
    return value

# === 4. 各行の状態を抽出 ===
layers = []
positions = []

for s in df.iloc[:, 0]:
    decoded = decode_28bit(s)
    bools = [b for b, _ in decoded]
    ints = [n for _, n in decoded]
    layers.append(layer_index(bools))
    positions.append(int_to_base7(ints))

time = np.arange(len(layers))

# === 5. 移動タイプの分類 ===
# 0:階層内移動 (intのみ変化)
# 1:階層移動   (boolが変化)
movement_type = [1 if layers[i] != layers[i-1] else 0 for i in range(1, len(layers))]

# === 6. 3Dプロット ===
fig = plt.figure(figsize=(12, 6))
ax = fig.add_subplot(111, projection='3d')

for i in range(1, len(layers)):
    color = 'red' if movement_type[i-1] == 1 else 'blue'
    ax.plot(time[i-1:i+1], layers[i-1:i+1], positions[i-1:i+1], color=color, lw=1)

ax.set_xlabel('Time (rows)')
ax.set_ylabel('Layer (bool-based)')
ax.set_zlabel('In-layer position (base-7 combined)')
ax.set_title('3D Hierarchical State Transitions\nRed: Layer change, Blue: In-layer move')
plt.tight_layout()
plt.show()
