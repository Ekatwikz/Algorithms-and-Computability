/**
 * @file graph_distance.cpp
 * @brief Tool to calculate distance between .homenda.txt graphs
 */
#include <algorithm>
#include <cstring>
#include <exception>
#include <iostream>
#include <span>

#include "graph.hpp"

using std::cerr;
using std::exception;
using std::span;

/**
 * @brief Max clique between two graphs.
 *
 * @param argc should be >=2
 * @param argv should have the filename to read at [1] and [2]\n
 * if either are "-", stdin will be read instead for that one\n
 * if [3] == "approx", an approximate algorithm will be used for the check
 * instead
 *
 * @return 0, or 1 for parse errors
 */
auto main(int argc, char* argv[]) -> int {
    auto args = span(argv, static_cast<size_t>(argc));
    if (argc < 2) {
        cerr << "Usage: " << args[0] << " <filename1>\n";
        return 1;
    }

    Graph g;
    try {
        g = Graph::fromFilename(args[1]);
    } catch (const exception& e) {
        cerr << "Oops! [" << e.what() << "]\n";
        return 1;
    }

    auto maxClique = g.maxClique();
    std::cout << "Max clique size: " << maxClique.size() << std::endl;
    std::cout << "Vertices of the max clique: {";
    std::for_each(maxClique.begin(), maxClique.end() - 1,
                  [](int vertex) { std::cout << vertex << ", "; });
    std::cout << maxClique.back() << "}" << std::endl;

    std::cout << g.toDotLang() << std::endl;
}
