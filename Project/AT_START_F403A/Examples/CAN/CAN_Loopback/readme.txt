/**
 @page  CAN Loopback mode Transmit data example
 @verbatim
  * File   : CAN/CAN_Loopback/readme.txt
  * Version: V1.2.9
  * Date   : 2021-01-15
  * Brief  : Description of the CAN Loopback mode Transmit data example.
  ********************************************************************************************
 @endverbatiom

@par Example Desctiption
 This demo is based on the AT-START-F403A board,in this demo,CAN work in LoopBack mode.
 LED4 toggle when transmit CAN data.
 User can use USBCAN to receive data.
 User can check register CAN->ESTS and CAN->TSTS to affirm no error causing when disconnect CAN_RX pin.
 
@note
 ??????	In main.c line 46:The following one of the define AT32_USE_CAN1 must be choose
 ??????	In main.c line 51:The following one of the define CAN1_NOREMAP and CAN1_REMAP10 and CAN1_REMAP11 must be choose
 
  */
 

