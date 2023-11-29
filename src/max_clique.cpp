/**
 * @file max_clique.cpp
 * @brief Tool to calculate distance between .homenda.txt graphs
 */
#include <algorithm>
#include <cstring>
#include <exception>
#include <iostream>
#include <span>

#include "graph.hpp"

using std::cerr;
using std::cout;
using std::exception;
using std::for_each;
using std::span;

/**
 * @brief Max clique between two graphs.
 *
 * @param argc should be >=2
 * @param argv should have the filename to read at [1]\n
 * if it's "-", stdin will be read instead\n
 * if [2] == "approx", an approximate algorithm will be used for the check
 * instead\n
 * if [3] is "dot", it'll convert the output to DOT language
 *
 * @return 0, or 1 for parse errors
 */
auto main(int argc, char* argv[]) -> int {
    auto args = span(argv, static_cast<size_t>(argc));
    if (argc < 2) {
        cerr << "Usage: " << args[0] << " <filename> [approx] [dot]\n";
        return 1;
    }

    Graph graph;
    try {
        graph = Graph::fromFilename(args[1]);
    } catch (const exception& e) {
        cerr << "Oops! [" << e.what() << "]\n";
        return 1;
    }

    AlgorithmAccuracy accuracy = (argc >= 3 && strcmp(args[2], "approx") == 0)
                                     ? AlgorithmAccuracy::APPROXIMATE
                                     : AlgorithmAccuracy::EXACT;

#ifdef DEBUG
    if (accuracy == AlgorithmAccuracy::APPROXIMATE) {
        std::cerr << "Finding approximation of max clique\n";
    }
#endif

    bool dotLang = ((argc >= 3 && strcmp("dot", args[2]) == 0) ||
                    (argc >= 4 && strcmp("dot", args[3]) == 0));

    auto maxClique = graph.maxCliqueGraph(accuracy);

    if (dotLang) {
        cout << maxClique.toDotLang();
    } else {
        cout << maxClique;
    }
}
