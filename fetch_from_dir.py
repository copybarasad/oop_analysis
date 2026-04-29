import os
import re

def load_cpp_project(project_path, extensions=(".h", ".hpp", ".c", ".cpp")):
    project_files = {}
    main_candidates = []

    for root, _, files in os.walk(project_path):
        for file in files:
            if file.endswith(extensions):
                file_path = os.path.join(root, file)

                try:
                    with open(file_path, "r", encoding="utf-8", errors="ignore") as f:
                        code = f.read()
                        filename_lower = file.lower()

                        if file.endswith((".h", ".hpp")):
                            project_files[file] = code

                        elif file.endswith(".cpp"):
                            # добавляем только файл с main
                            has_main = re.search(r'\bint\s+main\s*\(', code)
                            # при этом игнорируем тестовые файлы
                            is_test = any(word in filename_lower for word in ["test", "tests", "spec"])
                            if has_main and not is_test:
                                main_candidates.append((file, code))


                except Exception as e:
                    print(f"Ошибка при чтении {file_path}: {e}")
    if main_candidates:
        # берем последний найденный main
        selected_file, selected_code = main_candidates[-1]

        # нормализуем имя
        project_files["main.cpp"] = selected_code

        # предупреждение, если main несколько
        if len(main_candidates) > 1:
            print(f"[WARN] В проекте {project_path} найдено несколько main: {[f for f, _ in main_candidates]}. Используется {selected_file}")


    return project_files