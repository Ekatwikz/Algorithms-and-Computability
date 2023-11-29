/**
 * @file max_subgraph.cpp
 * @brief Tool to calculate maximum induced subgraph of two graphs
 */
#include <cstring>
#include <exception>
#include <iostream>
#include <span>

#include "graph.hpp"

using std::cerr;
using std::cout;
using std::exception;
using std::span;

/**
 * @brief Computes maximum induced subgraph of two graphs
 *
 * @param argc should be >=4
 * @param argv should have the filename to read at [1] and [2]\n
 * if either are "-", stdin will be read instead for that one\n
 * if [3] == "approx", an approximate algorithm will be used for the check
 * instead
 * if [4] is "dot", it'll convert the output to DOT language
 *
 * @return 0, or 1 for parse errors
 */
auto main(int argc, char* argv[]) -> int {
    auto args = span(argv, static_cast<size_t>(argc));
    if (argc < 3) {
        cerr << "Usage: " << args[0]
             << " <filename1> <filename2> [accuracy] [dot]\n";
        return 1;
    }

    const int maxNumberOfArgs = 5;

    Graph lhs;
    Graph rhs;
    try {
        lhs = Graph::fromFilename(args[1]);
        rhs = Graph::fromFilename(args[2]);
    } catch (const exception& e) {
        cerr << "Oops! [" << e.what() << "]\n";
        return 1;
    }

    AlgorithmAccuracy accuracy = (argc >= 4 && strcmp(args[3], "approx") == 0)
                                     ? AlgorithmAccuracy::APPROXIMATE
                                     : AlgorithmAccuracy::EXACT;

    Graph maxSubgraph = lhs.maxSubgraph(rhs, accuracy);
    if ((argc >= 4 && strcmp("dot", args[3]) == 0) ||
        (argc >= maxNumberOfArgs && strcmp("dot", args[4]) == 0)) {
        cout << maxSubgraph.toDotLang();
    } else {
        cout << maxSubgraph;
    }
}
