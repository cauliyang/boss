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

#ifndef FQ_UTILS_H
#define FQ_UTILS_H

#include <spdlog/spdlog.h>

#include <boost/iostreams/categories.hpp>  // input_filter_tag
#include <boost/iostreams/operations.hpp>  // get, put, WOULD_BLOCK
#include <cstdio>                          // EOF.
#include <filesystem>

namespace boss::fqsp {

  namespace fs = std::filesystem;
  enum class FqDirection;
  class fq_filter;

  /**
   * @brief define the direction of the fastq file
   */
  enum class FqDirection { Forward, Reverse };

  //--------------Definitions of fq input filter for splitting ncbi fq files----//
  class fq_filter {
  public:
    using char_type = char;
    using category = boost::iostreams::input_filter_tag;

    explicit constexpr fq_filter(FqDirection direction) : direction_(direction) {}

    template <typename Source> int get(Source& src) {
      int c;
      while ((c = boost::iostreams::get(src)) != EOF && c != boost::iostreams::WOULD_BLOCK
             && !is_return(c))
        spdlog::debug("Skip char: {}", c);

      return get_char(c);
    }

  private:
    int get_char(int c);

    [[nodiscard]] bool is_return(int c);

    [[nodiscard]] bool is_return_impl() const;

    FqDirection direction_;
    // line_count_ starts at 1 b
    // Max line_count_ is  2^64 - 1 = 18,446,744,073,709,551,615
    uint64_t line_count_{0};
    uint64_t saved_line_count_{0};
    bool has_return_{false};
    bool has_end_{false};
  };

  namespace details {

    //----------------------------Define helper functions---------------------------//
    [[nodiscard]] bool is_gzip(const fs::path& input);

    void split_fq_impl_gz(const fs::path& input, FqDirection direction);

    void split_fq_impl_txt(const fs::path& input, FqDirection direction);

    [[nodiscard]] bool check_filename(std::string_view filename);

    [[maybe_unused]] void print_char(const fs::path& path);

  }  // namespace details

  //--------------Definitions of functions for splitting ncbi fq files----//

  void split_fq(const fs::path& input);

  using details::check_filename;

}  // namespace boss::fqsp

#endif  // FQ_UTILS_H
