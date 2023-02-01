/**
  @page ADC_ADC1_DMA ADC1 DMA example
  
  @verbatim
  * File   : ADC/ADC1_DMA/readme.txt 
  * Version: V1.2.9
  * Date   : 2021-01-15
  * Brief  : Description of the ADC1 DMA example.
  ******************************************************************************
   @endverbatim

@par Example Description 

This example describes how to use the ADC1 and DMA to transfer continuously 
converted data from ADC1 to memory.
The ADC1 is configured to convert continuously channel14.
Each time an end of conversion occurs the DMA transfers, in circular mode, the
converted data from ADC1 DR register to the ADCConvertedValue variable.
The ADC1 clock is set to 24 MHz.

 */
