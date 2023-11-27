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

Graph::Graph(const std::vector<std::vector<int>>&& adjacencyMatrix)
    : vertexCount{adjacencyMatrix.size()},
      vertexAndEdgeCount{adjacencyMatrix.size()},
      adjacencyMatrix{adjacencyMatrix} {
    for (size_t i = 0; i < vertexCount; ++i) {
        for (size_t j = 0; j < vertexCount; ++j) {
            vertexAndEdgeCount += adjacencyMatrix[i][j];
        }
    }
}

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

[[nodiscard]] auto Graph::modularProduct(const Graph& rhs) -> Graph {
    size_t rhsVertexCount = rhs.vertexCount;
    size_t resultGraphVertexCount = vertexCount * rhsVertexCount;
    size_t minVertexCount = std::min(vertexCount, rhsVertexCount);
    size_t maxVertexCount = std::max(vertexCount, rhsVertexCount);

    vector<vector<int>> adjacencyMatrixOfResultGraph(
        resultGraphVertexCount, vector<int>(resultGraphVertexCount));

    for (size_t row = 0; row < resultGraphVertexCount; row++) {
        for (size_t col = 0; col < resultGraphVertexCount; col++) {
            size_t lhsRow = 0;
            size_t lhsCol = 0;
            size_t rhsRow = 0;
            size_t rhsCol = 0;

            if (vertexCount == minVertexCount) {
                lhsRow = row / maxVertexCount;
                lhsCol = col / maxVertexCount;
                rhsRow = row % maxVertexCount;
                rhsCol = col % maxVertexCount;
            } else {
                lhsRow = row / minVertexCount;
                lhsCol = col / minVertexCount;
                rhsRow = row % minVertexCount;
                rhsCol = col % minVertexCount;
            }

            if (row == col || rhsRow == rhsCol || lhsRow == lhsCol) {
                continue;
            }

            adjacencyMatrixOfResultGraph[row][col] =
                std::min(adjacencyMatrix[lhsRow][lhsCol], rhs[rhsRow][rhsCol]);

            if (adjacencyMatrix[lhsRow][lhsCol] == 0 &&
                rhs[rhsRow][rhsCol] == 0) {
                adjacencyMatrixOfResultGraph[row][col] = 1;
            }
        }
    }

    return Graph(std::move(adjacencyMatrixOfResultGraph));
}

[[nodiscard]] auto Graph::maxClique(bool estimation) const
    -> std::vector<size_t> {
    std::vector<size_t> currentClique;
    std::vector<std::vector<size_t>> maxCliques{{}};
    size_t currentExecution = 0;
    size_t maxExecutionLimit = ESTIMATE_MULTIPLIER * vertexCount * vertexCount;

    maxCliqueHelper(
        0, currentClique, maxCliques, estimation, currentExecution,
        maxExecutionLimit, [&](int vertex, auto currentClique) {
            return std::all_of(
                currentClique.begin(), currentClique.end(),
                [&](int cliqueVertex) {
                    return adjacencyMatrix[vertex][cliqueVertex] > 0 &&
                           adjacencyMatrix[cliqueVertex][vertex] > 0;
                });
        });
    return maxCliques[0];
}

auto Graph::maxCliqueHelper(size_t currentVertex,
                            std::vector<size_t>& currentClique,
                            std::vector<std::vector<size_t>>& maxCliques,
                            bool estimation, size_t& currentExecution,
                            size_t executionLimit, auto adjacencyFunction) const
    -> void {
    if (currentClique.size() > maxCliques[0].size()) {
        maxCliques.clear();
        maxCliques = {std::vector<size_t>(currentClique)};
    } else if (currentClique.size() == maxCliques[0].size()) {
        maxCliques.push_back(currentClique);
    }

    if (currentVertex == vertexCount) {
        return;
    }

    if (estimation) {
        if (++currentExecution >= executionLimit) {
            return;
        }
    }

    for (size_t i = currentVertex; i < vertexCount; ++i) {
        if (adjacencyFunction(i, currentClique)) {
            currentClique.push_back(i);
            maxCliqueHelper(i + 1, currentClique, maxCliques, estimation,
                            currentExecution, executionLimit,
                            adjacencyFunction);
            currentClique.pop_back();
        }
    }
}

[[nodiscard]] auto Graph::maxSubgraph(const Graph& rhs) -> Graph {
    Graph modProd = this->modularProduct(rhs);
    std::vector<size_t> maxClique = modProd.modifiedMaxClique();
    size_t maxCliqueSize = maxClique.size();

    std::vector<std::vector<int>> maxSubgraphAdjacencyMatrix(
        maxCliqueSize, std::vector<int>(maxCliqueSize));

    for (size_t row = 0; row < maxCliqueSize; row++) {
        for (size_t col = 0; col < maxCliqueSize; col++) {
            maxSubgraphAdjacencyMatrix[row][col] =
                modProd[maxClique[row]][maxClique[col]];
        }
    }

    // tweeks for isolated vertices
    int divisor = static_cast<int>(
        vertexCount == std::min(vertexCount, rhs.getVertexCount())
            ? vertexCount
            : rhs.getVertexCount());
    divisor = std::max(divisor, 1);

    for (size_t i = 0; i < maxCliqueSize; i++) {
        maxClique[i] /= divisor;
    }

    for (size_t row = 0; row < maxCliqueSize; row++) {
        for (size_t col = 0; col < maxCliqueSize; col++) {
            maxSubgraphAdjacencyMatrix[row][col] =
                std::min(adjacencyMatrix[maxClique[row]][maxClique[col]],
                         maxSubgraphAdjacencyMatrix[row][col]);
        }
    }

    return Graph(std::move(maxSubgraphAdjacencyMatrix));
}

[[nodiscard]] auto Graph::edgeCount(const std::vector<size_t>& clique) const
    -> size_t {
    size_t edgeCount = 0;

    for (size_t i = 0; i < clique.size(); ++i) {
        for (size_t j = i + 1; j < clique.size(); ++j) {
            edgeCount +=
                static_cast<size_t>(adjacencyMatrix[clique[i]][clique[j]]);
            edgeCount +=
                static_cast<size_t>(adjacencyMatrix[clique[j]][clique[i]]);
        }
    }

    return edgeCount;
}

[[nodiscard]] auto Graph::totalConnections(
    const std::vector<size_t>& clique) const -> size_t {
    size_t totalWeight = 0;

    for (size_t i = 0; i < clique.size(); ++i) {
        for (size_t j = i + 1; j < clique.size(); ++j) {
            totalWeight +=
                static_cast<size_t>(adjacencyMatrix[clique[i]][clique[j]] > 0);
            totalWeight +=
                static_cast<size_t>(adjacencyMatrix[clique[j]][clique[i]] > 0);
        }
    }

    return totalWeight;
}

[[nodiscard]] auto Graph::modifiedMaxClique(bool estimation) const
    -> std::vector<size_t> {
    std::vector<size_t> currentClique;
    std::vector<std::vector<size_t>> maxCliques{{}};
    size_t currentExecution = 0;
    size_t maxExecutionLimit = ESTIMATE_MULTIPLIER * vertexCount * vertexCount;

    maxCliqueHelper(
        0, currentClique, maxCliques, estimation, currentExecution,
        maxExecutionLimit, [&](int vertex, auto currentClique) {
            return std::all_of(
                currentClique.begin(), currentClique.end(),
                [&](int cliqueVertex) {
                    return adjacencyMatrix[vertex][cliqueVertex] > 0 ||
                           adjacencyMatrix[cliqueVertex][vertex] > 0;
                });
        });

    return *std::max_element(maxCliques.begin(), maxCliques.end(),
                             [this](const auto& lhs, const auto& rhs) {
                                 auto connections1 = totalConnections(lhs);
                                 auto connections2 = totalConnections(rhs);
                                 if (connections1 == connections2) {
                                     return edgeCount(lhs) < edgeCount(rhs);
                                 }

                                 return connections1 < connections2;
                             });
}
