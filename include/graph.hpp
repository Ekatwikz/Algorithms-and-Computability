/**
 * @file graph.hpp
 * @brief Graph representation definitions
 */
#include <fstream>
#include <functional>
#include <vector>

/**
 * @brief Little enum for choosing between approximate and exact algorithms
 */
enum class AlgorithmAccuracy { APPROXIMATE, EXACT };

/**
 * @brief Class that represents a graph as a matrix (vector of vectors) of
 * adjacencies
 */
class Graph {
   private:
    /**
     * @brief Number of vertices in the graph
     */
    size_t vertexCount;

    /**
     * @brief The size of the graph
     *
     * NB: This isn't necessarily == No. of vertices nor edges!
     */
    size_t vertexAndEdgeCount;

    /**
     * @brief The adjacency matrix of the graph
     *
     * A size*size adjacency matrix representation of the graph
     */
    std::vector<std::vector<int>> adjacencyMatrix;

   public:
    /**
     * @brief Default constructor
     *
     * Not sure if this should actually be default-constructible tho...
     * And even then, should it be =default?
     */
    Graph() : vertexCount{0}, vertexAndEdgeCount{0} {}

    /**
     * @brief constructs graph from provided adjacency matrix
     *
     * @param adjacencyMatrix is the adjacencyMatrix from which graph will be
     * constructed
     */
    explicit Graph(const std::vector<std::vector<int>>&& adjacencyMatrix);

    /**
     * @brief constructs graph from data in a stream
     *
     * @param graphStream is the stream to be read from
     *
     * @warning throws <a
     * href="https://en.cppreference.com/w/cpp/error/invalid_argument">invalid_argument</a>
     * if it fails to read the stream at any point
     */
    explicit Graph(std::istream& graphStream);

    /**
     * @brief constructs graph from data in a stream
     *
     * NB: This directly just calls the lvalue reference constructor\n
     * I can't remember if that's okay tbh
     *
     * @param graphStream is the stream to be read from
     */
    explicit Graph(std::istream&& graphStream) : Graph{graphStream} {}

    /**
     * @brief constructs graph from data in specified filename
     *
     * This maayybe should be a constructor somehow? not sure
     *
     * @param filename is the filename to stream from
     * if the filename is "-", this will read from stdin
     *
     * @return A new graph
     *
     * @warning throws <a
     * href="https://en.cppreference.com/w/cpp/error/invalid_argument">invalid_argument</a>
     * if it couldn't open the file
     */
    [[nodiscard]] static auto fromFilename(const std::string& filename)
        -> Graph;

    /**
     * @brief Return vertex count.
     * @return Number of vertices in the graph.
     */
    [[nodiscard]] auto getVertexCount() const -> size_t { return vertexCount; }

    /**
     * @brief returns string in DOT language
     *
     * Useful for generating visualization and stuff
     *
     * @return dotLang string, just "digraph{[i->j]...}" with no attributes or
     * any of the fancy language features used
     *
     * @see <a href="https://www.graphviz.org/doc/info/lang.html">DOT
     * language</a>
     */
    [[nodiscard]] auto toDotLang() const -> std::string;

    /**
     * @brief subscript operator, for convenience
     *
     * @param row the row number from the adjacency matrix
     *
     * @return the row xd
     */
    [[nodiscard]] auto operator[](size_t row) const -> std::vector<int>;

    /**
     * @brief for debugging convenience
     *
     * @param outputStream the stream to write to
     * @param graph the graph. (<a
     * href="https://www.youtube.com/watch?v=0mfJn604GT4">?</a>)
     *
     * @return the outputStream
     */
    friend auto operator<<(std::ostream& outputStream, const Graph& graph)
        -> std::ostream&;

    /**
     * @brief checks *isomorphisms* between 2 graphs
     *
     * @param lhs the lhs of A == B
     * @param rhs the rhs, lol
     *
     * @return `true` iff the pair is isomorphic
     */
    friend auto operator==(const Graph& lhs, const Graph& rhs) -> bool;

    /**
     * @brief checks for *approximate* isomorphism between 2 graphs
     *
     * The current implementation *always returns true for isomorphic graphs*,
     * but will return true for graphs with equal multisets of vertex degrees
     * anyway
     *
     * @param rhs the rhs of A approx== B
     *
     * @return `true` iff the pair is "approximately isomorphic"
     */
    [[nodiscard]] auto approxIsomorphicTo(const Graph& rhs) const -> bool;

    /**
     * @brief Returns size
     *
     * NB: This is ***NOT*** necessarily == vertex count nor edge count
     *
     * @return The size of the graph, whichever way we end up defining it
     */
    [[nodiscard]] auto getSize() const -> size_t;

    /**
     * @brief Returns distance to another graph
     *
     * @param rhs is the second argument of the metric
     * @param accuracy is whether we should approximate the slower parts of the
     * calculation,\n
     * this parameter is optional
     *
     * "Distance" in this case is defined by d(G1, G2),\n
     * where d (for now) is
     * \f$ max(\big| \big|  G1 \big|  - \big| G2 \big| \big|, 1)
     * * (1 - (G1 \cong G2)) \f$\n
     * and \f$\cong\f$ checks for isomorphism between graphs and converts the\n
     * resulting bool to 0 or 1 respectively
     *
     * @return The distance to the RHS
     */
    [[nodiscard]] auto metricDistanceTo(
        const Graph& rhs,
        AlgorithmAccuracy accuracy = AlgorithmAccuracy::EXACT) const -> size_t;

    /**
     * @brief Returns modular product of two graphs
     *
     * @param rhs is the graph with which the modular
     * product should be applied
     *
     * @return The modular product of the graphs
     */

    [[nodiscard]] auto modularProduct(const Graph& rhs) -> Graph;
    /**
     * @brief Checks if the given vertex is adjacent to all vertices in the
     * given clique.
     *
     * @param vertex Vertex to check.
     * @param currentClique Clique to check.
     *
     * @return True if the vertex is adjacent to all vertices in the clique,
     * false otherwise.
     */
    [[nodiscard]] auto isAdjacentToAllNodesInClique(
        size_t vertex, std::vector<int>& currentClique) const -> bool;
    /**
     * @brief Checks if the given vertex has one sided edge to all vertices in
     * the given clique.
     *
     * @param vertex Vertex to check.
     * @param currentClique Clique to check.
     *
     * @return True if the vertex has some edge to all vertices in the clique,
     * false otherwise.
     */
    [[nodiscard]] auto hasSomeEdgeToAllNodesInClique(
        size_t vertex, std::vector<int>& currentClique) const -> bool;

    /**
     * @brief Helper for maxClique, used for recursion.
     *
     * @param currentVertex Current vertex to check.
     * @param vertex Vertex to check.
     * @param currentClique Clique to check.
     */

    auto maxCliqueHelper(size_t currentVertex, std::vector<int>& currentClique,
                         std::vector<int>& maxClique) const -> void;
    /**
     * @brief Finds the maximum clique of the graph using Bron-Kerbosch
     * algorithm.
     *
     * @return Vector of vertices that form the maximum clique.
     */
    [[nodiscard]] auto maxClique() const -> std::vector<int>;

    /**
     * @brief Helper for modifiedMaxClique, used for recursion.
     *
     * @param currentVertex Current vertex to check.
     * @param vertex Vertex to check.
     * @param currentClique Clique to check.
     */
    auto modifiedMaxCliqueHelper(size_t currentVertex,
                                 std::vector<int>& currentClique,
                                 std::vector<int>& maxClique) const -> void;

    /**
     * @brief modidfied max clique algorithm for finding maximum induced
     * subgraphs.
     *
     * @return Vector of vertices that form the maximum clique.
     */
    [[nodiscard]] auto modifiedMaxClique() const -> std::vector<int>;
};
