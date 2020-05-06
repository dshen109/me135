/*******************************************************************************
* File Name: SpeakerPWM_PM.c
* Version 3.30
*
* Description:
*  This file provides the power management source code to API for the
*  PWM.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "SpeakerPWM.h"

static SpeakerPWM_backupStruct SpeakerPWM_backup;


/*******************************************************************************
* Function Name: SpeakerPWM_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  SpeakerPWM_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void SpeakerPWM_SaveConfig(void) 
{

    #if(!SpeakerPWM_UsingFixedFunction)
        #if(!SpeakerPWM_PWMModeIsCenterAligned)
            SpeakerPWM_backup.PWMPeriod = SpeakerPWM_ReadPeriod();
        #endif /* (!SpeakerPWM_PWMModeIsCenterAligned) */
        SpeakerPWM_backup.PWMUdb = SpeakerPWM_ReadCounter();
        #if (SpeakerPWM_UseStatus)
            SpeakerPWM_backup.InterruptMaskValue = SpeakerPWM_STATUS_MASK;
        #endif /* (SpeakerPWM_UseStatus) */

        #if(SpeakerPWM_DeadBandMode == SpeakerPWM__B_PWM__DBM_256_CLOCKS || \
            SpeakerPWM_DeadBandMode == SpeakerPWM__B_PWM__DBM_2_4_CLOCKS)
            SpeakerPWM_backup.PWMdeadBandValue = SpeakerPWM_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(SpeakerPWM_KillModeMinTime)
             SpeakerPWM_backup.PWMKillCounterPeriod = SpeakerPWM_ReadKillTime();
        #endif /* (SpeakerPWM_KillModeMinTime) */

        #if(SpeakerPWM_UseControl)
            SpeakerPWM_backup.PWMControlRegister = SpeakerPWM_ReadControlRegister();
        #endif /* (SpeakerPWM_UseControl) */
    #endif  /* (!SpeakerPWM_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: SpeakerPWM_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  SpeakerPWM_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void SpeakerPWM_RestoreConfig(void) 
{
        #if(!SpeakerPWM_UsingFixedFunction)
            #if(!SpeakerPWM_PWMModeIsCenterAligned)
                SpeakerPWM_WritePeriod(SpeakerPWM_backup.PWMPeriod);
            #endif /* (!SpeakerPWM_PWMModeIsCenterAligned) */

            SpeakerPWM_WriteCounter(SpeakerPWM_backup.PWMUdb);

            #if (SpeakerPWM_UseStatus)
                SpeakerPWM_STATUS_MASK = SpeakerPWM_backup.InterruptMaskValue;
            #endif /* (SpeakerPWM_UseStatus) */

            #if(SpeakerPWM_DeadBandMode == SpeakerPWM__B_PWM__DBM_256_CLOCKS || \
                SpeakerPWM_DeadBandMode == SpeakerPWM__B_PWM__DBM_2_4_CLOCKS)
                SpeakerPWM_WriteDeadTime(SpeakerPWM_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(SpeakerPWM_KillModeMinTime)
                SpeakerPWM_WriteKillTime(SpeakerPWM_backup.PWMKillCounterPeriod);
            #endif /* (SpeakerPWM_KillModeMinTime) */

            #if(SpeakerPWM_UseControl)
                SpeakerPWM_WriteControlRegister(SpeakerPWM_backup.PWMControlRegister);
            #endif /* (SpeakerPWM_UseControl) */
        #endif  /* (!SpeakerPWM_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: SpeakerPWM_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves the user configuration. Should be called
*  just prior to entering sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  SpeakerPWM_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void SpeakerPWM_Sleep(void) 
{
    #if(SpeakerPWM_UseControl)
        if(SpeakerPWM_CTRL_ENABLE == (SpeakerPWM_CONTROL & SpeakerPWM_CTRL_ENABLE))
        {
            /*Component is enabled */
            SpeakerPWM_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            SpeakerPWM_backup.PWMEnableState = 0u;
        }
    #endif /* (SpeakerPWM_UseControl) */

    /* Stop component */
    SpeakerPWM_Stop();

    /* Save registers configuration */
    SpeakerPWM_SaveConfig();
}


/*******************************************************************************
* Function Name: SpeakerPWM_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration. Should be called just after
*  awaking from sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  SpeakerPWM_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void SpeakerPWM_Wakeup(void) 
{
     /* Restore registers values */
    SpeakerPWM_RestoreConfig();

    if(SpeakerPWM_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        SpeakerPWM_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
