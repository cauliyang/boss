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

#include <boss/squeue.hpp>
#include <boss/timer.hpp>
#include <boss/utils.hpp>
#include <cxxopts.hpp>

auto main(int argc, char** argv) -> int {
  cxxopts::Options options(*argv, "squeue - A program to get the status of jobs in the queue.");

  // clang-format off
  options.add_options()
    ("h,help", "Show help")
    ("v,version", "Print the current version number")
    ("m,me", "Check Your own jobs' status", cxxopts::value<bool>()->default_value("false"))
    ("d,debug", "Enable debug mode", cxxopts::value<bool>()->default_value("false"))
  ;
  // clang-format on

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
    constexpr std::string_view squeue_cmd = "squeue";
    constexpr std::string_view squeue_me_cmd = "squeue --me";
    spdlog::debug("Checking if {} is available", squeue_cmd);
    spdlog::debug("Checking if {} is available", squeue_me_cmd);

    if (!boss::utils::check_cmd(squeue_cmd)) {
      spdlog::error("squeue command not found");
      std::exit(1);
    }

    if (auto me = result["me"].as<bool>(); me)
      spdlog::info("Checking your jobs' status");
    else {
      boss::squeue::Queues queues{};
      auto output = boss::utils::get_cmd_output(squeue_cmd);
      output.erase(output.begin());  // remove the header line
      queues.summary(output);
      queues.print_table();
    }

  } catch (const cxxopts::OptionException& e) {
    spdlog::error("{}", e.what());
    std::exit(1);
  }

  return 0;
}
