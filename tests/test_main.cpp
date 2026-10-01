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
