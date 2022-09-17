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

#include <boss/version.h>
#include <spdlog/spdlog.h>

#include <binary/utils.hpp>
#include <boss/fq_utils.hpp>
#include <boss/timer.hpp>
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
    if (!boss::fqsp::check_filename(input)) {
      std::exit(1);
    }

    boss::Timer timer{};
    boss::fqsp::split_fq(fs::path(input));
    spdlog::info("Finished in {:.4f} seconds", timer.elapsed());

  } catch (const cxxopts::OptionException& e) {
    spdlog::error("{}", e.what());
    std::exit(1);
  }

  return 0;
}
