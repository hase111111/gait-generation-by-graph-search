import util
import file_io
import matplotlib.pyplot as plt
import pandas as pd
from typing import Dict, Tuple, List

def count_up_leg_state(data: pd.DataFrame) -> Dict[str, int]:
    leg_state_data: Dict[str, int] = dict()
    raw_data = util.make_tupledata_list_from_csv(data)
    for r in raw_data:
        key = util.tupledata_to_simple_str(r)
        if key in leg_state_data:
            leg_state_data[key] += 1
        else:
            leg_state_data[key] = 1
    return leg_state_data

def count_up_hierarchy(data: pd.DataFrame) -> Dict[str, int]:
    hierarchy_data: Dict[str, int] = dict()
    raw_data = util.make_tupledata_list_from_csv(data)
    for r in raw_data:
        ints = [n for _, n in r]
        hierarchy_key = util.convert_hierarchy_str(ints)
        if hierarchy_key in hierarchy_data:
            hierarchy_data[hierarchy_key] += 1
        else:
            hierarchy_data[hierarchy_key] = 1
    return hierarchy_data

def count_up_transition(data: pd.DataFrame) -> Dict[Tuple[str, str], int]:
    transition_data: Dict[Tuple[str, str], int] = dict()
    raw_data = util.make_tupledata_list_from_csv(data)
    prev_tupledata_list = None
    for r in raw_data:
        if prev_tupledata_list is not None:
            key = (util.tupledata_to_simple_str(prev_tupledata_list), 
                   util.tupledata_to_simple_str(r))
            if key in transition_data:
                transition_data[key] += 1
            else:
                transition_data[key] = 1
        prev_tupledata_list = r
    return transition_data

def main1():
    file = file_io.get_file_paths("node_list1.csv")
    print("Number of CSV files read:", len(file))

    all_leg_state_counts: Dict[str, int] = dict()

    for i, df in enumerate(file):
        print(f"Processing file {i}: {df}")
        data = file_io.read_csv_file(df)
        leg_state_counts = count_up_leg_state(data)
        for key, count in leg_state_counts.items():
            if key in all_leg_state_counts:
                all_leg_state_counts[key] += count
            else:
                all_leg_state_counts[key] = count

    # 結果を表示
    sorted_leg_state = sorted(all_leg_state_counts.items(), key=lambda x: x[1], reverse=True)

    # 全部で何通りの階層が出現したか
    print(f"Total unique hierarchies: {len(sorted_leg_state)}")

    for key, count in sorted_leg_state:
        if count < 100:
            continue
        print(f"{key}: {count}")

def main2(do_print: bool = True):
    file = file_io.get_file_paths("node_list1.csv")
    if do_print:
        print("Number of CSV files read:", len(file))

    all_hierarchy_counts: Dict[str, int] = dict()

    for i, df in enumerate(file):
        if do_print:
            print(f"Processing file {i}: {df}")
        data = file_io.read_csv_file(df)
        hierarchy_counts = count_up_hierarchy(data)
        for key, count in hierarchy_counts.items():
            if key in all_hierarchy_counts:
                all_hierarchy_counts[key] += count
            else:
                all_hierarchy_counts[key] = count

    # 結果を表示
    sorted_hierarchy = sorted(all_hierarchy_counts.items(), key=lambda x: x[1], reverse=True)

    # 全部で何通りの階層が出現したか
    if do_print:    
        print(f"Total unique hierarchies: {len(sorted_hierarchy)}")

        for key, count in sorted_hierarchy:
            if count < 100:
                continue
            print(f"{key}: {count}")

    return sorted_hierarchy

def main3(do_print: bool = True):
    file = file_io.get_file_paths("node_list1.csv")
    if do_print:
        print("Number of CSV files read:", len(file))

    all_transition_counts: Dict[Tuple[str, str], int] = dict()

    for i, df in enumerate(file):
        if do_print:
            print(f"Processing file {i}: {df}")
        data = file_io.read_csv_file(df)
        transition_counts = count_up_transition(data)
        for key, count in transition_counts.items():
            if key in all_transition_counts:
                all_transition_counts[key] += count
            else:
                all_transition_counts[key] = count

    # 結果を表示
    sorted_transition = sorted(all_transition_counts.items(), key=lambda x: x[1], reverse=True)

    # 全部で何通りの遷移が出現したか
    if do_print:    
        print(f"Total unique transitions: {len(sorted_transition)}")

        for key, count in sorted_transition:
            if count < 100:
                continue
            print(f"{key}: {count}")

    return sorted_transition    

def main4():
    data = main2(do_print=False)

    # どの階層がどれだけ出現したかを円グラフで表示.
    labels: List[str] = [] 
    sizes: List[int] = []
    only_once_cnt: int = 0
    for key, count in data:
        if count < 2:
            only_once_cnt += count
            continue
        labels.append(key)
        sizes.append(count)
    if only_once_cnt > 0:
        labels.append("only once")
        sizes.append(only_once_cnt)
    plt.figure(figsize=(10,10))  # type: ignore
    plt.pie(sizes, labels=labels, autopct='%1.1f%%', startangle=140)  # type: ignore
    plt.axis('equal')  # type: ignore
    plt.title("Hierarchy Distribution")  # type: ignore
    plt.show()  # type: ignore

    # 棒グラフで，出現した回数ごとの階層数を表示.
    count_distribution: Dict[int, int] = dict()
    for _, count in data:
        if count in count_distribution:
            count_distribution[count] += 1
        else:
            count_distribution[count] = 1
    sorted_distribution = sorted(count_distribution.items())
    x = [str(k) for k, _ in sorted_distribution]
    y = [v for _, v in sorted_distribution]
    plt.figure(figsize=(20,10))  # type: ignore
    plt.bar(x, y)  # type: ignore
    plt.xlabel("Number of Occurrences")  # type: ignore
    plt.ylabel("Number of Hierarchies")  # type: ignore
    plt.title("Hierarchy Occurrence Distribution")  # type: ignore
    plt.xticks(fontsize=10, rotation=90)  # type: ignore
    # xのラベルを交互に上下にずらす
    for i, label in enumerate(plt.gca().get_xticklabels()):
        if i % 2 == 0:
            label.set_y(-0.04)
        else:
            label.set_y(-0.0)

    plt.show()  # type: ignore

    # 同じことを総数ベースでも表示.
    count_distribution: Dict[int, int] = dict()
    for _, count in data:
        if count in count_distribution:
            count_distribution[count] += count
        else:
            count_distribution[count] = count
    sorted_distribution = sorted(count_distribution.items())
    x = [str(k) for k, _ in sorted_distribution]
    y = [v for _, v in sorted_distribution]
    plt.figure(figsize=(20,10))  # type: ignore
    plt.bar(x, y)  # type: ignore
    plt.xlabel("Number of Occurrences")  # type: ignore
    plt.ylabel("Total Count of Hierarchies")  # type: ignore
    plt.title("Hierarchy Total Count Distribution")  # type: ignore
    plt.xticks(fontsize=10, rotation=90)  # type: ignore
    # xのラベルを交互に上下にずらす
    for i, label in enumerate(plt.gca().get_xticklabels()):
        if i % 2 == 0:
            label.set_y(-0.04)
        else:
            label.set_y(-0.0)
    plt.show()  # type: ignore

def main5():
    data = main3(do_print=False)

    # どの遷移がどれだけ出現したかを円グラフで表示
    labels: List[str] = []
    sizes: List[int] = []
    only_once_cnt: int = 0
    for key, count in data:
        if count < 3:
            only_once_cnt += count
            continue
        labels.append(f"{key[0]} -> {key[1]}")
        sizes.append(count)
    if only_once_cnt > 0:
        labels.append("only once")
        sizes.append(only_once_cnt)

    # 横軸のラベルサイズを小さくする
    plt.rcParams['font.size'] = 8  # type: ignore
    plt.figure(figsize=(10,10))  # type: ignore
    plt.pie(sizes, labels=labels, autopct='%1.1f%%', startangle=140)  # type: ignore
    plt.axis('equal')  # type: ignore
    plt.title("Transition Distribution")  # type: ignore
    plt.show()  # type: ignore

    # 棒グラフで，出現した回数ごとの遷移数を表示.
    count_distribution: Dict[int, int] = dict()
    for _, count in data:
        if count in count_distribution:
            count_distribution[count] += 1
        else:
            count_distribution[count] = 1
    sorted_distribution = sorted(count_distribution.items())
    x = [str(k) for k, _ in sorted_distribution]
    y = [v for _, v in sorted_distribution]
    plt.figure(figsize=(20,10))  # type: ignore
    plt.bar(x, y)  # type: ignore
    plt.xlabel("Number of Occurrences")  # type: ignore
    plt.ylabel("Number of Transitions")  # type: ignore
    plt.title("Transition Occurrence Distribution")  # type: ignore
    plt.xticks(fontsize=10, rotation=90)  # type: ignore
    # xのラベルを交互に上下にずらす
    for i, label in enumerate(plt.gca().get_xticklabels()):
        if i % 2 == 0:
            label.set_y(-0.04)
        else:
            label.set_y(-0.0)
    plt.show()  # type: ignore

    # 同じことを総数ベースでも表示
    count_distribution: Dict[int, int] = dict()
    for _, count in data:
        if count in count_distribution:
            count_distribution[count] += count
        else:
            count_distribution[count] = count
    sorted_distribution = sorted(count_distribution.items())
    x = [str(k) for k, _ in sorted_distribution]    
    y = [v for _, v in sorted_distribution] 
    plt.figure(figsize=(20,10))  # type: ignore
    plt.bar(x, y)  # type: ignore
    plt.xlabel("Number of Occurrences")  # type: ignore
    plt.ylabel("Total Count of Transitions")  # type: ignore
    plt.title("Transition Total Count Distribution")  # type: ignore
    plt.xticks(fontsize=10, rotation=90)  # type: ignore
    # xのラベルを交互に上下にずらす
    for i, label in enumerate(plt.gca().get_xticklabels()):
        if i % 2 == 0:
            label.set_y(-0.04)
        else:
            label.set_y(-0.0)
    plt.show()  # type: ignore

if __name__ == "__main__":
    main3()
