#include <catch.hpp>

#include <particles.hpp>

#include <algorithm>
#include <numeric>

TEST_CASE("Particles")
{
    char const* input = R"(p=<3,0,0>, v=<2,0,0>, a=<-1,0,0>
p=<4,0,0>, v=<0,0,0>, a=<-2,0,0>
)";

    SECTION("Parse input")
    {
        auto particles = parseInput(input);
        REQUIRE(particles.size() == 2);
        CHECK(particles[0].position == Vec3{ 3, 0, 0 });
        CHECK(particles[0].velocity == Vec3{ 2, 0, 0 });
        CHECK(particles[0].acceleration == Vec3{ -1, 0, 0 });

        CHECK(particles[1].position == Vec3{ 4, 0, 0 });
        CHECK(particles[1].velocity == Vec3{ 0, 0, 0 });
        CHECK(particles[1].acceleration == Vec3{ -2, 0, 0 });
    }

    SECTION("Vec3 Addition")
    {
        Vec3 x{1,2,3};
        x += Vec3{5, -2, 9};
        CHECK(x == Vec3{6, 0, 12});
    }

    SECTION("Simulate")
    {
        auto particles = parseInput(input);

        CHECK(simulate(particles) == 1);
        CHECK(particles[0].position == Vec3{ 4, 0, 0 });
        CHECK(particles[0].velocity == Vec3{ 1, 0, 0 });
        CHECK(particles[0].acceleration == Vec3{ -1, 0, 0 });

        CHECK(particles[1].position == Vec3{ 2, 0, 0 });
        CHECK(particles[1].velocity == Vec3{ -2, 0, 0 });
        CHECK(particles[1].acceleration == Vec3{ -2, 0, 0 });

        CHECK(simulate(particles) == 1);
        CHECK(particles[0].position == Vec3{ 4, 0, 0 });
        CHECK(particles[0].velocity == Vec3{ 0, 0, 0 });
        CHECK(particles[0].acceleration == Vec3{ -1, 0, 0 });

        CHECK(particles[1].position == Vec3{ -2, 0, 0 });
        CHECK(particles[1].velocity == Vec3{ -4, 0, 0 });
        CHECK(particles[1].acceleration == Vec3{ -2, 0, 0 });

        CHECK(simulate(particles) == 0);
        CHECK(particles[0].position == Vec3{ 3, 0, 0 });
        CHECK(particles[0].velocity == Vec3{ -1, 0, 0 });
        CHECK(particles[0].acceleration == Vec3{ -1, 0, 0 });

        CHECK(particles[1].position == Vec3{-8, 0, 0 });
        CHECK(particles[1].velocity == Vec3{ -6, 0, 0 });
        CHECK(particles[1].acceleration == Vec3{ -2, 0, 0 });
    }

    char const* input2 = R"(p=<-6,0,0>, v=<3,0,0>, a=<0,0,0>
p=<-4,0,0>, v=<2,0,0>, a=<0,0,0>
p=<-2,0,0>, v=<1,0,0>, a=<0,0,0>
p=<3,0,0>, v=<-1,0,0>, a=<0,0,0>)";

    SECTION("Simulate with collision")
    {
        auto particles = parseInput(input2);

        simulate(particles);
        resolveCollisions(particles);

        REQUIRE(particles.size() == 4);
        CHECK(particles[0].position == Vec3{ -3, 0, 0 });
        CHECK(particles[0].velocity == Vec3{ 3, 0, 0 });
        CHECK(particles[0].acceleration == Vec3{ 0, 0, 0 });

        CHECK(particles[1].position == Vec3{ -2, 0, 0 });
        CHECK(particles[1].velocity == Vec3{ 2, 0, 0 });
        CHECK(particles[1].acceleration == Vec3{ 0, 0, 0 });

        CHECK(particles[2].position == Vec3{ -1, 0, 0 });
        CHECK(particles[2].velocity == Vec3{ 1, 0, 0 });
        CHECK(particles[2].acceleration == Vec3{ 0, 0, 0 });

        CHECK(particles[3].position == Vec3{ 2, 0, 0 });
        CHECK(particles[3].velocity == Vec3{ -1, 0, 0 });
        CHECK(particles[3].acceleration == Vec3{ 0, 0, 0 });

        simulate(particles);
        resolveCollisions(particles);

        REQUIRE(particles.size() == 1);
        CHECK(particles[0].position == Vec3{ 1, 0, 0 });
        CHECK(particles[0].velocity == Vec3{ -1, 0, 0 });
        CHECK(particles[0].acceleration == Vec3{ 0, 0, 0 });

    }
}
