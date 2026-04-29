from graphs_to_LW import *
from sklearn.cluster import SpectralClustering
import numpy as np
import pandas as pd
from visualize_clusters import visualize_clusters


def cluster_graphs_spectral(similarity_matrix, n_clusters):
    # similarity_matrix -- матрица сходства
    # n_clusters — число кластеров


    model = SpectralClustering(
        n_clusters=n_clusters,
        affinity='precomputed'
    )

    labels = model.fit_predict(similarity_matrix)
    return labels


def print_cluster_similarities(similarity_matrix, labels, cluster_id, names=None):
    # Выводит коэффициенты сходства для всех пар внутри одного кластера

    # индексы объектов в кластере
    indices = np.where(labels == cluster_id)[0]

    print(f"\nCluster {cluster_id}, size = {len(indices)}")

    for i in range(len(indices)):
        for j in range(i + 1, len(indices)):
            idx_i = indices[i]
            idx_j = indices[j]

            sim = similarity_matrix[idx_i, idx_j]

            if names is not None:
                print(f"{names[idx_i]} — {names[idx_j]} : {sim:.4f}")
            else:
                print(f"{idx_i} — {idx_j} : {sim:.4f}")


if __name__ == "__main__":
    base_path = "/Users/nadya/Desktop/fqw/oop_analysis/CodeDatabase"

    projects = load_all_projects(base_path)
    graphs = build_all_graphs(projects)
    names, gk_graphs = prepare_grakel_graphs(graphs)

    similarity_matrix = wl_similarity_matrix(gk_graphs, h=4)

    # общий случай
    # matrix_spectral = cluster_graphs_spectral(similarity_matrix, n_clusters=15)
    # print("matrix_spectral", matrix_spectral)

    # можно посмотреть связи внутри кластера
    # labels = matrix_spectral
    # print_cluster_similarities(similarity_matrix, labels, cluster_id=0, names=names)

    # частный случай
    matrix_spectral = np.array(list(map( int,"12  5  1  4 12  3  7 11  3 13  8 14  3  9  3  6 12  1  8  1  8  7  7 10 10  7 12  0  7  7  1  1  1 12  1  0  9 10  1  1  1  7  8 13 10  1  3  0 12  1 11  8 11  9  8 12  6  0 10  1  9 14 10 11  1 10 10 10  1  1 12  3 1  7  7  3  8  3  8  9 13  9  1  8 14  1 13  1  8  3  0  3  7  8  1 14 8 11 10  1 10  8 12  1  9  5 11  8  0 14  6  8  0 13  1  8  8  5 10  5 13  8  8 13 13 10 13  2  6  3  1  3 13  8  1  1  6  1  1  7 10  0  7  1 0 11  8  1  7 12 12 12  3 13 12  1  4  9  8 13 10  6 12  5  4  1  1  6 9  7  1  7  1 11".split())))
    # print("matrix_spectral", list(map( int, matrix_spectral)))
    visualize_clusters(similarity_matrix,  matrix_spectral)

    # сохранение WL матрицы в файл
    # column = [str("G " + name) for name in names]
    # df = pd.DataFrame(similarity_matrix, index=column, columns=column)
    # df.to_csv("output.csv", index=True)
