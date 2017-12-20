#include <catch.hpp>

#include <spin_lock.hpp>

TEST_CASE("Spin Lock")
{
    SECTION("Spin steps")
    {
        Buffer b;
        b.push_back(0);

        std::size_t start_index = 0;
        start_index = spin_step(b, 3, 1, start_index);
        CHECK(start_index == 1);
        REQUIRE(b.size() == 2);
        CHECK(b[0] == 0);
        CHECK(b[1] == 1);

        start_index = spin_step(b, 3, 2, start_index);
        CHECK(start_index == 1);
        REQUIRE(b.size() == 3);
        CHECK(b[0] == 0);
        CHECK(b[1] == 2);
        CHECK(b[2] == 1);

        start_index = spin_step(b, 3, 3, start_index);
        CHECK(start_index == 2);
        CHECK(b == Buffer{0, 2, 3, 1});

        start_index = spin_step(b, 3, 4, start_index);
        CHECK(start_index == 2);
        CHECK(b == Buffer{0, 2, 4, 3, 1});

        start_index = spin_step(b, 3, 5, start_index);
        CHECK(start_index == 1);
        CHECK(b == Buffer{0, 5, 2, 4, 3, 1});

        start_index = spin_step(b, 3, 6, start_index);
        CHECK(start_index == 5);
        CHECK(b == Buffer{0, 5, 2, 4, 3, 6, 1});

        start_index = spin_step(b, 3, 7, start_index);
        CHECK(start_index == 2);
        CHECK(b == Buffer{0, 5, 7, 2, 4, 3, 6, 1});

        start_index = spin_step(b, 3, 8, start_index);
        CHECK(start_index == 6);
        CHECK(b == Buffer{0, 5, 7, 2, 4, 3, 8, 6, 1});

        start_index = spin_step(b, 3, 9, start_index);
        CHECK(start_index == 1);
        CHECK(b == Buffer{0, 9, 5, 7, 2, 4, 3, 8, 6, 1});
    }

    SECTION("Spin")
    {
        CHECK(spin(3, 9).first == Buffer{0, 9, 5, 7, 2, 4, 3, 8, 6, 1});
        CHECK(spin(3, 9).second == 1);

        auto const [b, i] = spin(3, 2017);
        REQUIRE(i >= 3);
        REQUIRE(i < b.size() - 3);
        REQUIRE(b[i] == 2017);
        CHECK(b[i - 1] == 151);
        CHECK(b[i - 2] == 1134);
        CHECK(b[i - 3] == 1512);
        CHECK(b[i + 1] == 638);
        CHECK(b[i + 2] == 1513);
        CHECK(b[i + 3] == 851);
    }

    SECTION("Spin after zero")
    {
        CHECK(spin_after_0(3, 1) == 1);
        CHECK(spin_after_0(3, 2) == 2);
        CHECK(spin_after_0(3, 3) == 2);
        CHECK(spin_after_0(3, 4) == 2);
        CHECK(spin_after_0(3, 5) == 5);
        CHECK(spin_after_0(3, 6) == 5);
        CHECK(spin_after_0(3, 7) == 5);
        CHECK(spin_after_0(3, 8) == 5);
        CHECK(spin_after_0(3, 9) == 9);
        CHECK(spin_after_0(3, 2017) == 1226);
    }
}
