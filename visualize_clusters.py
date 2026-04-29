import numpy as np
import matplotlib.pyplot as plt
import networkx as nx


# 1 кластер = 1 граф
# threshold - порог по сходству
def visualize_clusters(similarity, labels, threshold=0.7):

    unique_labels = sorted(set(labels))
    n_clusters = len([l for l in unique_labels if l != -1])

    cols = 3
    rows = (n_clusters // cols) + 1

    fig, axes = plt.subplots(rows, cols, figsize=(6 * cols, 6 * rows), constrained_layout=True)
    axes = axes.flatten()

    plot_idx = 0

    for cluster_id in unique_labels:
        indices = np.where(labels == cluster_id)[0]

        G = nx.Graph()

        # добавляем узлы
        for i in indices:
            G.add_node(i)

        # добавляем ребра
        if len(indices) >= 15:
            h = nx.Graph()

            for i in indices:
                for j in indices:
                    if i < j:
                        # Преобразует similarity_matrix в distance_matrix (1 = одинаковые --> 0 = одинаковые)
                        w = 1 - similarity[i, j]
                        h.add_edge(i, j, weight=w)

            mst = nx.minimum_spanning_tree(h)

            # возвращаем similarity как вес
            for u, v, d in mst.edges(data=True):
                d["weight"] = 1 - d["weight"]

            G = mst

        else:
            for i in indices:
                for j in indices:
                    if i < j:
                        weight = similarity[i, j]
                        G.add_edge(i, j, weight=weight)

        ax = axes[plot_idx]

        pos = nx.spring_layout(G, seed=42)

        # ребра
        edges = G.edges(data=True)
        widths = [d['weight'] * 3 for (_, _, d) in edges]

        nx.draw_networkx_nodes(G, pos, ax=ax, node_size=300)
        nx.draw_networkx_edges(G, pos, ax=ax, width=widths)

        # подписи узлов
        nx.draw_networkx_labels(G, pos, ax=ax, font_size=8)

        # подписи ребер (только сильные)
        edge_labels = {
            (u, v): f"{d['weight']:.2f}"
            for u, v, d in edges
            if d['weight'] > threshold + 0.1
        }

        nx.draw_networkx_edge_labels(G, pos, edge_labels=edge_labels, ax=ax, font_size=6)

        ax.set_title(f"Cluster {cluster_id} (n={len(indices)})")
        ax.axis("off")
        ax.set_facecolor("white")
        ax.patch.set_edgecolor("black")
        ax.patch.set_linewidth(2)

        plot_idx += 1

    # скрываем пустые subplot'ы
    for i in range(plot_idx, len(axes)):
        axes[i].axis("off")


    plt.show()
