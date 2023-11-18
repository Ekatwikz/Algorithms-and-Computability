#include <iostream>
#include <sstream>

#include "catch_amalgamated.hpp"
#include "graph.hpp"

// NOLINT is only acceptable here because of the external testing macros.
// Don't do this anywhere else.
// NOLINTBEGIN(cppcoreguidelines-avoid-do-while)
// NOLINTBEGIN(readability-function-cognitive-complexity)

TEST_CASE("Graph construction, isomorphism and distance sanity checks") {
    Graph pentagonFromMatrix1;
    Graph pentagonFromMatrix2;
    Graph squareWithOutcastFromMatrix;
    Graph strangeStarFromMatrix;
    Graph multiHexagonFromMatrix;
    Graph pairOfMultitrianglesFromMatrix;
    Graph nullGraph;
    Graph pentagon1;
    Graph pentagon2;
    Graph squareWithOutcast;
    Graph strangeStar;
    Graph multiHexagon;
    Graph pairOfMultitriangles;

    REQUIRE_NOTHROW(pentagonFromMatrix1 =
                        Graph(std::vector<std::vector<int>>{{0, 1, 0, 0, 1},
                                                            {0, 0, 3, 0, 0},
                                                            {0, 1, 0, 1, 0},
                                                            {0, 0, 1, 0, 1},
                                                            {1, 0, 0, 1, 0}}));
    REQUIRE_NOTHROW(pentagonFromMatrix2 =
                        Graph(std::vector<std::vector<int>>{{0, 0, 1, 1, 0},
                                                            {0, 0, 0, 1, 1},
                                                            {0, 0, 0, 0, 3},
                                                            {1, 1, 0, 0, 0},
                                                            {0, 1, 1, 0, 0}}));
    REQUIRE_NOTHROW(squareWithOutcastFromMatrix =
                        Graph(std::vector<std::vector<int>>{{0, 2, 0, 1, 0},
                                                            {1, 0, 1, 0, 0},
                                                            {0, 1, 0, 1, 0},
                                                            {1, 0, 1, 0, 0},
                                                            {0, 0, 0, 0, 0}}));
    REQUIRE_NOTHROW(strangeStarFromMatrix =
                        Graph(std::vector<std::vector<int>>{{0, 2, 1, 1, 1},
                                                            {1, 0, 0, 0, 0},
                                                            {1, 0, 0, 0, 0},
                                                            {1, 0, 0, 0, 0},
                                                            {1, 0, 0, 0, 0}}));
    REQUIRE_NOTHROW(multiHexagonFromMatrix = Graph(
                        std::vector<std::vector<int>>{{0, 1, 0, 3, 0, 1},
                                                      {1, 0, 1, 0, 0, 0},
                                                      {0, 1, 0, 1, 0, 0},
                                                      {0, 0, 1, 0, 1, 0},
                                                      {0, 0, 0, 1, 0, 1},
                                                      {1, 0, 0, 0, 1, 0}}));
    REQUIRE_NOTHROW(pairOfMultitrianglesFromMatrix = Graph(
                        std::vector<std::vector<int>>{{0, 1, 1, 3, 0, 0},
                                                      {1, 0, 1, 0, 0, 0},
                                                      {1, 1, 0, 0, 0, 0},
                                                      {0, 0, 0, 0, 1, 1},
                                                      {0, 0, 0, 1, 0, 1},
                                                      {0, 0, 0, 1, 1, 0}}));

    REQUIRE_THROWS_AS(nullGraph = Graph{std::istringstream{""}},
                      std::invalid_argument);
    REQUIRE_THROWS_AS(nullGraph = Graph{std::istringstream{"1"}},
                      std::invalid_argument);

    REQUIRE_NOTHROW(nullGraph = Graph{std::istringstream{"0"}});
    REQUIRE_NOTHROW(pentagon1 = Graph{std::istringstream{"5\n"
                                                         "0 1 0 0 1\n"
                                                         "0 0 3 0 0\n"
                                                         "0 1 0 1 0\n"
                                                         "0 0 1 0 1\n"
                                                         "1 0 0 1 0"}});
    REQUIRE_NOTHROW(pentagon2 = Graph{std::istringstream{"5\n"
                                                         "0 0 1 1 0\n"
                                                         "0 0 0 1 1\n"
                                                         "0 0 0 0 3\n"
                                                         "1 1 0 0 0\n"
                                                         "0 1 1 0 0"}});
    REQUIRE_NOTHROW(squareWithOutcast = Graph{std::istringstream{"5\n"
                                                                 "0 2 0 1 0\n"
                                                                 "1 0 1 0 0\n"
                                                                 "0 1 0 1 0\n"
                                                                 "1 0 1 0 0\n"
                                                                 "0 0 0 0 0"}});
    REQUIRE_NOTHROW(strangeStar = Graph{std::istringstream{"5\n"
                                                           "0 2 1 1 1\n"
                                                           "1 0 0 0 0\n"
                                                           "1 0 0 0 0\n"
                                                           "1 0 0 0 0\n"
                                                           "1 0 0 0 0"}});
    REQUIRE_NOTHROW(multiHexagon = Graph{std::istringstream{"6\n"
                                                            "0 1 0 3 0 1\n"
                                                            "1 0 1 0 0 0\n"
                                                            "0 1 0 1 0 0\n"
                                                            "0 0 1 0 1 0\n"
                                                            "0 0 0 1 0 1\n"
                                                            "1 0 0 0 1 0"}});

    REQUIRE_NOTHROW(pairOfMultitriangles =
                        Graph{std::istringstream{"6\n"
                                                 "0 1 1 3 0 0\n"
                                                 "1 0 1 0 0 0\n"
                                                 "1 1 0 0 0 0\n"
                                                 "0 0 0 0 1 1\n"
                                                 "0 0 0 1 0 1\n"
                                                 "0 0 0 1 1 0"}});

    // interesting note: squareWithOutcast and strangeStar are cospectral mates

    SECTION("Example graphs have sensible sizes") {
        REQUIRE(nullGraph.getSize() == 0);
        REQUIRE(pentagon1.getSize() == pentagon2.getSize());
        REQUIRE(squareWithOutcast.getSize() == strangeStar.getSize());

        REQUIRE(pentagonFromMatrix1.getSize() == pentagonFromMatrix2.getSize());
        REQUIRE(squareWithOutcastFromMatrix.getSize() ==
                strangeStarFromMatrix.getSize());

        REQUIRE(pentagon1.getSize() == pentagonFromMatrix1.getSize());
        REQUIRE(pentagon2.getSize() == pentagonFromMatrix2.getSize());
        REQUIRE(squareWithOutcast.getSize() ==
                squareWithOutcastFromMatrix.getSize());
        REQUIRE(strangeStar.getSize() == strangeStarFromMatrix.getSize());
    }

    SECTION("goofypentagon1 and goofypentagon2 are isomorphic") {
        REQUIRE(pentagon1 == pentagon2);
        REQUIRE(pentagonFromMatrix1 == pentagonFromMatrix2);
        REQUIRE(pentagon1 == pentagonFromMatrix1);
        REQUIRE(pentagon2 == pentagonFromMatrix2);
    }

    SECTION("square-with-outcast and strange-star are NOT isomorphic") {
        REQUIRE(squareWithOutcast != strangeStar);
        REQUIRE(squareWithOutcastFromMatrix != strangeStarFromMatrix);
        REQUIRE(squareWithOutcast != strangeStarFromMatrix);
        REQUIRE(squareWithOutcastFromMatrix != strangeStar);
    }

    SECTION("pentagon1 and pentagon2 have distance 0") {
        REQUIRE(pentagon1.metricDistanceTo(pentagon2) == 0);
        REQUIRE(pentagonFromMatrix1.metricDistanceTo(pentagonFromMatrix2) == 0);
    }

    SECTION("squareWithOutcast and strangeStar have distance 1 ") {
        REQUIRE(squareWithOutcast.metricDistanceTo(strangeStar) == 1);
        REQUIRE(squareWithOutcastFromMatrix.metricDistanceTo(
                    strangeStarFromMatrix) == 1);
    }

    SECTION(
        "nullgraph, squareWithOutcast and strangeStar each have same distance "
        "to pentagon1 as they do to pentagon2") {
        REQUIRE(nullGraph.metricDistanceTo(pentagon1) ==
                nullGraph.metricDistanceTo(pentagon2));
        REQUIRE(nullGraph.metricDistanceTo(pentagonFromMatrix1) ==
                nullGraph.metricDistanceTo(pentagonFromMatrix2));
        REQUIRE(squareWithOutcast.metricDistanceTo(pentagon1) ==
                squareWithOutcast.metricDistanceTo(pentagon2));
        REQUIRE(squareWithOutcast.metricDistanceTo(pentagonFromMatrix1) ==
                squareWithOutcast.metricDistanceTo(pentagonFromMatrix2));
        REQUIRE(strangeStar.metricDistanceTo(pentagon1) ==
                strangeStar.metricDistanceTo(pentagon2));
        REQUIRE(strangeStar.metricDistanceTo(pentagonFromMatrix1) ==
                strangeStar.metricDistanceTo(pentagonFromMatrix2));
    }

    // I'm actually not sure if this is slightly redundant tbh
    SECTION(
        "nullg, penta1 and penta2 each have same distance to square as they do "
        "to star") {
        REQUIRE(nullGraph.metricDistanceTo(squareWithOutcast) ==
                nullGraph.metricDistanceTo(strangeStar));
        REQUIRE(nullGraph.metricDistanceTo(squareWithOutcastFromMatrix) ==
                nullGraph.metricDistanceTo(strangeStarFromMatrix));
        REQUIRE(pentagon1.metricDistanceTo(squareWithOutcast) ==
                pentagon1.metricDistanceTo(strangeStar));
        REQUIRE(pentagon1.metricDistanceTo(squareWithOutcastFromMatrix) ==
                pentagon1.metricDistanceTo(strangeStarFromMatrix));
        REQUIRE(pentagon2.metricDistanceTo(squareWithOutcast) ==
                pentagon2.metricDistanceTo(strangeStar));
        REQUIRE(pentagon2.metricDistanceTo(squareWithOutcastFromMatrix) ==
                pentagon2.metricDistanceTo(strangeStarFromMatrix));
    }

    SECTION(
        "isomorphic graphs are also approximately isomorphic to each other") {
        REQUIRE(pentagon1.approxIsomorphicTo(pentagon2));
        REQUIRE(pentagon1.approxIsomorphicTo(pentagonFromMatrix1));
        REQUIRE(pentagon2.approxIsomorphicTo(pentagonFromMatrix2));
        REQUIRE(pentagonFromMatrix1.approxIsomorphicTo(pentagonFromMatrix2));
    }

    SECTION(
        "pair-of-multitriangles and multi-hexagon are approx isomorphic to "
        "each other, have approx distance 0 to each other (but not exactly), "
        "but are NOT isomorphic") {
        REQUIRE(pairOfMultitriangles.approxIsomorphicTo(multiHexagon));
        REQUIRE(pairOfMultitriangles.metricDistanceTo(
                    multiHexagon, AlgorithmAccuracy::APPROXIMATE) == 0);
        REQUIRE(pairOfMultitriangles.metricDistanceTo(multiHexagon) > 0);
        REQUIRE(pairOfMultitriangles != multiHexagon);

        REQUIRE(pairOfMultitrianglesFromMatrix.approxIsomorphicTo(
            multiHexagonFromMatrix));
        REQUIRE(pairOfMultitrianglesFromMatrix.metricDistanceTo(
                    multiHexagonFromMatrix, AlgorithmAccuracy::APPROXIMATE) ==
                0);
        REQUIRE(pairOfMultitrianglesFromMatrix.metricDistanceTo(
                    multiHexagonFromMatrix) > 0);
        REQUIRE(pairOfMultitrianglesFromMatrix != multiHexagonFromMatrix);
    }

    // TODO: Maybe we'll also check basic stuff like Graph::operator<< and the
    // like later on
}

// NOLINTEND(readability-function-cognitive-complexity)
// NOLINTEND(cppcoreguidelines-avoid-do-while)
