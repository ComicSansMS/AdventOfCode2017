#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <passphrase.hpp>

TEST_CASE("Passphrase")
{

    SECTION("Input Parsing")
    {
        int count = 0;
        iteratePassphrase("aa bb cc dd ee", [&count](std::string_view pwd) {
            switch(count) {
            case 0: CHECK(pwd == "aa"); break;
            case 1: CHECK(pwd == "bb"); break;
            case 2: CHECK(pwd == "cc"); break;
            case 3: CHECK(pwd == "dd"); break;
            case 4: CHECK(pwd == "ee"); break;
            }
            ++count;
        });
        CHECK(count == 5);
    }

    SECTION("Check for duplicates")
    {
        CHECK(!hasDuplicates("aa bb cc dd ee"));
        CHECK(hasDuplicates("aa bb cc dd aa"));
        CHECK(!hasDuplicates("aa bb cc dd aaa"));
    }

    SECTION("Check for anagrams")
    {
        CHECK(!hasAnagrams("abcde fghij"));
        CHECK(hasAnagrams("abcde xyz ecdab"));
        CHECK(!hasAnagrams("a ab abc abd abf abj"));
        CHECK(!hasAnagrams("iiii oiii ooii oooi oooo"));
        CHECK(hasAnagrams("oiii ioii iioi iiio"));
    }
}
