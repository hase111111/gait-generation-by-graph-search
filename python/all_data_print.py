import numpy as np
from collections import Counter
import util
import pandas as pd
import networkx as nx
import matplotlib.pyplot as plt
import file_io
import util
import frequency_analysis
import transition

def main():
    file = file_io.get_file_paths("node_list1.csv")

    # 各fileについて，plot_2d_pairs_alternate_colorを実行
    for f in file:
        print(f)
        csv = pd.read_csv(f)
        data = util.hierarcy_data_from_csv(csv)
        transition.plot_2d_pairs_alternate_color([util.bool_int_list_to_int(util.bitstr_to_bool_int_list(r)) for r in data], plot_type=("y", "z"), only_save=True,
                                        file_name=f.replace(".csv", "_2d_plot.png"))




if __name__ == "__main__":
    main()
