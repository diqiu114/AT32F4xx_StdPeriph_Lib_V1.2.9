/**
 @page  PWR_PVD example
 @verbatim
  * File   : PWR/PVD/readme.txt
  * Version: V1.2.9
  * Date   : 2021-01-15
  * Brief  : Description of the PWR_PVD example.
  ********************************************************************************************
 @endverbatiom

@par Example Desctiption
  This demo is based on the AT-START-F413 board.
  This example shows how to configure the programmable voltage detector usingan external interrupt line.
  In this example, EXTI line 16 is configured to generate an interrupt on each rising or falling edge 
  of the PVD output signal (which indicates that the Vdd voltage is below the PVD threshold).
  In the interrupt routine a led connected to a specific GPIO pin is toggled every time the voltage drops 
  below or  the target threshold.
  LED2-->GPIOC Pin2
  LED3-->GPIOC Pin3
  LED4-->GPIOC Pin5
 
  */


