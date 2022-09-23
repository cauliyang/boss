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
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

#include <boost/algorithm/string.hpp>
#include <boss/squeue.hpp>
#include <boss/utils.hpp>
#include <tabulate/table.hpp>

namespace boss::squeue {

  namespace ba = boost::algorithm;

  std::ostream& operator<<(std::ostream& os, Status s) {
    switch (s) {
      case Status::R:
        os << "running";
        break;
      case Status::PD:
        os << "pending";
        break;
      case Status::CG:
        os << "completing";
        break;
      case Status::ST:
        os << "stopped";
        break;
    }
    return os;
  }

  Status to_status(std::string_view status) {
    static std::map<std::string_view, Status> const status_map = {
        {"R", Status::R},
        {"PD", Status::PD},
        {"CG", Status::CG},
        {"ST", Status::ST},
    };
    spdlog::debug("status: {}", status);
    return status_map.at(status);
  }

  Queue::Queue(std::string_view name) : name_{name} {}

  void Queue::update(Status status) { status_count_[status]++; }

  void Queues::update(std::string_view queue_name, Status status) {
    spdlog::debug("queue_name: {}, status: {}", queue_name, status);
    auto [iter, success] = queues_.try_emplace(queue_name.data(), queue_name);
    iter->second.update(status);
  }

  // 1541748  short 298ALMFi  zlc6394  R    3:41:03      1 qnode 0389
  void Queues::summary(const std::vector<std::string>& data) {
    for (auto line : data) {
      ba::trim_if(line, ba::is_any_of(" "));
      auto tokens = parse_line(line);
      std::size_t status_index = tokens.size() == 9 ? 5 : 4;

      update(tokens.at(1), to_status(tokens.at(status_index)));
    }
  }

  std::vector<std::string> Queues::parse_line(std::string_view line) const {
    // split line  by space

    std::vector<std::string> tokens{};
    ba::split(tokens, line, ba::is_space(), ba::token_compress_on);
    return tokens;
  }

  void Queues::print_table() const {
    using namespace tabulate;
    Table table;
    table.format().font_style({FontStyle::bold});
    table.add_row({"Queue", "Running", "Pending", "Stopped"});

    for (auto& [name, queue] : queues_) {
      table.add_row({name, std::to_string(queue.running()), std::to_string(queue.pending()),
                     std::to_string(queue.stopped())});
    }

    for (size_t i = 0; i < 4; ++i) {
      table[0][i].format().font_color(Color::yellow).font_style({FontStyle::bold});
    }

    table.column(0).format().font_align(FontAlign::left);
    table.column(1).format().font_align(FontAlign::right);
    table.column(2).format().font_align(FontAlign::right);
    table.column(3).format().font_align(FontAlign::right);

    std::cout << table << "\n";
  }

}  // namespace boss::squeue