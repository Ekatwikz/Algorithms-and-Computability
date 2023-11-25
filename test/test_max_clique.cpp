#include <iostream>
#include <sstream>

#include "catch_amalgamated.hpp"
#include "graph.hpp"

// NOLINT is only acceptable here because of the external testing macros.
// Don't do this anywhere else.
// NOLINTBEGIN(cppcoreguidelines-avoid-do-while)
// NOLINTBEGIN(readability-function-cognitive-complexity)

TEST_CASE("Three Connected Graph") {
    Graph threeConnected =
        Graph{std::istringstream{"3\n"
                                 "0 2 1\n"
                                 "8 0 1\n"
                                 "1 2 0"}};

    SECTION("Max clique of threeConnectedGraph is") {
        auto vertices = threeConnected.maxClique();
        auto verticesEstimate = threeConnected.maxClique(true);
        REQUIRE(vertices.size() == 3);
        REQUIRE(verticesEstimate.size() == 3);
    }
}

TEST_CASE("Four Connected Graph") {
    Graph fourConnected =
        Graph{std::istringstream{"4\n"
                                 "0 3 1 1\n"
                                 "7 0 1 1\n"
                                 "1 1 0 4\n"
                                 "1 5 1 0"}};

    SECTION("Max clique of fourConnectedGraph is") {
        auto vertices = fourConnected.maxClique();
        auto verticesEstimate = fourConnected.maxClique(true);

        REQUIRE(vertices.size() == 4);
        REQUIRE(verticesEstimate.size() == 4);
    }
}

TEST_CASE("Disconnected Graph with 4 vertices") {
    Graph disconnectedGraph =
        Graph{std::istringstream{"4\n"
                                 "0 1 0 0\n"
                                 "1 0 0 0\n"
                                 "0 0 0 1\n"
                                 "0 0 1 0"}};

    SECTION("Max cliques of disconnectedGraph are") {
        auto vertices = disconnectedGraph.maxClique();
        auto verticesEstimate = disconnectedGraph.maxClique(true);

        REQUIRE(vertices.size() == 2);
        REQUIRE(verticesEstimate.size() == 2);
    }
}

TEST_CASE("Singleton Vertex in graph with 4 vertices") {
    Graph singletonGraph =
        Graph{std::istringstream{"4\n"
                                 "0 1 0 0\n"
                                 "1 0 1 0\n"
                                 "0 1 0 1\n"
                                 "0 0 1 0"}};

    SECTION("Max cliques of singletonGraph are") {
        auto vertices = singletonGraph.maxClique();
        auto verticesEstimate = singletonGraph.maxClique(true);

        REQUIRE(vertices.size() == 2);
        REQUIRE(verticesEstimate.size() == 2);
    }
}

TEST_CASE("Large Disconnected Graph with Multiple Components") {
    Graph largeDisconnectedGraph =
        Graph{std::istringstream{"8\n"
                                 "0 1 0 0 0 0 0 0\n"
                                 "1 0 0 0 0 0 0 0\n"
                                 "0 0 0 1 0 0 0 0\n"
                                 "0 0 1 0 0 0 0 0\n"
                                 "0 0 0 0 0 1 0 0\n"
                                 "0 0 0 0 1 0 0 0\n"
                                 "0 0 0 0 0 0 0 1\n"
                                 "0 0 0 0 0 0 1 0"}};

    SECTION("Max cliques of largeDisconnectedGraph are") {
        auto vertices = largeDisconnectedGraph.maxClique();
        auto verticesEstimate = largeDisconnectedGraph.maxClique(true);

        REQUIRE(vertices.size() == 2);
        REQUIRE(verticesEstimate.size() == 2);
    }
}

TEST_CASE("Singleton Vertex in Large Connected Graph") {
    Graph singletonLargeGraph =
        Graph{std::istringstream{"5\n"
                                 "0 4 0 0 0\n"
                                 "1 0 5 0 0\n"
                                 "0 1 0 1 0\n"
                                 "0 0 1 0 1\n"
                                 "0 0 0 1 0"}};

    SECTION("Max cliques of singletonLargeGraph are") {
        auto vertices = singletonLargeGraph.maxClique();
        auto verticesEstimate = singletonLargeGraph.maxClique(true);

        REQUIRE(vertices.size() == 2);
        REQUIRE(verticesEstimate.size() == 2);
    }
}

TEST_CASE("Graph with 10 Vertices and Max Clique Size of 5") {
    Graph tenVerticesGraph =
        Graph{std::istringstream{"10\n"
                                 "0 4 1 1 1 0 0 0 0 0\n"
                                 "1 0 1 1 1 1 0 0 0 0\n"
                                 "1 1 0 1 1 0 0 0 0 0\n"
                                 "1 1 1 0 1 0 0 0 0 0\n"
                                 "1 1 1 1 0 1 0 0 0 0\n"
                                 "0 1 0 0 1 0 1 0 0 0\n"
                                 "0 0 0 0 0 5 0 1 0 0\n"
                                 "0 0 0 0 0 0 1 0 1 0\n"
                                 "0 0 0 0 0 0 0 1 0 3\n"
                                 "0 0 0 0 0 0 0 0 1 0"}};

    SECTION("Max clique of tenVerticesGraph is") {
        auto vertices = tenVerticesGraph.maxClique();
        auto verticesEstimate = tenVerticesGraph.maxClique(true);

        REQUIRE(vertices.size() == 5);
        REQUIRE(verticesEstimate.size() == 5);
    }
}

TEST_CASE("Disconnected Graph with 6 Vertices") {
    Graph disconnectedGraph =
        Graph{std::istringstream{"6\n"
                                 "0 0 0 0 0 0\n"
                                 "0 0 0 0 0 0\n"
                                 "0 0 0 0 0 0\n"
                                 "0 0 0 0 0 0\n"
                                 "0 0 0 0 0 0\n"
                                 "0 0 0 0 0 0"}};

    SECTION("Max cliques of disconnectedGraph are") {
        auto vertices = disconnectedGraph.maxClique();
        auto verticesEstimate = disconnectedGraph.maxClique();

        REQUIRE(vertices.size() == 1);
        REQUIRE(verticesEstimate.size() == 1);
    }
}

// NOLINTEND(readability-function-cognitive-complexity)
// NOLINTEND(cppcoreguidelines-avoid-do-while)
