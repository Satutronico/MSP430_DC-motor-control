/*
 * MSP430G2553 Motor control
 *
 *  Creado el: 	15/01/2015				
 *      Autor:	Álvaro C. Satué
 *      
 */ 
 /* REFERENCIAS:
 *      MSP430G2253 ADC_3GPIO v1.c
 *  	Author: Ant Scranney
 *  	
 *  	
 *  	
 */

#include <msp430.h>

// Global Variables
unsigned int BufferADC[2] = {0};	// This will hold the Vel and Dir variables
unsigned int Vel = 0;
unsigned int Dir = 0;

int main(void)
{
	Conf_ADC();
	while(1)
	{Lectura_Poten();
	
	}
}


// 	INTERRUPCIONES

// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
	__bic_SR_register_on_exit(LPM3_bits); // Exit LPM3 on exiting ISR.
	
}

//	FUNCIONES
void Conf_ADC(void)	//Configuración inicial del ADC10
{
	  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
	  ADC10CTL1 = INCH_1 + CONSEQ_1;            // Entradas A1 y A0, secuencia de canales.
	  ADC10CTL0 = ADC10SHT_3 + MSC + ADC10ON + ADC10IE; //S&H de x64 ciclos reloj. Multiple conversión. ENcender ADC10. Interrupcion habilitada
	  ADC10DTC1 = 0x02;                         // 2 conversiones
	  ADC10AE0 |= 0x02;                         // Disable digital I/O on P1.0 to P1.1
}

void Lectura_Poten(void)
{
    ADC10CTL0 &= ~ENC;						// Deshabilita para poder configurar registros
    while (ADC10CTL1 & BUSY);               // Wait if ADC10 core is active
    ADC10SA = (unsigned int)BufferADC;		// Copies data in ADC10SA to unsigned int BufferADC array
    ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
 
    Vel = BufferADC[0];						// BufferADC array 0 copied to the variable Vel
    Dir = BufferADC[1];						// BufferADC array 1 copied to the variable Dir
	__bis_SR_register(LPM3_bits + GIE);   // Entra al modo bajo consumo con interrupciones habilitadas
}
