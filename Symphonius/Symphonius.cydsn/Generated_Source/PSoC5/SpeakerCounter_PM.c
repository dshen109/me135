/*******************************************************************************
* File Name: SpeakerCounter_PM.c  
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

#include "SpeakerCounter.h"

static SpeakerCounter_backupStruct SpeakerCounter_backup;


/*******************************************************************************
* Function Name: SpeakerCounter_SaveConfig
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
*  SpeakerCounter_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void SpeakerCounter_SaveConfig(void) 
{
    #if (!SpeakerCounter_UsingFixedFunction)

        SpeakerCounter_backup.CounterUdb = SpeakerCounter_ReadCounter();

        #if(!SpeakerCounter_ControlRegRemoved)
            SpeakerCounter_backup.CounterControlRegister = SpeakerCounter_ReadControlRegister();
        #endif /* (!SpeakerCounter_ControlRegRemoved) */

    #endif /* (!SpeakerCounter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: SpeakerCounter_RestoreConfig
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
*  SpeakerCounter_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void SpeakerCounter_RestoreConfig(void) 
{      
    #if (!SpeakerCounter_UsingFixedFunction)

       SpeakerCounter_WriteCounter(SpeakerCounter_backup.CounterUdb);

        #if(!SpeakerCounter_ControlRegRemoved)
            SpeakerCounter_WriteControlRegister(SpeakerCounter_backup.CounterControlRegister);
        #endif /* (!SpeakerCounter_ControlRegRemoved) */

    #endif /* (!SpeakerCounter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: SpeakerCounter_Sleep
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
*  SpeakerCounter_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void SpeakerCounter_Sleep(void) 
{
    #if(!SpeakerCounter_ControlRegRemoved)
        /* Save Counter's enable state */
        if(SpeakerCounter_CTRL_ENABLE == (SpeakerCounter_CONTROL & SpeakerCounter_CTRL_ENABLE))
        {
            /* Counter is enabled */
            SpeakerCounter_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            SpeakerCounter_backup.CounterEnableState = 0u;
        }
    #else
        SpeakerCounter_backup.CounterEnableState = 1u;
        if(SpeakerCounter_backup.CounterEnableState != 0u)
        {
            SpeakerCounter_backup.CounterEnableState = 0u;
        }
    #endif /* (!SpeakerCounter_ControlRegRemoved) */
    
    SpeakerCounter_Stop();
    SpeakerCounter_SaveConfig();
}


/*******************************************************************************
* Function Name: SpeakerCounter_Wakeup
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
*  SpeakerCounter_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void SpeakerCounter_Wakeup(void) 
{
    SpeakerCounter_RestoreConfig();
    #if(!SpeakerCounter_ControlRegRemoved)
        if(SpeakerCounter_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            SpeakerCounter_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!SpeakerCounter_ControlRegRemoved) */
    
}


/* [] END OF FILE */
