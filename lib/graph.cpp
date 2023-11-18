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
#include <unordered_map>

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

[[nodiscard]] auto Graph::approxIsomorphicTo(const Graph& rhs) const -> bool {
    // Lambda function to generate degree frequency map
    auto generateDegreeSequenceMap = [](const Graph& graph) {
        std::unordered_map<int, int> freqMap;

        auto graphAdjacencies = graph.adjacencyMatrix;
        for (size_t i = 0; i < graph.vertexCount; ++i) {
            int rowSum = std::accumulate(graphAdjacencies[i].begin(),
                                         graphAdjacencies[i].end(), 0);
            int colSum = std::accumulate(
                graphAdjacencies.begin(), graphAdjacencies.end(), 0,
                [i](int accumulator, const std::vector<int>& row) {
                    return accumulator + row[i];
                });

            ++freqMap[rowSum + colSum];
        }
        return freqMap;
    };

    std::unordered_map<int, int> freqMap1 = generateDegreeSequenceMap(*this);
    std::unordered_map<int, int> freqMap2 = generateDegreeSequenceMap(rhs);

#if DEBUG
    for (const auto& entry : freqMap1) {
        std::cerr << '(' << entry.first << ": " << entry.second << ") ";
    }
    std::cerr << '\n';

    for (const auto& entry : freqMap2) {
        std::cerr << '(' << entry.first << ": " << entry.second << ") ";
    }
    std::cerr << '\n';
#endif

    // Compare the frequency maps
    return freqMap1 == freqMap2;
}

// We say that Graph equality is true for isomorphisms
[[nodiscard]] auto operator==(const Graph& lhs, const Graph& rhs) -> bool {
    // Right now the implementation of approxIsomorphism will always return true
    // for isomorphic graphs
    if (!lhs.approxIsomorphicTo(rhs)) {
        return false;
    }

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

    bool permutationWasFound = isPermutationOf(lhs, rhs);
    while (!permutationWasFound &&
           std::next_permutation(permutation.begin(), permutation.end())) {
        permutationWasFound = isPermutationOf(lhs, rhs);
    }

    // Show the permuation if we found it
#if DEBUG
    if (permutationWasFound) {
        for (size_t i = 0; i < lhs.vertexCount; ++i) {
            std::cerr << i << "->" << permutation[i] << ", ";
        }

        std::cerr << "\n";
    }
#endif

    return permutationWasFound;
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

[[nodiscard]] auto Graph::isAdjacentToAllNodesInClique(
    int vertex, std::vector<int>& currentClique) const -> bool {
    for (size_t i = 0; i < currentClique.size(); ++i) {
        if (adjacencyMatrix[vertex][currentClique[i]] == 0 ||
            adjacencyMatrix[currentClique[i]][vertex] == 0) {
            return false;
        }
    }
    return true;
}

[[nodiscard]] auto Graph::hasSomeEdgeToAllNodesInClique(
    int vertex, std::vector<int>& currentClique) const -> bool {
    for (size_t i = 0; i < currentClique.size(); ++i) {
        if (adjacencyMatrix[vertex][currentClique[i]] == 0 &&
            adjacencyMatrix[currentClique[i]][vertex] == 0) {
            return false;
        }
    }
    return true;
}

[[nodiscard]] auto Graph::maxCliqueHelper(int currentVertex,
                                          std::vector<int>& currentClique,
                                          std::vector<int>& maxClique) const
    -> void {
    if (currentClique.size() > maxClique.size()) {
        maxClique = vector<int>(currentClique);
    }

    if (currentVertex == vertexCount) {
        return;
    }

    for (int i = currentVertex; i < vertexCount; ++i) {
        if (isAdjacentToAllNodesInClique(i, currentClique)) {
            currentClique.push_back(i);
            maxCliqueHelper(i + 1, currentClique, maxClique);
            currentClique.pop_back();
        }
    }
}

[[nodiscard]] auto Graph::maxClique() const -> std::vector<int> {
    std::vector<int> currentClique;
    std::vector<int> maxClique;
    maxCliqueHelper(0, currentClique, maxClique);
    return maxClique;
}

[[nodiscard]] auto Graph::modifiedMaxCliqueHelper(
    int currentVertex, std::vector<int>& currentClique,
    std::vector<int>& maxClique) const -> void {
    if (currentClique.size() > maxClique.size()) {
        maxClique = std::vector<int>(currentClique);
    }

    if (currentVertex == vertexCount) {
        return;
    }

    for (int i = currentVertex; i < vertexCount; ++i) {
        if (hasSomeEdgeToAllNodesInClique(i, currentClique)) {
            currentClique.push_back(i);
            modifiedMaxCliqueHelper(i + 1, currentClique, maxClique);
            currentClique.pop_back();
        }
    }
}

[[nodiscard]] auto Graph::modifiedMaxClique() const -> std::vector<int> {
    std::vector<int> currentClique;
    std::vector<int> maxClique;
    maxCliqueHelper(0, currentClique, maxClique);
    return maxClique;
}
