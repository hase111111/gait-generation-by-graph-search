import plotly.graph_objects as go
import numpy as np
import file_io
import util
import colorsys
from collections import Counter


def hsv_to_rgb_tuple(h, s=1.0, v=1.0):
    return colorsys.hsv_to_rgb(h, s, v)  # (r,g,b) 0〜1


def visualize_3d_layers(state_sequence):

    # --------------------
    # ノード／エッジ頻度
    # --------------------
    node_counts = Counter(state_sequence)

    edge_counts = Counter()
    for a, b in zip(state_sequence, state_sequence[1:]):
        edge_counts[(a, b)] += 1

    max_node_count = max(node_counts.values())
    max_edge_count = max(edge_counts.values())


    # --------------------
    # ノード座標（33点）
    # --------------------
    coords = {i: (np.cos(2*np.pi*i/33), np.sin(2*np.pi*i/33)) for i in range(33)}


    # --------------------
    # レイヤーz座標
    # --------------------
    layers = [layer for layer, node in state_sequence]
    min_layer, max_layer = min(layers), max(layers)
    all_layers = list(range(min_layer, max_layer + 1))

    z_pos = {layer: idx for idx, layer in enumerate(all_layers)}

    print(f"Total layers (including missing ones): {len(all_layers)}")
    print(f"z_pos mapping: {z_pos}")


    # --------------------
    # ノード（重複排除）
    # --------------------
    seen = set()
    nodes_x = []
    nodes_y = []
    nodes_z = []
    node_labels = []

    for layer, node in state_sequence:
        if (layer, node) in seen:
            continue
        seen.add((layer, node))

        x, y = coords[node]
        z = z_pos[layer]

        nodes_x.append(x)
        nodes_y.append(y)
        nodes_z.append(z)
        node_labels.append(f"{layer}:{node}")

    print("Nodes prepared.")


    # ==========================================================
    # ★ エッジ：透明度をビン分けしてまとめる（高速化の要）
    # ==========================================================

    # 透明度を 6 ビンに分割
    bins = np.linspace(0.1, 1.0, 6)  # 0.1, 0.28, 0.46, 0.64, 0.82, 1.0
    bin_data = {i: dict(x=[], y=[], z=[]) for i in range(len(bins)-1)}

    for a, b in zip(state_sequence, state_sequence[1:]):
        (layer1, node1), (layer2, node2) = a, b

        # エッジ透明度
        c = edge_counts[(a, b)]
        alpha = 0.1 + 0.9 * (c / max_edge_count)

        # 透明度をビンに分類
        idx = np.digitize(alpha, bins) - 1
        idx = min(max(idx, 0), len(bins)-2)

        # 座標
        x1, y1 = coords[node1]
        x2, y2 = coords[node2]
        z1 = z_pos[layer1]
        z2 = z_pos[layer2]

        # trace 用配列に蓄積
        bin_data[idx]["x"] += [x1, x2, None]
        bin_data[idx]["y"] += [y1, y2, None]
        bin_data[idx]["z"] += [z1, z2, None]

    # まとめて trace 化
    edge_traces = []
    for i in range(len(bins)-1):
        seg = bin_data[i]
        if len(seg["x"]) == 0:
            continue

        # 代表透明度
        alpha = (bins[i] + bins[i+1]) / 2
        color = f"rgba(0,0,0,{alpha})"

        trace = go.Scatter3d(
            x=seg["x"],
            y=seg["y"],
            z=seg["z"],
            mode="lines",
            line=dict(color=color, width=2),
            hoverinfo="none",
            showlegend=False
        )
        edge_traces.append(trace)

    print(f"Edge traces prepared: {len(edge_traces)} traces.")


    # ==========================================================
    # ★ ノード色：RGBA で透明度反映
    # ==========================================================
    layer_rgb = {}
    for layer in all_layers:
        hue = (hash(layer) % 360) / 360.0
        layer_rgb[layer] = hsv_to_rgb_tuple(hue)

    node_colors = []
    for (layer, node) in seen:
        r, g, b = layer_rgb[layer]
        r, g, b = int(r*255), int(g*255), int(b*255)

        # 出現頻度で透明度
        alpha = 0.1 + 0.9 * (node_counts[(layer, node)] / max_node_count)

        node_colors.append(f"rgba({r},{g},{b},{alpha})")

    node_trace = go.Scatter3d(
        x=nodes_x,
        y=nodes_y,
        z=nodes_z,
        mode="markers",
        marker=dict(
            size=5,
            color=node_colors
        ),
        text=node_labels,
        hoverinfo="text",
        showlegend=False
    )

    print("Node trace prepared.")


    # --------------------
    # Figure
    # --------------------
    fig = go.Figure(data=edge_traces + [node_trace])

    fig.update_layout(
        scene=dict(
            xaxis=dict(visible=False),
            yaxis=dict(visible=False),
            zaxis=dict(visible=False),
            aspectmode="cube"
        ),
        height=800,
        margin=dict(l=0, r=0, t=0, b=0)
    )

    print("Layout updated.")

    fig.show()


# ==========================================================
# データ読み込み部分（そのまま）
# ==========================================================
states_list = []
file = file_io.get_file_paths("node_list1.csv")

for f in file:
    if "flat" not in f:
        continue
    print(f"Processing file: {f}")
    data = file_io.read_csv_file(f)
    res = util.hierarcy_data_from_csv(data)
    pairs = [ (util.bool_int_list_to_int(util.bitstr_to_bool_int_list(r)), idx) for idx, r in enumerate(res) ]
    states = [(p[0][1], p[0][0]) for p in pairs]
    states_list.extend(states)

visualize_3d_layers(states_list)
