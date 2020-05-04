/*******************************************************************************
* File Name: LEDPWM_PM.c
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

#include "LEDPWM.h"

static LEDPWM_backupStruct LEDPWM_backup;


/*******************************************************************************
* Function Name: LEDPWM_SaveConfig
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
*  LEDPWM_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void LEDPWM_SaveConfig(void) 
{

    #if(!LEDPWM_UsingFixedFunction)
        #if(!LEDPWM_PWMModeIsCenterAligned)
            LEDPWM_backup.PWMPeriod = LEDPWM_ReadPeriod();
        #endif /* (!LEDPWM_PWMModeIsCenterAligned) */
        LEDPWM_backup.PWMUdb = LEDPWM_ReadCounter();
        #if (LEDPWM_UseStatus)
            LEDPWM_backup.InterruptMaskValue = LEDPWM_STATUS_MASK;
        #endif /* (LEDPWM_UseStatus) */

        #if(LEDPWM_DeadBandMode == LEDPWM__B_PWM__DBM_256_CLOCKS || \
            LEDPWM_DeadBandMode == LEDPWM__B_PWM__DBM_2_4_CLOCKS)
            LEDPWM_backup.PWMdeadBandValue = LEDPWM_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(LEDPWM_KillModeMinTime)
             LEDPWM_backup.PWMKillCounterPeriod = LEDPWM_ReadKillTime();
        #endif /* (LEDPWM_KillModeMinTime) */

        #if(LEDPWM_UseControl)
            LEDPWM_backup.PWMControlRegister = LEDPWM_ReadControlRegister();
        #endif /* (LEDPWM_UseControl) */
    #endif  /* (!LEDPWM_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: LEDPWM_RestoreConfig
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
*  LEDPWM_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void LEDPWM_RestoreConfig(void) 
{
        #if(!LEDPWM_UsingFixedFunction)
            #if(!LEDPWM_PWMModeIsCenterAligned)
                LEDPWM_WritePeriod(LEDPWM_backup.PWMPeriod);
            #endif /* (!LEDPWM_PWMModeIsCenterAligned) */

            LEDPWM_WriteCounter(LEDPWM_backup.PWMUdb);

            #if (LEDPWM_UseStatus)
                LEDPWM_STATUS_MASK = LEDPWM_backup.InterruptMaskValue;
            #endif /* (LEDPWM_UseStatus) */

            #if(LEDPWM_DeadBandMode == LEDPWM__B_PWM__DBM_256_CLOCKS || \
                LEDPWM_DeadBandMode == LEDPWM__B_PWM__DBM_2_4_CLOCKS)
                LEDPWM_WriteDeadTime(LEDPWM_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(LEDPWM_KillModeMinTime)
                LEDPWM_WriteKillTime(LEDPWM_backup.PWMKillCounterPeriod);
            #endif /* (LEDPWM_KillModeMinTime) */

            #if(LEDPWM_UseControl)
                LEDPWM_WriteControlRegister(LEDPWM_backup.PWMControlRegister);
            #endif /* (LEDPWM_UseControl) */
        #endif  /* (!LEDPWM_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: LEDPWM_Sleep
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
*  LEDPWM_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void LEDPWM_Sleep(void) 
{
    #if(LEDPWM_UseControl)
        if(LEDPWM_CTRL_ENABLE == (LEDPWM_CONTROL & LEDPWM_CTRL_ENABLE))
        {
            /*Component is enabled */
            LEDPWM_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            LEDPWM_backup.PWMEnableState = 0u;
        }
    #endif /* (LEDPWM_UseControl) */

    /* Stop component */
    LEDPWM_Stop();

    /* Save registers configuration */
    LEDPWM_SaveConfig();
}


/*******************************************************************************
* Function Name: LEDPWM_Wakeup
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
*  LEDPWM_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void LEDPWM_Wakeup(void) 
{
     /* Restore registers values */
    LEDPWM_RestoreConfig();

    if(LEDPWM_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        LEDPWM_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
