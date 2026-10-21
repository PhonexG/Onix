#include <catch2/catch_test_macros.hpp>
#include <onix/core/PieceTable.hpp>
#include <onix/core/LineIndex.hpp>
#include <onix/core/TextBuffer.hpp>
#include <stdexcept>

using onix::core::PieceTable;
using onix::core::LineIndex;
using onix::core::Position;
using onix::core::TextBuffer;

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
        REQUIRE(index.line_count() == 3);
        REQUIRE(index.line_to_offset(0) == 0);
        REQUIRE(index.line_to_offset(1) == 6);
        REQUIRE(index.line_to_offset(2) == 12);

        REQUIRE(index.offset_to_position(6) == Position{.line = 1, .column = 0});
        REQUIRE(index.offset_to_position(8) == Position{.line = 1, .column = 2});
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
        REQUIRE(index.line_count() == 3);
        REQUIRE(index.line_to_offset(0) == 0);
        REQUIRE(index.line_to_offset(1) == 1);
        REQUIRE(index.line_to_offset(2) == 2);
        REQUIRE(index.offset_to_position(1) == Position{.line = 1, .column = 0});
        REQUIRE(index.offset_to_position(2) == Position{.line = 2, .column = 0});
    }
}

TEST_CASE("TextBuffer - Integration operations", "[core][textbuffer]") {
    SECTION("Empty buffer") {
        TextBuffer buf;
        REQUIRE(buf.line_count() == 1);
        REQUIRE(buf.get_text().empty());
        REQUIRE(buf.get_line(0).empty());
    }

    SECTION("Initial text and get_line") {
        TextBuffer buf{"First line\nSecond line\nThird line"};
        REQUIRE(buf.line_count() == 3);
        REQUIRE(buf.get_line(0) == "First line\n");
        REQUIRE(buf.get_line(1) == "Second line\n");
        REQUIRE(buf.get_line(2) == "Third line");
    }

    SECTION("Insert by offset and by Position") {
        TextBuffer buf{"Hello\nWorld"};
        buf.insert_at(Position{.line = 0, .column = 5}, " there");
        REQUIRE(buf.get_text() == "Hello there\nWorld");
        REQUIRE(buf.line_count() == 2);
        REQUIRE(buf.get_line(0) == "Hello there\n");

        buf.insert(11, " Awesome");
        REQUIRE(buf.get_text() == "Hello there Awesome\nWorld");
        REQUIRE(buf.line_count() == 2);
        REQUIRE(buf.get_line(0) == "Hello there Awesome\n");
        REQUIRE(buf.get_line(1) == "World");
    }

    SECTION("Remove and sync line index") {
        TextBuffer buf{"One\nTwo\nThree"};
        REQUIRE(buf.line_count() == 3);

        buf.remove(3, 4);
        REQUIRE(buf.get_text() == "One\nThree");
        REQUIRE(buf.line_count() == 2);
        REQUIRE(buf.get_line(0) == "One\n");
        REQUIRE(buf.get_line(1) == "Three");
    }
}
