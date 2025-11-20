import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import util
import file_io

def _get_label_name(str="x"):
    if str == "x":
        return "Index"
    elif str == "y":
        return "Combination of Free Legs"
    elif str == "z":
        return "Hierarchy/Scalar Value"
    else:
        return ""

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

def _plot_ax_2d(ax, pairs, color1="red", color2="blue", plot_type=("x", "y"), draw_scatter=True):
    if plot_type[0] == "x":
        xs = list(range(len(pairs)))
    elif plot_type[0] == "y":
        xs = [p[0] for p in pairs]
    else:
        xs = [p[1] for p in pairs]

    if plot_type[1] == "x":
        ys = list(range(len(pairs)))
    elif plot_type[1] == "y":
        ys = [p[0] for p in pairs]
    else:
        ys = [p[1] for p in pairs]

    colors = [color1, color2]
    for i in range(len(xs) - 1):  # 線を1区間ずつ描く（色を交互）
        ax.plot(
            xs[i:i+2],
            ys[i:i+2],
            color=colors[i % 2]
        )

        if draw_scatter:
            ax.scatter(
                xs[i:i+2],
                ys[i:i+2],
                color="cyan"
            )

def plot_2d_pairs_alternate_color(pairs, color1="red", color2="blue", plot_type=("x", "y"), draw_scatter=True, only_save=False, file_name="2d_plot.png"):
    fig, ax = plt.subplots(figsize=(16, 8))

    _plot_ax_2d(ax, pairs, color1, color2, plot_type, draw_scatter)

    ax.set_xlabel(_get_label_name(plot_type[0]))
    ax.set_ylabel(_get_label_name(plot_type[1]))

    if plot_type[0] == "y":
        ax.xaxis.grid(True)
        ax.xaxis.set_major_locator(plt.MultipleLocator(1))  # 目盛間隔は1に設定
        # 範囲は0~35
        ax.set_xlim(-1, 36)

    plt.savefig(file_name,dpi=300)

    if only_save:
        plt.close()
    else:
        plt.show()

def main1():
    csvs = file_io.read_all_csv_files("node_list1.csv")
    print("Number of CSV files read:", len(csvs))

    combined_df = csvs[0]  # file_io.combine_csv_data(csvs)
    print("Combined DataFrame shape:", combined_df.shape)

    res = util.hierarcy_data_from_csv(combined_df)
    print("Result length:", len(res))

    plot_2d_pairs_alternate_color([util.bool_int_list_to_int(util.bitstr_to_bool_int_list(r)) for r in res], plot_type=("y", "z"))

def main2():
    file = file_io.get_file_paths("node_list1.csv")
    print("Number of CSV files read:", len(file))

    fig, ax = plt.subplots(figsize=(16, 8))

    for i, df in enumerate(file):
        print(f"Processing file {i}: {df}")
        data = file_io.read_csv_file(df)
        res = util.hierarcy_data_from_csv(data)
        pairs = [util.bool_int_list_to_int(util.bitstr_to_bool_int_list(r)) for r in res]

        file_name = file_io.get_prefix_before_first_underscore(file_io.get_upper_directory_name(df))

        # file_nameごとに異なる色でプロット
        color = file_io.generate_color_from_filename(file_name)
        _plot_ax_2d(ax, pairs, color1=color, color2=color, plot_type=("y", "z"), draw_scatter=False)

    ax.set_xlabel(_get_label_name("y"))
    ax.set_ylabel(_get_label_name("z"))

    ax.xaxis.grid(True)
    ax.xaxis.set_major_locator(plt.MultipleLocator(1))  # 目盛間隔は1に設定
    ax.set_xlim(-1, 36)  # 範囲は0~35
    ax.set_ylim(-1, 7 ** 6 + 1)  # 範囲は0~35
    
    plt.savefig("2d_combined_plot.png",dpi=300)
    plt.show()

def main3(filter_str, raw=True, show_plot=True):
    file = file_io.get_file_paths("node_list1.csv")
    print("Number of CSV files read:", len(file))

    fig, ax = plt.subplots(figsize=(16, 8))

    for i, df in enumerate(file):
        # # stepdown か stepupを含むものはスキップする
        # if "stepdown" in df or "stepup" in df:
        #     continue
        if filter_str not in df:
            continue
        print(f"Processing file {i}: {df}")
        data = file_io.read_csv_file(df)
        res = util.hierarcy_data_from_csv(data)
        pairs = [util.bool_int_list_to_int(util.bitstr_to_bool_int_list(r), raw) for r in res]

        file_name = file_io.get_prefix_before_first_underscore(file_io.get_upper_directory_name(df))

        # file_nameごとに異なる色でプロット.
        _plot_ax_2d(ax, pairs, color1="red", color2="blue", plot_type=("y", "z"), draw_scatter=False)

    ax.set_xlabel(_get_label_name("y"))
    ax.set_ylabel(_get_label_name("z"))

    ax.xaxis.grid(True)
    ax.xaxis.set_major_locator(plt.MultipleLocator(1))  # 目盛間隔は1に設定
    ax.set_xlim(-1, 36)  # 範囲は0~35
    if raw:
        ax.set_ylim(-1, 7 ** 6 + 1)  # 範囲は0~7^6
    else:
        ax.set_ylim(-5000, 5000) 
    
    plt.savefig(f"2d_combined_plot_{filter_str}_{'raw' if raw else 'scalar'}.png",dpi=300)

    if show_plot:
        plt.show()

if __name__ == "__main__":
    main3("", raw=True, show_plot=True)
