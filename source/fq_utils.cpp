#include <binary/utils.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boss/fq_utils.hpp>
#include <future>
#include <iostream>

namespace boss::fqsp {

  namespace io = boost::iostreams;

  void details::split_fq_impl_txt(const fs::path& input, FqDirection direction) {
    io::filtering_istream in;
    in.push(fq_filter(direction));
    in.push(io::file_source(input.string(), std::ios_base::in));
    std::ofstream out{fmt::format("{}.{}{}", input.stem().string(),
                                  direction == FqDirection::Forward ? "1" : "2",
                                  input.extension().string())};
    io::copy(in, out);
    out.close();
  }

  void details::split_fq_impl_gz(const fs::path& input, FqDirection direction) {
    spdlog::debug("Processing file: {} {}", input.string(),
                  direction == FqDirection::Forward ? "Forward" : "Reverse");
    std::ofstream output_file{fmt::format("{}.{}{}", input.stem().stem().string(),
                                          direction == FqDirection::Forward ? "1" : "2", ".fq.gz"),
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

  bool details::check_filename(std::string_view filename) {
    if (!binary::utils::check_file_path(filename)) {
      spdlog::error("File {} does not exist", filename);
      return false;
    }

    auto path = fs::path(filename);

    if (is_gzip(path)) {
      if (!(path.stem().extension() == ".fq")) {
        spdlog::error("File {} is not a valid .fq.gz file", filename);
        return false;
      }
      return true;
    }

    if (!(path.extension() == ".fq")) {
      spdlog::error("File {} is not a valid .fq file", filename);
      return false;
    }

    return true;
  }

  void split_fq(const fs::path& input) {
    std::future<void> future;
    if (details::is_gzip(input)) {
      future = std::async(details::split_fq_impl_gz, input, FqDirection::Forward);
      // main thread
      details::split_fq_impl_gz(input, FqDirection::Reverse);
    } else {
      future = std::async(details::split_fq_impl_txt, input, FqDirection::Forward);
      details::split_fq_impl_txt(input, FqDirection::Reverse);
    }
    future.wait();
  }

}  // namespace boss::fqsp
