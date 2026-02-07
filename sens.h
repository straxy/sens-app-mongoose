/*
 * Include for sens
 */

#ifndef __SENS_H__
#define __SENS_H__

#include "mongoose/mongoose_glue.h"

extern void sens_get_data(struct data *data);
extern void sens_get_enable(struct enable *data);
extern void sens_set_enable(struct enable *data);

#endif /* __SENS_H__ */
