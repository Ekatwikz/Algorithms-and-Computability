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
    vertexAndEdgeCount = vertexCount;
    adjacencyMatrix.resize(vertexCount, std::vector<int>(vertexCount));

    // Read the matrix data from the file
    for (size_t i = 0; i < vertexCount; ++i) {
        for (size_t j = 0; j < vertexCount; ++j) {
            if (!(graphStream >> adjacencyMatrix[i][j])) {
                // TODO: add info about which i,j to this string mebbe?
                throw invalid_argument("Failed to read matrix data");
            }

            vertexAndEdgeCount += adjacencyMatrix[i][j];
        }
    }
}

[[nodiscard]] auto Graph::getSize() const -> size_t {
    return vertexAndEdgeCount;
}

auto Graph::toDotLang() const -> string {
    std::stringstream dotStream;
    dotStream << "digraph {\n";

    for (size_t i = 0; i < vertexCount; ++i) {
        for (size_t j = 0; j < vertexCount; ++j) {
            // TODO: use some modern c++ iteration over the matrix here?
            for (int k = 0; k < adjacencyMatrix[i][j]; ++k) {
                dotStream << "  " << i << " -> " << j << "\n";
            }
        }
    }

    dotStream << "}";

    return dotStream.str();
}

auto Graph::fromFilename(const string& filename) -> Graph {
    if ("-" == filename) {
        return Graph{cin};
    }

    ifstream file{filename};
    if (!file.is_open()) {
        // TODO: add info about filename?
        throw invalid_argument("Failed to open file");
    }

    Graph graph = Graph{file};
    file.close();
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

[[nodiscard]] auto Graph::approxIsomorphicTo(const Graph& rhs) const -> bool {
    // TODO: flesh out
    return getSize() == rhs.getSize();
}

[[nodiscard]] auto Graph::metricDistanceTo(const Graph& rhs,
                                           AlgorithmAccuracy accuracy) const
    -> size_t {
    // No idea why std::abs is ambiguous here tbh
    auto absSizeDiff = std::abs(static_cast<int64_t>(
        getSize() > rhs.getSize() ? getSize() - rhs.getSize()
                                  : rhs.getSize() - getSize()));

    // it's (maybe?) important to note that the compiler
    // can't actually optimize for the case of sizeDiff>0, and that little
    // isomorphism check for graphs is extremely expensive
    if (absSizeDiff > 0) {
        return absSizeDiff;
    }

    // 0 for ismorphisms, 1 otherwise if we get to this point
    return 1 - static_cast<size_t>(accuracy == AlgorithmAccuracy::EXACT
                                       ? *this == rhs
                                       : approxIsomorphicTo(rhs));
}
