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
// Created by li002252 on 9/22/22.
//

#ifndef BOSS_ALL_SQUEUE_HPP
#define BOSS_ALL_SQUEUE_HPP
#include <boost/process.hpp>
#include <string>
#include <vector>

namespace boss::squeue {

  namespace bp = boost::process;

  [[nodiscard]] bool check_cmd(std::string_view cmd);

  std::vector<std::string> get_cmd_output(boost::filesystem::path const& cmd);

  void summary(std::vector<std::string> const& data);

}  // namespace boss::squeue

#endif  // BOSS_ALL_SQUEUE_HPP
