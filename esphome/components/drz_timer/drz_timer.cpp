#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

#include "drz_timer.h"

namespace esphome {
namespace drztimer {

static const char *const TAG = "drz_timer";

float Timer::get_setup_priority() const { return setup_priority::IO; }

void Timer::setup() {
  ESP_LOGD(TAG, "Setting up timer...");
  _is_running = false;
  _time_left = 15.0f;
}

void Timer::loop() {
  if(!_is_running) {
    return;
  }

  uint32_t now = esp_timer_get_time();
  uint32_t delta = now - this->_last_loop_time;
  float delta_seconds = delta / 1000000.0;

  _time_left -= delta_seconds;

  if(_time_left <= 0) {
    _time_left = 0;
    _is_running = false;
    ESP_LOGD(TAG, "Timer finished");
    if(this->_end_trigger != nullptr)
      this->_end_trigger->trigger();
  }

  if (this->time_left != nullptr)
    this->time_left->publish_state(_time_left);
  if (this->timer_active != nullptr)
    this->timer_active->publish_state(_is_running);

  this->_last_loop_time = now;
}

void Timer::Start(float length) {
  _time_left = length;
  _is_running = true;
  ESP_LOGD(TAG, "Timer started with %f seconds", length);
  if(this->_start_trigger != nullptr)
    this->_start_trigger->trigger();
  if(this->time_left != nullptr)
    this->time_left->publish_state(_time_left);
  if (this->timer_active != nullptr)
    this->timer_active->publish_state(_is_running);
}

void Timer::Stop() {
  _is_running = false;
  ESP_LOGD(TAG, "Timer stopped");
  if(this->_stop_trigger != nullptr)
    this->_stop_trigger->trigger();
  if (this->timer_active != nullptr)
    this->timer_active->publish_state(_is_running);
}

void Timer::Reset() {
  _is_running = false;
  _time_left = 15.0f;
  ESP_LOGD(TAG, "Timer reset");
  if(this->_stop_trigger != nullptr)
    this->_stop_trigger->trigger();
  if (this->time_left != nullptr)
    this->time_left->publish_state(_time_left);
  if (this->timer_active != nullptr)
    this->timer_active->publish_state(_is_running);
}

}  // namespace drztimer
}  // namespace esphome
