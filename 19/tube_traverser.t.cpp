#include <catch.hpp>

#include <tube_traverser.hpp>

#include <algorithm>
#include <numeric>

TEST_CASE("Tube Traverser")
{
    char const* input = R"(     |          
     |  +--+    
     A  |  C    
 F---|----E|--+ 
     |  |  |  D 
     +B-+  +--+ 
                
)";

    SECTION("Parse input")
    {
        Maze maze(input);

        CHECK(maze.getChar(Coordinates{ 5, 2 }) == 'A');
        CHECK(maze.getChar(Coordinates{ 1, 3 }) == 'F');
        CHECK(maze.getChar(Coordinates{ 6, 5 }) == 'B');

        CHECK(maze.findEntry() == Coordinates{ 5, 0 });
    }

    SECTION("Maze traversal")
    {
        Maze maze(input);

        CHECK(maze.traverse().first == "ABCDEF");
        CHECK(maze.traverse().second == 38);
    }

}
