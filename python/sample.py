import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import util

def plot_3d_pairs_alternate_color(pairs):
    xs = list(range(len(pairs)))
    ys = [p[0] for p in pairs]
    zs = [p[1] for p in pairs]

    fig = plt.figure(figsize=(16, 12))
    ax = fig.add_subplot(111, projection='3d')

    # 点の描画
    ax.scatter(xs, ys, zs)

    # 線を1区間ずつ描く（色を交互）
    colors = ["red", "blue"]
    for i in range(len(xs) - 1):
        ax.plot(
            xs[i:i+2],
            ys[i:i+2],
            zs[i:i+2],
            color=colors[i % 2]
        )

    ax.set_xlabel("Index")
    ax.set_ylabel("Combination of Free Legs")
    ax.set_zlabel("Hierarchy/Scalar Value")

    ax.view_init(elev=20, azim=-30)
    plt.savefig("3d_plot.png",dpi=300)

    plt.show()
    return

def plot_2d_pairs_alternate_color(pairs, plot_type=("x", "y"), only_save=False):
    xs = list(range(len(pairs)))
    ys = [p[0] for p in pairs]
    zs = [p[1] for p in pairs]

    fig, ax = plt.subplots(figsize=(16, 8))

    # 線を1区間ずつ描く（色を交互）
    colors = ["red", "blue"]
    for i in range(len(xs) - 1):
        # plot_typeによって描画方法を変更
        if plot_type == ("x", "y"):
            ax.plot(
                xs[i:i+2],
                ys[i:i+2],
                color=colors[i % 2]
            )
            ax.scatter(
                xs[i:i+2],
                ys[i:i+2],
                color="cyan"
            )
            ax.set_xlabel("Index")
            ax.set_ylabel("Combination of Free Legs")
        elif plot_type == ("x", "z"):
            ax.plot(
                xs[i:i+2],
                zs[i:i+2],
                color=colors[i % 2]
            )
            ax.scatter(
                xs[i:i+2],
                zs[i:i+2],
                color="cyan"
            )
            ax.set_xlabel("Index")
            ax.set_ylabel("Hierarchy/Scalar Value")
        else:
            ax.plot(
                ys[i:i+2],
                zs[i:i+2],
                color=colors[i % 2]
            )
            ax.scatter(
                ys[i:i+2],
                zs[i:i+2],
                color="cyan"
            )
            ax.set_xlabel("Combination of Free Legs")
            ax.set_ylabel("Hierarchy/Scalar Value")

    plt.savefig("2d_plot.png",dpi=300)

    plt.show()
    return

file_path1 = "long_flat_20251016_1308_53/node_list1.csv"
df = pd.read_csv(file_path1, header=None)

res = util.hierarcy_data_from_csv(df)
plot_3d_pairs_alternate_color([util.bool_int_list_to_int(util.bitstr_to_bool_int_list(r)) for r in res]) 

exit()
