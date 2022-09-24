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

#include <boss/utils.hpp>

namespace boss::utils {
  std::string operator+(std::string_view lhs, std::string_view rhs) {
    return std::string(lhs) + std::string(rhs);
  }
  std::string operator+(std::string_view lhs, std::string rhs) { return lhs.data() + rhs; }
  std::string operator+(std::string lhs, std::string_view rhs) { return lhs + rhs.data(); }
}  // namespace boss::utils
