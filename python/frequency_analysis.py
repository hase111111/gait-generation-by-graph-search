from collections import Counter
import util
import pandas as pd
import networkx as nx
import matplotlib.pyplot as plt
import file_io
import util

def analyze_transition_pairs1(states):
    """
    states: [ [(int,bool)], [(int,bool)], ... ] のリスト
    2つの遷移を1セットとして頻度をカウントする.
    """
    pair_counter = Counter()

    # 2遷移 → 状態4つで1セット
    for i in range(len(states) - 3):
        s1 = tuple(states[i])
        s2 = tuple(states[i+1])
        s3 = tuple(states[i+2])
        s4 = tuple(states[i+3])
        
        # (S1→S2, S2→S3) を1まとまりとしてカウント
        pair_counter[((s1, s2), (s3, s4))] += 1

    # 反転して，同じことをする
    rev = util.tuple_list_reverse(states)
    for i in range(len(rev) - 3):
        s1 = tuple(rev[i])
        s2 = tuple(rev[i+1])
        s3 = tuple(rev[i+2])
        s4 = tuple(rev[i+3])
        
        pair_counter[((s1, s2), (s3, s4))] += 1

    return pair_counter

def analyze_transition_pairs2(states):
    """
    states: [ [(int,bool)], [(int,bool)], ... ] のリスト
    2つの遷移を1セットとして頻度をカウントする.
    """
    pair_counter = Counter()

    # 2遷移 → 状態３つで1セット
    for i in range(len(states) - 2):
        s1 = tuple(states[i])
        s2 = tuple(states[i+1])
        s3 = tuple(states[i+2])
        
        # (S1→S2, S2→S3) を1まとまりとしてカウント
        pair_counter[(s1, s3)] += 1

    # 反転して，同じことをする
    rev = util.tuple_list_reverse(states)
    for i in range(len(rev) - 2):
        s1 = tuple(rev[i])
        s2 = tuple(rev[i+1])
        s3 = tuple(rev[i+2])
        
        pair_counter[(s1, s3)] += 1

    return pair_counter

def count_stride_transitions(states, stride=2, start=0, use_reverse=True):
    """
    states: 状態列（[(bool,int)*6] のリスト）
    stride: 何個おきに間引くか（2なら 0,2,4,...）
    start: どのインデックスから始めるか（0 or 1 など）
    use_reverse: 反転列でも同じようにカウントするかどうか
    """

    counter = Counter()

    # ---- 正方向 ----
    subseq = [tuple(s) for s in states[start::stride]]

    for i in range(len(subseq) - 1):
        s1 = subseq[i]
        s2 = subseq[i+1]
        counter[(s1, s2)] += 1

    # ---- 反転方向も見る場合 ----
    if use_reverse:
        rev = list(reversed(states))
        subseq_rev = [tuple(s) for s in rev[start::stride]]
        for i in range(len(subseq_rev) - 1):
            s1 = subseq_rev[i]
            s2 = subseq_rev[i+1]
            counter[(s1, s2)] += 1

    return counter

def draw_state_graph(result, min_count=1, layout="spring"):
    """
    states: 状態列
    min_count: 何回以上出現した遷移を描画するか
    layout: "spring", "circular", "kamada_kawai"
    """

    G = nx.DiGraph()

    for (s1, s2), count in result.items():
        if count < min_count:
            continue
        
        # ノード名（読みやすい文字列）へ変換
        s1_str = util.tuple_list_to_simple_str(s1[0]) + "->" + util.tuple_list_to_simple_str(s1[1]) + f":{util.bool_list_to_leg_ground_int([b for b, _ in s1[0]])}" 
        s2_str = util.tuple_list_to_simple_str(s2[0]) + "->" + util.tuple_list_to_simple_str(s2[1]) + f":{util.bool_list_to_leg_ground_int([b for b, _ in s2[0]])}"

        G.add_node(s1_str)
        G.add_node(s2_str)

        # エッジに重み（count）を付与
        G.add_edge(s1_str, s2_str, weight=count)

    # レイアウト選択
    if layout == "spring":
        pos = nx.spring_layout(G, k=1.0, seed=0)
    elif layout == "circular":
        pos = nx.circular_layout(G)
    elif layout == "kamada_kawai":
        pos = nx.kamada_kawai_layout(G)
    else:
        pos = nx.spring_layout(G)

    # エッジ太さ＝頻度
    weights = [G[u][v]['weight'] for u, v in G.edges()]

    # 色付きで描画
    colors = [util.get_color_by_leg_ground(int(n.split(":")[-1])) for n in G.nodes()]

    plt.figure(figsize=(16, 12))
    nx.draw(
        G, pos,
        with_labels=True,
        node_size=600,
        node_color=colors,
        edge_color="gray",
        width=[w * 0.2 for w in weights],   # エッジ太さ
        font_size=8
    )

    # エッジラベル（頻度）
    edge_labels = {(u, v): G[u][v]['weight'] for u, v in G.edges()}
    nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels, font_size=8)

    plt.title("State Transition Graph (Frequency-weighted)", fontsize=16)
    plt.show()

    return G

def draw_stride_state_graph(states, stride=2, start=0,
                            min_count=1, layout="spring", use_reverse=False):
    """
    states: 状態列
    stride: 何個おきに間引くか（2なら 0,2,4,...）
    start: どのインデックスから始めるか
    min_count: 表示する遷移の最小頻度
    layout: spring / circular / kamada_kawai
    use_reverse: 反転列もカウントに含めるか
    """

    trans_counter = count_stride_transitions(
        states, stride=stride, start=start, use_reverse=use_reverse
    )

    G = nx.DiGraph()

    for (s1, s2), count in trans_counter.items():
        if count < min_count:
            continue

        s1_str = util.tuple_list_to_simple_str(s1)
        s2_str = util.tuple_list_to_simple_str(s2)

        G.add_node(s1_str)
        G.add_node(s2_str)
        G.add_edge(s1_str, s2_str, weight=count)

    # --- レイアウト ---
    if layout == "spring":
        pos = nx.spring_layout(G, k=1.0, seed=0)
    elif layout == "circular":
        pos = nx.circular_layout(G)
    elif layout == "kamada_kawai":
        pos = nx.kamada_kawai_layout(G)
    else:
        pos = nx.spring_layout(G)

    # --- エッジ太さ ---
    weights = [G[u][v]['weight'] for u, v in G.edges()]

    plt.figure(figsize=(16, 12))
    nx.draw(
        G, pos,
        with_labels=True,
        node_size=1200,
        node_color="lightgreen",
        edge_color="gray",
        width=[w * 0.25 for w in weights],
        font_size=8,
    )

    # --- エッジラベル（頻度） ---
    edge_labels = {(u, v): G[u][v]['weight'] for u, v in G.edges()}
    nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels, font_size=8)

    plt.title(
        f"State Transition Graph (stride={stride}, start={start})",
        fontsize=16
    )
    plt.show()

    return G

def main1():
    csvs = file_io.read_all_csv_files("node_list1.csv")
    print("Number of CSV files read:", len(csvs))

    combined_df =csvs[10]  # file_io.combine_csv_data(csvs)
    print("Combined DataFrame shape:", combined_df.shape)
    res = util.hierarcy_data_from_csv(combined_df)
    data = [util.bitstr_to_bool_int_list(r) for r in res]
    result = analyze_transition_pairs1(data)

    # printする
    length = len(result)
    one_cnt = sum(1 for cnt in result.values() if cnt == 1)
    print(f"Total unique transition pairs: {length}")
    print(f"Transition pairs with count 1: {one_cnt}")
    print(f"Percentage of count 1 pairs: {one_cnt / length * 100:.2f}%")

    for key, count in result.most_common(20):
        s1, s2 = key
        s1_1, s1_2 = s1
        s2_1, s2_2 = s2
        print(
            f"From: {util.tuple_list_to_simple_str(s1_1)} -> {util.tuple_list_to_simple_str(s1_2)} "
            f"To: {util.tuple_list_to_simple_str(s2_1)} -> {util.tuple_list_to_simple_str(s2_2)} | Count: {count}"
        )
        print (
            f"  Ints: {util.bool_int_list_to_int(s1_1)} -> {util.bool_int_list_to_int(s1_2)} "
            f"=> {util.bool_int_list_to_int(s2_1)} -> {util.bool_int_list_to_int(s2_2)}"
        )

    draw_state_graph(result, min_count=3, layout="spring")


if __name__ == "__main__":
    main1()
