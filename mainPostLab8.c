/*
 * File:   mainPostLab8.c
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
#include <stdio.h>
#include <stdint.h>
#include <string.h>

/*
 * Variables globales
 */
const char data[] = "Carpe Diem \n\r";
const char menu[] = "----------------------------- MENÚ ----------------------------- \n\r";
const char data1[] = "Digite en su teclado el número de la acción que desea realizar \n\r";
const char data2[] = "1. Leer el potenciómetro \n\r";
const char data3[] = "2. Enviar ASCII \n\r";
const char data4[] = "Ingrese una opción:   ";
const char data5[] = "Opción no válida. Eliga un valor entre 1 y 2 \n\r";
const char data6[] = "El valor del potenciómetro es: ";
const char data7[] = "\n\r";
const char num1[] = "1 \n\r";
const char num2[] = "2 \n\r";
int ans = 0;
char pot[];

/*
 * Constantes
 */
#define _XTAL_FREQ 1000000

/*
 * Prototipos de funcion
 */
void setup(void);
void print_string(char* cadena);

/*
 * Interrupciones
 */
void __interrupt() isr(void)
{
    if(PIR1bits.RCIF)
    {
        PORTD = RCREG;
        ans = PORTD;
    }
}

void main(void) {
    setup();
    print_string(menu);
    print_string(data1);
    print_string(data2);
    print_string(data3);
    print_string(data4);
    
    while(ans == 0);
    
    if (ans == 49){
        print_string(num1);
        ADCON0bits.CHS = 0; 
        ADCON0bits.GO = 1; // Iniciar la conversión
        while(ADCON0bits.GO); // Esperar a que termine la conversión
        int adc_value_1 = ADRESH; // Leer el valor convertido
        PORTB = (char) adc_value_1;
        __delay_ms(10);
        
        sprintf(pot, "%d", adc_value_1);
        print_string(data6);
        print_string(pot);
        print_string(data7);
        print_string(data7);
    }
    else if (ans == 50) {
        print_string(num2);
        print_string(data);
        print_string(data7);
    }
    else{
        print_string(data5);
        print_string(data7);
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

/*
 * Funciones
 */

void print_string(char* cadena){
    int largo = strlen(cadena);
    int i;
    __delay_ms(100);
        
    for (i=0; i < largo; i++){
        __delay_ms(10);
        if (PIR1bits.TXIF) //Siempre está en 1, a menos que esté enviando datos
        {
            TXREG = cadena[i];
        }
    }
    return;
}