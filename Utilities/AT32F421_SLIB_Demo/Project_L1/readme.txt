/**
  ******************************************************************************
  * File   : Project_L1/readme.txt 
  * Version: V1.2.9
  * Date   : 2021-01-15
  * Brief  : readme
  ******************************************************************************
  */
 
@par Example Description 

   In this Project_L1 project the MCU is already preloaded with FIR-Filter SLIB protected IP-Code (done in Project_L0 project).
   To call the IP-Code FIR_lowpass_filter() function:
   - The fir_filter.h header file is included in main.c file
   - Symbol definition file (fir_filter_symbol.txt for MDK-ARM) containing the protected function symbols 
     and addresses is included in this project. Noting that symbol definition file have to be generated in Project_L0 
	   in order to replace the existing one in this project.
	 
 @note The application needs to ensure that the SysTick time base is always set to 1 millisecond to have correct operation.
 
 @par Directory contents 

  - Project_L1/src/srcmain.c                             Main program
  - Project_L1/src/system_at32f4xx.c                     AT32F4xx system clock configuration file
  - Project_L1/src/at32f4xx_it.c                         Interrupt handlers 
  - Project_L1/src/math_helper.c                         Calculation of SNR 
  - Project_L1/src/arm_fir_data.c                        Contains FIR_Filter's Input signal and Reference output signal computed with MATLAB
  - Project_L1/inc/at32f4xx_it.h                         Interrupt handlers header file
  - Project_L1/inc/math_helper.h                         math_helper header file

Related IP-Code files provided from Project_L0:
  - Project_L1/MDK_V5/fir_filter_symbol.txt              Symbol definition file for MDK-ARM project containing SLIB protected code symbols and addresses
  - Project_L1/inc/fir_filter.h                          FIR-Filter SLIB protected IP-Code header file
      
@par Hardware and Software environment  

  - This example runs on AT32F421C8T7 devices.
  - This example has been tested with AT-START-F421 V1.0 board.
  - This example has been tested with the following toolchain:
      - Keil MDK-ARM uVision V5.23.0.0 
      - IAR Embedded Workbench V7.40.3

@par How to use it ? 

Before running this project the following conditions must be met:
- The same toolchain and compiler version must be used for both projects in Project_L0 and Project_L1: for instance if MDK-ARM V5.23 is used 
  in Project_L0 then it must be used for Project_L1 too.
- Check if any Read out protection, write protection or SLIB protection is enabled, if yes disable this protection 

In order to make the program work, you must do the following :

Configurate the compiler to generate an execute only IP-Code avoiding any data read from it (literal pools and branch tables) . 
	1. Open project located in Project_L0 directory and choose your preferred toolchain 
	2. Select the SLIB-IP-Code-XO project configuration
	3. Rebuild all files. 
	4. Run the example following the sequence below:
		(a) Power on the board then load the code, WHITE LED should toggle continuously, if not press RESET button;
        (b) Press the user button key to execute the IP-code called in main.c file,
		(c) If the IP-Code is successfully executed, the GREEN LED should toggle continuously. Else the RED LED toggles continuously.  
		        

 */ 



