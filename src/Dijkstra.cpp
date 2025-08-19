#include "Dijkstra.h"
#include <queue>
#include <vector>
#include <limits>

DijkstraResults runDijkstra(Graph& graph, int source) {
    int numVertices = graph.getNumVertices();
    std::vector<double> distances(numVertices, std::numeric_limits<double>::max());
    std::vector<int> predecessors(numVertices , -1);

    // define pq
    using State = std::pair<double, int>;
    std::priority_queue<State, std::vector<State>, std::greater<State>> pq;

    distances[source] = 0.0;
    pq.push({0.0, source});

    while (!pq.empty()) {
        int d = pq.top().first; int v = pq.top().second; // src vertex
        pq.pop();

        for (const auto& edge : graph.getConnections(v)) {
            double altWeight = edge.weight + d; int u = edge.dest;
            if (altWeight < distances[u]) {
                distances[u] = altWeight;
                predecessors[u] = v;
                pq.push({altWeight, u});
            }
        }
    }
    DijkstraResults result;
    result.distances = distances;
    result.predecessors = predecessors;
    return result;
}