/**
 * @file graph.hpp
 * @brief Graph representation definitions
 */
#include <fstream>
#include <vector>

/**
 * @brief Class that represents a graph as a matrix (vector of vectors) of
 * adjacencies
 */
class Graph {
   private:
    size_t vertexCount;
    std::vector<std::vector<int>>
        adjacencyMatrix;  // size*size square adjacency matrix, should we use
                          // this representation or something else?

   public:
    /**
     * @brief Default constructor
     *
     * Not sure if this should actually be default-constructible tho...
     */
    Graph() : vertexCount{0} {}

    /**
     * @brief constructs graph from data in a stream
     *
     * Not sure if this should actually be default-constructible tho...
     *
     * @param graphStream is the stream to be read from
     *
     * @warning throws <a
     * href="https://en.cppreference.com/w/cpp/error/invalid_argument">invalid_argument</a>
     * if it fails to read the stream at any point
     */
    explicit Graph(std::istream& graphStream);

    /**
     * @brief constructs graph from data in specified filename
     *
     * This maayybe should be a constructor somehow? not sure
     *
     * @param filename is the filename to stream from
     * if the filename is "-", this will read from stdin
     *
     * @warning throws <a
     * href="https://en.cppreference.com/w/cpp/error/invalid_argument">invalid_argument</a>
     * if it couldn't open the file
     */
    [[nodiscard]] static auto fromFilename(const std::string& filename)
        -> Graph;

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
     * @param row the row in the adjacency matrix
     */
    [[nodiscard]] auto operator[](size_t row) const -> std::vector<int>;

    /**
     * @brief stream operator, for convenience
     *
     * @param outputStream the stream to write to
     * @param graph the graph. (<a
     * href="https://www.youtube.com/watch?v=0mfJn604GT4">?</a>)
     *
     * @return the outputStream
     */
    friend auto operator<<(std::ostream& outputStream, const Graph& graph)
        -> std::ostream&;
};
