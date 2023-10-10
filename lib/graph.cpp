/**
 * @file graph.cpp
 * @brief Graph representation implementations
 */
#include "graph.hpp"

#include <iostream>
#include <sstream>
#include <stdexcept>

using std::cin;
using std::ifstream;
using std::invalid_argument;
using std::istream;
using std::ostream;
using std::string;
using std::vector;

Graph::Graph(istream& graphStream) : Graph{} {
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

auto operator<<(ostream& outputStream, const Graph& graph) -> ostream& {
    outputStream << graph.vertexCount << "\n";
    for (size_t i = 0; i < graph.vertexCount; ++i) {
        for (size_t j = 0; j < graph.vertexCount; ++j) {
            outputStream << graph[i][j] << " ";
        }
        outputStream << "\n";
    }

    return outputStream;
}
