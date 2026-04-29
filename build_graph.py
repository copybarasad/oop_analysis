import re
import networkx as nx


def extract_dependencies(project_files):
    edges = []

    for class_name, code in project_files.items():
        short_class_name = class_name.split(".")[0]
        includes = re.findall(r'#include\s+"([\w\d_]+)\.(h|hpp)"', code)
        for included_class in includes:
            edges.append((short_class_name, included_class[0]))

        # класс внутри класса
        classes = re.findall(r'class\s+(\w+)\s*{', code)
        if len(classes) > 1:
            outer_class = classes[0]
            nested_classes = classes[1:]
            for nested in nested_classes:
                edges.append((outer_class, nested))
    return edges


def build_dependency_graph(project_files, project_name=None):
    G = nx.DiGraph()
    for class_name in project_files.keys():
        short_class_name = class_name.split(".")[0]
        G.add_node(short_class_name)

    for src, dest in extract_dependencies(project_files):
        if src in G and dest in G:  # только если оба класса присутствуют
            G.add_edge(src, dest)

    # расстановка labels
    main = None
    for node in G.nodes():
        if node == "main":
            main = node
            break
    if main is None:
        print(f'в проекте {project_name} main не найден')
        return

    dist = nx.single_source_shortest_path_length(G, main)

    # назначение структурных меток
    for node in G.nodes():
        if node == main:
            G.nodes[node]["label"] = "entry"
        else:
            d = dist.get(node, -1)  # -1 если недостижим
            G.nodes[node]["label"] = f"dist{d}" # учитывает расстояние от main
    return G


