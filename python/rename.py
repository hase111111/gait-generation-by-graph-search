import os

BASE_DIR = "result"

def rename_folders(base_dir):
    for name in os.listdir(base_dir):
        old_path = os.path.join(base_dir, name)

        # フォルダのみ対象
        if not os.path.isdir(old_path):
            continue

        new_name = None

        if name.startswith("time_check_pruning_legacy_"):
            new_name = name.replace(
                "time_check_pruning_legacy_", "legacy_", 1
            )

        elif name.startswith("time_check_pruning_"):
            new_name = name.replace(
                "time_check_pruning_", "pruning_", 1
            )

        elif name.startswith("time_check_normal_"):
            new_name = name.replace(
                "time_check_normal_", "normal_", 1
            )

        elif name.startswith("time_check_"):
            new_name = name.replace(
                "time_check_", "normal_", 1
            )

        # リネーム実行
        if new_name:
            new_path = os.path.join(base_dir, new_name)
            print(f"{name} -> {new_name}")
            os.rename(old_path, new_path)


if __name__ == "__main__":
    rename_folders(BASE_DIR)
