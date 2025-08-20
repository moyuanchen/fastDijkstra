#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include "Graph.h"
#include "Dijkstra.h"
#include "BMSSP.h"
#include "BatchHeap.h"
#include "FindPivot.h"

namespace py = pybind11;

PYBIND11_MODULE(fastdijkstra, m) {
    m.doc() = "Fast Dijkstra and BMSSP algorithms for shortest path computation achieving O(m log^(2/3) n) complexity";

    // Edge struct
    py::class_<Edge>(m, "Edge")
        .def(py::init<>())
        .def_readwrite("dest", &Edge::dest)
        .def_readwrite("weight", &Edge::weight);

    // Graph class
    py::class_<Graph>(m, "Graph")
        .def(py::init<int>())
        .def(py::init<int, const std::vector<std::vector<int>>&>())
        .def(py::init<int, const std::vector<std::vector<int>>&, const std::vector<double>&>())
        .def("getNumVertices", &Graph::getNumVertices, "Get the number of vertices in the graph")
        .def("getConnections", &Graph::getConnections, "Get connections from a source vertex")
        .def("addEdge", &Graph::addEdge, "Add an edge to the graph", 
             py::arg("src"), py::arg("dest"), py::arg("weight") = 1.0)
        .def("calcK", &Graph::calcK, "Calculate K parameter for BMSSP algorithm")
        .def("calcT", &Graph::calcT, "Calculate T parameter for BMSSP algorithm")
        .def("getT", &Graph::getT, "Get T parameter")
        .def("getK", &Graph::getK, "Get K parameter")
        .def("printAdjacencyList", &Graph::printAdjacencyList, "Print the adjacency list");

    // DijkstraResults struct
    py::class_<DijkstraResults>(m, "DijkstraResults")
        .def(py::init<>())
        .def_readwrite("predecessors", &DijkstraResults::predecessors)
        .def_readwrite("distances", &DijkstraResults::distances);

    // BaseCaseResults struct for BMSSP base case
    py::class_<BaseCaseResults>(m, "BaseCaseResults")
        .def(py::init<>())
        .def_readwrite("B", &BaseCaseResults::B)
        .def_readwrite("U", &BaseCaseResults::U);

    // BMSSPResult struct for main BMSSP algorithm
    py::class_<BMSSPResult>(m, "BMSSPResult")
        .def(py::init<>())
        .def_readwrite("new_bound", &BMSSPResult::new_bound)
        .def_readwrite("completed_vertices", &BMSSPResult::completed_vertices);

    // FindPivotResult struct for pivot selection
    py::class_<FindPivotResult>(m, "FindPivotResult")
        .def(py::init<>())
        .def_readwrite("pivots", &FindPivotResult::pivots)
        .def_readwrite("nearby", &FindPivotResult::nearby);

    // Block struct for BatchHeap
    py::class_<Block>(m, "Block")
        .def(py::init<>())
        .def_readwrite("upper_bound", &Block::upper_bound)
        .def_readwrite("block", &Block::block);

    // PullResults struct for BatchHeap operations
    py::class_<PullResults>(m, "PullResults")
        .def(py::init<>())
        .def_readwrite("vertices", &PullResults::vertices)
        .def_readwrite("new_bound", &PullResults::new_bound);

    // Main algorithm functions
    m.def("runDijkstra", &runDijkstra, 
          "Run Dijkstra's algorithm for single-source shortest paths", 
          py::arg("graph"), py::arg("source"));
    
    m.def("runBaseCase", &runBaseCase, 
          "Run base case for BMSSP algorithm using Bellman-Ford-like method", 
          py::arg("graph"), py::arg("src"), py::arg("B"));
    
    m.def("runBMSSP", &runBMSSP, 
          "Run BMSSP recursive algorithm achieving O(m log^(2/3) n) complexity.\n"
          "Main algorithm calls this with parameters:\n"
          "- level = ⌈(log n)/t⌉\n"
          "- S = {s} (single source)\n" 
          "- B = ∞ (infinite bound)\n"
          "The algorithm uses FindPivots and partial sorting for optimal performance.", 
          py::arg("graph"), py::arg("distances"), py::arg("predecessors"),
          py::arg("level"), py::arg("B"), py::arg("S"));

    m.def("findPivots", &findPivots,
          "FindPivots procedure (Algorithm 1) - crucial for BMSSP efficiency.\n"
          "Shows that only at most |U|/k vertices of S are useful in recursive calls.",
          py::arg("graph"), py::arg("B"), py::arg("S"), py::arg("d_hat"));

    // Version info
    m.attr("__version__") = "0.1.0";
}
