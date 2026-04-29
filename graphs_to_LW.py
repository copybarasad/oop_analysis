from build_graph import *
import os
from grakel import Graph
from fetch_from_dir import load_cpp_project
from grakel import GraphKernel

def load_all_projects(base_path):
    projects = {}

    for project_name in os.listdir(base_path): # массив имен файлов лежащих в папке
        project_path = os.path.join(base_path, project_name)

        if os.path.isdir(project_path):
            files = load_cpp_project(project_path)
            projects[project_name] = files
    return projects

def build_all_graphs(projects):
    graphs_b = {}
    # projects -- тип: dict структура: (имя проекта: dict)
    for name_project, files in projects.items():
        G = build_dependency_graph(files, project_name=name_project)
        if G is None:
            continue
        else:
            graphs_b[name_project] = G

    return graphs_b

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
    k = gk.fit_transform(graphs)

    return k

def prepare_grakel_graphs(graphs_dict):
    names_p = []
    gk_graphs_p = []

    for name, G in graphs_dict.items():
        if G.number_of_nodes() == 0:
            print(f"ПУСТОЙ ГРАФ: {name}")
            continue

        labels = nx.get_node_attributes(G, "label")
        if not labels:
            print(f"НЕТ LABELS: {name}")
            continue

        names_p.append(name)
        gk_graphs_p.append(convert_to_grakel(G))

    return names_p, gk_graphs_p