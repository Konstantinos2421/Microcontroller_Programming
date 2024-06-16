#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#define minThreshold 8
#define maxThreshold 128
#define T1 20

int counter = 1;

int main(void){
	
	PORTD.DIR |= 0b00000111;
	PORTD.OUT |= 0b00000111;
	PORTF.PIN5CTRL |= PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc;
	PORTF.PIN6CTRL |= PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc;
	
	ADC0.CTRLA |= ADC_RESSEL_10BIT_gc | ADC_FREERUN_bm | ADC_ENABLE_bm;
	ADC0.MUXPOS |= ADC_MUXPOS_AIN7_gc;
	ADC0.DBGCTRL |= ADC_DBGRUN_bm;
	ADC0.WINLT = minThreshold;
	ADC0.WINHT = maxThreshold;
	ADC0.INTCTRL |= ADC_WCMP_bm;
	ADC0.CTRLE |= ADC_WINCM_OUTSIDE_gc;
	
	TCA0.SPLIT.CTRLA |= TCA_SPLIT_CLKSEL_DIV1024_gc;
	TCA0.SPLIT.CTRLB |= TCA_SPLIT_HCMP0EN_bm;
	TCA0.SPLIT.CTRLD |= TCA_SPLIT_ENABLE_bm;
	
	sei();
	
	ADC0.COMMAND |= ADC_STCONV_bm;
	while(1){
	}
	
	cli();
}

ISR(ADC0_WCOMP_vect){
	cli();
	
	int intflags = ADC0.INTFLAGS;
	ADC0.INTFLAGS = intflags;
	
	if(ADC0.RES < minThreshold){
		ADC0.INTCTRL = 0;
		PORTD.OUTCLR = 0b00000001;
	}else if (ADC0.RES > maxThreshold){
		ADC0.INTCTRL = 0;
		PORTD.OUTCLR = 0b00000010;
	}
	
	sei();
}


ISR(PORTF_PORT_vect){
	cli();
	
	if(((PORTF.INTFLAGS & 0b00100000) == 0b00100000) && ((PORTD.OUT & 0b00000001) == 0b00000000)){
		//(switch5 & LED0) = on
		TCA0.SPLIT.LCNT= (minThreshold - ADC0.RES)*((20*1000000)/1024);
		TCA0.SPLIT.INTCTRL |= TCA_SPLIT_LUNF_bm;
		TCA0.SPLIT.CTRLA |= TCA_SPLIT_ENABLE_bm;
		
	}else if(((PORTF.INTFLAGS & 0b01000000) == 0b01000000) && ((PORTD.OUT & 0b00000010) == 0b00000000)){
		//(switch6 & LED1) = on
		TCA0.SPLIT.HPER = T1;
		TCA0.SPLIT.HCMP0 = T1 * 0.5;
		TCA0.SPLIT.INTCTRL |= TCA_SPLIT_HUNF_bm;
		TCA0.SPLIT.CTRLA |= TCA_SPLIT_ENABLE_bm;
		
	}else{
		PORTD.OUTCLR = 0b00000111;
		PORTD.OUT |= 0b00000111;
		ADC0.INTCTRL |= ADC_WCMP_bm;
	}
	
	int intflags = PORTF.INTFLAGS;
	PORTF.INTFLAGS = intflags;
	
	sei();
}

ISR(TCA0_LUNF_vect) {
	cli();
	
	int intflags = TCA0.SPLIT.INTFLAGS;
	TCA0.SPLIT.INTFLAGS = intflags;
	
	TCA0.SPLIT.CTRLA &= 0b11111110; //timer = off
	TCA0.SPLIT.INTCTRL = 0; //LUNF = off
	
	PORTD.OUT |= 0b00000001;
	ADC0.INTCTRL |= ADC_WCMP_bm;
	
	sei(); //change RES value
}

ISR(TCA0_HUNF_vect) {
	cli();
	
	int intflags = TCA0.SPLIT.INTFLAGS;
	TCA0.SPLIT.INTFLAGS = intflags;
	
	if(counter == 4){
		PORTD.OUT |= 0b00000110;
		counter = 1;
		TCA0.SPLIT.CTRLA &= 0b11111110; //timer = off
		TCA0.SPLIT.INTCTRL = 0; //HUNF = off
		ADC0.INTCTRL |= ADC_WCMP_bm;
	}else{
		counter++;
		PORTD.OUTTGL = 0b00000100;
	}
	
	sei(); //change RES value
}
