#include <catch2/catch_test_macros.hpp>
#include <onix/core/PieceTable.hpp>
#include <onix/core/LineIndex.hpp>
#include <stdexcept>

using onix::core::PieceTable;
using onix::core::LineIndex;
using onix::core::Position;

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

TEST_CASE("LineIndex - Basic operations", "[core][lineindex]") {
    SECTION("Empty LineIndex") {
        LineIndex index;
        REQUIRE(index.line_count() == 1);
        REQUIRE(index.line_to_offset(0) == 0);
        REQUIRE(index.offset_to_position(0) == Position{.line = 0, .column = 0});
        REQUIRE_THROWS_AS(index.line_to_offset(1), std::out_of_range);
    }

    SECTION("Single line without newline") {
        LineIndex index{"Hello"};
        REQUIRE(index.line_count() == 1);
        REQUIRE(index.line_to_offset(0) == 0);
        REQUIRE(index.offset_to_position(3) == Position{.line = 0, .column = 3});
    }

    SECTION("Multiple lines") {
        LineIndex index{"Hello\nWorld\n!"};
        // 'H' 'e' 'l' 'l' 'o' '\n' -> 0..5
        // 'W' 'o' 'r' 'l' 'd' '\n' -> 6..11
        // '!'                      -> 12
        REQUIRE(index.line_count() == 3);
        REQUIRE(index.line_to_offset(0) == 0);
        REQUIRE(index.line_to_offset(1) == 6);
        REQUIRE(index.line_to_offset(2) == 12);

        // Позиція символу 'W' (offset 6)
        REQUIRE(index.offset_to_position(6) == Position{.line = 1, .column = 0});
        // Позиція символу 'r' (offset 8)
        REQUIRE(index.offset_to_position(8) == Position{.line = 1, .column = 2});
        // Позиція символу '!' (offset 12)
        REQUIRE(index.offset_to_position(12) == Position{.line = 2, .column = 0});
    }

    SECTION("Trailing newline") {
        LineIndex index{"Line 1\n"};
        REQUIRE(index.line_count() == 2);
        REQUIRE(index.line_to_offset(0) == 0);
        REQUIRE(index.line_to_offset(1) == 7);
        REQUIRE(index.offset_to_position(7) == Position{.line = 1, .column = 0});
    }

    SECTION("Consecutive newlines") {
        LineIndex index{"\n\n"};
        // 0: '\n' -> рядок 0 (offset 0)
        // 1: '\n' -> рядок 1 (offset 1)
        // 2: ""   -> рядок 2 (offset 2)
        REQUIRE(index.line_count() == 3);
        REQUIRE(index.line_to_offset(0) == 0);
        REQUIRE(index.line_to_offset(1) == 1);
        REQUIRE(index.line_to_offset(2) == 2);
        REQUIRE(index.offset_to_position(1) == Position{.line = 1, .column = 0});
        REQUIRE(index.offset_to_position(2) == Position{.line = 2, .column = 0});
    }
}
