#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define Tb 40
#define Tl 20

int switch_option = 1;

int main(){
	PORTD.DIR |= 0b00000011;
	PORTD.OUT |= 0b00000011;
	PORTF.PIN5CTRL |= PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc;
	
	sei();
	while (1) {
	}
	cli();
}

ISR(PORTF_PORT_vect){
	cli();
	
	int intflags = PORTF.INTFLAGS;
	PORTF.INTFLAGS = intflags;
	
	if(switch_option == 1){
		TCA0.SPLIT.CTRLA = TCA_SPLIT_CLKSEL_DIV1024_gc;
		TCA0.SPLIT.CTRLD = TCA_SPLIT_ENABLE_bm;
		TCA0.SPLIT.INTCTRL = TCA_SPLIT_LUNF_bm;
		TCA0.SPLIT.LPER = Tl;
		TCA0.SPLIT.LCMP0 = Tl * 0.5;
		TCA0.SPLIT.HPER = Tb;
		TCA0.SPLIT.HCMP0 = Tb * 0.6;
		TCA0.SPLIT.CTRLB = 0b00010001;
		TCA0.SPLIT.CTRLA |= TCA_SPLIT_ENABLE_bm;
		switch_option = 2;
	}else if(switch_option == 2){
		TCA0.SPLIT.LPER = Tl/2;
		TCA0.SPLIT.LCMP0 = (Tl/2) * 0.5;
		switch_option = 3;
	}else if(switch_option == 3){
		TCA0.SPLIT.CTRLA = 0;
		PORTD.OUT |= 0b00000011;
		switch_option = 1;
	}
	sei();
}

ISR(TCA0_LUNF_vect){
	cli();
	if((TCA0.SPLIT.INTFLAGS & 0b00000010) == 0b00000010){
		PORTD.OUTTGL = 0b00000011;
	}else{
		PORTD.OUTTGL = 0b00000001;
	}
	
	int intflags = TCA0.SPLIT.INTFLAGS;
	TCA0.SPLIT.INTFLAGS = intflags;
	sei();
}


