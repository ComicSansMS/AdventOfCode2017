#include <catch.hpp>

#include <hexgrid.hpp>

#include <sstream>
#include <string>

TEST_CASE("Hexgrid")
{
    SECTION("Parse input")
    {
        char const* input = "n,se,sw,ne,nw,s";
        auto path = parseInput(input);
        CHECK(path == Path{ Direction::N, Direction::SE, Direction::SW, Direction::NE, Direction::NW, Direction::S });
    }

    SECTION("Cubic Coordinates Default Construction")
    {
        CubicCoordinates cc;
        CHECK(cc.x == 0);
        CHECK(cc.y == 0);
        CHECK(cc.z == 0);
    }

    SECTION("Cubic Coordinates Element Construction")
    {
        CubicCoordinates cc(1, 2, -3);
        CHECK(cc.x == 1);
        CHECK(cc.y == 2);
        CHECK(cc.z == -3);
    }

    SECTION("Cubic Coordinates Comparison")
    {
        CubicCoordinates cc(15, 16, -23);
        CHECK(!(cc == CubicCoordinates(1, 2, -3)));
        CHECK(cc == CubicCoordinates(15, 16, -23));
        CHECK(cc != CubicCoordinates(1, 2, -3));
        CHECK(!(cc != CubicCoordinates(15, 16, -23)));
    }

    SECTION("Cubic Coordinates Output")
    {
        CubicCoordinates cc(1, 2, -3);
        std::stringstream sstr;
        sstr << cc;
        CHECK(sstr.str() == "(1, 2, -3)");
    }

    SECTION("Stepping in Cubic Coordinates")
    {
        CubicCoordinates cc;
        CHECK(takeStep(cc, Direction::N) == CubicCoordinates(0, 1, -1));
        CHECK(takeStep(cc, Direction::NE) == CubicCoordinates(1, 0, -1));
        CHECK(takeStep(cc, Direction::SE) == CubicCoordinates(1, -1, 0));
        CHECK(takeStep(cc, Direction::S) == CubicCoordinates(0, -1, 1));
        CHECK(takeStep(cc, Direction::SW) == CubicCoordinates(-1, 0, 1));
        CHECK(takeStep(cc, Direction::NW) == CubicCoordinates(-1, 1, 0));
    }

    SECTION("Distance in Cubic Coordinates")
    {
        CubicCoordinates cc(1, 2, -3);
        CHECK(distance(cc, takeStep(takeStep(takeStep(cc, Direction::NE), Direction::NE), Direction::S)) == 2);
        CHECK(distanceFromOrigin(cc) == 3);
    }

    SECTION("Walk path")
    {
        CubicCoordinates origin;
        CHECK(walkPath(origin, parseInput("n,ne,n")) == CubicCoordinates(1, 2, -3));
        CHECK(walkPath(origin, parseInput("ne,ne,s,s")) == walkPath(origin, parseInput("se,se")));
        CHECK(walkPath(origin, parseInput("se,sw,se,sw,sw")) == walkPath(origin, parseInput("s,s,sw")));
    }

    SECTION("Get path")
    {
        CubicCoordinates origin;
        CHECK(getPath(origin, parseInput("n,ne,n")) == std::vector<CubicCoordinates>{
            CubicCoordinates(0, 0, 0),
            CubicCoordinates(0, 1, -1),
            CubicCoordinates(1, 1, -2),
            CubicCoordinates(1, 2, -3)});
    }

    SECTION("Path Distances")
    {
        CubicCoordinates origin;
        CHECK(distanceFromOrigin(walkPath(origin, parseInput("ne,ne,ne"))) == 3);
        CHECK(distanceFromOrigin(walkPath(origin, parseInput("ne,ne,sw,sw"))) == 0);
        CHECK(distanceFromOrigin(walkPath(origin, parseInput("ne,ne,s,s"))) == 2);
        CHECK(distanceFromOrigin(walkPath(origin, parseInput("se,sw,se,sw,sw"))) == 3);
    }
}
