import matplotlib.pyplot as plt
from matplotlib.axes import Axes
import os
import util
import file_io
from typing import List, Tuple, Dict, Set
from collections import defaultdict

StateKey = Tuple[Tuple[bool, int], ...]
EdgeKey = Tuple[StateKey, StateKey]

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


def _tupledata_to_state_key(tupledata: List[Tuple[bool, int]]) -> StateKey:
    return tuple(tupledata)


def _state_key_to_label(state: StateKey) -> str:
    return util.tupledata_to_simple_str(list(state))


def _filter_files_by_base_dir(file_list: List[str], base_dir: str = "result") -> List[str]:
    """
    file_list から base_dir 配下のファイルのみを抽出する.
    例: base_dir="result" の場合，result_ は除外される.
    """
    base_name = os.path.normpath(base_dir)
    filtered: List[str] = []

    for file_path in file_list:
        normalized = os.path.normpath(file_path)
        parts = normalized.split(os.sep)
        if base_name in parts:
            filtered.append(file_path)

    return filtered


def count_state_and_transition_frequencies(
    file_list: List[str],
) -> Tuple[Dict[StateKey, int], Dict[EdgeKey, int]]:
    """
    状態の出現頻度と，隣接状態間の遷移頻度を集計する.
    """
    state_counts: Dict[StateKey, int] = defaultdict(int)
    edge_counts: Dict[EdgeKey, int] = defaultdict(int)

    for index, file_path in enumerate(file_list):
        print(f"Processing file {index}: {file_path}")
        data = file_io.read_csv_file(file_path)
        states = util.make_tupledata_list_from_csv(data)

        state_keys = [_tupledata_to_state_key(state) for state in states]
        for state in state_keys:
            state_counts[state] += 1

        for j in range(len(state_keys) - 1):
            edge_counts[(state_keys[j], state_keys[j + 1])] += 1

    return state_counts, edge_counts


def plot_state_transition_network(
    file_list: List[str],
    state_count_threshold: int = 1,
    edge_count_threshold: int = 0,
    layout: str = "spring",
    show_edge_labels: bool = True,
    show_plot: bool = True,
    file_name: str = "state_transition_network.png",
):
    """
    状態遷移を NetworkX の有向グラフとして可視化する.

    state_count_threshold 以下のノードは非表示.
    edge_count_threshold 以下のエッジは非表示.
    """
    if state_count_threshold < 0:
        raise ValueError("state_count_threshold は 0 以上で指定してください.")
    if edge_count_threshold < 0:
        raise ValueError("edge_count_threshold は 0 以上で指定してください.")

    try:
        import networkx as nx
    except ModuleNotFoundError as exc:
        raise ModuleNotFoundError(
            "networkx が必要です. `pip install networkx` を実行してください."
        ) from exc

    state_counts, edge_counts = count_state_and_transition_frequencies(file_list)

    visible_states: Set[StateKey] = {
        state for state, count in state_counts.items()
        if count > state_count_threshold
    }

    graph = nx.DiGraph()
    state_to_label: Dict[StateKey, str] = {}

    for state in visible_states:
        label = _state_key_to_label(state)
        state_to_label[state] = label
        leg_pattern = util.bool_list_to_leg_ground_int([int(value) for value, _ in state])
        graph.add_node(label, count=state_counts[state], leg_pattern=leg_pattern)

    for (from_state, to_state), count in edge_counts.items():
        if count <= edge_count_threshold:
            continue
        if from_state not in visible_states or to_state not in visible_states:
            continue
        graph.add_edge(state_to_label[from_state], state_to_label[to_state], weight=count)

    print(f"Total unique states: {len(state_counts)}")
    print(f"Visible states (> {state_count_threshold}): {graph.number_of_nodes()}")
    print(f"Visible edges (> {edge_count_threshold}): {graph.number_of_edges()}")

    if graph.number_of_nodes() == 0:
        print("表示対象のノードがありませんでした. 閾値を下げてください.")
        return graph

    if layout == "spring":
        pos = nx.spring_layout(graph, k=1.0, seed=0)
    elif layout == "circular":
        pos = nx.circular_layout(graph)
    elif layout == "kamada_kawai":
        pos = nx.kamada_kawai_layout(graph)
    else:
        pos = nx.spectral_layout(graph)

    node_counts = [graph.nodes[node]["count"] for node in graph.nodes()]
    max_node_count = max(node_counts) if node_counts else 1
    node_sizes = [
        300 + 1700 * (graph.nodes[node]["count"] / max_node_count)
        for node in graph.nodes()
    ]
    node_colors = [
        util.get_color_by_leg_ground(graph.nodes[node]["leg_pattern"])
        for node in graph.nodes()
    ]

    edge_weights = [graph[u][v]["weight"] for u, v in graph.edges()]
    max_edge_weight = max(edge_weights) if edge_weights else 1
    edge_widths = [
        0.5 + 4.5 * (graph[u][v]["weight"] / max_edge_weight)
        for u, v in graph.edges()
    ]

    plt.figure(figsize=(20, 14))  # type: ignore
    nx.draw(
        graph,
        pos,
        with_labels=True,
        node_size=node_sizes,
        node_color=node_colors,
        edge_color="gray",
        width=edge_widths,
        arrows=True,
        font_size=8,
    )

    if show_edge_labels and graph.number_of_edges() <= 250:
        edge_labels = {(u, v): graph[u][v]["weight"] for u, v in graph.edges()}
        nx.draw_networkx_edge_labels(graph, pos, edge_labels=edge_labels, font_size=7)

    plt.title(
        f"State Transition Network (state > {state_count_threshold}, edge > {edge_count_threshold})"
    )  # type: ignore
    plt.subplots_adjust(left=0.02, right=0.98, top=0.94, bottom=0.06)  # type: ignore
    plt.savefig(file_name, dpi=300)  # type: ignore

    if show_plot:
        plt.show()  # type: ignore

    return graph

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

def main5(
    file_list: List[str],
    raw: bool = True,
    show_plot: bool = True,
    name: str = "no_name"
) -> None:
    """
    main3 をベースに，
    ・横線：赤
    ・縦線：青
    ・斜め線は描画しない
    ・出現頻度に応じて濃度(alpha)を変更
    """

    from collections import defaultdict

    # 横線: (x1, x2, y) -> count
    horiz_edges: Dict[Tuple[int, int, int], int] = defaultdict(int)
    # 縦線: (y1, y2, x) -> count
    vert_edges: Dict[Tuple[int, int, int], int] = defaultdict(int)

    # ===== 1. 出現頻度カウント =====
    for i, df in enumerate(file_list):
        print(f"Processing file {i}: {df}")
        data = file_io.read_csv_file(df)
        res = util.make_tupledata_list_from_csv(data)
        pairs = [util.bool_int_list_to_int(r, raw) for r in res]

        for j in range(len(pairs) - 1):
            (x1, y1) = pairs[j]
            (x2, y2) = pairs[j + 1]

            # 横線
            if y1 == y2 and x1 != x2:
                horiz_edges[(x1, x2, y1)] += 1

            # 縦線
            elif x1 == x2 and y1 != y2:
                vert_edges[(y1, y2, x1)] += 1

            # 斜め線は無視

    max_h = max(horiz_edges.values()) if horiz_edges else 1
    max_v = max(vert_edges.values()) if vert_edges else 1

    print(f"Horizontal edges: {len(horiz_edges)}, max freq: {max_h}")
    print(f"Vertical edges: {len(vert_edges)}, max freq: {max_v}")

    # ===== 2. プロット =====
    fig, ax = plt.subplots(figsize=(16, 8))  # type: ignore

    # 横線（赤）
    for (x1, x2, y), count in horiz_edges.items():
        alpha = count / max_h * 0.9 + 0.1
        ax.plot(
            [x1, x2],
            [y, y],
            color="red",
            alpha=alpha,
            linewidth=1.5
        )  # type: ignore

    # 縦線（青）
    for (y1, y2, x), count in vert_edges.items():
        alpha = count / max_v * 0.9 + 0.1
        ax.plot(
            [x, x],
            [y1, y2],
            color="blue",
            alpha=alpha,
            linewidth=1.5
        )  # type: ignore

    # ===== 3. 軸設定 =====
    ax.set_xlabel(_get_label_name("y"))  # type: ignore
    ax.set_ylabel(_get_label_name("z"))  # type: ignore

    ax.xaxis.grid(True)  # type: ignore
    ax.xaxis.set_major_locator(plt.MultipleLocator(1))  # type: ignore
    ax.set_xlim(-1, 36)

    if raw:
        ax.set_ylim(-1, 7 ** 6 + 1)
    else:
        ax.set_ylim(-5000, 5000)

    plt.savefig(
        f"2d_frequency_axis_only_{name}_{'raw' if raw else 'scalar'}.png",
        dpi=300
    )  # type: ignore

    if show_plot:
        plt.show()  # type: ignore


def main6(
    file_list: List[str],
    state_count_threshold: int = 1,
    edge_count_threshold: int = 0,
    layout: str = "spring",
    show_plot: bool = True,
    show_edge_labels: bool = True,
    target_base_dir: str = "result",
    name: str = "no_name",
) -> None:
    """
    状態間遷移を NetworkX で可視化する.

    state_count_threshold 以下の出現回数の状態は表示しない.
    """
    if not file_list:
        print("file_list が空です")
        return

    target_files = _filter_files_by_base_dir(file_list, base_dir=target_base_dir)
    if not target_files:
        print(f"{target_base_dir} 配下に対象ファイルがありませんでした")
        return

    print("Number of input files:", len(file_list))
    print(f"Number of target files in '{target_base_dir}':", len(target_files))
    print("state_count_threshold:", state_count_threshold)
    print("edge_count_threshold:", edge_count_threshold)

    plot_state_transition_network(
        file_list=target_files,
        state_count_threshold=state_count_threshold,
        edge_count_threshold=edge_count_threshold,
        layout=layout,
        show_edge_labels=show_edge_labels,
        show_plot=show_plot,
        file_name=f"state_transition_network_{name}.png",
    )

if __name__ == "__main__":
    # main1()

    # main2()

    # file = file_io.get_file_paths("node_list1.csv")
    # print("Number of CSV files read:", len(file))
    # filtered_files = [f for f in file if "rough" in f] 
    # main3(filtered_files, raw=True, show_plot=True, name="node_list1")

    file = file_io.get_file_paths("node_list1.csv")
    print("Number of CSV files read:", len(file))
    filtered_files = [f for f in file if "normal" in f] 
    main4(filtered_files, raw=True, show_plot=True, shrink=True, name="node_list1")

    # file = file_io.get_file_paths("node_list1.csv")
    # print("Number of CSV files read:", len(file))
    # filtered_files = [f for f in file if "rough" in f] 
    # main5(filtered_files, raw=True, show_plot=True, name="node_list1")

    # file = file_io.get_file_paths("node_list1.csv")
    # filtered_files = [f for f in file if "rough" in f]
    # main6(filtered_files, state_count_threshold=150, edge_count_threshold=2,
    #       layout="spring", show_plot=True, show_edge_labels=False, name="node_list1")
