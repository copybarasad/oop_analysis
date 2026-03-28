from build_graph import *
from networkx.algorithms import isomorphism
from fetch_from_dir import load_cpp_project

if __name__ == "__main__":
    # project_path = "/Users/nadya/Desktop/oop-and-etc./src1/lab1"
    project_path1 = "/Users/nadya/Desktop/fqw/oop_analysis/CodeDatabase/14"
    project_path2 = "/Users/nadya/Desktop/fqw/oop_analysis/CodeDatabase/19"
    files1 = load_cpp_project(project_path1)
    files2 = load_cpp_project(project_path2)

    G1 = build_dependency_graph(files1)
    G2 = build_dependency_graph(files2)
    GM = isomorphism.GraphMatcher(G1, G2)
    print(GM.is_isomorphic())

    # print("Связи:", G.edges)
    # for edge in G.edges:
    #     print(f"{edge[0]} → {edge[1]}")

    # plt.figure(figsize=(10, 6))
    # nx.draw(G, with_labels=True, node_size=1200, node_color="lightblue", arrows=True)
    # plt.show()
