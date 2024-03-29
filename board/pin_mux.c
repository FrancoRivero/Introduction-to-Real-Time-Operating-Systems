/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL ***********
PinsProfile:
- !!product 'Pins v2.0'
- !!processor 'MK64FN1M0xxx12'
- !!package 'MK64FN1M0VLL12'
- !!mcu_data 'ksdk2_0'
- !!processor_version '1.0.9'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL *
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"
#include "board.h"


#define PIN16_IDX                       16u   /*!< Pin number for pin 16 in a port */

#define PIN17_IDX                       17u   /*!< Pin number for pin 17 in a port */

#define SOPT5_UART0TXSRC_UART_TX      0x00u   /*!< UART 0 transmit data source select: UART0_TX pin */

#define SW2								 6u
#define SW3								 4u

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL ***********
BOARD_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '62', peripheral: UART0, signal: RX, pin_signal: PTB16/SPI1_SOUT/UART0_RX/FTM_CLKIN0/FB_AD17/EWM_IN}
  - {pin_num: '63', peripheral: UART0, signal: TX, pin_signal: PTB17/SPI1_SIN/UART0_TX/FTM_CLKIN1/FB_AD16/EWM_OUT_b}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL *
*/

/*FUNCTION***********************
 * 
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 * 
 END*************************/
void BOARD_InitBootPins(void) {
  BOARD_InitPins();
}


/*FUNCTION***********************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 END*************************/
void BOARD_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortB);                           /* Port B Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortE);
  CLOCK_EnableClock(kCLOCK_PortA);
  CLOCK_EnableClock(kCLOCK_PortC);

  PORT_SetPinMux(PORTB, PIN16_IDX, kPORT_MuxAlt3);           /* PORTB16 (pin 62) is configured as UART0_RX */
  PORT_SetPinMux(PORTB, PIN17_IDX, kPORT_MuxAlt3);           /* PORTB17 (pin 63) is configured as UART0_TX */
  PORT_SetPinMux(BOARD_LED_RED_GPIO_PORT, BOARD_LED_RED_GPIO_PIN, kPORT_MuxAsGpio);
  PORT_SetPinMux(BOARD_LED_GREEN_GPIO_PORT, BOARD_LED_GREEN_GPIO_PIN, kPORT_MuxAsGpio);
  PORT_SetPinMux(BOARD_LED_BLUE_GPIO_PORT, BOARD_LED_BLUE_GPIO_PIN, kPORT_MuxAsGpio);
  //  PORT_SetPinMux(PORTC, SW2, kPORT_MuxAsGpio);
  PORT_SetPinMux(BOARD_SW2_PORT, BOARD_SW2_GPIO_PIN, kPORT_MuxAsGpio);
  // Setup the SW2 pin interrupt
  PORT_SetPinInterruptConfig(BOARD_SW2_PORT, BOARD_SW2_GPIO_PIN, kPORT_InterruptEitherEdge );
  EnableIRQ(BOARD_SW2_IRQ);
  // Setup the GPIO pin to be an input
  GPIO_PinInit(BOARD_SW2_GPIO, BOARD_SW2_GPIO_PIN, &(gpio_pin_config_t){kGPIO_DigitalInput, 0});

  SIM->SOPT5 = ((SIM->SOPT5 &
    (~(SIM_SOPT5_UART0TXSRC_MASK)))                          /* Mask bits to zero which are setting */
      | SIM_SOPT5_UART0TXSRC(SOPT5_UART0TXSRC_UART_TX)       /* UART 0 transmit data source select: UART0_TX pin */
    );
}

/***************************
 * EOF
 **************************/
