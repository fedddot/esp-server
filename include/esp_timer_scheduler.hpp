#ifndef ESP_TIMER_SCHEDULER_HPP
#define ESP_TIMER_SCHEDULER_HPP

#include <cstddef>
#include <optional>
#include <stdexcept>

#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"

#include "timer_scheduler.hpp"

namespace esp {
    class EspTimerScheduler: public manager::TimerScheduler {
    public:
        EspTimerScheduler() = default;
        EspTimerScheduler(const EspTimerScheduler&) = delete;
        EspTimerScheduler& operator=(const EspTimerScheduler&) = delete;

        manager::Instance<TaskGuard> schedule_task(const Task& task, const std::size_t period_ms) override {
            if (m_running_task.has_value()) {
                throw std::runtime_error("a task is already running, unschedule it first");
            }
            if (task == nullptr || period_ms == 0) {
                throw std::invalid_argument("task cannot be null and period must be greater than zero");
            }
            m_running_task = task;
            TimerHandle_t timer_handle = xTimerCreate(
                "ScheduledTimer",
                pdMS_TO_TICKS(period_ms),
                pdTRUE,
                this,
                &EspTimerScheduler::timer_callback
            );
            if (timer_handle == nullptr) {
                m_running_task.reset();
                throw std::runtime_error("failed to create scheduled timer");
            }
            if (xTimerStart(timer_handle, 0) != pdPASS) {
                xTimerDelete(timer_handle, 0);
                m_running_task.reset();
                throw std::runtime_error("failed to start scheduled timer");
            }
            return manager::Instance<TaskGuard>(new EspTimerGuard(timer_handle, this));
        }
    private:
        std::optional<Task> m_running_task;

        static void timer_callback(TimerHandle_t xTimer) {
            auto scheduler = static_cast<EspTimerScheduler *>(pvTimerGetTimerID(xTimer));
            if (!scheduler) {
                return;
            }
            if (!scheduler->m_running_task.has_value()) {
                return;
            }
            const auto& task = scheduler->m_running_task.value();
            task();
        }

        class EspTimerGuard: public TaskGuard {
        public:
            EspTimerGuard(TimerHandle_t timer_handle, EspTimerScheduler* scheduler)
                : m_timer_handle(timer_handle), m_scheduler(scheduler) {
                if (m_timer_handle == nullptr || m_scheduler == nullptr) {
                    throw std::invalid_argument("invalid timer handle or scheduler ptr received");
                }
            }
            EspTimerGuard(const EspTimerGuard&) = delete;
            EspTimerGuard& operator=(const EspTimerGuard&) = delete;

            ~EspTimerGuard() noexcept override {
                unschedule();
            }

            void unschedule() override {
                if (m_timer_handle) {
                    xTimerStop(m_timer_handle, 0);
                    xTimerDelete(m_timer_handle, 0);
                    m_timer_handle = nullptr;
                }
                m_scheduler->m_running_task.reset();
            }
        private:
            TimerHandle_t m_timer_handle;
            EspTimerScheduler* m_scheduler;
        };
    };
}

#endif // ESP_TIMER_SCHEDULER_HPP
