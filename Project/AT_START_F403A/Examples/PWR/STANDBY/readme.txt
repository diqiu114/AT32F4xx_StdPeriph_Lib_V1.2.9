/**
 @page  PWR_STANDBY example
 @verbatim
  * File   : PWR/STANDBY/readme.txt
  * Version: V1.2.9
  * Date   : 2021-01-15
  * Brief  : Description of the PWR_STANDBY example.
  ********************************************************************************************
 @endverbatiom

@par Example Desctiption
  This demo is based on the AT-START-F403A board.
  The system enters and exits Standby mode as following:
	Initializes and turn off LED.Configure RTC.Within 3 second Turn on LED,then 
	within 3 second enter Standby mode,LED will turn off right now.The  RTC Alarm 
	will wake-up the system within 3 second.As the infinite loop.
 
  */


