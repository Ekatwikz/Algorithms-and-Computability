/**
 * @file parse_graph.cpp
 * @brief Tool to read and convert .homenda.txt files
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
 * @brief Graph parser
 *
 * Takes a graph, reads it, spits it back out\n
 * Decent for some simple sanity tests\n
 * Can also be used to generate visuals\n
 *
 * @param argc should be >=2
 * @param argv should have the filename to read at [1]\n
 * if this is "-", it will read stdin instead\n
 * if [2] is "dot", it'll convert the output to DOT language\n
 * piping this to dot (from graphviz) can generate images\n
 * Eg:\n
 * <code>/path/to/[this-binary-name] /path/to/some-graph.homenda.txt | dot -Tpng
 * -o something.png</code>
 *
 * @return 0, or 1 if something got goofed up
 * @see https://graphviz.org/#what-is-graphviz
 */
auto main(int argc, char* argv[]) -> int {
    auto args = span(argv, static_cast<size_t>(argc));
    if (argc < 2) {
        cerr << "Usage: " << args[0] << " <filename>\n";
        return 1;
    }

    Graph graph;
    try {
        graph = Graph::fromFilename(args[1]);
    } catch (const exception& e) {
        cerr << "Oops! [" << e.what() << "]\n";
        return 1;
    }

    // convert to dot if we're told to
    if (argc < 3 || strcmp("dot", args[2]) != 0) {
        cout << graph;
    } else {
        cout << graph.toDotLang();
    }
}
