import re
import networkx as nx
import matplotlib.pyplot as plt

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


def build_dependency_graph(project_files):

    G = nx.DiGraph() # возможно стоит поменять на MultiDiGraph
    for class_name in project_files.keys():
        short_class_name = class_name.split(".")[0]
        G.add_node(short_class_name)

    for src, dest in extract_dependencies(project_files):
        if src in G and dest in G:  # только если оба класса присутствуют
            G.add_edge(src, dest)

    return G


