  #include <catch2/catch_test_macros.hpp>

  TEST_CASE("Sanity check - environment is ready", "[core]") {
      REQUIRE(1 + 1 == 2);
  }
  