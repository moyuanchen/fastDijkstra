#include "BMSSP.h"
#include "Graph.h"
#include <queue>
#include <vector>

BaseCaseResults runBaseCase(Graph& graph, int B) {
    int numVertices = graph.getNumVertices();
    int k = graph.getK();
    std::vector<double> distances(numVertices, std::numeric_limits<double>::max());
    std::vector<int> predecessors(numVertices , -1);
    // define pq
    using State = std::pair<double, int>;
    std::priority_queue<State, std::vector<State>, std::greater<State>> pq;

    distances[source] = 0.0;
    pq.push({0.0, source});

    int settled_nodes = 0;
    // The loop now has two conditions, just as you suggested
    while (!pq.empty() && settled_nodes < k + 1) {
        double distance = pq.top().first;
        int vertex = pq.top().second;
        pq.pop();

        // This check prevents us from processing an outdated, longer path to a vertex
        if (distance > distances[vertex]) {
            continue;
        }

        // We have now settled this vertex
        settled_nodes++;

        // ... the rest of the logic to relax neighbors ...
    }
    
    // ... return the results ...


}