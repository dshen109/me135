/*******************************************************************************
* File Name: LEDPWM.c
* Version 3.30
*
* Description:
*  The PWM User Module consist of an 8 or 16-bit counter with two 8 or 16-bit
*  comparitors. Each instance of this user module is capable of generating
*  two PWM outputs with the same period. The pulse width is selectable between
*  1 and 255/65535. The period is selectable between 2 and 255/65536 clocks.
*  The compare value output may be configured to be active when the present
*  counter is less than or less than/equal to the compare value.
*  A terminal count output is also provided. It generates a pulse one clock
*  width wide when the counter is equal to zero.
*
* Note:
*
*******************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "LEDPWM.h"

/* Error message for removed <resource> through optimization */
#ifdef LEDPWM_PWMUDB_genblk1_ctrlreg__REMOVED
    #error PWM_v3_30 detected with a constant 0 for the enable or \
         constant 1 for reset. This will prevent the component from operating.
#endif /* LEDPWM_PWMUDB_genblk1_ctrlreg__REMOVED */

uint8 LEDPWM_initVar = 0u;


/*******************************************************************************
* Function Name: LEDPWM_Start
********************************************************************************
*
* Summary:
*  The start function initializes the pwm with the default values, the
*  enables the counter to begin counting.  It does not enable interrupts,
*  the EnableInt command should be called if interrupt generation is required.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  LEDPWM_initVar: Is modified when this function is called for the
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void LEDPWM_Start(void) 
{
    /* If not Initialized then initialize all required hardware and software */
    if(LEDPWM_initVar == 0u)
    {
        LEDPWM_Init();
        LEDPWM_initVar = 1u;
    }
    LEDPWM_Enable();

}


/*******************************************************************************
* Function Name: LEDPWM_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the
*  customizer of the component placed onto schematic. Usually called in
*  LEDPWM_Start().
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void LEDPWM_Init(void) 
{
    #if (LEDPWM_UsingFixedFunction || LEDPWM_UseControl)
        uint8 ctrl;
    #endif /* (LEDPWM_UsingFixedFunction || LEDPWM_UseControl) */

    #if(!LEDPWM_UsingFixedFunction)
        #if(LEDPWM_UseStatus)
            /* Interrupt State Backup for Critical Region*/
            uint8 LEDPWM_interruptState;
        #endif /* (LEDPWM_UseStatus) */
    #endif /* (!LEDPWM_UsingFixedFunction) */

    #if (LEDPWM_UsingFixedFunction)
        /* You are allowed to write the compare value (FF only) */
        LEDPWM_CONTROL |= LEDPWM_CFG0_MODE;
        #if (LEDPWM_DeadBand2_4)
            LEDPWM_CONTROL |= LEDPWM_CFG0_DB;
        #endif /* (LEDPWM_DeadBand2_4) */

        ctrl = LEDPWM_CONTROL3 & ((uint8 )(~LEDPWM_CTRL_CMPMODE1_MASK));
        LEDPWM_CONTROL3 = ctrl | LEDPWM_DEFAULT_COMPARE1_MODE;

         /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
        LEDPWM_RT1 &= ((uint8)(~LEDPWM_RT1_MASK));
        LEDPWM_RT1 |= LEDPWM_SYNC;

        /*Enable DSI Sync all all inputs of the PWM*/
        LEDPWM_RT1 &= ((uint8)(~LEDPWM_SYNCDSI_MASK));
        LEDPWM_RT1 |= LEDPWM_SYNCDSI_EN;

    #elif (LEDPWM_UseControl)
        /* Set the default compare mode defined in the parameter */
        ctrl = LEDPWM_CONTROL & ((uint8)(~LEDPWM_CTRL_CMPMODE2_MASK)) &
                ((uint8)(~LEDPWM_CTRL_CMPMODE1_MASK));
        LEDPWM_CONTROL = ctrl | LEDPWM_DEFAULT_COMPARE2_MODE |
                                   LEDPWM_DEFAULT_COMPARE1_MODE;
    #endif /* (LEDPWM_UsingFixedFunction) */

    #if (!LEDPWM_UsingFixedFunction)
        #if (LEDPWM_Resolution == 8)
            /* Set FIFO 0 to 1 byte register for period*/
            LEDPWM_AUX_CONTROLDP0 |= (LEDPWM_AUX_CTRL_FIFO0_CLR);
        #else /* (LEDPWM_Resolution == 16)*/
            /* Set FIFO 0 to 1 byte register for period */
            LEDPWM_AUX_CONTROLDP0 |= (LEDPWM_AUX_CTRL_FIFO0_CLR);
            LEDPWM_AUX_CONTROLDP1 |= (LEDPWM_AUX_CTRL_FIFO0_CLR);
        #endif /* (LEDPWM_Resolution == 8) */

        LEDPWM_WriteCounter(LEDPWM_INIT_PERIOD_VALUE);
    #endif /* (!LEDPWM_UsingFixedFunction) */

    LEDPWM_WritePeriod(LEDPWM_INIT_PERIOD_VALUE);

        #if (LEDPWM_UseOneCompareMode)
            LEDPWM_WriteCompare(LEDPWM_INIT_COMPARE_VALUE1);
        #else
            LEDPWM_WriteCompare1(LEDPWM_INIT_COMPARE_VALUE1);
            LEDPWM_WriteCompare2(LEDPWM_INIT_COMPARE_VALUE2);
        #endif /* (LEDPWM_UseOneCompareMode) */

        #if (LEDPWM_KillModeMinTime)
            LEDPWM_WriteKillTime(LEDPWM_MinimumKillTime);
        #endif /* (LEDPWM_KillModeMinTime) */

        #if (LEDPWM_DeadBandUsed)
            LEDPWM_WriteDeadTime(LEDPWM_INIT_DEAD_TIME);
        #endif /* (LEDPWM_DeadBandUsed) */

    #if (LEDPWM_UseStatus || LEDPWM_UsingFixedFunction)
        LEDPWM_SetInterruptMode(LEDPWM_INIT_INTERRUPTS_MODE);
    #endif /* (LEDPWM_UseStatus || LEDPWM_UsingFixedFunction) */

    #if (LEDPWM_UsingFixedFunction)
        /* Globally Enable the Fixed Function Block chosen */
        LEDPWM_GLOBAL_ENABLE |= LEDPWM_BLOCK_EN_MASK;
        /* Set the Interrupt source to come from the status register */
        LEDPWM_CONTROL2 |= LEDPWM_CTRL2_IRQ_SEL;
    #else
        #if(LEDPWM_UseStatus)

            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            LEDPWM_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            LEDPWM_STATUS_AUX_CTRL |= LEDPWM_STATUS_ACTL_INT_EN_MASK;

             /* Exit Critical Region*/
            CyExitCriticalSection(LEDPWM_interruptState);

            /* Clear the FIFO to enable the LEDPWM_STATUS_FIFOFULL
                   bit to be set on FIFO full. */
            LEDPWM_ClearFIFO();
        #endif /* (LEDPWM_UseStatus) */
    #endif /* (LEDPWM_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: LEDPWM_Enable
********************************************************************************
*
* Summary:
*  Enables the PWM block operation
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  This works only if software enable mode is chosen
*
*******************************************************************************/
void LEDPWM_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (LEDPWM_UsingFixedFunction)
        LEDPWM_GLOBAL_ENABLE |= LEDPWM_BLOCK_EN_MASK;
        LEDPWM_GLOBAL_STBY_ENABLE |= LEDPWM_BLOCK_STBY_EN_MASK;
    #endif /* (LEDPWM_UsingFixedFunction) */

    /* Enable the PWM from the control register  */
    #if (LEDPWM_UseControl || LEDPWM_UsingFixedFunction)
        LEDPWM_CONTROL |= LEDPWM_CTRL_ENABLE;
    #endif /* (LEDPWM_UseControl || LEDPWM_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: LEDPWM_Stop
********************************************************************************
*
* Summary:
*  The stop function halts the PWM, but does not change any modes or disable
*  interrupts.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  If the Enable mode is set to Hardware only then this function
*  has no effect on the operation of the PWM
*
*******************************************************************************/
void LEDPWM_Stop(void) 
{
    #if (LEDPWM_UseControl || LEDPWM_UsingFixedFunction)
        LEDPWM_CONTROL &= ((uint8)(~LEDPWM_CTRL_ENABLE));
    #endif /* (LEDPWM_UseControl || LEDPWM_UsingFixedFunction) */

    /* Globally disable the Fixed Function Block chosen */
    #if (LEDPWM_UsingFixedFunction)
        LEDPWM_GLOBAL_ENABLE &= ((uint8)(~LEDPWM_BLOCK_EN_MASK));
        LEDPWM_GLOBAL_STBY_ENABLE &= ((uint8)(~LEDPWM_BLOCK_STBY_EN_MASK));
    #endif /* (LEDPWM_UsingFixedFunction) */
}

#if (LEDPWM_UseOneCompareMode)
    #if (LEDPWM_CompareMode1SW)


        /*******************************************************************************
        * Function Name: LEDPWM_SetCompareMode
        ********************************************************************************
        *
        * Summary:
        *  This function writes the Compare Mode for the pwm output when in Dither mode,
        *  Center Align Mode or One Output Mode.
        *
        * Parameters:
        *  comparemode:  The new compare mode for the PWM output. Use the compare types
        *                defined in the H file as input arguments.
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void LEDPWM_SetCompareMode(uint8 comparemode) 
        {
            #if(LEDPWM_UsingFixedFunction)

                #if(0 != LEDPWM_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemodemasked = ((uint8)((uint8)comparemode << LEDPWM_CTRL_CMPMODE1_SHIFT));
                #else
                    uint8 comparemodemasked = comparemode;
                #endif /* (0 != LEDPWM_CTRL_CMPMODE1_SHIFT) */

                LEDPWM_CONTROL3 &= ((uint8)(~LEDPWM_CTRL_CMPMODE1_MASK)); /*Clear Existing Data */
                LEDPWM_CONTROL3 |= comparemodemasked;

            #elif (LEDPWM_UseControl)

                #if(0 != LEDPWM_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemode1masked = ((uint8)((uint8)comparemode << LEDPWM_CTRL_CMPMODE1_SHIFT)) &
                                                LEDPWM_CTRL_CMPMODE1_MASK;
                #else
                    uint8 comparemode1masked = comparemode & LEDPWM_CTRL_CMPMODE1_MASK;
                #endif /* (0 != LEDPWM_CTRL_CMPMODE1_SHIFT) */

                #if(0 != LEDPWM_CTRL_CMPMODE2_SHIFT)
                    uint8 comparemode2masked = ((uint8)((uint8)comparemode << LEDPWM_CTRL_CMPMODE2_SHIFT)) &
                                               LEDPWM_CTRL_CMPMODE2_MASK;
                #else
                    uint8 comparemode2masked = comparemode & LEDPWM_CTRL_CMPMODE2_MASK;
                #endif /* (0 != LEDPWM_CTRL_CMPMODE2_SHIFT) */

                /*Clear existing mode */
                LEDPWM_CONTROL &= ((uint8)(~(LEDPWM_CTRL_CMPMODE1_MASK |
                                            LEDPWM_CTRL_CMPMODE2_MASK)));
                LEDPWM_CONTROL |= (comparemode1masked | comparemode2masked);

            #else
                uint8 temp = comparemode;
            #endif /* (LEDPWM_UsingFixedFunction) */
        }
    #endif /* LEDPWM_CompareMode1SW */

#else /* UseOneCompareMode */

    #if (LEDPWM_CompareMode1SW)


        /*******************************************************************************
        * Function Name: LEDPWM_SetCompareMode1
        ********************************************************************************
        *
        * Summary:
        *  This function writes the Compare Mode for the pwm or pwm1 output
        *
        * Parameters:
        *  comparemode:  The new compare mode for the PWM output. Use the compare types
        *                defined in the H file as input arguments.
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void LEDPWM_SetCompareMode1(uint8 comparemode) 
        {
            #if(0 != LEDPWM_CTRL_CMPMODE1_SHIFT)
                uint8 comparemodemasked = ((uint8)((uint8)comparemode << LEDPWM_CTRL_CMPMODE1_SHIFT)) &
                                           LEDPWM_CTRL_CMPMODE1_MASK;
            #else
                uint8 comparemodemasked = comparemode & LEDPWM_CTRL_CMPMODE1_MASK;
            #endif /* (0 != LEDPWM_CTRL_CMPMODE1_SHIFT) */

            #if (LEDPWM_UseControl)
                LEDPWM_CONTROL &= ((uint8)(~LEDPWM_CTRL_CMPMODE1_MASK)); /*Clear existing mode */
                LEDPWM_CONTROL |= comparemodemasked;
            #endif /* (LEDPWM_UseControl) */
        }
    #endif /* LEDPWM_CompareMode1SW */

#if (LEDPWM_CompareMode2SW)


    /*******************************************************************************
    * Function Name: LEDPWM_SetCompareMode2
    ********************************************************************************
    *
    * Summary:
    *  This function writes the Compare Mode for the pwm or pwm2 output
    *
    * Parameters:
    *  comparemode:  The new compare mode for the PWM output. Use the compare types
    *                defined in the H file as input arguments.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void LEDPWM_SetCompareMode2(uint8 comparemode) 
    {

        #if(0 != LEDPWM_CTRL_CMPMODE2_SHIFT)
            uint8 comparemodemasked = ((uint8)((uint8)comparemode << LEDPWM_CTRL_CMPMODE2_SHIFT)) &
                                                 LEDPWM_CTRL_CMPMODE2_MASK;
        #else
            uint8 comparemodemasked = comparemode & LEDPWM_CTRL_CMPMODE2_MASK;
        #endif /* (0 != LEDPWM_CTRL_CMPMODE2_SHIFT) */

        #if (LEDPWM_UseControl)
            LEDPWM_CONTROL &= ((uint8)(~LEDPWM_CTRL_CMPMODE2_MASK)); /*Clear existing mode */
            LEDPWM_CONTROL |= comparemodemasked;
        #endif /* (LEDPWM_UseControl) */
    }
    #endif /*LEDPWM_CompareMode2SW */

#endif /* UseOneCompareMode */


#if (!LEDPWM_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: LEDPWM_WriteCounter
    ********************************************************************************
    *
    * Summary:
    *  Writes a new counter value directly to the counter register. This will be
    *  implemented for that currently running period and only that period. This API
    *  is valid only for UDB implementation and not available for fixed function
    *  PWM implementation.
    *
    * Parameters:
    *  counter:  The period new period counter value.
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  The PWM Period will be reloaded when a counter value will be a zero
    *
    *******************************************************************************/
    void LEDPWM_WriteCounter(uint8 counter) \
                                       
    {
        CY_SET_REG8(LEDPWM_COUNTER_LSB_PTR, counter);
    }


    /*******************************************************************************
    * Function Name: LEDPWM_ReadCounter
    ********************************************************************************
    *
    * Summary:
    *  This function returns the current value of the counter.  It doesn't matter
    *  if the counter is enabled or running.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  The current value of the counter.
    *
    *******************************************************************************/
    uint8 LEDPWM_ReadCounter(void) 
    {
        /* Force capture by reading Accumulator */
        /* Must first do a software capture to be able to read the counter */
        /* It is up to the user code to make sure there isn't already captured data in the FIFO */
          (void)CY_GET_REG8(LEDPWM_COUNTERCAP_LSB_PTR_8BIT);

        /* Read the data from the FIFO */
        return (CY_GET_REG8(LEDPWM_CAPTURE_LSB_PTR));
    }

    #if (LEDPWM_UseStatus)


        /*******************************************************************************
        * Function Name: LEDPWM_ClearFIFO
        ********************************************************************************
        *
        * Summary:
        *  This function clears all capture data from the capture FIFO
        *
        * Parameters:
        *  None
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void LEDPWM_ClearFIFO(void) 
        {
            while(0u != (LEDPWM_ReadStatusRegister() & LEDPWM_STATUS_FIFONEMPTY))
            {
                (void)LEDPWM_ReadCapture();
            }
        }

    #endif /* LEDPWM_UseStatus */

#endif /* !LEDPWM_UsingFixedFunction */


/*******************************************************************************
* Function Name: LEDPWM_WritePeriod
********************************************************************************
*
* Summary:
*  This function is used to change the period of the counter.  The new period
*  will be loaded the next time terminal count is detected.
*
* Parameters:
*  period:  Period value. May be between 1 and (2^Resolution)-1.  A value of 0
*           will result in the counter remaining at zero.
*
* Return:
*  None
*
*******************************************************************************/
void LEDPWM_WritePeriod(uint8 period) 
{
    #if(LEDPWM_UsingFixedFunction)
        CY_SET_REG16(LEDPWM_PERIOD_LSB_PTR, (uint16)period);
    #else
        CY_SET_REG8(LEDPWM_PERIOD_LSB_PTR, period);
    #endif /* (LEDPWM_UsingFixedFunction) */
}

#if (LEDPWM_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: LEDPWM_WriteCompare
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare1 value when the PWM is in Dither
    *  mode. The compare output will reflect the new value on the next UDB clock.
    *  The compare output will be driven high when the present counter value is
    *  compared to the compare value based on the compare mode defined in
    *  Dither Mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  This function is only available if the PWM mode parameter is set to
    *  Dither Mode, Center Aligned Mode or One Output Mode
    *
    *******************************************************************************/
    void LEDPWM_WriteCompare(uint8 compare) \
                                       
    {
        #if(LEDPWM_UsingFixedFunction)
            CY_SET_REG16(LEDPWM_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG8(LEDPWM_COMPARE1_LSB_PTR, compare);
        #endif /* (LEDPWM_UsingFixedFunction) */

        #if (LEDPWM_PWMMode == LEDPWM__B_PWM__DITHER)
            #if(LEDPWM_UsingFixedFunction)
                CY_SET_REG16(LEDPWM_COMPARE2_LSB_PTR, (uint16)(compare + 1u));
            #else
                CY_SET_REG8(LEDPWM_COMPARE2_LSB_PTR, (compare + 1u));
            #endif /* (LEDPWM_UsingFixedFunction) */
        #endif /* (LEDPWM_PWMMode == LEDPWM__B_PWM__DITHER) */
    }


#else


    /*******************************************************************************
    * Function Name: LEDPWM_WriteCompare1
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare1 value.  The compare output will
    *  reflect the new value on the next UDB clock.  The compare output will be
    *  driven high when the present counter value is less than or less than or
    *  equal to the compare register, depending on the mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void LEDPWM_WriteCompare1(uint8 compare) \
                                        
    {
        #if(LEDPWM_UsingFixedFunction)
            CY_SET_REG16(LEDPWM_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG8(LEDPWM_COMPARE1_LSB_PTR, compare);
        #endif /* (LEDPWM_UsingFixedFunction) */
    }


    /*******************************************************************************
    * Function Name: LEDPWM_WriteCompare2
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare value, for compare1 output.
    *  The compare output will reflect the new value on the next UDB clock.
    *  The compare output will be driven high when the present counter value is
    *  less than or less than or equal to the compare register, depending on the
    *  mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void LEDPWM_WriteCompare2(uint8 compare) \
                                        
    {
        #if(LEDPWM_UsingFixedFunction)
            CY_SET_REG16(LEDPWM_COMPARE2_LSB_PTR, compare);
        #else
            CY_SET_REG8(LEDPWM_COMPARE2_LSB_PTR, compare);
        #endif /* (LEDPWM_UsingFixedFunction) */
    }
#endif /* UseOneCompareMode */

#if (LEDPWM_DeadBandUsed)


    /*******************************************************************************
    * Function Name: LEDPWM_WriteDeadTime
    ********************************************************************************
    *
    * Summary:
    *  This function writes the dead-band counts to the corresponding register
    *
    * Parameters:
    *  deadtime:  Number of counts for dead time
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void LEDPWM_WriteDeadTime(uint8 deadtime) 
    {
        /* If using the Dead Band 1-255 mode then just write the register */
        #if(!LEDPWM_DeadBand2_4)
            CY_SET_REG8(LEDPWM_DEADBAND_COUNT_PTR, deadtime);
        #else
            /* Otherwise the data has to be masked and offset */
            /* Clear existing data */
            LEDPWM_DEADBAND_COUNT &= ((uint8)(~LEDPWM_DEADBAND_COUNT_MASK));

            /* Set new dead time */
            #if(LEDPWM_DEADBAND_COUNT_SHIFT)
                LEDPWM_DEADBAND_COUNT |= ((uint8)((uint8)deadtime << LEDPWM_DEADBAND_COUNT_SHIFT)) &
                                                    LEDPWM_DEADBAND_COUNT_MASK;
            #else
                LEDPWM_DEADBAND_COUNT |= deadtime & LEDPWM_DEADBAND_COUNT_MASK;
            #endif /* (LEDPWM_DEADBAND_COUNT_SHIFT) */

        #endif /* (!LEDPWM_DeadBand2_4) */
    }


    /*******************************************************************************
    * Function Name: LEDPWM_ReadDeadTime
    ********************************************************************************
    *
    * Summary:
    *  This function reads the dead-band counts from the corresponding register
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Dead Band Counts
    *
    *******************************************************************************/
    uint8 LEDPWM_ReadDeadTime(void) 
    {
        /* If using the Dead Band 1-255 mode then just read the register */
        #if(!LEDPWM_DeadBand2_4)
            return (CY_GET_REG8(LEDPWM_DEADBAND_COUNT_PTR));
        #else

            /* Otherwise the data has to be masked and offset */
            #if(LEDPWM_DEADBAND_COUNT_SHIFT)
                return ((uint8)(((uint8)(LEDPWM_DEADBAND_COUNT & LEDPWM_DEADBAND_COUNT_MASK)) >>
                                                                           LEDPWM_DEADBAND_COUNT_SHIFT));
            #else
                return (LEDPWM_DEADBAND_COUNT & LEDPWM_DEADBAND_COUNT_MASK);
            #endif /* (LEDPWM_DEADBAND_COUNT_SHIFT) */
        #endif /* (!LEDPWM_DeadBand2_4) */
    }
#endif /* DeadBandUsed */

#if (LEDPWM_UseStatus || LEDPWM_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: LEDPWM_SetInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  This function configures the interrupts mask control of theinterrupt
    *  source status register.
    *
    * Parameters:
    *  uint8 interruptMode: Bit field containing the interrupt sources enabled
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void LEDPWM_SetInterruptMode(uint8 interruptMode) 
    {
        CY_SET_REG8(LEDPWM_STATUS_MASK_PTR, interruptMode);
    }


    /*******************************************************************************
    * Function Name: LEDPWM_ReadStatusRegister
    ********************************************************************************
    *
    * Summary:
    *  This function returns the current state of the status register.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8 : Current status register value. The status register bits are:
    *  [7:6] : Unused(0)
    *  [5]   : Kill event output
    *  [4]   : FIFO not empty
    *  [3]   : FIFO full
    *  [2]   : Terminal count
    *  [1]   : Compare output 2
    *  [0]   : Compare output 1
    *
    *******************************************************************************/
    uint8 LEDPWM_ReadStatusRegister(void) 
    {
        return (CY_GET_REG8(LEDPWM_STATUS_PTR));
    }

#endif /* (LEDPWM_UseStatus || LEDPWM_UsingFixedFunction) */


#if (LEDPWM_UseControl)


    /*******************************************************************************
    * Function Name: LEDPWM_ReadControlRegister
    ********************************************************************************
    *
    * Summary:
    *  Returns the current state of the control register. This API is available
    *  only if the control register is not removed.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8 : Current control register value
    *
    *******************************************************************************/
    uint8 LEDPWM_ReadControlRegister(void) 
    {
        uint8 result;

        result = CY_GET_REG8(LEDPWM_CONTROL_PTR);
        return (result);
    }


    /*******************************************************************************
    * Function Name: LEDPWM_WriteControlRegister
    ********************************************************************************
    *
    * Summary:
    *  Sets the bit field of the control register. This API is available only if
    *  the control register is not removed.
    *
    * Parameters:
    *  uint8 control: Control register bit field, The status register bits are:
    *  [7]   : PWM Enable
    *  [6]   : Reset
    *  [5:3] : Compare Mode2
    *  [2:0] : Compare Mode2
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void LEDPWM_WriteControlRegister(uint8 control) 
    {
        CY_SET_REG8(LEDPWM_CONTROL_PTR, control);
    }

#endif /* (LEDPWM_UseControl) */


#if (!LEDPWM_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: LEDPWM_ReadCapture
    ********************************************************************************
    *
    * Summary:
    *  Reads the capture value from the capture FIFO.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: The current capture value
    *
    *******************************************************************************/
    uint8 LEDPWM_ReadCapture(void) 
    {
        return (CY_GET_REG8(LEDPWM_CAPTURE_LSB_PTR));
    }

#endif /* (!LEDPWM_UsingFixedFunction) */


#if (LEDPWM_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: LEDPWM_ReadCompare
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare output when the PWM Mode parameter is
    *  set to Dither mode, Center Aligned mode, or One Output mode.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value
    *
    *******************************************************************************/
    uint8 LEDPWM_ReadCompare(void) 
    {
        #if(LEDPWM_UsingFixedFunction)
            return ((uint8)CY_GET_REG16(LEDPWM_COMPARE1_LSB_PTR));
        #else
            return (CY_GET_REG8(LEDPWM_COMPARE1_LSB_PTR));
        #endif /* (LEDPWM_UsingFixedFunction) */
    }

#else


    /*******************************************************************************
    * Function Name: LEDPWM_ReadCompare1
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare1 output.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value.
    *
    *******************************************************************************/
    uint8 LEDPWM_ReadCompare1(void) 
    {
        return (CY_GET_REG8(LEDPWM_COMPARE1_LSB_PTR));
    }


    /*******************************************************************************
    * Function Name: LEDPWM_ReadCompare2
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare2 output.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value.
    *
    *******************************************************************************/
    uint8 LEDPWM_ReadCompare2(void) 
    {
        return (CY_GET_REG8(LEDPWM_COMPARE2_LSB_PTR));
    }

#endif /* (LEDPWM_UseOneCompareMode) */


/*******************************************************************************
* Function Name: LEDPWM_ReadPeriod
********************************************************************************
*
* Summary:
*  Reads the period value used by the PWM hardware.
*
* Parameters:
*  None
*
* Return:
*  uint8/16: Period value
*
*******************************************************************************/
uint8 LEDPWM_ReadPeriod(void) 
{
    #if(LEDPWM_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(LEDPWM_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG8(LEDPWM_PERIOD_LSB_PTR));
    #endif /* (LEDPWM_UsingFixedFunction) */
}

#if ( LEDPWM_KillModeMinTime)


    /*******************************************************************************
    * Function Name: LEDPWM_WriteKillTime
    ********************************************************************************
    *
    * Summary:
    *  Writes the kill time value used by the hardware when the Kill Mode
    *  is set to Minimum Time.
    *
    * Parameters:
    *  uint8: Minimum Time kill counts
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void LEDPWM_WriteKillTime(uint8 killtime) 
    {
        CY_SET_REG8(LEDPWM_KILLMODEMINTIME_PTR, killtime);
    }


    /*******************************************************************************
    * Function Name: LEDPWM_ReadKillTime
    ********************************************************************************
    *
    * Summary:
    *  Reads the kill time value used by the hardware when the Kill Mode is set
    *  to Minimum Time.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8: The current Minimum Time kill counts
    *
    *******************************************************************************/
    uint8 LEDPWM_ReadKillTime(void) 
    {
        return (CY_GET_REG8(LEDPWM_KILLMODEMINTIME_PTR));
    }

#endif /* ( LEDPWM_KillModeMinTime) */

/* [] END OF FILE */
