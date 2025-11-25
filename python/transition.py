import matplotlib.pyplot as plt
from matplotlib.axes import Axes
import util
import file_io
from typing import List, Tuple, Dict, Set
from collections import defaultdict

def _get_label_name(label : str = "x") -> str:
    """
    座標軸のラベル名を取得する関数. \n
    label: "x", "y", "z" のいずれか.
    """
    if label == "x":
        return "Index"
    elif label == "y":
        return "Combination of Free Legs"
    elif label == "z":
        return "Hierarchy/Scalar Value"
    else:
        raise ValueError("Invalid label name")

def plot_3d_pairs_alternate_color(pairs : List[Tuple[int, int]]) -> None:
    """
    3Dプロットを作成する関数. \n
    pairs: (階層:int, 遊脚の組み合わせ:int) のタプルのリスト.
    """
    xs = list(range(len(pairs)))
    ys = [p[0] for p in pairs]
    zs = [p[1] for p in pairs]

    fig = plt.figure(figsize=(16, 12))  #type: ignore
    ax = fig.add_subplot(111, projection='3d')  #type: ignore

    # 点の描画.
    ax.scatter(xs, ys, zs)  #type: ignore

    # 線を1区間ずつ描く (色を交互).
    colors = ["red", "blue"]
    for i in range(len(xs) - 1):
        ax.plot(  #type: ignore
            xs[i:i+2],
            ys[i:i+2],
            zs[i:i+2],
            color=colors[i % 2]
        )

    ax.set_xlabel("Index")  #type: ignore
    ax.set_ylabel("Combination of Free Legs")  #type: ignore
    ax.set_zlabel("Hierarchy/Scalar Value")  #type: ignore

    ax.view_init(elev=20, azim=-30)  #type: ignore
    plt.savefig("3d_plot.png",dpi=300)  #type: ignore

    plt.show()  #type: ignore

def _plot_ax_2d(ax : Axes, pairs : List[Tuple[int, int]], 
                color1 : str ="red", color2 : str ="blue", 
                plot_type : Tuple[str, str]=("x", "y"), 
                draw_scatter : bool =True) -> None:
    """
    2Dプロットを描画する関数. \n
    ax: 描画先のAxesオブジェクト. \n
    pairs: (遊脚の組み合わせ:int, 階層:int) のタプルのリスト \n
    """
    if plot_type[0] == "x":
        xs = list(range(len(pairs)))
    elif plot_type[0] == "y":
        xs = [p[0] for p in pairs]
    elif plot_type[0] == "z":
        xs = [p[1] for p in pairs]
    else:
        raise ValueError(f"Invalid plot_type[0] value : {plot_type[0]=}")

    if plot_type[1] == "x":
        ys = list(range(len(pairs)))
    elif plot_type[1] == "y":
        ys = [p[0] for p in pairs]
    elif plot_type[1] == "z":
        ys = [p[1] for p in pairs]
    else:
        raise ValueError(f"Invalid plot_type[1] value : {plot_type[1]=}")

    colors = [color1, color2]
    for i in range(len(xs) - 1):  # 線を1区間ずつ描く (色を交互).
        ax.plot(  # type: ignore
            xs[i:i+2],
            ys[i:i+2],
            color=colors[i % 2]
        )

        if draw_scatter:
            ax.scatter(  # type: ignore
                xs[i:i+2],
                ys[i:i+2],
                color="cyan"
            )

def plot_2d_pairs_alternate_color(
        pairs: List[Tuple[int, int]], 
        color1: str ="red", color2: str ="blue", 
        plot_type: Tuple[str, str]=("x", "y"), 
        draw_scatter: bool =True, only_save: bool =False, 
        file_name: str ="2d_plot.png"):
    fig, ax = plt.subplots(figsize=(16, 8))  # type: ignore

    _plot_ax_2d(ax, pairs, color1, color2, plot_type, draw_scatter)

    ax.set_xlabel(_get_label_name(plot_type[0]))  # type: ignore
    ax.set_ylabel(_get_label_name(plot_type[1]))  # type: ignore

    if plot_type[0] == "y":
        # 目盛間隔は1に設定, 範囲は0~35
        ax.xaxis.grid(True)  # type: ignore
        ax.xaxis.set_major_locator(plt.MultipleLocator(1))  # type: ignore
        ax.set_xlim(-1, 36)

    plt.savefig(file_name,dpi=300)  # type: ignore

    if only_save:
        plt.close()
    else:
        plt.show()  # type: ignore

def main1():
    """
    node_list1.csv を読み込み，一番目のCSVデータを使ってプロットを作成する関数.
    """
    csvs = file_io.read_all_csv_files("node_list1.csv")

    print("Number of CSV files read:", len(csvs))
    print(f"Number of rows in first CSV: {len(csvs[0])}")

    res = util.make_tupledata_list_from_csv(csvs[0])

    # # 生データを表示.
    # for idx, r in enumerate(res):
    #     str1 = util.tupledata_to_simple_str(r)
    #     print(f"Index: {idx}, str {str1}")

    plot_2d_pairs_alternate_color([util.bool_int_list_to_int(r) for r in res],
                                  plot_type=("y", "z"))


def main2():
    """
    複数のCSVファイルを読み込み，2Dプロットを1つにまとめて描画する関数. \n
    色をファイル名ごとに変える.
    """
    file = file_io.get_file_paths("node_list1.csv")
    print("Number of CSV files read:", len(file))

    fig, ax = plt.subplots(figsize=(16, 8))  # type: ignore

    for i, df in enumerate(file):
        print(f"Processing file {i}: {df}")
        data = file_io.read_csv_file(df)
        res = util.make_bitdata_list_from_csv(data)
        pairs = [util.bool_int_list_to_int(util.bitdata_to_tupledata(r)) for r in res]

        file_name = file_io.get_prefix_before_first_underscore(file_io.get_upper_directory_name(df))

        # file_nameごとに異なる色でプロット
        color = file_io.generate_color_from_filename(file_name)
        _plot_ax_2d(ax, pairs, color1=color, color2=color, plot_type=("y", "z"), draw_scatter=False)

    ax.set_xlabel(_get_label_name("y"))  # type: ignore
    ax.set_ylabel(_get_label_name("z"))  # type: ignore

    ax.xaxis.grid(True)  # type: ignore
    ax.xaxis.set_major_locator(plt.MultipleLocator(1))  # type: ignore
    ax.set_xlim(-1, 36)  # 範囲は0~35
    ax.set_ylim(-1, 7 ** 6 + 1)  # 範囲は0~35
    
    plt.savefig("2d_combined_plot.png",dpi=300)  # type: ignore
    plt.show()  # type: ignore

def main3(file_list: List[str], raw: bool =True, show_plot: bool =True, name : str ="no_name") -> None:
    """
    複数のCSVファイルを読み込み，2Dプロットを1つにまとめて描画する関数. \n
    """
    fig, ax = plt.subplots(figsize=(16, 8))  # type: ignore

    for i, df in enumerate(file_list):
        print(f"Processing file {i}: {df}")
        data = file_io.read_csv_file(df)
        res = util.make_tupledata_list_from_csv(data)
        pairs = [util.bool_int_list_to_int(r, raw) for r in res]
        _plot_ax_2d(ax, pairs, color1="red", color2="blue", plot_type=("y", "z"), draw_scatter=False)

    ax.set_xlabel(_get_label_name("y"))  # type: ignore
    ax.set_ylabel(_get_label_name("z"))  # type: ignore

    ax.xaxis.grid(True)  # type: ignore
    ax.xaxis.set_major_locator(plt.MultipleLocator(1))  # type: ignore
    ax.set_xlim(-1, 36)  # 範囲は0~35
    if raw:
        ax.set_ylim(-1, 7 ** 6 + 1)  # 範囲は0~7^6
    else:
        ax.set_ylim(-5000, 5000) 
    
    plt.savefig(f"2d_combined_plot_{name}_{'raw' if raw else 'scalar'}.png",dpi=300)  # type: ignore

    if show_plot:
        plt.show()  # type: ignore

def main4(file_list: List[str], raw: bool =True, show_plot: bool =True, shrink: bool = False,name : str ="no_name") -> None:
    """
    複数のCSVファイルを読み込み，頻度分布を1つにまとめて描画する関数. \n
    """
    edge1: Dict[Tuple[int, int, int], int] = defaultdict(int)  # x1,x2,y
    edge2: Dict[Tuple[int, int, int], int] = defaultdict(int)  # y1,y2,x
    node_set: Dict[Tuple[int, int], int] = defaultdict(int)  # x,y
    hierarchy_set: Set[int] = set()
    hierarchy_dict: Dict[int, int] = defaultdict(int)
    leg_set: Set[int] = set()

    _file = file_io.get_file_paths("node_list1.csv")
    for df in _file:
        _data = file_io.read_csv_file(df)
        _res = util.make_tupledata_list_from_csv(_data)
        pairs = [util.bool_int_list_to_int(r, raw) for r in _res]
        for p in pairs:
            hierarchy_set.add(p[1])

    for i, df in enumerate(file_list):
        print(f"Processing file {i}: {df}")
        data = file_io.read_csv_file(df)
        res = util.make_tupledata_list_from_csv(data)
        pairs = [util.bool_int_list_to_int(r, raw) for r in res]

        switch_f = True
        for j in range(len(pairs) - 1):
            e1 = pairs[j]
            e2 = pairs[j + 1]
            node_set[e1] += 1
            leg_set.add(e1[0])
            leg_set.add(e2[0])
            hierarchy_set.add(e1[1])
            hierarchy_set.add(e2[1])
            hierarchy_dict[e1[1]] += 1
            if switch_f:
                key = (e1[0], e2[0], e1[1])
                edge1[key] += 1
            else:
                key = (e1[1], e2[1], e1[0])
                edge2[key] += 1

            switch_f = not switch_f

    # 値の最大値をそれぞれ取得.
    max_value1 = max(edge1.values()) if edge1 else 1
    max_value2 = max(edge2.values()) if edge2 else 1
    max_node_count = max(node_set.values()) if node_set else 1
    max_hierarchy_count = max(hierarchy_dict.values()) if hierarchy_dict else 1
    print(f"Max edge1 count: {max_value1}, Max edge2 count: {max_value2}, Max node count: {max_node_count}")
    print(f"Total unique edges1: {len(edge1)}, Total unique edges2: {len(edge2)}, Total unique nodes: {len(node_set)}")
    print(f"Max hierarchy occurrence count: {max_hierarchy_count}")
    print(f"Total unique legs: {len(leg_set)}, Total unique hierarchies: {len(hierarchy_set)}")

    # シュリンク処理.
    v_to_shrink : Dict[int, int] = dict()
    if shrink:
        sorted_hierarchies = sorted(hierarchy_set)
        for new_idx, old_value in enumerate(sorted_hierarchies):
            v_to_shrink[old_value] = new_idx

        # edge1のシュリンク
        new_edge1: Dict[Tuple[int, int, int], int] = defaultdict(int)
        for (x1, x2, y), count in edge1.items():
            new_y = v_to_shrink[y]
            new_edge1[(x1, x2, new_y)] += count
        edge1 = new_edge1

        # edge2のシュリンク
        new_edge2: Dict[Tuple[int, int, int], int] = defaultdict(int)
        for (y1, y2, x), count in edge2.items():
            new_y1 = v_to_shrink[y1]
            new_y2 = v_to_shrink[y2]
            new_edge2[(new_y1, new_y2, x)] += count
        edge2 = new_edge2

        # node_setのシュリンク
        new_node_set: Dict[Tuple[int, int], int] = defaultdict(int)
        for (x, y), count in node_set.items():
            new_y = v_to_shrink[y]
            new_node_set[(x, new_y)] += count
        node_set = new_node_set

    # プロット作成. 横線は"red"，縦線は"blue"とし，頻度が高いほど濃くなるようにする.
    if shrink:
        plt.rcParams["ytick.labelsize"] = 6
    fig, ax = plt.subplots(figsize=(16, 8))  # type: ignore

    # 辺の描画.
    for (x1, x2, y), count in edge1.items():
        intensity = count / max_value1 * 0.9 + 0.1  # 0.1~1.0の範囲に正規化
        color = (1.0, 0.0, 0.0, intensity)  # 赤色のRGBA
        ax.plot([x1, x2], [y, y], color=color)  # type: ignore

    for (y1, y2, x), count in edge2.items():
        intensity = count / max_value2 * 0.9 + 0.1  # 0.1~1.0の範囲に正規化
        color = (0.0, 0.0, 1.0, intensity)
        ax.plot([x, x], [y1, y2], color=color)  # type: ignore

    # ノードの描画.
    for (x, y), count in node_set.items():
        if count < max_node_count * 0.001:
            continue
        ax.scatter(x, y, color="black", s=20,   # type: ignore
                   alpha=count / max_node_count * 0.9 + 0.1,
                   zorder=2)  

    ax.set_xlabel(_get_label_name("x"))  # type: ignore
    ax.set_ylabel(_get_label_name("y"))  # type: ignore
    ax.xaxis.grid(True)  # type: ignore
    ax.xaxis.set_major_locator(plt.MultipleLocator(1))  # type: ignore
    ax.set_xlim(-1, 36)  # 範囲は0~35
    if shrink:
        # シュリンク前の値に対応するラベルを設定.
        labels : Dict[int, str] = dict()
        for (old_value, new_idx) in v_to_shrink.items():
            # 頻度が少ないものは省略.
            if hierarchy_dict.get(old_value, 0) < max_hierarchy_count * 0.05:
                continue
            labels[new_idx] = str(old_value)
        ax.set_yticks(list(labels.keys()), labels=list(map(util.int_to_base7_str, labels.values())))  # type: ignore
        # 既存の目盛りラベルを取得.
        labels = ax.get_yticklabels()  # type: ignore

        for i, label in enumerate(labels):
            # 偶数番目のラベルだけ x 方向にずらす.
            if i % 2 == 0:
                label.set_x(0.0)  # type: ignore
            else:
                label.set_x(-0.02)  # type: ignore
    else:
        if raw :
            ax.set_ylim(-1, 7 ** 6 + 1)  # 範囲は0~7^6
        else:
            ax.set_ylim(-5000, 5000)
    plt.savefig(f"2d_frequency_plot_{name}_{'raw' if raw else 'scalar'}.png",dpi=300)  # type: ignore
    if show_plot:
        plt.show()  # type: ignore

if __name__ == "__main__":
    # main1()

    # main2()

    # file = file_io.get_file_paths("node_list1.csv")
    # print("Number of CSV files read:", len(file))
    # filtered_files = [f for f in file if "flat" in f] 
    # main3(filtered_files, raw=True, show_plot=True, name="node_list1")

    file = file_io.get_file_paths("node_list1.csv")
    print("Number of CSV files read:", len(file))
    filtered_files = [f for f in file if "" in f] 
    main4(filtered_files, raw=True, show_plot=True, shrink=True, name="node_list1")
