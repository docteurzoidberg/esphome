#pragma once

#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/sensor/sensor.h"



namespace esphome {
namespace drztimer {

class Timer : public Component {
 public:
  void setup() override;
  void loop() override;

  float get_setup_priority() const override;

  void set_inital_time(float time) { this->_inital_time = time; }
  void set_time_left(sensor::Sensor *sensor) { this->time_left = sensor; }
  void set_timer_active(binary_sensor::BinarySensor *sensor) { this->timer_active = sensor; }

  void Start();
  void Stop();
  void Reset();

  Trigger<> *get_end_trigger() const { return this->_end_trigger; }
  Trigger<> *get_start_trigger() const { return this->_start_trigger; }
  Trigger<> *get_stop_trigger() const { return this->_stop_trigger; }

 protected:
  sensor::Sensor *time_left{nullptr};
  binary_sensor::BinarySensor *timer_active{nullptr};

  bool _is_running = false;
  uint32_t _last_loop_time = 0;
  float _time_left = 0.0f;
  float _inital_time = 0.0f;
  Trigger<> *_end_trigger = new Trigger<>();
  Trigger<> *_start_trigger = new Trigger<>();
  Trigger<> *_stop_trigger = new Trigger<>();
};



}  // namespace drztimer
}  // namespace esphome
