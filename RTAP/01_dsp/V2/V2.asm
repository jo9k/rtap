/********************************************************
 *
 * V2.ASM		FIR filterprogram for ADSP21369
 * Filename:               	V22.asm
 *
 * Description:
 *
 * The program shows how  to effectivly perform a convolution 
 * with the multiple instructions feature of the ADSP-21369
 *
 * Registers affected:
 *	
 * F0    F8     I0    	I1
 * F4    F12    I8	I2
 *	
 * Used parameters:
 * F0 = input sample x(n)
 * R1 = number of taps in the filter minus 1
 * B0 = address of the delay line buffer
 * M0 = modify value for the delay line buffer
 * L0 = length of the delay line buffer
 * B8 = address of the coefficent buffer
 * M8 = modify value of the coefficent buffer
 * L8 = length of the coefficent buffer
 *
 ********************************************************/

#define	SAMPLE 200		/*number of input samples to be filtered*/
#define	TAPS 23			/*length of filter*/

#include <def21369.h>

.SECTION/PM seg_pm32;

	/*FIR coefficients stored in file */
	.VAR     coefs[TAPS]="fircoefs.dat"; 				
.ENDSEG;
		
.SEGMENT/DM seg_dm32; 

	/*buffer that holds the delay line*/
	.VAR	dline[TAPS]; 

	/*input samples stored in file */
	.VAR	inbuf[SAMPLE] = "rect.dat";
				
	/*buffer that contains output coefficients*/
	.VAR	outbuf[SAMPLE];		
.ENDSEG; 



.SECTION/PM seg_pmco;

/* Set the scope of the '_main' symbol to global. This makes the symbol
   available for reference in object files that are linked to the current
   one. Use '.EXTERN' to refer to a global symbol from another file.*/
.GLOBAL _main;

/* Declare the '_main' symbol. */
_main:

	/*enable interrupts*/
	BIT SET MODE1 IRPTEN;	
	
	/* enable circular buffering */
	BIT SET MODE1 CBUFEN;
	
	L0=TAPS; 
	
	/*delay line buffer pointer initialisation*/
	B0=dline;		
	
	/*set modify-register M0=1*/
	M0=1;			
	
	/*set modify-register M1=1*/
	M1=1;			
	
	L1=@inbuf;
	
	/*input buffer pointer initialisation*/
	B1=inbuf;		
	
	L2=@outbuf;
	
	/*output buffer pointer initialisation*/
	B2=outbuf;		
	
	L8=TAPS;
	
	/*coefficient buffer pointer initialisation*/
	B8=coefs;		
	
	/*set modify-register M8=1*/	
	M8=1;			
	
	/*initialize delay line buffer to zero*/
	CALL fir_init (DB);	
	
	/* Set coefficient to second filter coeff */
	MODIFY(I8,M8);	    
	
	R0=TAPS;		
	
	/*enable interrupt IRQ0I*/
	BIT SET IMASK IRQ0I;	

done:	
	/*wait for interrupt */
	JUMP done;		

fir_init:
	LCNTR=R0, DO zero1 UNTIL LCE;

zero1: 	
	
	/*initialize the delay line to 0*/
	DM(I0,M0)=0;		
	
	R0=SAMPLE;
	
	LCNTR=R0, DO zero2 UNTIL LCE;
zero2: 	
	
	/*initialize the output buffer to 0*/
	DM(I2,M1)=0;		
	
	RTS; 
	
/* Delimit the '_main' symbol so the linker knows which code is associated
   with the symbol. */
._main.END:

.GLOBAL _filter;

_filter: 

	/* Filter one input sample. */
	CALL fir(DB);		
	
	/* At first setup the loop counter for circular buffer */
	R1=TAPS-3;

	/* Read in h(1) (the second filter coefficient) and x(k-1) */
	R8=R8 XOR R8, F0=DM(I0,M0), F4=PM(I8,M8);
	
	/* ======> Call fir subroutine */
	
	/*result is stored in outbuf */
	DM(I2,M1)=F0;		
	
	RTI;
fir:	
	
	/* Calculate x(k-1)*h(1) and load next values, x(k-2) and h(2) */
	F12 = F0*F4,  F0=DM(I0,M0), F4=PM(I8,M8);
				
	/*set loop to iterate taps-3 times*/
	LCNTR=R1, DO macs UNTIL LCE;
	     			
macs:	

	/* In loop: Calculate next product and add (MAC operation) */
	F12=F0*F4,  F8=F8+F12,  F0=DM(I0,M0),  F4=PM(I8,M8);
          			
	/* Loop is over, read in the input sample and the filter coefficient h(0) */
	F12=F0*F4,  F8=F8+F12,  F0=DM(I1,M1),  F4=PM(I8,M8);

	/* Trigger termination of function but with two instructions to follow */
	RTS(DB);
	
	/* Write the input into the delay line and calculate final product */
	F12=F0*F4,  F8=F8+F12, DM(I0,0) = F0;
				
._filter.END:

	/* perform last add and store result in F0*/
	F0=F8+F12;		

.ENDSEG;  
