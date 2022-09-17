// BioinfOrmaticS toolboxeS aka BOSS
// Copyright (c) 2022 Yangyang Li <yangyang.li@northwestern.edu>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

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
    if (direction == FqDirection::Reverse) out << '\n';
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
    if (direction == FqDirection::Reverse) out << '\n';
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

  int fq_filter::get_char(int c) {
    ++saved_line_count_;
    return c;
  }
  bool fq_filter::is_skip(int c) {
    if (c == '\n') {
      ++line_count_;  // count lines 0-based index
      if (saved_line_count_ == 0) return true;
    }

    if (FqDirection::Forward == direction_) {
      // forward reads : first 4 lines block start
      return 1 == ((line_count_ >> 2) & 1);
    } else {
      // reverse reads : second 4 lines block start
      return 0 == ((line_count_ >> 2) & 1);
    }
  }
}  // namespace boss::fqsp
