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
#include <map>
#include <string>
#include <vector>

namespace boss::squeue {

  enum class Status {
    R,   // running
    PD,  // pending
    CG,  // completing
    ST,  // stopped
  };

  std::ostream& operator<<(std::ostream& os, Status s);

  Status to_status(std::string_view status);

  class Queue {
  public:
    explicit Queue(std::string_view name);
    void update(Status status);

    [[nodiscard]] auto running() const -> int { return status_count_.at(Status::R); }
    [[nodiscard]] auto pending() const -> int { return status_count_.at(Status::PD); }
    [[nodiscard]] auto completing() const -> int { return status_count_.at(Status::CG); }
    [[nodiscard]] auto stopped() const -> int { return status_count_.at(Status::ST); }

  private:
    friend class Queues;
    std::string name_{};
    std::map<Status, int> status_count_{
        {Status::R, 0},
        {Status::PD, 0},
        {Status::CG, 0},
        {Status::ST, 0},
    };
  };

  class Queues {
  public:
    Queues() = default;
    void summary(const std::vector<std::string>& data);
    void print_table() const;

  private:
    void update(std::string_view queue_name, Status status);
    [[nodiscard]] std::vector<std::string> parse_line(std::string_view line) const;

    std::map<std::string, Queue> queues_{};
  };

}  // namespace boss::squeue

#endif  // BOSS_ALL_SQUEUE_HPP
