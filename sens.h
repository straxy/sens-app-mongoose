/**
 * @file sens.h
 * @brief Header file defining interfaces for interacting with a sensing system
 * using Mongoose.
 */

#ifndef __SENS_H__
#define __SENS_H__

#include "mongoose/mongoose_glue.h"

/**
 * @brief Function to get data from the sensing system.
 *
 * This function retrieves data from the sensing system and stores it in the
 * provided struct.
 *
 * @param data Pointer to a struct where the retrieved data will be stored.
 */
extern void sens_get_data(struct data *data);

/**
 * @brief Function to get the current enable state of the sensing system.
 *
 * This function retrieves the current enable state from the sensing system and
 * stores it in the provided struct.
 *
 * @param data Pointer to a struct where the retrieved enable state will be
 * stored.
 */
extern void sens_get_enable(struct enable *data);

/**
 * @brief Function to set the enable state of the sensing system.
 *
 * This function sets the enable state of the sensing system based on the
 * provided data.
 *
 * @param data Pointer to a struct containing the new enable state.
 */
extern void sens_set_enable(struct enable *data);

#endif /* __SENS_H__ */
