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

//
// Created by li002252 on 9/23/22.
//

#ifndef BOSS_ALL_UTILS_HPP
#define BOSS_ALL_UTILS_HPP
#include <spdlog/spdlog.h>

#include <boost/algorithm/string.hpp>
#include <boost/process.hpp>
#include <cctype>
#include <iostream>
#include <ranges>
#include <string>

namespace boss::utils {

  using namespace std::string_literals;

  std::string operator+(std::string_view lhs, std::string_view rhs);
  std::string operator+(std::string_view lhs, std::string rhs);
  std::string operator+(std::string lhs, std::string_view rhs);

  template <typename... Args>
  std::vector<std::string> cmd_output(std::string_view cmd, Args&&... args) {
    auto res = std::vector<std::string>();

    auto cmd_path = boost::process::search_path(cmd.data());

    if (cmd_path.empty()) {
      spdlog::error("Command {} not found", cmd);
      return res;
    }

    std::string cmd_args = (" "s + ... + args);

    auto final_cmd = cmd_path.string() + cmd_args;

    spdlog::debug("cmd: {}", final_cmd);

    boost::process::ipstream is;  // reading pipe-stream
    boost::process::child c(final_cmd, boost::process::std_out > is);

    std::string line;

    while (c.running() && std::getline(is, line) && !line.empty()) res.push_back(line);

    c.wait();

    return res;
  }

  template <std::ranges::input_range Range> void printr(Range const& range) {
    int index = 0;
    for (auto const& item : range) {
      std::cout << index++ << ": " << item << "\n";
    }
  }

}  // namespace boss::utils

#endif  // BOSS_ALL_UTILS_HPP
