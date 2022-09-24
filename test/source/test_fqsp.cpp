#include <doctest/doctest.h>
#include <spdlog/spdlog.h>

#include <binary/utils.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boss/fq_utils.hpp>
#include <iostream>

TEST_SUITE("fqsp") {
  namespace bp = boss::fqsp;
  namespace bu = binary::utils;
  namespace fs = std::filesystem;
  constexpr std::string_view input_gz = "../../test/data/test.fq.gz";
  constexpr std::string_view input = "../../test/data/test.fq";

  void test_gz_write(const fs::path& path) {
    namespace io = boost::iostreams;
    std::ofstream out{fmt::format("{}{}", path.string(), ".gz")};

    io::filtering_ostream fis;
    fis.push(io::gzip_compressor());
    fis.push(out);

    fis << "Hello World!\n";
    io::close(fis);
    out.close();
  }

  void test_gz_read(const fs::path& path) {
    namespace io = boost::iostreams;
    std::ifstream file(path.string(), std::ios_base::in | std::ios_base::binary);
    io::filtering_streambuf<io::input> in;
    in.push(io::gzip_decompressor());
    in.push(file);
    io::copy(in, std::cout);
  }

  TEST_CASE("is_gzip") {
    CHECK(bu::check_file_path({input_gz, input}));
    CHECK(bp::details::is_gzip(input_gz));
    CHECK_FALSE(bp::details::is_gzip(input));
  }

  TEST_CASE("read_fq") { CHECK_NOTHROW(test_gz_read(input_gz)); }
  TEST_CASE("write_fq") {
    CHECK_NOTHROW(test_gz_write("hello.txt"));
    CHECK(bu::check_file_path("hello.txt.gz"));
    fs::remove("hello.txt.gz");
  }

  TEST_CASE("test command line for gz fq") {
    CHECK_NOTHROW(bp::split_fq(input_gz));
    CHECK(bu::check_file_path("test.1.fq.gz"));
    CHECK(bu::check_file_path("test.2.fq.gz"));
    fs::remove("test.1.fq.gz");
    fs::remove("test.2.fq.gz");
  }

  TEST_CASE("test command line for txt fq") {
    CHECK_NOTHROW(bp::split_fq(input));
    CHECK(bu::check_file_path("test.1.fq"));
    CHECK(bu::check_file_path("test.2.fq"));
    fs::remove("test.1.fq");
    fs::remove("test.2.fq");
  }
}
