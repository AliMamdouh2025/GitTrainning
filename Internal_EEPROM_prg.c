 /******************************************************************************
 *
 * Module: EEPROM
 *
 * File Name: Internal_EEPROM_interface.h
 *
 * Description: Interface file for the EEPROM AVR driver
 *
 * Author: Ali Mamdouh
 *
 *******************************************************************************/

/**
 * \file Internal_EEPROM_prg.c
 * \section Brief
 * This is EEPROM services Prg
 * \section Details
 * The module implement Internal_EEPROM functions
 * \section Scope
 * Public
 *
 *
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include "common_macros.h"
#include "std_types.h"
#include "Internal_EEPROM_cfg.h"
#include "Internal_EEPROM_interface.h"
#include "Internal_EEPROM_private.h"

uint8 x;
uint16 PWMVAL = 100;
#define PULL 1238

/***********************************[1]***********************************************
 * Service Name: MADC_VidGetSWVersion
 * Service ID[hex]: 0x01
 * Sync/Async: Synchronous
 * Renterancy: Reentrant
 * Parameters (in): VersionInfo
 * Parameters (InOut): None
 * Parameters (Out): None
 * Return value: None
 * Description: Set the Software Version of ADC_Driver.
 ************************************************************************************/
 
 /**
 * \section Service_Name
 * MADC_VidGetSWVersion
 *
 * \section Description
 *  Store in a structure the version of ADC driver.
 * 
 * \section Req_ID
 * ADC_0x01
 *
 * \section Sync_Async
 * Synchronous
 *
 * \section Renterancy
 * Reentrant
 *
 * \section Parameters
 * \param[in] None
 * \param[inout] None
 * \param[out] SW_Ver (pointer to struct)\ VersionInfo--->Hold the Task's minor and major versions
 *
 * \section Return_value
 * None
 *
 * \section Rational
 * Set the Software Version of ADC_Driver.
 *
 * 
 * \section Activity_Diagram
 *
 * @startuml 
 *   start
 *  :Take major version;
 *  :Take minor version;
 * end
 * @enduml
 *
 *
 *
 * \section Sequence_Diagram
 *
 * @startuml
 * MADC_VidGetSWVersion_function -> MADC_private_file: MADC_SW_MAJOR_VERSION Enumerator value Request
 * MADC_private_file --> MADC_VidGetSWVersion_function: MADC_SW_MAJOR_VERSION Enumerator value Response
 * MADC_VidGetSWVersion_function -> MADC_private_file: MADC_SW_MINOR_VERSION Enumerator value Request
 * MADC_private_file --> MADC_VidGetSWVersion_function: MADC_SW_MINOR_VERSION Enumerator value Response
 * @enduml
*/
 
 
 
 

#if EEPROM_VERSION == ENABLE
void MADC_VidGetSWVersion(SW_Ver *VersionInfo) {

	VersionInfo->Major = EEPROM_SW_MAJOR_VERSION;
	VersionInfo->Minor = EEPROM_SW_MINOR_VERSION;
}
#endif


#if(EEPROM_INTERRUPT_STATE == EEPROM_INTERRUPT_ENABLE)

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtrReceive)(void) = NULL_PTR;

void EEEPROM_READY_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_callBackPtrReceive = a_ptr;
}

ISR(EE_RDY_vect)
{
	if(g_callBackPtrReceive != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtrReceive)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}
#endif



/***********************************[2]***********************************************
 * Service Name: EEPROM_init
 * Service ID[hex]: 0x04
 * Sync/Async: Synchronous
 * Renterancy: Reentrant
 * Parameters (in): None
 * Parameters (InOut): None
 * Parameters (Out): None
 * Return value: None
 * Description:initializing
 ************************************************************************************/
 
 
 /**
 * \section Service_Name
 * EEPROM_init
 *
 * \section Description
 * initialize EEPROM.
 * 
 * \section Req_ID
 * EEPROM_0x02
 *
 * \section Sync_Async
 * Synchronous
 *
 * \section Renterancy
 * Reentrant
 *
 * \section Parameters
 * \param[in]  None
 * \param[inout] None
 * \param[out] None
 *
 * \section Return_value
 * None
 *
 * \section Rational
 * initialize EEPROM.
 *
 * \section Activity_Diagram
 *
 * @startuml 
 * start
 * :Enable or disable EEPROM interrupt(based on configration);
 * end
 * @enduml
 *
 *
 *
 *
 */

void Internal_EEPROM_init(void)
{
#if(EEPROM_INTERRUPT_STATE == EEPROM_INTERRUPT_ENABLE)
	SET_BIT(EECR,EERIE);
#elif(EEPROM_INTERRUPT_STATE == EEPROM_INTERRUPT_DISABLE)
	CLEAR_BIT(EECR,EERIE);
#endif

}

/***********************************[3]***********************************************
 * Service Name: EEPROM_write
 * Service ID[hex]: 0x03
 * Sync/Async: Synchronous
 * Renterancy: Reentrant
 * Parameters (in): u16Address,ucData
 * Parameters (InOut): None
 * Parameters (Out): None
 * Return value: None
 * Description:Writing to EEPROM(Note: you must enable Optimizer -o3)
 ************************************************************************************/
 
 
 /**
 * \section Service_Name
 * EEPROM_write
 *
 * \section Description
 * Writing to eeprom if the data is diffrent for the first time, if the same data is already exist it returns.it waits until bootloader finish(based on configration).
 * 
 * \section Req_ID
 * EEPROM_0x03
 *
 * \section Sync_Async
 * Synchronous
 *
 * \section Renterancy
 * Reentrant
 *
 * \section Parameters
 * \param[in]  uint16\ u16Address--->Hold Address of data
 * \param[in]  uint8\ ucData--->Hold the data
 * \param[inout] None
 * \param[out] None
 *
 * \section Return_value
 * None
 *
 * \section Rational
 * Writing to EEPROM.
 *
 * \section Activity_Diagram
 *
 * @startuml 
 * start
 * :Disable global interrupt;
 * :Polling for completion of previous write;
 * :Polling for completion of bootloader operations(based on Configration);
 * :Set up address register;
 * :ُEnable Reading access;
 * :Read from EEPROM;
 *	if (check if value is repeated or not?) then (Yes)
 *	#green:Return from function;
 * end
 *  else (no)
 * :ُEnable Writing access; 
 * :Write data to EEPROM;
 * #green:Enable Global Interrupts;
 * endif
 * end
 * @enduml
 *
 *
 *
 * \section Sequence_Diagram
 *
 * @startuml
 * "EEPROM_write\nFunction" -> "CLEAR_BIT\nMacro": SREG Register and bit7  to be cleared Request
 * "CLEAR_BIT\nMacro" -[#0000FF]-> "EEPROM_write\nFunction": Clearing SREG bit7 Response 
 *
 * == EEPROM_UPPDATE_OPTIMIZER_ENABLED ==
 *
 * "EEPROM_write\nFunction" -> "SET_BIT\nMacro": EECR Register and EERE bit to be set Request
 * "SET_BIT\nMacro" -[#0000FF]-> "EEPROM_write\nFunction": Setting EERE bit Response
 *
 *
 * @enduml
 *
 *
 *
 * \section Timing_Diagram
 *
 * @startuml
 * robust "EEPROM" as EEPROM
 * concise "Function" as Func
 *
 * Func is Cannot_Write
 * EEPROM is Block_Access
 * 
 * @Func
 * 0 is EnWrite
 * Func -> EEPROM : SET_EEME  
 * +4 is Cannot_Write
 *
 * @EEPROM
 * 0 is Allow_Access
 * +4 is Block_Access
 * @enduml
 *
*/








void EEPROM_write(uint16 u16Address, uint8 ucData)
{
	/* Disable Global Interrupts to protect EEPROM*/
	CLEAR_BIT(SREG,7);

	/* Wait for completion of previous write */
	while(EECR & (1<<EEWE));

	/* Wait for SPMEN to be cleared(important When using bootloader) */
#if(BOOTLOADER_STATE == BOOTLOADER_USED)
	while(SPMCR & (1<<SPMEN));
#endif

#if(EEPROM_UPPDATE_OPTIMIZER == ENABLE)
	/* Set up address register */
	EEARH = u16Address>>8;
	EEARL = u16Address;

	/* Start eeprom read by writing EERE */
	SET_BIT(EECR,EERE);

	/*check if value is repeated or not */
	if(EEDR == ucData)
	{
		return;
	}
	else
	{
		/*Continue writing normally*/
	}



#endif



	/* Set up address and data registers */
	EEARH = u16Address>>8;
	EEARL = u16Address;
	EEDR = ucData;

	/* Write logical one to EEMWE */
	SET_BIT(EECR,EEMWE);

	/* Start eeprom write by setting EEWE */
	SET_BIT(EECR,EEWE);

	//SET_BIT(PORTC,1);


	/* Enable Global Interrupts */
	SET_BIT(SREG,7);

}





/***********************************[4]***********************************************
 * Service Name: EEPROM_Read
 * Service ID[hex]: 0x04
 * Sync/Async: Synchronous
 * Renterancy: Reentrant
 * Parameters (in): u16Address
 * Parameters (InOut): None
 * Parameters (Out): None
 * Return value: None
 * Description:Reading from EEPROM(Note: you must enable Optimizer -o3)
 ************************************************************************************/
 
 
 /**
 * \section Service_Name
 * EEPROM_Read
 *
 * \section Description
 * Reading from EEPROM.
 * 
 * \section Req_ID
 * EEPROM_0x04
 *
 * \section Sync_Async
 * Synchronous
 *
 * \section Renterancy
 * Reentrant
 *
 * \section Parameters
 * \param[in]  uint16\ u16Address--->Hold Address of data
 * \param[inout] None
 * \param[out] None
 *
 * \section Return_value
 * None
 *
 * \section Rational
 * Reading from EEPROM.
 *
 * \section Activity_Diagram
 *
 * @startuml 
 * start
 * :Disable global interrupt;
 * :Wait for completion of previous write;
 * :Set up address and read registers;
 * :reading data;
 * :Enable Global Interrupts;
 * #green:return data from EEPROM;
 * end
 * @enduml
 *
 *
 * \section Timing_Diagram
 *
 * @startuml
 * robust "EEPROM" as EEPROM
 * concise "Function" as Func
 *
 * Func is Cannot_Read
 * EEPROM is Block_Access
 * 
 * @Func
 * 0 is EnRead
 * Func -> EEPROM : SET_EEME  
 * +4 is Cannot_Read
 *
 * @EEPROM
 * 0 is Allow_Access
 * +4 is Block_Access
 * @enduml
 *
 */





unsigned char EEPROM_read(uint16 u16Address)
{
	/* Disable Global Interrupts to protect EEPROM*/
	CLEAR_BIT(SREG,7);
	
	/* Wait for completion of previous write */
	while(EECR & (1<<EEWE));

	/* Wait for SPMEN to be cleared(important When using bootloader) */
#if(BOOTLOADER_STATE == BOOTLOADER_USED)
	while(SPMCR & (1<<SPMEN));
#endif

	/* Set up address register */
	EEARH = u16Address>>8;
	EEARL = u16Address;

	/* Start eeprom read by writing EERE */
	SET_BIT(EECR,EERE);


	/* Enable Global Interrupts */
	SET_BIT(SREG,7);

	/* Return data from data register */
	return EEDR;
}

