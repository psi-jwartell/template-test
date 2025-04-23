#ifndef EO_MODULE_H
#define EO_MODULE_H

#include <stdint.h>
#include "imc.h"

#define SUBM_ID (0) // This should reference the IMC ID

void eo_get_housekeeping(imc_can_frame_t *can_frame);
void eo_init();
void eo_change_setting(uint8_t index);

#endif /* EO_MODULE_H */