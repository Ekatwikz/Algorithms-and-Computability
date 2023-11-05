/**
 * @file graph.cpp
 * @brief Graph representation implementations
 */
#include "graph.hpp"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <sstream>
#include <stdexcept>

using std::cin;
using std::cout;
using std::ifstream;
using std::invalid_argument;
using std::string;
using std::swap;
using std::vector;

Graph::Graph(std::istream& graphStream) : Graph{} {
    // Read the first line to get the number of rows/columns
    if (!(graphStream >> vertexCount)) {
        throw std::invalid_argument("Failed to read matrix size");
    }

    // Initialize the matrix with the specified size
    adjacencyMatrix.resize(vertexCount, std::vector<int>(vertexCount));

    // Read the matrix data from the file
    for (size_t i = 0; i < vertexCount; ++i) {
        for (size_t j = 0; j < vertexCount; ++j) {
            if (!(graphStream >> adjacencyMatrix[i][j])) {
                // TODO: add info about which i,j to this string mebbe?
                throw invalid_argument("Failed to read matrix data");
            }
        }
    }
}

auto Graph::toDotLang() const -> string {
    std::stringstream dotStream;
    dotStream << "digraph {\n";

    for (size_t i = 0; i < vertexCount; ++i) {
        for (size_t j = 0; j < vertexCount; ++j) {
            for (int k = 0; k < adjacencyMatrix[i][j]; ++k) {
                dotStream << "  " << i << " -> " << j << "\n";
            }
        }
    }

    dotStream << "}";

    return dotStream.str();
}

auto Graph::fromFilename(const string& filename) -> Graph {
    Graph graph;
    if ("-" == filename) {
        graph = Graph{cin};
    } else {
        ifstream file{filename};

        if (!file.is_open()) {
            // TODO: add info about filename?
            throw invalid_argument("Failed to open file");
        }

        graph = Graph{file};
        file.close();
    }

    return graph;
}

auto Graph::operator[](size_t row) const -> vector<int> {
    return adjacencyMatrix[row];
}

auto operator<<(std::ostream& outputStream, const Graph& graph)
    -> std::ostream& {
    outputStream << graph.vertexCount << "\n";
    for (size_t i = 0; i < graph.vertexCount; ++i) {
        for (size_t j = 0; j < graph.vertexCount; ++j) {
            outputStream << graph[i][j] << " ";
        }
        outputStream << "\n";
    }

    return outputStream;
}

// We say that Graph equality is true for isomorphisms
[[nodiscard]] auto operator==(const Graph& lhs, const Graph& rhs) -> bool {
    if (lhs.vertexCount != rhs.vertexCount) {
        // Can't have an isomorphism between differently sized graphs
        return false;
    }

    // TODO: Mayybe create hashset of degrees for each graph and compare,
    // would catch a lot more non-isomorphisms earlier, and only costs O(n^2)

    vector<size_t> permutation(lhs.vertexCount);
    std::iota(permutation.begin(), permutation.end(), 0);
    auto isPermutationOf = [&permutation](const Graph& lhs, const Graph& rhs) {
        return std::all_of(
            permutation.begin(), permutation.end(), [&](size_t lhsPos) {
                return std::all_of(
                    permutation.begin(), permutation.end(), [&](size_t rhsPos) {
                        return lhs[lhsPos][rhsPos] ==
                               rhs[permutation[lhsPos]][permutation[rhsPos]];
                    });
            });
    };

    bool foundPermutation = isPermutationOf(lhs, rhs);
    while (!foundPermutation &&
           std::next_permutation(permutation.begin(), permutation.end())) {
        foundPermutation = isPermutationOf(lhs, rhs);
    }

    // Show the permuation if we found it
#if DEBUG
    if (foundPermutation) {
        for (size_t i = 0; i < lhs.vertexCount; ++i) {
            std::cerr << i << "->" << permutation[i] << ", ";
        }

        std::cerr << "\n";
    }
#endif

    return foundPermutation;
}
