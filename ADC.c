
#include "MKL27Z644.h"

#define SUCCESS 0
#define ERROR 1

typedef unsigned char uint_8;
uint_8 ADC_bfnReadADC(uint_8 bChannel,uint_8 * bpADCValue);
void ADC_vfnDriverInit(void);


int main(void) {
    volatile static float i = 0 ;
    uint_8 * pointer;
    uint_8 data=0;
    pointer=&data;
    uint_8 chanelito=0b11010;
    ADC_vfnDriverInit();
    while(1) {
        i=ADC_bfnReadADC(chanelito,pointer);
        if(i==0){
        	i++;
        }
        else{
        	i--;
        }
    }
    return 0 ;
}


void ADC_vfnDriverInit(void){
	SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK; //Enciende el ADC0
	SIM->SOPT7=SIM_SOPT7_ADC0PRETRGSEL(0);//trigger select
	ADC0->SC1[0]=ADC_SC1_DIFF(0); //Single-ended
	ADC0->CFG1=ADC_CFG1_ADIV(0); //divisor de 1, queda en 8MHz de frecuencia.
	ADC0->CFG1=ADC_CFG1_MODE(0); //8 bits de resolucion
	ADC0->CFG1=ADC_CFG1_ADICLK(0); //divisor de clock en 1.
	ADC0->CFG1=ADC_CFG1_ADLPC(0); //normal mode
	ADC0->CFG2=ADC_CFG2_ADHSC(0); //normal conversion sequence selected
	ADC0->SC2=ADC_SC2_DMAEN(0); //DMA desactivado
	ADC0->SC2=ADC_SC2_ADTRG(0); //Software Trigger Select
	ADC0->SC3=ADC_SC3_AVGE(1); //Hardware Average Enable
	ADC0->SC3=ADC_SC3_AVGS(3); //16 samples averaged
}

uint_8 ADC_bfnReadADC(uint_8 bChannel,uint_8 * bpADCValue){
	ADC0->SC1[0]=ADC_SC1_ADCH(bChannel); //Selecciona el sensor de temperatura.
	while((ADC0->SC2&ADC_SC2_ADACT_MASK));
	while(!(ADC0->SC1[0]&ADC_SC1_COCO_MASK ));
	if(ADC_SC1_COCO_MASK)
	{
		*bpADCValue=ADC0->R[0];
		return SUCCESS;

	}
	return ERROR;
}

