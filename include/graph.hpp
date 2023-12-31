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

    /**
     * @brief constant used for finding estimate in maxClique.
     */
    static constexpr size_t ESTIMATE_MULTIPLIER = 10;

    /**
     * @brief Helper for maxClique, used for recursion.
     *
     * @param currentVertex Current vertex to check.
     * @param currentClique Clique to check.
     * @param maxCliques Maximum cliques of the graph.
     * @param estimation To check if an estimation of max clique is
     * required.
     * @param currentExecution Keeps track of the current execution. Used for
     * estimation.
     * @param executionLimit Maximum executions allowed. Used for estimation.
     * @param adajcencyFunction Checks if a vertex is adjacent to all the
     * vertices in the clique.
     */
    auto maxCliqueHelper(size_t currentVertex,
                         std::vector<size_t>& currentClique,
                         std::vector<std::vector<size_t>>& maxCliques,
                         AlgorithmAccuracy accuracy, size_t& currentExecution,
                         size_t executionLimit, auto adjacencyFunction) const
        -> void;

    /**
     * @brief Checks the number of connections in a clique. Used for
     * finding max induced subgraph.
     *
     * @param clique The clique whose connections are being checked.
     *
     * @return Number of connections in the clique.
     */
    [[nodiscard]] auto totalConnections(const std::vector<size_t>& clique) const
        -> size_t;

    /**
     * @brief Returns the sum of edge in the clique. Used for
     * finding max induced subgraph.
     *
     * @param clique The clique whose connections are being checked.
     *
     * @return Number of edges in the clique.
     */
    [[nodiscard]] auto edgeCount(const std::vector<size_t>& clique) const
        -> size_t;

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
     * @brief Returns graph size
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
     * "Distance" in this case is defined by \f$d(G_1, G_2)\f$,\n
     * where \f$d\f$ (for now) is
     * \f$ max(\big| \big|  G_1 \big|  - \big| G_2 \big| \big|, 1)
     * * (1 - (G_1 \cong G_2)) \f$\n
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
     * @brief Finds the maximum clique of the graph using Bron-Kerbosch
     * algorithm.
     *
     * @param accuracy decides whether to use a simple approximation instead
     *
     * @return Vector of vertices that form the maximum clique.
     */
    [[nodiscard]] auto maxClique(
        AlgorithmAccuracy accuracy = AlgorithmAccuracy::EXACT) const
        -> std::vector<size_t>;

    /**
     * @brief modidfied max clique algorithm for finding maximum induced
     * subgraphs.
     *
     * @param accuracy decides whether to use a simple approximation instead
     *
     * @return Vector of vertices that form the maximum clique.
     */
    [[nodiscard]] auto modifiedMaxClique(
        AlgorithmAccuracy accuracy = AlgorithmAccuracy::EXACT) const
        -> std::vector<size_t>;

    /**
     * @brief Returns maximum induced subgraph of two graphs
     *
     * @param rhs is the graph with which the maximum
     * induced subgrraph should be applied
     * @param accuracy decides whether to just use a simple approximation
     * instead
     *
     * @return The maximum induced subgraph of the graphs
     */
    [[nodiscard]] auto maxSubgraph(
        const Graph& rhs, AlgorithmAccuracy accuracy = AlgorithmAccuracy::EXACT)
        -> Graph;

    /**
     * @brief Graph of max clique.
     *
     * @param accuracy Determines whether to return the approximation or exact
     * solution.
     *
     * @return Vector of vertices that form the maximum clique.
     */
    [[nodiscard]] auto maxCliqueGraph(AlgorithmAccuracy accuracy) const
        -> Graph;

    /**
     * @brief Gives the induced subgraph given by the vertices.
     *
     * @param vertices of the subgraph.
     *
     * @return Induced subgraph.
     */
    [[nodiscard]] auto subGraph(std::vector<size_t>& vertices) const -> Graph;
};
