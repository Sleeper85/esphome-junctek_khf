# Updated : 2024.08.09
# Version : 1.1.1
# GitHub  : https://github.com/Sleeper85/esphome-junctek_khf

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, sensor
from esphome.const import (
    CONF_RAW,
    CONF_ID,
    CONF_ADDRESS,
    CONF_INPUT,
    CONF_NUMBER,
    CONF_HARDWARE_UART,
    CONF_UPDATE_INTERVAL,
    CONF_VOLTAGE,
    CONF_CURRENT,
    CONF_POWER,
    CONF_BATTERY_LEVEL,
    CONF_TEMPERATURE,
    STATE_CLASS_MEASUREMENT,
    UNIT_EMPTY,
    UNIT_VOLT,
    UNIT_AMPERE,
    UNIT_WATT,
    UNIT_PERCENT,
    UNIT_CELSIUS,
    ICON_EMPTY,
    DEVICE_CLASS_VOLTAGE,
    DEVICE_CLASS_CURRENT,
    DEVICE_CLASS_POWER,
    DEVICE_CLASS_ENERGY,
    DEVICE_CLASS_BATTERY,
    DEVICE_CLASS_TEMPERATURE,
)

CODEOWNERS = ["@Sleeper85"]

DEPENDENCIES = ["uart"]

AUTO_LOAD = ["sensor"]

CONF_CHARGED_KWH="charged_kwh"
CONF_DISCHARGED_KWH="discharged_kwh"
CONF_BATTERY_CAPACITY_AH="battery_capacity_ah"
CONF_SOC100_VOLTAGE="soc100_voltage"
CONF_SOC0_VOLTAGE="soc0_voltage"
CONF_OVER_VOLTAGE_PROTECTION="over_voltage_protection"
CONF_UNDER_VOLTAGE_PROTECTION="under_voltage_protection"
CONF_OVER_DISCHARGE_CURENT_PROTECTION="over_discharge_current_protection"
CONF_OVER_CHARGE_CURENT_PROTECTION="over_charge_current_protection"
CONF_OVER_POWER_PROTECTION="over_power_protection"
CONF_OVER_TEMPERATURE_PROTECTION="over_temperature_protection"
CONF_UNDER_TEMPERATURE_PROTECTION="under_temperature_protection"

UNIT_AMPERE_HOUR = "Ah"
UNIT_KILOWATT_HOUR = "kWh"

ICON_CURRENT_DC = "mdi:current-dc"

SENSORS = [
    CONF_VOLTAGE,
    CONF_CURRENT,
    CONF_POWER,
    CONF_BATTERY_LEVEL,
    CONF_TEMPERATURE,
    CONF_CHARGED_KWH,
    CONF_DISCHARGED_KWH,
    CONF_BATTERY_CAPACITY_AH,
    CONF_SOC100_VOLTAGE,
    CONF_SOC0_VOLTAGE,
    CONF_OVER_VOLTAGE_PROTECTION,
    CONF_UNDER_VOLTAGE_PROTECTION,
    CONF_OVER_DISCHARGE_CURENT_PROTECTION,
    CONF_OVER_CHARGE_CURENT_PROTECTION,
    CONF_OVER_POWER_PROTECTION,
    CONF_OVER_TEMPERATURE_PROTECTION,
    CONF_UNDER_TEMPERATURE_PROTECTION,
]

CONF_INVERT_CURRENT="invert_current"

JunctekKHF = cg.global_ns.class_(
    "JunctekKHF", cg.Component, uart.UARTDevice
)

CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(JunctekKHF),
            cv.Optional(CONF_ADDRESS, default=1): cv.int_range(1, 255),
            cv.Optional(CONF_VOLTAGE): sensor.sensor_schema(
                unit_of_measurement=UNIT_VOLT,
                icon=ICON_EMPTY,
                accuracy_decimals=2,
                device_class=DEVICE_CLASS_VOLTAGE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_CURRENT): sensor.sensor_schema(
                unit_of_measurement=UNIT_AMPERE,
                icon=ICON_CURRENT_DC,
                accuracy_decimals=2,
                device_class=DEVICE_CLASS_CURRENT,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_POWER): sensor.sensor_schema(
                unit_of_measurement=UNIT_WATT,
                icon=ICON_EMPTY,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_POWER,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_BATTERY_LEVEL): sensor.sensor_schema(
                unit_of_measurement=UNIT_PERCENT,
                icon=ICON_EMPTY,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_BATTERY,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
             cv.Optional(CONF_TEMPERATURE): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                icon=ICON_EMPTY,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_TEMPERATURE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_CHARGED_KWH): sensor.sensor_schema(
                unit_of_measurement=UNIT_KILOWATT_HOUR,
                icon=ICON_EMPTY,
                accuracy_decimals=3,
                device_class=DEVICE_CLASS_POWER,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_DISCHARGED_KWH): sensor.sensor_schema(
                unit_of_measurement=UNIT_KILOWATT_HOUR,
                icon=ICON_EMPTY,
                accuracy_decimals=3,
                device_class=DEVICE_CLASS_POWER,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_BATTERY_CAPACITY_AH): sensor.sensor_schema(
                unit_of_measurement=UNIT_AMPERE_HOUR,
                icon=ICON_EMPTY,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_ENERGY,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_SOC100_VOLTAGE): sensor.sensor_schema(
                unit_of_measurement=UNIT_VOLT,
                icon=ICON_EMPTY,
                accuracy_decimals=2,
                device_class=DEVICE_CLASS_VOLTAGE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_SOC0_VOLTAGE): sensor.sensor_schema(
                unit_of_measurement=UNIT_VOLT,
                icon=ICON_EMPTY,
                accuracy_decimals=2,
                device_class=DEVICE_CLASS_VOLTAGE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_OVER_VOLTAGE_PROTECTION): sensor.sensor_schema(
                unit_of_measurement=UNIT_VOLT,
                icon=ICON_EMPTY,
                accuracy_decimals=2,
                device_class=DEVICE_CLASS_VOLTAGE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_UNDER_VOLTAGE_PROTECTION): sensor.sensor_schema(
                unit_of_measurement=UNIT_VOLT,
                icon=ICON_EMPTY,
                accuracy_decimals=2,
                device_class=DEVICE_CLASS_VOLTAGE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_OVER_DISCHARGE_CURENT_PROTECTION): sensor.sensor_schema(
                unit_of_measurement=UNIT_AMPERE,
                icon=ICON_CURRENT_DC,
                accuracy_decimals=2,
                device_class=DEVICE_CLASS_CURRENT,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_OVER_CHARGE_CURENT_PROTECTION): sensor.sensor_schema(
                unit_of_measurement=UNIT_AMPERE,
                icon=ICON_CURRENT_DC,
                accuracy_decimals=2,
                device_class=DEVICE_CLASS_CURRENT,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_OVER_POWER_PROTECTION): sensor.sensor_schema(
                unit_of_measurement=UNIT_WATT,
                icon=ICON_EMPTY,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_POWER,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_OVER_TEMPERATURE_PROTECTION): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                icon=ICON_EMPTY,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_TEMPERATURE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_UNDER_TEMPERATURE_PROTECTION): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                icon=ICON_EMPTY,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_TEMPERATURE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_INVERT_CURRENT, default=False): cv.boolean, 
        }
    ).extend(uart.UART_DEVICE_SCHEMA)
    )

async def setup_conf(config, key, hub):
    if key in config:
        conf = config[key]
        sens = await sensor.new_sensor(conf)
        cg.add(getattr(hub, f"set_{key}_sensor")(sens))


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID], config[CONF_ADDRESS], config[CONF_INVERT_CURRENT])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
    for key in SENSORS:
        await setup_conf(config, key, var)
