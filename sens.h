/**
 * @file sens.h
 * @brief Header file defining interfaces for interacting with a sensing system
 * using Mongoose.
 */

#ifndef __SENS_H__
#define __SENS_H__

#include "mongoose/mongoose_glue.h"

/**
 * @brief Get the current data from custom QEMU sensors.
 *
 * This function reads the current data value from the sysfs files
 * associated with I2C, SPI, and MMSens devices.
 *
 * @param data A pointer to the struct data containing the sensor data
 * values.
 */
extern void sens_get_data(struct data *data);

/**
 * @brief Get the enable state for custom QEMU sensors.
 *
 * This function reads the enable state from the sysfs files
 * associated with I2C, SPI, and MMSens devices.
 *
 * @param data A pointer to the struct enable containing the sensor enable
 * states.
 */
extern void sens_get_enable(struct enable *data);

/**
 * @brief Set the enable status for custom QEMU sensors.
 *
 * This function is responsible for setting the enable status of I2C, SPI, and
 * MMSens sensors based on the provided data structure.
 *
 * @param data Pointer to a struct containing the enable statuses for different
 * sensors.
 */
extern void sens_set_enable(struct enable *data);

#endif /* __SENS_H__ */
