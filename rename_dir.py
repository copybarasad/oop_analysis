import os
from pathlib import Path
import shutil


def rename_student_folders_global(root_path: str):
    root = Path(root_path)
    print(root)

    if not root.exists() or not root.is_dir():
        raise ValueError("Указанный путь не существует или не является папкой")

    student_dirs = []

    for group_dir in sorted(root.iterdir(), key=lambda x: x.name):
        if not group_dir.is_dir():
            continue

        for student_dir in sorted(group_dir.iterdir(), key=lambda x: x.name):
            if student_dir.is_dir():
                student_dirs.append(student_dir)

    if not student_dirs:
        print("Папки студентов не найдены")
        return
    print(f"Найдено {len(student_dirs)} папок студентов")

    # временные имена (чтобы избежать коллизий) и перемещение
    temp_dirs = []
    for i, student_dir in enumerate(student_dirs):
        temp_path = root/ f"__temp__global__{i}"
        shutil.move(str(student_dir), str(temp_path))
        temp_dirs.append(temp_path)

    # переименование
    for index, temp_dir in enumerate(temp_dirs, start=1):
        new_path = root / str(index)
        temp_dir.rename(new_path)

    # удаление папок групп
    for group_dir in root.iterdir():
        if group_dir.is_dir() and group_dir.name.startswith("__temp__") is False:
            # если это старая группа и она пустая — удаляем
            try:
                group_dir.rmdir()
            except OSError:
                pass


if __name__ == "__main__":
    root_folder = "/Users/nadya/Desktop/fqw/oop_analysis/CodeDatabase" # меняется на путь до папки с папками/файлами студентов
    rename_student_folders_global(root_folder)
