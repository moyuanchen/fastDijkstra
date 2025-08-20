"""
Example usage of the fastdijkstra package.

This demonstrates the BMSSP algorithm which achieves O(m log^(2/3) n) complexity
through recursive calls with partial sorting and FindPivots procedure.
"""

import fastdijkstra as fd
import math

def example_dijkstra():
    """Example of using Dijkstra's algorithm."""
    print("=== Dijkstra's Algorithm Example ===")

    # Create a graph with 5 vertices
    # Graph structure:
    #   0 --2--> 1 --1--> 2
    #   |        |        |
    #   4        7        3
    #   |        |        |
    #   v        v        v
    #   3 <--1-- 4 --2--> 2

    graph = fd.Graph(5)
    graph.addEdge(0, 1, 2.0)
    graph.addEdge(0, 3, 4.0)
    graph.addEdge(1, 2, 1.0)
    graph.addEdge(1, 4, 7.0)
    graph.addEdge(2, 4, 2.0)
    graph.addEdge(4, 3, 1.0)

    # Run Dijkstra from vertex 0
    result = fd.runDijkstra(graph, 0)

    print(f"Shortest distances from vertex 0:")
    for i, dist in enumerate(result.distances):
        print(f"  To vertex {i}: {dist}")

    print(f"Predecessors:")
    for i, pred in enumerate(result.predecessors):
        print(f"  Vertex {i}: predecessor {pred}")

def example_bmssp_main_algorithm():
    """
    Example of the main BMSSP algorithm.

    The main algorithm calls BMSSP with parameters:
    - l = ⌈(log n)/t⌉ (initial level)
    - S = {s} (single source vertex)
    - B = ∞ (infinite bound)

    This achieves O(m log^(2/3) n) complexity through:
    - Partial sorting with heap-like data structures
    - FindPivots procedure limiting useful vertices to |U|/k
    - Recursive calls with factor of roughly 1/2t reduction
    """
    print("\n=== BMSSP Main Algorithm Example ===")

    # Create a larger graph for BMSSP demonstration
    graph = fd.Graph(8)
    edges_data = [
        (0, 1, 1.0), (0, 2, 4.0), (1, 2, 2.0), (1, 3, 5.0),
        (2, 3, 1.0), (2, 4, 3.0), (3, 4, 1.0), (3, 5, 2.0),
        (4, 5, 1.0), (5, 6, 2.0), (6, 7, 1.0), (4, 7, 4.0)
    ]

    for src, dest, weight in edges_data:
        graph.addEdge(src, dest, weight)

    # Calculate graph parameters K and T for optimal performance
    graph.calcK()
    graph.calcT()

    n = graph.getNumVertices()
    t = graph.getT()
    k = graph.getK()

    print(f"Graph: {n} vertices, K={k}, T={t}")

    # Initialize distance and predecessor arrays
    distances = [float('inf')] * n
    predecessors = [-1] * n
    source = 0
    distances[source] = 0.0

    # Main algorithm parameters as specified:
    # l = ⌈(log n)/t⌉, S = {s}, B = ∞
    level = math.ceil(math.log(n) / t) if t > 0 else 1
    S = [source]  # Single source vertex
    B = float('inf')  # Infinite bound

    print(f"BMSSP parameters: level={level}, source={source}, bound=∞")
    print(f"Expected complexity: O(m log^(2/3) n)")

    # Run the main BMSSP algorithm
    result = fd.runBMSSP(graph, distances, predecessors, level, B, S)

    print(f"BMSSP completed with new_bound={result.new_bound}")
    print(f"Completed vertices: {len(result.completed_vertices)}")

    print(f"Final shortest distances from vertex {source}:")
    for i, dist in enumerate(distances):
        if dist != float('inf'):
            print(f"  To vertex {i}: {dist}")
        else:
            print(f"  To vertex {i}: unreachable")

def example_bmssp_components():
    """Example showing individual BMSSP components."""
    print("\n=== BMSSP Components Example ===")

    # Create test graph
    graph = fd.Graph(6)
    edges_data = [
        (0, 1, 1.0), (0, 2, 4.0), (1, 2, 2.0), (1, 3, 5.0),
        (2, 3, 1.0), (2, 4, 3.0), (3, 4, 1.0), (3, 5, 2.0),
        (4, 5, 1.0)
    ]

    for src, dest, weight in edges_data:
        graph.addEdge(src, dest, weight)

    graph.calcK()
    graph.calcT()

    # Demonstrate base case (Bellman-Ford-like method)
    print("Base case demonstration:")
    base_result = fd.runBaseCase(graph, 0, 10.0)
    print(f"  Base case result: B={base_result.B}, |U|={len(base_result.U)}")
    print(f"  U vertices: {sorted(list(base_result.U))}")

    # Demonstrate FindPivots procedure
    print("\nFindPivots procedure demonstration:")
    print("(FindPivots shows that only ≤ |U|/k vertices of S are useful)")

    # Note: This would require proper setup of S and d_hat vectors
    # which depends on the current state of the algorithm
    print("  FindPivots is called internally during BMSSP recursion")
    print("  It limits the number of useful source vertices to |U|/k")
    print("  This is crucial for maintaining the O(m log^(2/3) n) complexity")

def example_algorithm_analysis():
    """Show the algorithmic complexity analysis."""
    print("\n=== BMSSP Algorithm Analysis ===")
    print("Key algorithmic insights:")
    print("1. Recursion levels bounded by O((log n)/t)")
    print("2. Each level selects ~2^(l-1)*t vertices for next recursive call")
    print("3. FindPivots limits useful vertices to |U|/k")
    print("4. Partial sorting with heap-like structures improves performance")
    print("5. Problem size reduces by factor ~1/2t per recursive call")
    print("6. Total complexity: O(m log^(2/3) n)")
    print("\nThe algorithm avoids full sorting of S across all levels,")
    print("which would provide no improvement over standard approaches.")

if __name__ == "__main__":
    try:
        example_dijkstra()
        example_bmssp_main_algorithm()
        example_bmssp_components()
        example_algorithm_analysis()
    except ImportError as e:
        print(f"Error: {e}")
        print("Please build the package first using:")
        print("  pip install -e .")
        print("or:")
        print("  python setup.py build_ext --inplace")
