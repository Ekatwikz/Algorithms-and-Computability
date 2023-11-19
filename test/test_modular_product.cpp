#include <iostream>
#include <sstream>

#include "catch_amalgamated.hpp"
#include "graph.hpp"

// NOLINT is only acceptable here because of the external testing macros.
// Don't do this anywhere else.
// NOLINTBEGIN(cppcoreguidelines-avoid-do-while)
// NOLINTBEGIN(readability-function-cognitive-complexity)

TEST_CASE("Modular product tests") {
    Graph bidirectTwoGraph;
    Graph onedirectTwoGraph;
    Graph wikiExampleAandB;
    Graph isolatedThreeVert;
    Graph isolatedTwoVert;
    Graph multiEdgeTriangleGraph;
    Graph multiEdgeTwoGraph;

    bidirectTwoGraph =
        Graph{std::istringstream{"2\n"
                                 "0 1\n"
                                 "1 0"}};
    onedirectTwoGraph =
        Graph{std::istringstream{"2\n"
                                 "0 1\n"
                                 "0 0"}};
    wikiExampleAandB =
        Graph{std::istringstream{"3\n"
                                 "0 1 0\n"
                                 "1 0 1\n"
                                 "0 1 0"}};
    isolatedThreeVert =
        Graph{std::istringstream{"3\n"
                                 "0 0 0\n"
                                 "0 0 0\n"
                                 "0 0 0"}};
    isolatedTwoVert =
        Graph{std::istringstream{"2\n"
                                 "0 0\n"
                                 "0 0"}};
    multiEdgeTriangleGraph =
        Graph{std::istringstream{"3\n"
                                 "0 3 1\n"
                                 "0 0 0\n"
                                 "1 0 1"}};
    multiEdgeTwoGraph =
        Graph{std::istringstream{"2\n"
                                 "0 5\n"
                                 "3 2"}};

    SECTION(
        "Modular Product Result of bidirectTwoGraph and onedirectTwoGraph is "
        "correct") {
        REQUIRE(bidirectTwoGraph.modularProduct(onedirectTwoGraph) ==
                Graph{std::istringstream{"4\n"
                                         "0 0 0 1\n"
                                         "0 0 0 0\n"
                                         "0 1 0 0\n"
                                         "0 0 0 0"}});
    }

    SECTION(
        "Modular Product Result of wikiExampleA and wikiExampleB is correct") {
        REQUIRE(wikiExampleAandB.modularProduct(wikiExampleAandB) ==
                Graph{std::istringstream{"9\n"
                                         "0 0 0 0 1 0 0 0 1\n"
                                         "0 0 0 1 0 1 0 0 0\n"
                                         "0 0 0 0 1 0 1 0 0\n"
                                         "0 1 0 0 0 0 0 1 0\n"
                                         "1 0 1 0 0 0 1 0 1\n"
                                         "0 1 0 0 0 0 0 1 0\n"
                                         "0 0 1 0 1 0 0 0 0\n"
                                         "0 0 0 1 0 1 0 0 0\n"
                                         "1 0 0 0 1 0 0 0 0"}});
    }

    SECTION(
        "Modular Product Result of isolated3Vert and isolated2Vert is "
        "correct") {
        REQUIRE(isolatedThreeVert.modularProduct(isolatedTwoVert) ==
                Graph{std::istringstream{"6\n"
                                         "0 0 0 1 0 1\n"
                                         "0 0 1 0 1 0\n"
                                         "0 1 0 0 0 1\n"
                                         "1 0 0 0 1 0\n"
                                         "0 1 0 1 0 0\n"
                                         "1 0 1 0 0 0"}});
    }

    SECTION(
        "Modular Product Result of multiEdgeTriangleGraph and multiEdge2Graph "
        "is correct") {
        REQUIRE(multiEdgeTriangleGraph.modularProduct(multiEdgeTwoGraph) ==
                Graph{std::istringstream{"6\n"
                                         "0 0 0 0 3 1\n"
                                         "0 0 0 0 0 0\n"
                                         "0 0 0 1 0 0\n"
                                         "0 3 1 0 0 0\n"
                                         "0 0 0 0 0 0\n"
                                         "1 0 0 0 0 0"}});
    }
}

// NOLINTEND(readability-function-cognitive-complexity)
// NOLINTEND(cppcoreguidelines-avoid-do-while)
