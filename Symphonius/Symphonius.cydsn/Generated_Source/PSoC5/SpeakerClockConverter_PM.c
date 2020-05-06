/*******************************************************************************
* File Name: SpeakerClockConverter_PM.c  
* Version 3.0
*
*  Description:
*    This file provides the power management source code to API for the
*    Counter.  
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "SpeakerClockConverter.h"

static SpeakerClockConverter_backupStruct SpeakerClockConverter_backup;


/*******************************************************************************
* Function Name: SpeakerClockConverter_SaveConfig
********************************************************************************
* Summary:
*     Save the current user configuration
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  SpeakerClockConverter_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void SpeakerClockConverter_SaveConfig(void) 
{
    #if (!SpeakerClockConverter_UsingFixedFunction)

        SpeakerClockConverter_backup.CounterUdb = SpeakerClockConverter_ReadCounter();

        #if(!SpeakerClockConverter_ControlRegRemoved)
            SpeakerClockConverter_backup.CounterControlRegister = SpeakerClockConverter_ReadControlRegister();
        #endif /* (!SpeakerClockConverter_ControlRegRemoved) */

    #endif /* (!SpeakerClockConverter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: SpeakerClockConverter_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  SpeakerClockConverter_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void SpeakerClockConverter_RestoreConfig(void) 
{      
    #if (!SpeakerClockConverter_UsingFixedFunction)

       SpeakerClockConverter_WriteCounter(SpeakerClockConverter_backup.CounterUdb);

        #if(!SpeakerClockConverter_ControlRegRemoved)
            SpeakerClockConverter_WriteControlRegister(SpeakerClockConverter_backup.CounterControlRegister);
        #endif /* (!SpeakerClockConverter_ControlRegRemoved) */

    #endif /* (!SpeakerClockConverter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: SpeakerClockConverter_Sleep
********************************************************************************
* Summary:
*     Stop and Save the user configuration
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  SpeakerClockConverter_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void SpeakerClockConverter_Sleep(void) 
{
    #if(!SpeakerClockConverter_ControlRegRemoved)
        /* Save Counter's enable state */
        if(SpeakerClockConverter_CTRL_ENABLE == (SpeakerClockConverter_CONTROL & SpeakerClockConverter_CTRL_ENABLE))
        {
            /* Counter is enabled */
            SpeakerClockConverter_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            SpeakerClockConverter_backup.CounterEnableState = 0u;
        }
    #else
        SpeakerClockConverter_backup.CounterEnableState = 1u;
        if(SpeakerClockConverter_backup.CounterEnableState != 0u)
        {
            SpeakerClockConverter_backup.CounterEnableState = 0u;
        }
    #endif /* (!SpeakerClockConverter_ControlRegRemoved) */
    
    SpeakerClockConverter_Stop();
    SpeakerClockConverter_SaveConfig();
}


/*******************************************************************************
* Function Name: SpeakerClockConverter_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  SpeakerClockConverter_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void SpeakerClockConverter_Wakeup(void) 
{
    SpeakerClockConverter_RestoreConfig();
    #if(!SpeakerClockConverter_ControlRegRemoved)
        if(SpeakerClockConverter_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            SpeakerClockConverter_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!SpeakerClockConverter_ControlRegRemoved) */
    
}


/* [] END OF FILE */
