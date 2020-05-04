/*******************************************************************************
* File Name: LEDOffInterrupt.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_LEDOffInterrupt_H)
#define CY_ISR_LEDOffInterrupt_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void LEDOffInterrupt_Start(void);
void LEDOffInterrupt_StartEx(cyisraddress address);
void LEDOffInterrupt_Stop(void);

CY_ISR_PROTO(LEDOffInterrupt_Interrupt);

void LEDOffInterrupt_SetVector(cyisraddress address);
cyisraddress LEDOffInterrupt_GetVector(void);

void LEDOffInterrupt_SetPriority(uint8 priority);
uint8 LEDOffInterrupt_GetPriority(void);

void LEDOffInterrupt_Enable(void);
uint8 LEDOffInterrupt_GetState(void);
void LEDOffInterrupt_Disable(void);

void LEDOffInterrupt_SetPending(void);
void LEDOffInterrupt_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the LEDOffInterrupt ISR. */
#define LEDOffInterrupt_INTC_VECTOR            ((reg32 *) LEDOffInterrupt__INTC_VECT)

/* Address of the LEDOffInterrupt ISR priority. */
#define LEDOffInterrupt_INTC_PRIOR             ((reg8 *) LEDOffInterrupt__INTC_PRIOR_REG)

/* Priority of the LEDOffInterrupt interrupt. */
#define LEDOffInterrupt_INTC_PRIOR_NUMBER      LEDOffInterrupt__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable LEDOffInterrupt interrupt. */
#define LEDOffInterrupt_INTC_SET_EN            ((reg32 *) LEDOffInterrupt__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the LEDOffInterrupt interrupt. */
#define LEDOffInterrupt_INTC_CLR_EN            ((reg32 *) LEDOffInterrupt__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the LEDOffInterrupt interrupt state to pending. */
#define LEDOffInterrupt_INTC_SET_PD            ((reg32 *) LEDOffInterrupt__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the LEDOffInterrupt interrupt. */
#define LEDOffInterrupt_INTC_CLR_PD            ((reg32 *) LEDOffInterrupt__INTC_CLR_PD_REG)


#endif /* CY_ISR_LEDOffInterrupt_H */


/* [] END OF FILE */
