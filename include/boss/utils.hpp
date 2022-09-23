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
#include <boost/algorithm/string.hpp>
#include <boost/process.hpp>
#include <cctype>
#include <iostream>
#include <ranges>
#include <string>

namespace boss::utils {

  [[nodiscard]] bool check_cmd(std::string_view cmd);
  std::vector<std::string> get_cmd_output(boost::filesystem::path const& cmd);
  std::vector<std::string> get_cmd_output(std::string_view cmd);

  template <std::ranges::input_range Range> void printr(Range const& range) {
    int index = 0;
    for (auto const& item : range) {
      std::cout << index++ << ": " << item << "\n";
    }
  }

}  // namespace boss::utils

#endif  // BOSS_ALL_UTILS_HPP
