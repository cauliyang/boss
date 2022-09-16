#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boss/fq_utils.hpp>
#include <iostream>

namespace boss::fqsp {

  namespace io = boost::iostreams;

  void details::split_fq_impl_txt(const fs::path& input, FqDirection direction) {
    io::filtering_istream in;
    in.push(fq_filter(direction));
    in.push(io::file_source(input.string(), std::ios_base::in));
    std::ofstream out{fmt::format("{}{}{}", input.stem().string(),
                                  direction == FqDirection::Forward ? "1" : "2",
                                  input.extension().string())};
    io::copy(in, out);
    out.close();
  }

  void details::split_fq_impl_gz(const fs::path& input, FqDirection direction) {
    spdlog::debug("Processing file: {} {}", input.string(),
                  direction == FqDirection::Forward ? "Forward" : "Reverse");
    std::ofstream output_file{
        fmt::format("{}{}{}", input.stem().string(), direction == FqDirection::Forward ? "1" : "2",
                    input.extension().string()),
        std::ios_base::out | std::ios_base::binary};

    io::filtering_istream in;
    in.push(fqsp::fq_filter(direction));
    in.push(io::gzip_decompressor());
    in.push(io::file_source(input.string(), std::ios_base::in | std::ios_base::binary));

    io::filtering_ostream out;
    out.push(io::gzip_compressor());
    out.push(output_file);

    io::copy(in, out);
    output_file.close();
  }

  bool details::is_gzip(const fs::path& input) { return input.extension() == ".gz"; }

  void split_fq(const fs::path& input) {
    if (details::is_gzip(input)) {
      details::split_fq_impl_gz(input, FqDirection::Forward);
      details::split_fq_impl_gz(input, FqDirection::Reverse);
    } else {
      details::split_fq_impl_txt(input, FqDirection::Forward);
      details::split_fq_impl_txt(input, FqDirection::Reverse);
    }
  }

  void test_gz_write(const fs::path& path) {
    std::ofstream out{fmt::format("{}{}", path.string(), ".gz")};

    io::filtering_ostream fis;
    fis.push(io::gzip_compressor());
    fis.push(out);

    fis << "Hello World!\n";
    io::close(fis);
    out.close();
  }

  void test_gz_read(const fs::path& path) {
    std::ifstream file(path.string(), std::ios_base::in | std::ios_base::binary);
    io::filtering_streambuf<io::input> in;
    in.push(io::gzip_decompressor());
    in.push(file);
    boost::iostreams::copy(in, std::cout);
  }
}  // namespace boss::fqsp
