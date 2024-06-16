#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define T1 250
#define T2 50
#define T3 80

int pedestrian_button_enabled = 1;
int t2_passed = 1;
int t3_passed = 0;

int main() {
	PORTD.DIR |= 0b00000111;
	PORTD.OUT |= 0b00000011;
	PORTD.OUTCLR = 0b00000100;
	PORTF.PIN5CTRL |= PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc;
	
	TCA0.SPLIT.CTRLD = TCA_SPLIT_SPLITM_bm;
	TCA0.SPLIT.CTRLA = TCA_SPLIT_CLKSEL_DIV1024_gc | TCA_SPLIT_ENABLE_bm;
	TCA0.SPLIT.CTRLB = TCA_SPLIT_LCMP0EN_bm;
	TCA0.SPLIT.INTCTRL = TCA_SPLIT_HUNF_bm;
	TCA0.SPLIT.LCMP0 = 0;
	TCA0.SPLIT.HCNT = T1;
	
	sei();
	while(1) {
	}
	
	cli();
}

ISR(PORTF_PORT_vect){
	cli();
	
	int interruptFlags = PORTF.INTFLAGS;
	PORTF.INTFLAGS = interruptFlags;
	
	if(pedestrian_button_enabled){
		PORTD.OUT |= 0b00000100;
		PORTD.OUTCLR = 0b00000001;
		
		pedestrian_button_enabled = 0;
		
		TCA0.SPLIT.LCNT = T2;
		TCA0.SPLIT.INTCTRL |= TCA_SPLIT_LUNF_bm;
	}
	
	sei();
}

ISR(TCA0_HUNF_vect){
	cli();
	int interruptFlags = TCA0.SPLIT.INTFLAGS;
	TCA0.SPLIT.INTFLAGS = interruptFlags;
	
	PORTD.OUT |= 0b00000100;
	PORTD.OUTCLR = 0b00000011;
	
	pedestrian_button_enabled = 0;
	
	TCA0.SPLIT.HCNT = T1;
	TCA0.SPLIT.LCNT = T2;
	TCA0.SPLIT.INTCTRL |= TCA_SPLIT_LUNF_bm;
	
	sei();
}


ISR(TCA0_LUNF_vect){
	cli();
	int interruptFlags = TCA0.SPLIT.INTFLAGS;
	TCA0.SPLIT.INTFLAGS = interruptFlags;
	
	if(t2_passed){
		PORTD.OUT |= 0b00000011;
		PORTD.OUTCLR = 0b00000100;
		TCA0.SPLIT.LCNT = T3;
		t2_passed = 0;
		t3_passed = 1;
	}else if(t3_passed){
		pedestrian_button_enabled = 1;
		t2_passed = 1;
		t3_passed = 0;
	}
	
	sei();
}