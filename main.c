#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define T1 1
#define T2 2
#define T3 3
#define THRESHOLD 16

int corners = 0;
int switch_pressed = 0;
int timer = 0; // 1->T1, 2->T2, 3->T3
int t1_passed;
int t2_passed;
int t3_passed;

int main(){
	PORTD.DIR |= 0b00000111;
	PORTF.PIN5CTRL |= PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc;
	
	sei();
	while(1) {
		if(switch_pressed == 0 && corners == 8)
			break;
		if(switch_pressed == 1 && corners == 0)
			break;
		
		t1_passed = 0;
		t2_passed = 0;
		t3_passed = 0;
		
		PORTD.OUT |= 0b00000101;
		PORTD.OUTCLR = 0b00000010;
		
		ADC0.CTRLA |= ADC_RESSEL_10BIT_gc | ADC_ENABLE_bm;
		ADC0.CTRLA &= 0b11111101; 
		ADC0.MUXPOS |= ADC_MUXPOS_AIN7_gc;
		ADC0.DBGCTRL |= ADC_DBGRUN_bm;
		ADC0.COMMAND = ADC_STCONV_bm;
		
		if(ADC0.RES > THRESHOLD){
			if(switch_pressed == 0){
				PORTD.OUT |= 0b00000010;
				PORTD.OUTCLR = 0b00000001;
				corners++;
				PORTD.OUT |= 0b00000001;
			}else if(switch_pressed == 1){
				PORTD.OUT |= 0b00000010;
				PORTD.OUTCLR = 0b00000100;
				corners--;
				PORTD.OUT |= 0b00000100;
			}
		}else{
			timer = 1;
			TCA0.SINGLE.CNT = 0;
			TCA0.SINGLE.CTRLB = 0;
			TCA0.SINGLE.CMP0 = T1;
			TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1024_gc | TCA_SINGLE_ENABLE_bm;
			TCA0.SINGLE.CTRLA |= 1;
			TCA0.SINGLE.INTCTRL = TCA_SINGLE_CMP0_bm;
			
			while(!t1_passed){}
				
			while(!t2_passed){}
		}
	}
	cli();
	
}


ISR(PORTF_PORT_vect){
	int intflags = PORTF.INTFLAGS;
	PORTF.INTFLAGS= intflags;
	
	switch_pressed = 1;
	PORTD.OUTCLR = 0b00000111;
	timer = 3;
	TCA0.SINGLE.CNT = 0;
	TCA0.SINGLE.CMP0 = T3;
	
	while(!t3_passed){}
	PORTD.OUT |= 0b00000111;
	
	main();
	exit(0);
}


ISR(TCA0_CMP0_vect){
	cli();
	
	int intflags = TCA0.SINGLE.INTFLAGS;
	TCA0.SINGLE.INTFLAGS = intflags;
	
	if(timer == 1){
		t1_passed = 1;

		ADC0.CTRLA |= ADC_FREERUN_bm | ADC_ENABLE_bm;
		ADC0.WINLT |= THRESHOLD;
		ADC0.INTCTRL |= ADC_WCMP_bm;
		ADC0.CTRLE |= ADC_WINCM0_bm;
		ADC0.COMMAND |= ADC_STCONV_bm;
		
		timer = 2;
		TCA0.SINGLE.CNT = 0;
		TCA0.SINGLE.CMP0 = T2;
	}else if(timer == 2){
		t2_passed = 1;
	}else if(timer == 3){
		t3_passed = 1;
	}
	
	sei();
}


ISR(ADC0_WCOMP_vect){
	cli();
	
	int intflags = ADC0.INTFLAGS;
	ADC0.INTFLAGS = intflags;
	
	ADC0.INTCTRL = 0;
	
	PORTD.OUT |= 0b00000010;
	if (switch_pressed == 0) {
		PORTD.OUTCLR = 0b00000100;
		corners++;
		PORTD.OUT |= 0b00000100;
	} else if (switch_pressed == 1) {
		PORTD.OUTCLR = 0b00000001;
		corners--;
		PORTD.OUT |= 0b00000001;
	}
	
	
	sei();
}