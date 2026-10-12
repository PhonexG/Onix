#include <catch2/catch_test_macros.hpp>
#include <onix/core/PieceTable.hpp>

using onix::core::PieceTable;

TEST_CASE("PieceTable - Empty buffer initialization", "[core][piecetable]") {
    PieceTable pt;
    
    REQUIRE(pt.empty());
    REQUIRE(pt.size() == 0);
    REQUIRE(pt.get_text().empty());
}

TEST_CASE("PieceTable - Initial text buffer", "[core][piecetable]") {
    PieceTable pt{"Hello world!"};
    
    REQUIRE_FALSE(pt.empty());
    REQUIRE(pt.size() == 12);
    REQUIRE(pt.get_text() == "Hello world!");    
}

TEST_CASE("PieceTable - Insert operations", "[core][piecetable]") {
    SECTION("Insert into empty buffer") {
        PieceTable pt;
        pt.insert(0, "Hello");

        REQUIRE(pt.size() == 5);
        REQUIRE(pt.get_text() == "Hello");
    }

    SECTION("Insert at the end (append)") {
        PieceTable pt{"Hello"};
        pt.insert(5, " World");

        REQUIRE(pt.size() == 11);
        REQUIRE(pt.get_text() == "Hello World");
    }

    SECTION("Insert at the beginning (prepend)") {
        PieceTable pt{"World"};
        pt.insert(0, "Hello ");

        REQUIRE(pt.size() == 11);
        REQUIRE(pt.get_text() == "Hello World");
    }

    SECTION("Insert in the middle (split piece)") {
        PieceTable pt{"HelloWorld"};
        pt.insert(5, " Beautiful ");

        REQUIRE(pt.size() == 21);
        REQUIRE(pt.get_text() == "Hello Beautiful World");
    }
}
