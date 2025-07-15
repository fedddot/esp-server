#ifndef ESP_TIMER_SCHEDULER_HPP
#define ESP_TIMER_SCHEDULER_HPP

#include <cstddef>
#include <stdexcept>

#include "timer_scheduler.hpp"

namespace esp {
    class EspTimerScheduler: public manager::TimerScheduler {
    public:
        EspTimerScheduler() = default;
        EspTimerScheduler(const EspTimerScheduler&) = delete;
        EspTimerScheduler& operator=(const EspTimerScheduler&) = delete;

        manager::Instance<TaskGuard> schedule_task(const Task& task, const std::size_t period_ms) override {
            throw std::runtime_error("NOT_IMPLEMENTED");
        }
    private:
    };
}

#endif // ESP_TIMER_SCHEDULER_HPP
