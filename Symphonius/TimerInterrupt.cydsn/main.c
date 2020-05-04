/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"

uint8 switchPeriod = 0;

/** Set the LED to turn on for *time* seconds at *pwmMag*. */ 
void setLED(int pwmMag, int time) {
    LEDDuration_WritePeriod(time * 100);
    LEDPWM_WriteCompare1(pwmMag);
    LEDDuration_WriteCounter(0);
}

CY_ISR(TimerInterrupt)
{
    if (switchPeriod) {
        // Set LED to be on for 1 second and PWM to be half.
        switchPeriod = 0;
        setLED(130, 1);
    } else {
        // Set LED to be on for 2 seconds and PWM to be full
        switchPeriod = 1;
        setLED(255, 2);
    }
    Timer_1_ReadStatusRegister();
}

CY_ISR(LEDOffInterrupt) {
    LEDPWM_WriteCompare1(0);
    LEDDuration_ReadStatusRegister();
}

int main(void)
{
    Timer_1_Start();
    LEDDuration_Start();
    
    TimerInterrupt_Start();
    TimerInterrupt_StartEx(TimerInterrupt);
    
    LEDOffInterrupt_Start();
    LEDOffInterrupt_StartEx(LEDOffInterrupt);
    
    LEDPWM_Start();

    // 5 second cycle.
    Timer_1_WritePeriod(500);
    
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
