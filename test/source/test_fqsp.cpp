#include <doctest/doctest.h>
#include <spdlog/spdlog.h>

#include <binary/utils.hpp>
#include <boss/fq_utils.hpp>

TEST_SUITE("fqsp") {
  namespace bp = boss::fqsp;
  namespace bu = binary::utils;
  namespace fs = std::filesystem;
  constexpr std::string_view input_gz = "../../test/data/test.fq.gz";
  constexpr std::string_view input = "../../test/data/test.fq";

  TEST_CASE("is_gzip") {
    spdlog::info("current path: {}", fs::current_path().string());
    CHECK(bu::check_file_path({input_gz, input}));
    CHECK(bp::details::is_gzip(input_gz));
    CHECK_FALSE(bp::details::is_gzip(input));
  }

  TEST_CASE("read_fq") { CHECK_NOTHROW(bp::test_gz_read(input_gz)); }
}