#include "eo_module.h"
#include "atmega_common.h"

/* Simulate housekeeping data collection. Replace this function's internals with actual HK collection */
void eo_get_housekeeping(imc_can_frame_t *can_frame){
	for(int i = 0; i < can_frame->dlc; ++i){
		can_frame->data[i] = i;
	}
}

static inline void _init_item_table_value(uint8_t index, uint32_t value, atmega_item_permission_t permission){
	submodule_item_table[index].value = value;
	submodule_item_table[index].permission = permission;
}

void eo_init(){
	// Initialize the EO component here. The function name is shared but its declaration should be different per-submodule.
	// You should initialize submodule-specific item table settings and configure items necessary for the submodule to function 
	// (for instance, the SN65 loopback enable on the synthesizer). 

	/** Item table information */
	// Common items 0x0000-0x001F (0-31)
	// Profile items 0x0020-0x003F (32-63)
	// Submodule-specific items 0x0040-0x007F (64-127)

	/** COMMON SECTION */
	// Change these to the appropriate values. They are placeholders for now
	// TODO: Some items also need to be saved to/read from NVM
	_init_item_table_value(IMC_SUBMODULE_SOFTWARE_VERSION, 0x00FF, RO);
	_init_item_table_value(IMC_SUBMODULE_HARDWARE_VERSION, 0xFF00, RO);
	_init_item_table_value(IMC_SUBMODULE_SOFTWARE_VERSION, 0x00FF, RO);
	_init_item_table_value(IMC_SUBMODULE_PROFILE, 0x0000, RO);
	_init_item_table_value(IMC_SUBMODULE_CAN_ID, 0x0000, RW);
	_init_item_table_value(IMC_SUBMODULE_CAN_HK_RATE, 0x01, RO);
	/** END COMMON SECTION */
	
	/** EO SECTION */

	/** PUT EO SPECIFIC ITEM TABLE ITEMS HERE */
}

void eo_change_setting(uint8_t index){
	// Handle EO component settings here. For example, writing synth registers.
	switch(index){
		case IMC_SUBMODULE_CAN_ID:
		break;
		case IMC_SUBMODULE_CAN_HK_RATE:
		break;
		default:
		break;
	}
}