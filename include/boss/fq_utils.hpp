#ifndef FQ_UTILS_H
#define FQ_UTILS_H

#include <spdlog/spdlog.h>

#include <boost/iostreams/categories.hpp>  // input_filter_tag
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/operations.hpp>  // get, put, WOULD_BLOCK
#include <cstdio>                          // EOF.
#include <filesystem>

namespace boss::fqsp {

  namespace fs = std::filesystem;
  enum class FqDirection;
  class fq_filter;

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
      if (c == '\n') {
        --saved_line_count_;
      }
      return c;
    }

    [[nodiscard]] constexpr bool is_save_start() const {
      if (FqDirection::Forward == direction_) {
        // forward reads : first 4 lines block start
        return 0 == ((line_count_ >> 2) & 1);
      } else {
        // reverse reads : second 4 lines block start
        return 1 == ((line_count_ >> 2) & 1);
      }
    }

    [[nodiscard]] constexpr bool is_skip(int c) {
      if (c == '\n') ++line_count_;  // count lines 0-based index
      if (is_save_start()) {
        saved_line_count_ = 4;
        return false;
      }
      return saved_line_count_ == 0;
    }

    FqDirection direction_;
    // line_count_ starts at 1 b
    // Max line_count_ is  2^64 - 1 = 18,446,744,073,709,551,615
    uint64_t line_count_{0};
    int saved_line_count_{0};
  };

  namespace details {

    struct txt_tag {};
    struct gzip_tag : txt_tag {};
    struct bzip2_tag : txt_tag {};

    txt_tag format_category(std::string_view path) {
      if (path.ends_with(".gz")) {
        return gzip_tag{};
      } else if (path.ends_with(".bz2")) {
        return bzip2_tag{};
      } else {
        return txt_tag{};
      }
    }

    //----------------------------Define helper functions---------------------------//
    void split_fq_impl(const fs::path& input, FqDirection direction, gzip_tag) {
      boost::iostreams::filtering_istream in;
      in.push(fq_filter(direction));
      in.push(boost::iostreams::gzip_decompressor());
      in.push(boost::iostreams::file_source(input, std::ios_base::in | std::ios_base::binary));

      std::ofstream out{fmt::format("{}{}{}", input.stem().string(),
                                    direction == FqDirection::Forward ? "1" : "2",
                                    input.extension().string())};
      boost::iostreams::copy(in, out);
      out.close();
    }

    void split_fq_impl(const fs::path& input, FqDirection direction, txt_tag) {
      boost::iostreams::filtering_istream in;
      in.push(fq_filter(direction));
      in.push(boost::iostreams::file_source(input, std::ios_base::in));
      std::ofstream out{fmt::format("{}{}{}", input.stem().string(),
                                    direction == FqDirection::Forward ? "1" : "2",
                                    input.extension().string())};
      boost::iostreams::copy(in, out);
      out.close();
    }

  }  // namespace details

  //--------------Definitions of functions for splitting ncbi fq files----//

  void split_fq(const fs::path& input) {
    details::split_fq_impl(input, FqDirection::Forward, details::format_category(input.string()));
    details::split_fq_impl(input, FqDirection::Reverse, details::format_category(input.string()));
  }

}  // namespace boss::fqsp

#endif  // FQ_UTILS_H