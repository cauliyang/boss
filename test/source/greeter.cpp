#include <boss/version.h>
#include <doctest/doctest.h>

#include <boss/fq_utils.hpp>
#include <string>

TEST_CASE("Greeter") {
  Greeter greeter("Tests");

  CHECK(greeter.greet(LanguageCode::EN) == "Hello, Tests!");
  CHECK(greeter.greet(LanguageCode::DE) == "Hallo Tests!");
  CHECK(greeter.greet(LanguageCode::ES) == "¡Hola Tests!");
  CHECK(greeter.greet(LanguageCode::FR) == "Bonjour Tests!");
}

TEST_CASE("Greeter version") {
  static_assert(std::string_view(BOSS_VERSION) == std::string_view("1.0"));
  CHECK(std::string(BOSS_VERSION) == std::string("1.0"));
}