/**
 * @file modular_product.cpp
 * @brief Tool to calculate modular product of two graphs
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
 * @brief Computes modular product of two graphs
 *
 * @param argc should be >=2
 * @param argv should have the filename to read at [1] and [2]\n
 * if either are "-", stdin will be read instead for that one\n
 *
 * @return 0, or 1 for parse errors
 */
auto main(int argc, char* argv[]) -> int {
    auto args = span(argv, static_cast<size_t>(argc));
    if (argc < 2) {
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

    Graph modProduct = lhs.modularProduct(rhs);
    if (argc < 2 || strcmp("dot", args[2]) != 0) {
        cout << modProduct;
    } else {
        cout << modProduct.toDotLang();
    }
}
