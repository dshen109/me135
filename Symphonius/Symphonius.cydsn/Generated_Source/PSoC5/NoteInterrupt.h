/*******************************************************************************
* File Name: NoteInterrupt.h
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
#if !defined(CY_ISR_NoteInterrupt_H)
#define CY_ISR_NoteInterrupt_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void NoteInterrupt_Start(void);
void NoteInterrupt_StartEx(cyisraddress address);
void NoteInterrupt_Stop(void);

CY_ISR_PROTO(NoteInterrupt_Interrupt);

void NoteInterrupt_SetVector(cyisraddress address);
cyisraddress NoteInterrupt_GetVector(void);

void NoteInterrupt_SetPriority(uint8 priority);
uint8 NoteInterrupt_GetPriority(void);

void NoteInterrupt_Enable(void);
uint8 NoteInterrupt_GetState(void);
void NoteInterrupt_Disable(void);

void NoteInterrupt_SetPending(void);
void NoteInterrupt_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the NoteInterrupt ISR. */
#define NoteInterrupt_INTC_VECTOR            ((reg32 *) NoteInterrupt__INTC_VECT)

/* Address of the NoteInterrupt ISR priority. */
#define NoteInterrupt_INTC_PRIOR             ((reg8 *) NoteInterrupt__INTC_PRIOR_REG)

/* Priority of the NoteInterrupt interrupt. */
#define NoteInterrupt_INTC_PRIOR_NUMBER      NoteInterrupt__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable NoteInterrupt interrupt. */
#define NoteInterrupt_INTC_SET_EN            ((reg32 *) NoteInterrupt__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the NoteInterrupt interrupt. */
#define NoteInterrupt_INTC_CLR_EN            ((reg32 *) NoteInterrupt__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the NoteInterrupt interrupt state to pending. */
#define NoteInterrupt_INTC_SET_PD            ((reg32 *) NoteInterrupt__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the NoteInterrupt interrupt. */
#define NoteInterrupt_INTC_CLR_PD            ((reg32 *) NoteInterrupt__INTC_CLR_PD_REG)


#endif /* CY_ISR_NoteInterrupt_H */


/* [] END OF FILE */
