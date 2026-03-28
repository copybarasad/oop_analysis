import os


def load_cpp_project(project_path, extensions=(".h", ".hpp")):
    project_files = {}

    for root, _, files in os.walk(project_path):
        for file in files:
            if file.endswith(extensions):
                file_path = os.path.join(root, file)
                try:
                    with open(file_path, "r", encoding="utf-8", errors="ignore") as f:
                        project_files[file] = f.read()
                except Exception as e:
                    print(f"Ошибка при чтении {file_path}: {e}")

    return project_files