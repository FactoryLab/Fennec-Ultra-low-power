/*
 * An example project that configures the ATSAML21 to an ultra low power mode
 * On the Factorylab Fennec the power consumption will drop to ~4.4uA
 */ 

#include "sam.h"

int main(void) {
	while(SUPC->INTFLAG.bit.VCORERDY != 1) { } // before modifying the regulator settings, make sure the voltage is stable
	SUPC->BOD12.bit.ENABLE = 0; // disable the Brown Out Detector 1.2V to save energy
	SUPC->BOD33.bit.ENABLE = 0; // disable the Brown Out Detector 3.3V to save energy
	SUPC->VREG.bit.RUNSTDBY = 0; // do not keep the main voltage regulator running in standby mode (instead, switch to the low power one)
	SUPC->VREG.bit.LPEFF = 0; // do not enable the low power efficient regulator, as it is not stable when the supply voltage is < 2.6V
	SUPC->VREG.bit.ENABLE = 1; // enable the voltage regulator
	
	OSCCTRL->OSC16MCTRL.reg = OSCCTRL_OSC16MCTRL_ENABLE | OSCCTRL_OSC16MCTRL_FSEL_4 | OSCCTRL_OSC16MCTRL_ONDEMAND; // enable the internal 16MHz oscillator to run on 4MHz
	while(OSCCTRL->STATUS.bit.OSC16MRDY != 1) { } // wait until the oscillator has started
	
	GCLK->GENCTRL[0].bit.SRC = GCLK_GENCTRL_SRC_OSC16M; // set the source of the main clock source to the internal 16MHz oscillator
	while(GCLK->SYNCBUSY.bit.GENCTRL0 != 0) { } // wait until the main clock has been reconfigured
	
	PM->SLEEPCFG.bit.SLEEPMODE = PM_SLEEPCFG_SLEEPMODE_STANDBY; // set sleep mode to standby
	while(PM->SLEEPCFG.bit.SLEEPMODE != PM_SLEEPCFG_SLEEPMODE_STANDBY) { } // wait for reconfiguration of the sleep mode
	
	// set the source of the clocks to a disabled generator
	// NOTE: Although this is recommended in the application note, it does not seem to have any significant effect
	GCLK->PCHCTRL[EIC_GCLK_ID].bit.GEN =			8;
	GCLK->PCHCTRL[USB_GCLK_ID].bit.GEN =			8;
	GCLK->PCHCTRL[SERCOM0_GCLK_ID_CORE].bit.GEN =	8;
	GCLK->PCHCTRL[SERCOM0_GCLK_ID_SLOW].bit.GEN =	8;
	GCLK->PCHCTRL[SERCOM1_GCLK_ID_CORE].bit.GEN =	8;
	GCLK->PCHCTRL[SERCOM1_GCLK_ID_SLOW].bit.GEN =	8;
	GCLK->PCHCTRL[SERCOM2_GCLK_ID_CORE].bit.GEN =	8;
	GCLK->PCHCTRL[SERCOM2_GCLK_ID_SLOW].bit.GEN =	8;
	GCLK->PCHCTRL[SERCOM3_GCLK_ID_CORE].bit.GEN =	8;
	GCLK->PCHCTRL[SERCOM3_GCLK_ID_SLOW].bit.GEN =	8;
	GCLK->PCHCTRL[SERCOM4_GCLK_ID_CORE].bit.GEN =	8;
	GCLK->PCHCTRL[SERCOM4_GCLK_ID_SLOW].bit.GEN =	8;
	GCLK->PCHCTRL[SERCOM5_GCLK_ID_CORE].bit.GEN =	8;
	GCLK->PCHCTRL[SERCOM5_GCLK_ID_SLOW].bit.GEN =	8;
	GCLK->PCHCTRL[TCC0_GCLK_ID].bit.GEN =			8;
	GCLK->PCHCTRL[TCC1_GCLK_ID].bit.GEN =			8;
	GCLK->PCHCTRL[TCC2_GCLK_ID].bit.GEN =			8;
	GCLK->PCHCTRL[TC0_GCLK_ID].bit.GEN =			8;
	GCLK->PCHCTRL[TC1_GCLK_ID].bit.GEN =			8;
	GCLK->PCHCTRL[TC4_GCLK_ID].bit.GEN =			8;
	GCLK->PCHCTRL[EVSYS_GCLK_ID_0].bit.GEN =		8;
	GCLK->PCHCTRL[EVSYS_GCLK_ID_1].bit.GEN =		8;
	GCLK->PCHCTRL[EVSYS_GCLK_ID_2].bit.GEN =		8;
	GCLK->PCHCTRL[EVSYS_GCLK_ID_3].bit.GEN =		8;
	GCLK->PCHCTRL[EVSYS_GCLK_ID_4].bit.GEN =		8;
	GCLK->PCHCTRL[EVSYS_GCLK_ID_5].bit.GEN =		8;
	GCLK->PCHCTRL[EVSYS_GCLK_ID_6].bit.GEN =		8;
	GCLK->PCHCTRL[EVSYS_GCLK_ID_7].bit.GEN =		8;
	GCLK->PCHCTRL[EVSYS_GCLK_ID_8].bit.GEN =		8;
	GCLK->PCHCTRL[EVSYS_GCLK_ID_9].bit.GEN =		8;
	GCLK->PCHCTRL[EVSYS_GCLK_ID_10].bit.GEN =		8;
	GCLK->PCHCTRL[EVSYS_GCLK_ID_11].bit.GEN =		8;
	GCLK->PCHCTRL[ADC_GCLK_ID].bit.GEN =			8;
	GCLK->PCHCTRL[AC_GCLK_ID].bit.GEN =				8;
	GCLK->PCHCTRL[PTC_GCLK_ID].bit.GEN =			8;
	GCLK->PCHCTRL[CCL_GCLK_ID].bit.GEN =			8;
	
	// WARNING: the settings below will seriously impair the functionality of the device as the data communication with these peripherals will no longer be possible
	// NOTE: Although this is recommended in the application note, it does not seem to have any significant effect
	// disable peripheral data busses
	MCLK->APBAMASK.reg = MCLK_APBAMASK_PM | MCLK_APBAMASK_OSCCTRL |
	MCLK_APBAMASK_OSC32KCTRL | MCLK_APBAMASK_SUPC; // disable all except the power manager, oscillator control, oscillator 32k control and the supply controller
	MCLK->APBBMASK.reg = MCLK_APBBMASK_NVMCTRL; // disable all except the the nvm controller
	MCLK->APBCMASK.reg = 0; // disable all
	MCLK->APBDMASK.reg = 0; // disable all
	MCLK->APBEMASK.reg = 0; // disable all
	MCLK->AHBMASK.reg = MCLK_AHBMASK_NVMCTRL; // disable all on the high speed bus except the nvm controller (as we need to fetch code from the flash to execute)
	
	int i = 0;
    while (1) {
		__DSB(); // flush all memory caches
		__WFI(); // go to sleep and wait for interrupt
		
		// we woke from an interrupt, increment integer and wait for next interrupt
		i++;
    }
}
