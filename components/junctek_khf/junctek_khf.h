// Updated : 2024.08.09
// Version : 1.1.1
// GitHub  : https://github.com/Sleeper85/esphome-junctek_khf

#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"

using namespace esphome;

class JunctekKHF
  : public esphome::Component
  , public uart::UARTDevice
{
public:
  JunctekKHF(unsigned address = 1, bool invert_current=false);

  void set_voltage_sensor(sensor::Sensor *voltage_sensor) { voltage_sensor_ = voltage_sensor; }
  void set_current_sensor(sensor::Sensor *current_sensor) { current_sensor_ = current_sensor; }
  void set_power_sensor(sensor::Sensor *power_sensor) { power_sensor_ = power_sensor; }
  void set_battery_level_sensor(sensor::Sensor *battery_level_sensor) { battery_level_sensor_ = battery_level_sensor; }
  void set_discharged_kwh_sensor(sensor::Sensor *discharged_kwh_sensor) { discharged_kwh_sensor_ = discharged_kwh_sensor; }
  void set_charged_kwh_sensor(sensor::Sensor *charged_kwh_sensor) { charged_kwh_sensor_ = charged_kwh_sensor; }
  void set_temperature_sensor(sensor::Sensor *temperature_sensor) { temperature_sensor_ = temperature_sensor; }
  void set_battery_capacity_ah_sensor(sensor::Sensor *battery_capacity_ah_sensor) { battery_capacity_ah_sensor_ = battery_capacity_ah_sensor; }
  void set_soc100_voltage_sensor(sensor::Sensor *soc100_voltage_sensor) { soc100_voltage_sensor_ = soc100_voltage_sensor; }
  void set_soc0_voltage_sensor(sensor::Sensor *soc0_voltage_sensor) { soc0_voltage_sensor_ = soc0_voltage_sensor; }
  void set_over_voltage_protection_sensor(sensor::Sensor *over_voltage_protection_sensor) { over_voltage_protection_sensor_ = over_voltage_protection_sensor; }
  void set_under_voltage_protection_sensor(sensor::Sensor *under_voltage_protection_sensor) { under_voltage_protection_sensor_ = under_voltage_protection_sensor; }
  void set_over_discharge_current_protection_sensor(sensor::Sensor *over_discharge_current_protection_sensor) { over_discharge_current_protection_sensor_ = over_discharge_current_protection_sensor; }
  void set_over_charge_current_protection_sensor(sensor::Sensor *over_charge_current_protection_sensor) { over_charge_current_protection_sensor_ = over_charge_current_protection_sensor; }
  void set_over_power_protection_sensor(sensor::Sensor *over_power_protection_sensor) { over_power_protection_sensor_ = over_power_protection_sensor; }
  void set_over_temperature_protection_sensor(sensor::Sensor *over_temperature_protection_sensor) { over_temperature_protection_sensor_ = over_temperature_protection_sensor; }
  void set_under_temperature_protection_sensor(sensor::Sensor *under_temperature_protection_sensor) { under_temperature_protection_sensor_ = under_temperature_protection_sensor; }

  void dump_config() override;
  void loop() override;

  float get_setup_priority() const; // override;

protected:
  bool readline();
  void handle_line();
  void handle_status(const char* buffer);
  void handle_settings(const char* buffer);
  void request_data(uint8_t data_id);
  void decode_data(std::vector<uint8_t> data);
  bool verify_checksum(int checksum, const char* buffer);

  const unsigned address_;

  sensor::Sensor* voltage_sensor_{nullptr};
  sensor::Sensor* current_sensor_{nullptr};
  sensor::Sensor* power_sensor_{nullptr};
  sensor::Sensor* battery_level_sensor_{nullptr};
  sensor::Sensor* discharged_kwh_sensor_{nullptr};
  sensor::Sensor* charged_kwh_sensor_{nullptr};
  sensor::Sensor* temperature_sensor_{nullptr};
  sensor::Sensor* battery_capacity_ah_sensor_{nullptr};
  sensor::Sensor* soc100_voltage_sensor_{nullptr};
  sensor::Sensor* soc0_voltage_sensor_{nullptr};
  sensor::Sensor* over_voltage_protection_sensor_{nullptr};
  sensor::Sensor* under_voltage_protection_sensor_{nullptr};
  sensor::Sensor* over_discharge_current_protection_sensor_{nullptr};
  sensor::Sensor* over_charge_current_protection_sensor_{nullptr};
  sensor::Sensor* over_power_protection_sensor_{nullptr};
  sensor::Sensor* over_temperature_protection_sensor_{nullptr};
  sensor::Sensor* under_temperature_protection_sensor_{nullptr};

  static constexpr int MAX_LINE_LEN = 120;
  std::array<char, MAX_LINE_LEN> line_buffer_;
  size_t line_pos_ = 0;

  optional<float> battery_capacity_;
  optional<unsigned long> last_settings_;
  optional<unsigned long> last_status_;
  bool invert_current_;
};
