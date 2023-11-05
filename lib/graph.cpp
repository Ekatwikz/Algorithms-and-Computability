/**
 * @file graph.cpp
 * @brief Graph representation implementations
 */
#include "graph.hpp"

#include <iostream>
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

    // Check for trivial equality:
    bool foundPermutation = true;
    for (size_t j = 1; foundPermutation && j < lhs.vertexCount; ++j) {
        // We only need to check left of the diagonal because our matrices
        // should have symmetry along it, and the diagonal is also assumed to
        // strictly contain 0s, so we can do j=1 and check onl k<j
        for (size_t k = 0; k < j; ++k) {
            if (lhs[j][k] != rhs[j][k]) {
                foundPermutation = false;
                break;
            }
        }
    }

    // pseudoStack is an encoding of the stack state. pseudoStack[i] encodes the
    // loop counter for when generate(k-1, perm) is called
    vector<size_t> pseudoStack(lhs.vertexCount, 0);
    vector<size_t> perm(lhs.vertexCount);
    for (size_t i = 0; i < lhs.vertexCount; ++i) {
        perm[i] = i;
    }

    // TODO: Move the entire Heap's algorithm stuff to an iterator class?

    // And then check if a permutation is necessary,
    // I use a version of Heap's algorithm
    // (https://en.wikipedia.org/wiki/Heap%27s_algorithm) to go through all n!
    // permutations
    // i acts similarly to a stack pointer for pseudoStack here
    for (size_t i = 1; !foundPermutation && i < lhs.vertexCount;) {
        if (pseudoStack[i] < i) {
            if ((i % 2) != 0U) {
                swap(perm[pseudoStack[i]], perm[i]);
            } else {
                swap(perm[0], perm[i]);
            }

            foundPermutation = true;
            for (size_t j = 0; foundPermutation && j < lhs.vertexCount; ++j) {
                for (size_t k = 0; k < lhs.vertexCount; ++k) {
                    if (lhs[j][k] != rhs[perm[j]][perm[k]]) {
                        foundPermutation = false;
                        break;
                    }
                }
            }

            // Swap has occurred ending the loop. Simulate the increment of the
            // loop counter
            ++pseudoStack[i];

            // Simulate recursive call reaching the base case by bringing the
            // pointer to the base case analog in the array
            i = 1;
        } else {
            // Calling generate(i+1, perm) has ended as the loop terminated.
            // Reset the state and simulate popping the stack by incrementing
            // the pointer.
            // loop counter
            pseudoStack[i] = 0;
            ++i;
        }
    }

    // Show the permuation if we found it
#if DEBUG
    if (foundPermutation) {
        for (size_t i = 0; i < lhs.vertexCount; ++i) {
            std::cerr << i << "->" << perm[i] << ", ";
        }

        std::cerr << "\n";
    }
#endif

    return foundPermutation;
}
