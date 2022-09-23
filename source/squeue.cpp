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
#include <boss/squeue.hpp>

namespace boss::squeue {

  std::vector<std::string> get_cmd_output(boost::filesystem::path const& cmd) {
    bp::ipstream is;  // reading pipe-stream
    bp::child c(cmd, bp::std_out > is);

    std::vector<std::string> data;
    std::string line;

    while (c.running() && std::getline(is, line) && !line.empty()) data.push_back(line);

    c.wait();

    return data;
  }

  bool check_cmd(std::string_view cmd) {
    auto cmd_path = bp::search_path(cmd.data());
    return !cmd_path.empty();
  }

  // 1541748  short 298ALMFi  zlc6394  R    3:41:03      1 qnode0389

}  // namespace boss::squeue