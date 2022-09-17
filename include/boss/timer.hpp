//
// Created by li002252 on 9/17/22.
//

#ifndef BOSS_ALL_TIMER_HPP
#define BOSS_ALL_TIMER_HPP
#include <chrono>

namespace boss {

  template <typename Clock = std::chrono::high_resolution_clock> class Timer {
  public:
    using clock_type = Clock;

    constexpr Timer() : start_(clock_type::now()) {}

    [[nodiscard]] constexpr auto elapsed() const {
      return std::chrono::duration<double>(clock_type::now() - start_).count();
    }

    [[maybe_unused]] constexpr auto reset() { start_ = clock_type::now(); }

  private:
    typename clock_type::time_point start_{};
  };
}  // namespace boss
#endif  // BOSS_ALL_TIMER_HPP
