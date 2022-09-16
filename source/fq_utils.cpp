#include <boss/fq_utils.hpp>
namespace boss {

  bool is_gzip(std::string_view path) { return path.ends_with(".gz"); }
}  // namespace boss
