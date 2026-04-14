from build_graph import *
from networkx.algorithms import isomorphism
from fetch_from_dir import load_cpp_project
from grakel import GraphKernel
import numpy as np
from grakel.utils import graph_from_networkx
import networkx as nx
from grakel import Graph
import os

def load_all_projects(base_path):
    projects = {}

    for project_name in os.listdir(base_path): # массив имен файлов лежащих в папке
        project_path = os.path.join(base_path, project_name)

        if os.path.isdir(project_path):
            files = load_cpp_project(project_path)
            projects[project_name] = files

    return projects

def build_all_graphs(projects):
    graphs = {}

    for name, files in projects.items():
        G = build_dependency_graph(files)
        graphs[name] = G

    return graphs

def convert_to_grakel(G):
    edges = list(G.edges())
    labels = nx.get_node_attributes(G, "label")

    if not labels:
        raise ValueError("Нет labels -- GraKeL упадет")

    return Graph(edges, node_labels=labels)

def wl_similarity_matrix(graphs, h=3):
    # задаем wl kernel + базовый kernel
    gk = GraphKernel(kernel=[{"name": "weisfeiler_lehman", "n_iter": h},
                             {"name": "vertex_histogram"}],
                     normalize=True)

    # считаем матрицу сходства
    k = gk.fit_transform(gk_graphs)

    return k

def prepare_grakel_graphs(graphs_dict):
    # преобразуем все графы networkx в grakel (проверяем на корректность перед этим)
    names = []
    gk_graphs = []

    for name, G in graphs_dict.items():
        if G.number_of_nodes() == 0:
            print(f"ПУСТОЙ ГРАФ: {name}")
            continue

        labels = nx.get_node_attributes(G, "label")
        if not labels:
            print(f"НЕТ LABELS: {name}")
            continue

        names.append(name)
        gk_graphs.append(convert_to_grakel(G))

    return names, gk_graphs

if __name__ == "__main__":
    base_path = "/Users/nadya/Desktop/fqw/oop_analysis/CodeDatabase"

    projects = load_all_projects(base_path)
    graphs = build_all_graphs(projects)
    names, gk_graphs = prepare_grakel_graphs(graphs)

    K = wl_similarity_matrix(gk_graphs, h=3)
    print(K)

    # plt.figure(figsize=(10, 6))
    # nx.draw(G, with_labels=True, node_size=1200, node_color="lightblue", arrows=True)
    # plt.show()
