/*
 * File:   mainPrelab8.c
 * Author: Amilcar Rodriguez
 *
 */

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// LIBRERIAS
#include <xc.h>
#include <stdint.h>

/*
 * Variables globales
 */
const char data = 246;

/*
 * Constantes
 */
#define _XTAL_FREQ 1000000

/*
 * Prototipos de funcion
 */
void setup(void);


/*
 * Interrupciones
 */
void __interrupt() isr(void)
{
    if(PIR1bits.RCIF)
    {
        PORTD = RCREG;
    }
}

void main(void) {
    
    setup();
    while(1){
        __delay_ms(500);
        
        if (PIR1bits.TXIF) //Siempre está en 1, a menos que esté enviando datos
        {
            TXREG = data;
        }
    }
}

void setup(void){
    // CONFIGURACION DE ENTRADAS Y SALIDAS
    // Pines digitales
    ANSEL = 0;
    ANSELH = 0;
    
    TRISB = 0;
    TRISD = 0;
    PORTD = 0x00;
    
    // Configuracion oscilador interno
    OSCCONbits.IRCF = 0b100; // 1MHz
    OSCCONbits.SCS = 1;
    
    //Configuracion de TX y RX
    TXSTAbits.SYNC = 0;     //Asincrono
    TXSTAbits.BRGH = 1;     // (High Baud Rate Select bit) en High Speed
    
    BAUDCTLbits.BRG16 = 1;
    
    SPBRG = 25;
    SPBRGH = 0;
    
    RCSTAbits.SPEN = 1;     //Serial Port enabled
    RCSTAbits.RX9 = 0;      // 8-bit reception
    RCSTAbits.CREN = 1;     //Continuous Receive enabled
    
    TXSTAbits.TXEN = 1;     // Transmit enabled
    
    
    
    //Configuracion de las interrupciones
    PIR1bits.RCIF = 0;
    PIE1bits.RCIE = 1;
    INTCONbits.PEIE = 1; // Interrupciones perifericas
    INTCONbits.GIE = 1; // Interrupciones globales
    
    // Configuracion ADC
    ANSELbits.ANS0 = 1;
    ANSELbits.ANS1 = 1;
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 1;
    ADCON0bits.ADCS = 0b10; // FOSC/32
    __delay_ms(1);
    ADCON1bits.ADFM = 0; //Justificado a la izquierda
    ADCON1bits.VCFG0 = 0; //Voltaje de ref. a GND
    ADCON1bits.VCFG1 = 0; //Voltaje de ref. a 5v
    ADCON0bits.ADON = 1; // ADC is enabled
    ADIF = 0;
    
    return;
}
