#include <boss/version.h>
#include <spdlog/spdlog.h>

#include <binary/utils.hpp>
#include <boss/fq_utils.hpp>
#include <cxxopts.hpp>
#include <iostream>
#include <string>

auto main(int argc, char** argv) -> int {
  namespace fs = std::filesystem;
  cxxopts::Options options(
      *argv, "fqsp - A program to split ncbi fq files into forward and reverse files.");

  // clang-format off
  options.add_options()
    ("h,help", "Show help")
    ("v,version", "Print the current version number")
    ("input", "Input fq file", cxxopts::value<std::string>())
    ("o,output", "Output fq file", cxxopts::value<std::string>())
    ("d,debug", "Enable debug mode", cxxopts::value<bool>()->default_value("false"))
  ;
  // clang-format on

  options.positional_help("[input.fq|.fq.gz]");
  options.parse_positional({"input"});
  auto result = options.parse(argc, argv);

  if (result["help"].as<bool>()) {
    std::cout << options.help() << '\n';
    return 0;
  }

  if (result["version"].as<bool>()) {
    spdlog::info("version {}", BOSS_VERSION);
    return 0;
  }

  if (result["debug"].as<bool>()) {
    spdlog::set_level(spdlog::level::debug);
  }

  try {
    auto input = result["input"].as<std::string>();
    if (!binary::utils::check_file_path(input)) {
      spdlog::error("Input file does not exist: {}", input);
      std::exit(1);
    }

    boss::fqsp::split_fq(fs::path(input));

  } catch (const cxxopts::OptionException& e) {
    spdlog::error("{}", e.what());
    std::exit(1);
  }

  return 0;
}
