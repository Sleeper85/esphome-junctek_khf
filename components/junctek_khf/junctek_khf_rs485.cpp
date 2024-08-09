// Updated : 2024.08.09
// Version : 1.1.1
// GitHub  : https://github.com/Sleeper85/esphome-junctek_khf

#include "junctek_khf.h"

#include "esphome/core/log.h"
#include "esphome/core/optional.h"

#include <string>
#include <string.h>

#include <setjmp.h>

static jmp_buf parsing_failed;
static const char *const TAG = "Junctek KH-F";

esphome::optional<int> try_getval(const char*& cursor)
{
  long val;
  const char* pos = cursor;
  char* end = nullptr;
  val = strtoll(pos, &end, 10);
  if (end == pos || end == nullptr)
  {
    return nullopt;
  }
  if (*end != ',' && *end != '.')
  {
    ESP_LOGD("JunctekKHF", "Error no coma %s", cursor);
    return nullopt;
  }
  cursor = end + 1; // Skip coma
  return val;
}

// Get a value where it's expected to be "<number>[,.], incrementing the cursor past the end"
int getval(const char*& cursor)
{
  auto val = try_getval(cursor);
  if (!val)
  {
    longjmp(parsing_failed, 1);
  }
  return *val;
}
  

JunctekKHF::JunctekKHF(unsigned address, bool invert_current)
  : address_(address)
  , invert_current_(invert_current)
{
}

void JunctekKHF::dump_config()
{
  ESP_LOGCONFIG(TAG, "junctek_khf:");
  ESP_LOGCONFIG(TAG, "  address: %d", this->address_);
  ESP_LOGCONFIG(TAG, "  invert_current: %s", this->invert_current_ ? "True" : "False");
}

// Get R51 datas
void JunctekKHF::handle_settings(const char* buffer)
{
  ESP_LOGD("JunctekKHF", "Settings %s", buffer);
  const char* cursor = buffer;
  const int address = getval(cursor);
  if (address != this->address_)
    return;
  const int checksum = getval(cursor);
  if (! verify_checksum(checksum, cursor))
    return;

  const float overVoltageProtection = getval(cursor) / 100.0;
  const float underVoltageProtection = getval(cursor) / 100.0;
  const float overDischargeCurrentProtection = getval(cursor) / 100.0;
  const float overChargeCurrentProtection = getval(cursor) / 100.00;
  const float overPowerProtection = getval(cursor) / 100.00;
  const float overTemperatureProtection = getval(cursor) - 100.0;
  const int protectionRecoveryTimeSecond = getval(cursor);
  const int protectionDelayTimeSecond = getval(cursor);
  const float batteryCapacityAh = getval(cursor) / 10.0;
  const int voltageCalibration = getval(cursor);
  const int currentCalibration = getval(cursor);
  const float temperatureCalibration = getval(cursor) - 100.0;
  const int undefinedFunction1 = getval(cursor);
  const int relayMode = getval(cursor); // 0-Normally_Open, 1-Normally_Closed
  const int currentMultiplier = getval(cursor);
  const int timeCalibration = getval(cursor);
  const int dataLogging = getval(cursor); // 0-Enabled, 1-Disabled
  const float soc100Voltage = getval(cursor) / 100.0;
  const float soc0Voltage = getval(cursor) / 100.0;
  const int fullChargeCurrentPercent = getval(cursor);
  const float monitoringTimeMinute = getval(cursor) / 10.0;
  const float underTemperatureProtection = getval(cursor) - 100.0;
  const int temperatureUnit = getval(cursor); // 0-Celsius, 1-Fahrenheit
  const int BluetoothPassword = getval(cursor);
  const int dataLoggingIntervalSecond = getval(cursor); // 59 = 60s
  const int undefinedFunction2 = getval(cursor);

  // Save the capacity for calculating the SoC %
  this->battery_capacity_ = batteryCapacityAh;

  ESP_LOGD("JunctekKHF", "Received : %f %f %f", batteryCapacityAh, soc100Voltage, soc0Voltage);
  if (battery_capacity_ah_sensor_)
    this->battery_capacity_ah_sensor_->publish_state(batteryCapacityAh);
  if (soc100_voltage_sensor_)
    this->soc100_voltage_sensor_->publish_state(soc100Voltage);
  if (soc0_voltage_sensor_)
    this->soc0_voltage_sensor_->publish_state(soc0Voltage);
  if (over_voltage_protection_sensor_)
    this->over_voltage_protection_sensor_->publish_state(overVoltageProtection);
  if (under_voltage_protection_sensor_)
    this->under_voltage_protection_sensor_->publish_state(underVoltageProtection);
  if (over_discharge_current_protection_sensor_)
    this->over_discharge_current_protection_sensor_->publish_state(overDischargeCurrentProtection);
  if (over_charge_current_protection_sensor_)
    this->over_charge_current_protection_sensor_->publish_state(overChargeCurrentProtection);
  if (over_power_protection_sensor_)
    this->over_power_protection_sensor_->publish_state(overPowerProtection);
  if (over_temperature_protection_sensor_)
    this->over_temperature_protection_sensor_->publish_state(overTemperatureProtection);
  if (under_temperature_protection_sensor_)
    this->under_temperature_protection_sensor_->publish_state(underTemperatureProtection);

  this->last_settings_ = millis();
}

// Get R50 datas
void JunctekKHF::handle_status(const char* buffer)
{
  ESP_LOGD("JunctekKHF", "Status %s", buffer);
  const char* cursor = buffer;
  const int address = getval(cursor);
  if (address != this->address_)
    return;
 
  const int checksum = getval(cursor);
  if (! verify_checksum(checksum, cursor))
    return;

  const float voltage = getval(cursor) / 100.0;
  const float current = getval(cursor) / 100.0;
  const float remaining_ah = getval(cursor) / 1000.0;
  const float discharged_kwh = getval(cursor) / 100000.0;
  const float charged_kwh = getval(cursor) / 100000.0;
  const float runtime_sec = getval(cursor);
  const float temperature = getval(cursor) - 100.0;
  const float no_function = getval(cursor);
  const int output_status = getval(cursor);
  const int current_direction = getval(cursor);
  const int remaining_time_min = getval(cursor);
  const int time_adjustment = getval(cursor);
  const int date = getval(cursor);
  const int time = getval(cursor);

  ESP_LOGD("JunctekKHF", "Received : %f %f %f %f", voltage, current, remaining_ah, temperature);
  // Voltage
  if (voltage_sensor_)
    this->voltage_sensor_->publish_state(voltage);
  // Current and Power
  if (current_sensor_) {
    // 0-discharging, 1-charging
    float adjustedCurrent = current_direction == 0 ? -current : current;
    if (invert_current_) adjustedCurrent *= -1;
    current_sensor_->publish_state(adjustedCurrent);
    if (power_sensor_) {
      float power = (voltage * adjustedCurrent);
      power_sensor_->publish_state(power);
    }
  }
  // State of Charge
  if (battery_level_sensor_ && this->battery_capacity_)
    this->battery_level_sensor_->publish_state(remaining_ah * 100.0 / *this->battery_capacity_); // Battery SoC %
  // Discharged kWh
  if (discharged_kwh_sensor_)
    this->discharged_kwh_sensor_->publish_state(discharged_kwh);
  // Charged kWh
  if (charged_kwh_sensor_)
    this->charged_kwh_sensor_->publish_state(charged_kwh);
  // Temperature
  if (temperature_sensor_)
    this->temperature_sensor_->publish_state(temperature);

  this->last_status_ = millis();
}

void JunctekKHF::handle_line()
{
  // A failure in parsing will return back to here with a non-zero value
  if (setjmp(parsing_failed))
    return;
  
  const char* buffer = &this->line_buffer_[0];
  // if the line does not start with `:r` do nothing
  if (buffer[0] != ':' || buffer[1] != 'r')
    return;
  if (strncmp(&buffer[2], "50=", 3) == 0)
    handle_status(&buffer[5]);
  else if (strncmp(&buffer[2], "51=", 3) == 0)
    handle_settings(&buffer[5]);

  return;
}

// Junctek KH-F received UART-TTL (not RS485) datas with display connected
// Display request ":R50=1,\n"
// Module response ":r50=1,46,5319,396,254929,25304424,26717081,91107,122,0,99,0,3877,100,101,0,\r\n"
// Display request ":R51=1,\n"
// Module response ":r51=1,65,0,0,0,0,0,255,0,0,2800,100,100,95,0,0,1,100,0,2752,2450,2,20,255,0,0,59,0,\r\n"

bool JunctekKHF::readline()
{
  while (available()) {
    const char readch = read();
    if (readch > 0) {
      switch (readch) {
        case '\n': // Ignore new-lines
          // break;
        case '\r': // Return on CR
          this->line_pos_ = 0; // Reset position index ready for next time
          return true;
        default:   // Will be executed if there is no match
          if (this->line_pos_ < MAX_LINE_LEN - 1)
          {
            this->line_buffer_[this->line_pos_++] = readch;
            this->line_buffer_[this->line_pos_] = 0;
          }
      }
    }
  }
  return false;
}

bool JunctekKHF::verify_checksum(int checksum, const char* buffer)
{
  long total = 0;
  while (auto val = try_getval(buffer))
  {
    total += *val;
  }
  const bool checksum_valid = (total % 255) + 1 == checksum;
  ESP_LOGD("JunctekKHF", "Received : checksum %d total %ld valid %d", checksum, total, checksum_valid);
  return checksum_valid;
}

void JunctekKHF::loop()
{
  const unsigned long start_time = millis();

  // Use this block for RS485 communication only
  // Send R51 command every 30s
  if (!this->last_settings_ || (*this->last_settings_ + (30 * 1000)) < start_time)
  {
    this->last_settings_ = start_time;
    char buffer[20];
    sprintf(buffer, ":R51=%d,2,1,\r\n", this->address_);
    write_str(buffer);
  }
  // Send R50 command every 10s
  if (!this->last_status_ || (*this->last_status_ + (10 * 1000)) < start_time)
  {
    this->last_status_ = start_time;
    char buffer[20];
    sprintf(buffer, ":R50=%d,2,1,\r\n", this->address_);
    write_str(buffer);
  }
  
  if (readline()) handle_line();
}

float JunctekKHF::get_setup_priority() const
{
  return setup_priority::DATA;
}
