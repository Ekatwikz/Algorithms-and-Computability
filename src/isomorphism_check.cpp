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
 * Also prints the mapping that represents the isomorphism to stderr\n
 * TODO: ^if DEBUG is set
 *
 * @param argc should be >=3
 * @param argv should have the filename to read at [1] and [2]\n
 * if either are "-", stdin will be read instead
 *
 * @return 0, or 1 for parse errors
 */
auto main(int argc, char* argv[]) -> int {
    // get filename superdupermodernly lol, probably might change this
    // also, TODO: use getopt here?
    auto args = span(argv, static_cast<size_t>(argc));
    if (argc < 3) {
        cerr << "Usage: " << args[0] << " <filenam1> <filename2>\n";
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

    // NB: This is an overriden operator
    cout << (lhs == rhs ? "" : "NOT ") << "Isomorphic.\n";
}
