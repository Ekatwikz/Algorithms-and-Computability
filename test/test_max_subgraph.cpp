#include <iostream>
#include <sstream>

#include "catch_amalgamated.hpp"
#include "graph.hpp"

// NOLINT is only acceptable here because of the external testing macros.
// Don't do this anywhere else.
// NOLINTBEGIN(cppcoreguidelines-avoid-do-while)
// NOLINTBEGIN(readability-function-cognitive-complexity)

TEST_CASE("Maximum induced subgraph tests") {
    Graph bidirectTwoGraph;
    Graph onedirectTwoGraph;
    Graph isolatedThreeVert;
    Graph isolatedTwoVert;
    Graph multiEdgeTriangleGraph;
    Graph multiEdgeTwoGraph;
    Graph modProductWikiExample;

    bidirectTwoGraph =
        Graph{std::istringstream{"2\n"
                                 "0 1\n"
                                 "1 0"}};
    onedirectTwoGraph =
        Graph{std::istringstream{"2\n"
                                 "0 1\n"
                                 "0 0"}};
    modProductWikiExample =
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
        "Maximum induced subgraph of bidirectTwoGraph and onedirectTwoGraph is "
        "correct") {
        REQUIRE(bidirectTwoGraph.maxSubgraph(onedirectTwoGraph) ==
                Graph{std::istringstream{"2\n"
                                         "0 1\n"
                                         "0 0"}});
    }

    SECTION(
        "Maximum induced subgraph of wikiExampleA and wikiExampleB is "
        "correct") {
        REQUIRE(modProductWikiExample.maxSubgraph(modProductWikiExample) ==
                Graph{std::istringstream{"3\n"
                                         "0 1 0\n"
                                         "1 0 1\n"
                                         "0 1 0"}});
    }

    SECTION(
        "Maximum induced subgraph of isolated3Vert and isolated2Vert is "
        "correct") {
        REQUIRE(isolatedThreeVert.maxSubgraph(isolatedTwoVert) ==
                Graph{std::istringstream{"2\n"
                                         "0 0\n"
                                         "0 0"}});
    }

    SECTION(
        "Maximum induced subgraph of multiEdgeTriangleGraph and "
        "multiEdge2Graph "
        "is correct") {
        REQUIRE(multiEdgeTriangleGraph.maxSubgraph(multiEdgeTwoGraph) ==
                Graph{std::istringstream{"2\n"
                                         "0 1\n"
                                         "1 0"}});
    }
}

// NOLINTEND(readability-function-cognitive-complexity)
// NOLINTEND(cppcoreguidelines-avoid-do-while)
