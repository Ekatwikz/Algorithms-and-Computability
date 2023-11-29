/**
 * @file isomorphism_check.cpp
 * @brief Tool to check for isomorphisms between .homenda.txt graphs
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
 * @brief Isomorphism checker
 *
 * Takes 2 graphs and checks whether they're isomorphic\n
 * Also prints the mapping that represents the isomorphism to stderr
 * if DEBUG is set
 *
 * @param argc should be >=3
 * @param argv should have the filename to read at [1] and [2]\n
 * if either are "-", stdin will be read instead for that one\n
 * if [3] == "approx", an approximate algorithm will be used for the check
 * instead
 *
 * @return 0, or 1 for parse errors
 */
auto main(int argc, char* argv[]) -> int {
    auto args = span(argv, static_cast<size_t>(argc));
    if (argc < 3) {
        cerr << "Usage: " << args[0] << " <filename1> <filename2>\n";
        return 1;
    }

    Graph lhs;
    Graph rhs;
    try {
        lhs = Graph::fromFilename(args[1]);
        rhs = Graph::fromFilename(args[2]);
    } catch (const exception& e) {
        cerr << "Oops! [" << e.what() << "]\n";
        return 1;
    }

    bool isomorphismExists = argc >= 4 && strcmp(args[3], "approx") == 0
                                 ? lhs.approxIsomorphicTo(rhs)

                                 // NB: This is an overridden operator
                                 : lhs == rhs;
    cout << (isomorphismExists ? "" : "NOT ") << "Isomorphic.\n";
}
