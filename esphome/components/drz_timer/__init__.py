from esphome import automation

import esphome.codegen as cg
import esphome.config_validation as cv

from esphome.components import binary_sensor, sensor
from esphome.const import CONF_ID, UNIT_SECOND, ICON_TIMER

MULTI_CONF = True

AUTO_LOAD = ["binary_sensor", "sensor"]

drztimer_ns = cg.esphome_ns.namespace("drztimer")
Timer = drztimer_ns.class_("Timer", cg.Component)

CONF_TIMER_ACTIVE = "timer_active"
CONF_TIME_LEFT = "time_left"
CONF_TIME = "time"

CONF_ON_END = "on_end"
CONF_ON_START = "on_start"
CONF_ON_STOP = "on_stop"

CONFIG_SCHEMA = cv.COMPONENT_SCHEMA.extend(
    {
        # id
        cv.GenerateID(): cv.declare_id(Timer),
        cv.Required(CONF_TIME): cv.positive_time_period_seconds,
        # time_left sensor
        cv.Optional(CONF_TIME_LEFT): sensor.sensor_schema(
            unit_of_measurement=UNIT_SECOND, icon=ICON_TIMER, accuracy_decimals=1
        ),
        # timer_active binary_sensor
        cv.Optional(CONF_TIMER_ACTIVE): binary_sensor.binary_sensor_schema(),
        # triggers
        cv.Optional(CONF_ON_END): automation.validate_automation(single=True),
        cv.Optional(CONF_ON_START): automation.validate_automation(single=True),
        cv.Optional(CONF_ON_STOP): automation.validate_automation(single=True),
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    cg.add(var.set_inital_time(config[CONF_TIME]))

    if CONF_TIME_LEFT in config:
        sens = await sensor.new_sensor(config[CONF_TIME_LEFT])
        cg.add(var.set_time_left(sens))

    if CONF_TIMER_ACTIVE in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_TIMER_ACTIVE])
        cg.add(var.set_timer_active(sens))

    if CONF_ON_END in config:
        await automation.build_automation(
            var.get_end_trigger(), [], config[CONF_ON_END]
        )

    if CONF_ON_START in config:
        await automation.build_automation(
            var.get_start_trigger(), [], config[CONF_ON_START]
        )

    if CONF_ON_STOP in config:
        await automation.build_automation(
            var.get_stop_trigger(), [], config[CONF_ON_STOP]
        )
