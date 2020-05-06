/*******************************************************************************
* File Name: TurnSpeakerOff.h
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
#if !defined(CY_ISR_TurnSpeakerOff_H)
#define CY_ISR_TurnSpeakerOff_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void TurnSpeakerOff_Start(void);
void TurnSpeakerOff_StartEx(cyisraddress address);
void TurnSpeakerOff_Stop(void);

CY_ISR_PROTO(TurnSpeakerOff_Interrupt);

void TurnSpeakerOff_SetVector(cyisraddress address);
cyisraddress TurnSpeakerOff_GetVector(void);

void TurnSpeakerOff_SetPriority(uint8 priority);
uint8 TurnSpeakerOff_GetPriority(void);

void TurnSpeakerOff_Enable(void);
uint8 TurnSpeakerOff_GetState(void);
void TurnSpeakerOff_Disable(void);

void TurnSpeakerOff_SetPending(void);
void TurnSpeakerOff_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the TurnSpeakerOff ISR. */
#define TurnSpeakerOff_INTC_VECTOR            ((reg32 *) TurnSpeakerOff__INTC_VECT)

/* Address of the TurnSpeakerOff ISR priority. */
#define TurnSpeakerOff_INTC_PRIOR             ((reg8 *) TurnSpeakerOff__INTC_PRIOR_REG)

/* Priority of the TurnSpeakerOff interrupt. */
#define TurnSpeakerOff_INTC_PRIOR_NUMBER      TurnSpeakerOff__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable TurnSpeakerOff interrupt. */
#define TurnSpeakerOff_INTC_SET_EN            ((reg32 *) TurnSpeakerOff__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the TurnSpeakerOff interrupt. */
#define TurnSpeakerOff_INTC_CLR_EN            ((reg32 *) TurnSpeakerOff__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the TurnSpeakerOff interrupt state to pending. */
#define TurnSpeakerOff_INTC_SET_PD            ((reg32 *) TurnSpeakerOff__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the TurnSpeakerOff interrupt. */
#define TurnSpeakerOff_INTC_CLR_PD            ((reg32 *) TurnSpeakerOff__INTC_CLR_PD_REG)


#endif /* CY_ISR_TurnSpeakerOff_H */


/* [] END OF FILE */
