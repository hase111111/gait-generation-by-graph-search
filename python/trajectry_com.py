import csv
import math
import os
import random
from typing import List, Optional, Tuple

import matplotlib.pyplot as plt

import file_io


TARGET_FILE = "simulation_result_detail1.csv"
BASE_DIR = "result"

# simulation_result_detail1.csv の列 (0始まり)
COM_X_COL = 15
COM_Y_COL = 16
COM_Z_COL = 17


def _safe_float(value: str) -> Optional[float]:
	try:
		return float(value)
	except (ValueError, TypeError):
		return None


def _safe_int(value: str) -> Optional[int]:
	try:
		return int(value)
	except (ValueError, TypeError):
		return None


def get_simulation_result_files(
	target_file: str = TARGET_FILE,
	base_dir: str = BASE_DIR,
) -> List[str]:
	"""
	指定ディレクトリ配下の simulation_result_detail1.csv のパス一覧を返す.
	"""
	base_name = os.path.normpath(base_dir)
	all_files = file_io.get_file_paths(target_file)

	result: List[str] = []
	for path in all_files:
		norm_path = os.path.normpath(path)
		parts = norm_path.split(os.sep)
		if base_name in parts:
			result.append(norm_path)
	return result


def read_com_xy_trajectory(
	csv_path: str,
	start_step: int = 0,
	end_step: Optional[int] = None,
	x_col: int = COM_X_COL,
	y_col: int = COM_Y_COL,
	encoding: str = "shift_jis",
) -> List[Tuple[float, float]]:
	"""
	CSVから重心(x, y)の時系列を抽出する.

	- step列(0列目)が整数の行のみ対象
	- x_col / y_col は 0始まり列番号
	"""
	points: List[Tuple[float, float]] = []

	with open(csv_path, encoding=encoding) as file:
		reader = csv.reader(file)
		for row in reader:
			if len(row) <= max(x_col, y_col):
				continue

			step = _safe_int(row[0])
			if step is None:
				continue
			if step < start_step:
				continue
			if end_step is not None and step > end_step:
				continue

			x_val = _safe_float(row[x_col])
			y_val = _safe_float(row[y_col])
			if x_val is None or y_val is None:
				continue

			points.append((x_val, y_val))

	return points


def _add_gaussian_noise_to_points(
	points: List[Tuple[float, float]],
	noise_std_ratio: float,
	rng: random.Random,
	min_std: float = 1.0,
) -> List[Tuple[float, float]]:
	"""
	元データを基準に，軌跡へ滑らかなノイズを加える.
	
	- 先頭点(基準点)は固定する
	- 基準点まわりの微小回転・微小スケーリング
	- 時系列に沿った低周波ドリフト
	
	これにより，元軌跡に近い見た目を保ちつつ，
	「似ているが少し異なる」データを作る.
	"""
	if noise_std_ratio <= 0.0 or len(points) < 2:
		return points

	xs = [point[0] for point in points]
	ys = [point[1] for point in points]
	x_span = max(xs) - min(xs)
	y_span = max(ys) - min(ys)
	anchor_x, anchor_y = points[0]

	# 基準点まわりの微小回転・スケール
	rotate_std_rad = noise_std_ratio * 0.8
	angle = rng.gauss(0.0, rotate_std_rad)
	cos_a = math.cos(angle)
	sin_a = math.sin(angle)

	scale_x = 1.0 + rng.gauss(0.0, noise_std_ratio * 0.25)
	scale_y = 1.0 + rng.gauss(0.0, noise_std_ratio * 0.25)
	scale_x = max(0.95, min(1.05, scale_x))
	scale_y = max(0.95, min(1.05, scale_y))

	# 低周波ドリフト（t=0 で 0 になるように補正）
	amp_x = max(x_span * noise_std_ratio * 0.35, min_std * 0.1)
	amp_y = max(y_span * noise_std_ratio * 0.35, min_std * 0.1)
	freq_x = rng.uniform(0.4, 1.2)
	freq_y = rng.uniform(0.4, 1.2)
	phase_x = rng.uniform(0.0, 2.0 * math.pi)
	phase_y = rng.uniform(0.0, 2.0 * math.pi)
	base_x = amp_x * math.sin(phase_x)
	base_y = amp_y * math.sin(phase_y)

	new_points: List[Tuple[float, float]] = []
	denom = max(1, len(points) - 1)
	for idx, (x_val, y_val) in enumerate(points):
		t = idx / denom

		rx = (x_val - anchor_x) * scale_x
		ry_ = (y_val - anchor_y) * scale_y

		tx = rx * cos_a - ry_ * sin_a
		ty = rx * sin_a + ry_ * cos_a

		drift_x = amp_x * math.sin(2.0 * math.pi * freq_x * t + phase_x) - base_x
		drift_y = amp_y * math.sin(2.0 * math.pi * freq_y * t + phase_y) - base_y

		new_points.append(
			(
				anchor_x + tx + drift_x,
				anchor_y + ty + drift_y,
			)
		)

	# 先頭点は元データに固定.
	new_points[0] = points[0]

	return new_points


def _plot_segments_alternate_color(
	xs: List[float],
	ys: List[float],
	color1: str = "red",
	color2: str = "blue",
) -> None:
	colors = [color1, color2]
	for index in range(len(xs) - 1):
		plt.plot(  # type: ignore
			xs[index:index + 2],
			ys[index:index + 2],
			color=colors[index % 2],
			linewidth=1.2,
		)


def plot_com_trajectory(
	points: List[Tuple[float, float]],
	title: str = "Center of Mass Trajectory",
	file_name: str = "com_trajectory.png",
	only_save: bool = False,
	draw_all_points: bool = False,
	x_label: str = "CoM X [mm]",
	y_label: str = "CoM Y [mm]",
) -> None:
	"""
	1本の重心移動軌跡を2Dで描画する.
	"""
	if len(points) < 2:
		raise ValueError("軌跡の点数が不足しています. 2点以上必要です.")

	xs = [point[0] for point in points]
	ys = [point[1] for point in points]

	plt.figure(figsize=(8, 8))  # type: ignore
	_plot_segments_alternate_color(xs, ys)

	if draw_all_points:
		plt.scatter(xs, ys, color="cyan", s=8, alpha=0.7)  # type: ignore

	plt.scatter(xs[0], ys[0], color="green", s=60, label="start", zorder=3)  # type: ignore
	plt.scatter(xs[-1], ys[-1], color="black", s=60, label="end", zorder=3)  # type: ignore

	plt.xlabel(x_label)  # type: ignore
	plt.ylabel(y_label)  # type: ignore
	plt.title(title)  # type: ignore
	plt.grid(True)  # type: ignore
	plt.axis("equal")  # type: ignore
	plt.legend()  # type: ignore
	plt.tight_layout()  # type: ignore

	plt.savefig(file_name, dpi=300)  # type: ignore

	if only_save:
		plt.close()
	else:
		plt.show()  # type: ignore


def _label_from_file_path(csv_path: str) -> str:
	folder_name = file_io.get_upper_directory_name(csv_path)
	parts = folder_name.split("_")
	if len(parts) < 6:
		return folder_name

	option = parts[0]
	terrain = "_".join(parts[2:-3])
	return f"{option}_{terrain}"


def plot_com_trajectory_from_csv(
	csv_path: str,
	start_step: int = 0,
	end_step: Optional[int] = None,
	file_name: str = "com_trajectory_single.png",
	only_save: bool = False,
	x_col: int = COM_X_COL,
	y_col: int = COM_Y_COL,
	x_label: str = "CoM X [mm]",
	y_label: str = "CoM Y [mm]",
) -> None:
	points = read_com_xy_trajectory(
		csv_path,
		start_step=start_step,
		end_step=end_step,
		x_col=x_col,
		y_col=y_col,
	)
	if len(points) < 2:
		raise ValueError(f"有効な軌跡点が不足しています: {csv_path}")

	title = f"CoM Trajectory: {_label_from_file_path(csv_path)}"
	plot_com_trajectory(
		points,
		title=title,
		file_name=file_name,
		only_save=only_save,
		x_label=x_label,
		y_label=y_label,
	)


def plot_com_trajectory_multi(
	file_list: List[str],
	start_step: int = 0,
	end_step: Optional[int] = None,
	file_name: str = "com_trajectory_combined.png",
	only_save: bool = False,
	save_file: bool = True,
	x_col: int = COM_X_COL,
	y_col: int = COM_Y_COL,
	x_label: str = "CoM X [mm]",
	y_label: str = "CoM Y [mm]",
	title: str = "Center of Mass Trajectories (Combined)",
	x_range: Optional[Tuple[float, float]] = None,
	y_range: Optional[Tuple[float, float]] = None,
	noise_std_ratio: float = 0.0,
	rng: Optional[random.Random] = None,
) -> None:
	"""
	複数CSVの重心軌跡を1枚に重ねて描画する.
	"""
	if noise_std_ratio < 0.0:
		raise ValueError("noise_std_ratio は 0以上で指定してください.")
	if rng is None:
		rng = random.Random()

	plt.figure(figsize=(10, 8))  # type: ignore

	plotted_count = 0
	for csv_path in file_list:
		points = read_com_xy_trajectory(
			csv_path,
			start_step=start_step,
			end_step=end_step,
			x_col=x_col,
			y_col=y_col,
		)
		if len(points) < 2:
			continue
		if noise_std_ratio > 0.0:
			points = _add_gaussian_noise_to_points(points, noise_std_ratio=noise_std_ratio, rng=rng)

		xs = [point[0] for point in points]
		ys = [point[1] for point in points]

		label = _label_from_file_path(csv_path)
		color = file_io.generate_color_from_filename(label)
		plt.plot(xs, ys, color=color, linewidth=1.0, alpha=0.85, label=label)  # type: ignore
		plotted_count += 1

	if plotted_count == 0:
		plt.close()
		raise ValueError("描画可能な軌跡がありませんでした.")

	plt.xlabel(x_label)  # type: ignore
	plt.ylabel(y_label)  # type: ignore
	plt.title(title)  # type: ignore
	plt.grid(True)  # type: ignore
	ax = plt.gca()  # type: ignore
	if x_range is not None:
		ax.set_xlim(x_range[0], x_range[1])  # type: ignore
	if y_range is not None:
		ax.set_ylim(y_range[0], y_range[1])  # type: ignore
	ax.set_aspect("equal", adjustable="box")  # type: ignore
	if x_range is not None or y_range is not None:
		print(f"Applied display range: x={ax.get_xlim()}, y={ax.get_ylim()}")
	legend_ncol = min(4, max(1, plotted_count))
	ax.legend(  # type: ignore
		loc="upper center",
		bbox_to_anchor=(0.5, -0.12),
		borderaxespad=0.0,
		fontsize=6,
		ncol=legend_ncol,
	)
	plt.tight_layout(rect=[0.0, 0.12, 1.0, 1.0])  # type: ignore

	if save_file:
		plt.savefig(file_name, dpi=300, bbox_inches="tight")  # type: ignore

	if only_save:
		plt.close()
	else:
		plt.show()  # type: ignore


def main1() -> None:
	"""
	対象ディレクトリの先頭1ファイルを使って重心軌跡を描画する.
	"""
	files = get_simulation_result_files(base_dir=BASE_DIR)
	print("Number of CSV files:", len(files))
	if not files:
		print("CSVが見つかりませんでした")
		return

	target = files[0]
	print("Target file:", target)
	plot_com_trajectory_from_csv(
		target,
		start_step=0,
		end_step=100,
		file_name="com_trajectory_single.png",
	)


def main2(keyword: str = "rough", max_files: int = 30) -> None:
	"""
	キーワードで絞り込んだ複数ファイルの軌跡を重ね描画する.
	"""
	files = get_simulation_result_files(base_dir=BASE_DIR)
	filtered = [path for path in files if keyword in path]
	if max_files > 0:
		filtered = filtered[:max_files]

	print("Number of all CSV files:", len(files))
	print("Number of filtered CSV files:", len(filtered))
	if not filtered:
		print("条件に一致するCSVがありませんでした")
		return

	plot_com_trajectory_multi(
		filtered,
		start_step=0,
		end_step=100,
		file_name=f"com_trajectory_combined_{keyword}.png",
	)


def main3(
	file_list: List[str],
	name: str = "custom",
	x_range: Optional[Tuple[float, float]] = None,
	y_range: Optional[Tuple[float, float]] = None,
) -> None:
	"""
	任意のファイルリストを受け取って重ね描画する.
	x_range, y_range を指定すると表示範囲を制限できる.
	"""
	if not file_list:
		print("file_list が空です")
		return

	plot_com_trajectory_multi(
		file_list,
		start_step=0,
		end_step=5000,
		file_name=f"com_trajectory_combined_{name}.png",
		x_range=x_range,
		y_range=y_range,
	)


def main4(
	file_list: List[str],
	name: str = "custom_xz",
	x_range: Optional[Tuple[float, float]] = None,
	y_range: Optional[Tuple[float, float]] = None,
) -> None:
	"""
	任意のファイルリストを受け取り，重心の x-z 軌跡を重ね描画する.
	x_range, y_range を指定すると表示範囲を制限できる.
	"""
	if not file_list:
		print("file_list が空です")
		return

	plot_com_trajectory_multi(
		file_list,
		start_step=0,
		end_step=5000,
		file_name=f"com_trajectory_xz_combined_{name}.png",
		x_col=COM_X_COL,
		y_col=COM_Z_COL,
		x_label="CoM X [mm]",
		y_label="CoM Z [mm]",
		title="Center of Mass Trajectories (X-Z Combined)",
		x_range=x_range,
		y_range=y_range,
	)


def main5(
	file_list: List[str],
	name: str = "custom_noisy",
	x_range: Optional[Tuple[float, float]] = None,
	y_range: Optional[Tuple[float, float]] = None,
	output_count: int = 5,
	noise_std_ratio: float = 0.01,
	random_seed: Optional[int] = None,
) -> None:
	"""
	main3 を踏襲しつつ，重心軌跡(x-y)に微小ノイズを加えた図を複数表示する.
	画像ファイルは保存せず，表示のみ行う.
	"""
	if not file_list:
		print("file_list が空です")
		return
	if output_count < 1:
		raise ValueError("output_count は1以上で指定してください.")
	if noise_std_ratio <= 0.0:
		raise ValueError("noise_std_ratio は 0より大きい値で指定してください.")

	print(
		f"Generating noisy plots: count={output_count}, "
		f"noise_std_ratio={noise_std_ratio}, seed={random_seed}"
	)

	for index in range(output_count):
		seed_i = None if random_seed is None else random_seed + index
		rng_i = random.Random(seed_i)

		plot_com_trajectory_multi(
			file_list,
			start_step=0,
			end_step=5000,
			file_name=f"com_trajectory_combined_{name}_noisy_{index + 1:02d}.png",
			only_save=False,
			save_file=False,
			x_col=COM_X_COL,
			y_col=COM_Y_COL,
			x_label="CoM X [mm]",
			y_label="CoM Y [mm]",
			x_range=x_range,
			y_range=y_range,
			title=f"Center of Mass Trajectories XY (Noisy {index + 1})",
			noise_std_ratio=noise_std_ratio,
			rng=rng_i,
		)
		print(f"Displayed: noisy plot {index + 1}/{output_count}")


if __name__ == "__main__":
	# main1()

	# files = get_simulation_result_files(base_dir="result")
	# print("Number of CSV files:", len(files))

	# filtered_files = [file_path for file_path in files if "second_success_stepup" in file_path]
	# print("Number of filtered CSV files:", len(filtered_files))

	# main4(filtered_files, name="second_success_stepup", x_range=(0.0, 70000.0), y_range=(-8000.0, 8000.0))

	files = get_simulation_result_files(base_dir="result")
	print("Number of CSV files:", len(files))

	filtered_files = [file_path for file_path in files if "second_success_stepdown" in file_path]
	print("Number of filtered CSV files:", len(filtered_files))

	main5(
		filtered_files,
		name="second_success_stepdown",
		x_range=(0.0, 70000.0),
		y_range=(-6000.0, 6000.0),
		output_count=1,
		noise_std_ratio=0.008,
		random_seed=None,
	)
    