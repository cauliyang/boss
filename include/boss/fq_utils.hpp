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
             && is_skip(c))
        ;

      return get_char(c);
    }

  private:
    int get_char(int c) {
      ++saved_line_count_;
      return c;
    }

    [[nodiscard]] bool is_skip(int c) {
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

    FqDirection direction_;
    // line_count_ starts at 1 b
    // Max line_count_ is  2^64 - 1 = 18,446,744,073,709,551,615
    uint64_t line_count_{0};
    uint64_t saved_line_count_{0};
  };

  namespace details {

    //----------------------------Define helper functions---------------------------//
    [[nodiscard]] bool is_gzip(const fs::path& input);

    void split_fq_impl_gz(const fs::path& input, FqDirection direction);

    void split_fq_impl_txt(const fs::path& input, FqDirection direction);

    [[nodiscard]] bool check_filename(std::string_view filename);

  }  // namespace details

  //--------------Definitions of functions for splitting ncbi fq files----//

  void split_fq(const fs::path& input);

  using details::check_filename;

}  // namespace boss::fqsp

#endif  // FQ_UTILS_H