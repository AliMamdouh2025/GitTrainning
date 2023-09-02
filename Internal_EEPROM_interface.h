 /******************************************************************************
 *
 * Module: EEPROM
 *
 * File Name: Internal_EEPROM_interface.h
 *
 * Description: Header file for the EEPROM AVR driver
 *
 * Author: Ali Mamdouh
 *
 *******************************************************************************/

#ifndef INTERNAL_EEPROM_INTERFACE_H_
#define INTERNAL_EEPROM_INTERFACE_H_


typedef struct 
{
	uint8 EEPROM_SW_MAJOR_VERSION;
	uint8 EEPROM_SW_MINOR_VERSION;
}SW_Ver;

#define MADC_SW_MAJOR_VERSION 1
#define MADC_SW_MINOR_VERSION 0

void Internal_EEPROM_init(void);
void EEPROM_write(uint16 u16Address, uint8 ucData);
unsigned char EEPROM_read(uint16 u16Address);
void MADC_VidGetSWVersion(SW_Ver *VersionInfo);



#endif /* INTERNAL_EEPROM_INTERFACE_H_ */
