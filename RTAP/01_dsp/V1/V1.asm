/*****************************************************************************
 * V1.asm
 *****************************************************************************/
#include <def21369.h>

.SECTION/PM seg_dm32;

#define	SAMPLES	9		
             
    /* Input */
	.VAR input[SAMPLES]=  -4.,-3.,-2.,-1. ,0., 1.,2.,3.,4.;

	/* Output */		
	.VAR output[SAMPLES]= 0.,0.,0.,0.,0.,0.,0.,0.,0.;	
    
.ENDSEG; 



.SECTION/PM seg_pmco;

/* Set the scope of the '_main' symbol to global. This makes the symbol
   available for reference in object files that are linked to the current
   one. Use '.EXTERN' to refer to a global symbol from another file.*/
.GLOBAL _main;

/* Declare the '_main' symbol. */
_main:

	/* Set bit called IRPTEN to enable interrupts */
	BIT SET MODE1 IRPTEN;	

	/* Set irq0i interrupt bit */ 
    BIT SET IMASK IRQ0I;    

	/* set value for modify-register M0 */		   
    M0=1;			

	/* set length of ringbuffer B0 */ 
    L0=@input;		

	/* name buffer B0, I0 is set at the same time */ 
    B0=input;	
    
 	/* set length of ringbuffer B1 */
    L1=@output; 		

    /* name buffer B1, set I1 */
    B1=output;		

	/* Switch to low power mode and expect interrupt */
end:
	idle;			
	JUMP end;	

/* Delimit the '_main' symbol so the linker knows which code is associated
   with the symbol. */
._main.END:

.GLOBAL _convolution;

_convolution: 
	/* put the current value of buffer addressed by I0 to register
	   F0 and increase buffer with M0 */
	R0=DM(I0,M0);

	/* Here the modification for the input value can be placed:
	   Calculate the absolute value (ASM insn abs) */
	
	/* put value from register F0 to buffer B1 and
 	   increase with M0 */
    DM(I1,M0)=R0;

_convolution.END:
	 RTI;
 
.ENDSEG;