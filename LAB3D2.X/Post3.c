//*****************************************************************************
/*
 * File:   main.c
 * Author: Bryan
 * Ejemplo de implementación de la comunicación SPI 
 * Código Maestro
 * Created on 4 de agosto 2023, 03:32 PM
 */
//*****************************************************************************
//*****************************************************************************
// Palabra de configuración
//*****************************************************************************
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
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

//*****************************************************************************
// Definición e importación de librerías
//*****************************************************************************
#include <xc.h>
#include <stdint.h>
#include "SPI.h"
#include "LCD8B.h"
//*****************************************************************************
// Definición de variables
//*****************************************************************************
#define _XTAL_FREQ 8000000
#define RS RE1
#define EN RE2
uint8_t adcVal, contador, slave2;
//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);

//*****************************************************************************
// Código Principal
//*****************************************************************************
void main(void) {
    setup();
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){
       PORTCbits.RC1 = 0;       //Slave Select
       __delay_ms(1);
       
       spiWrite(1);
       adcVal = spiRead();       //aqui leo ADC
       //__delay_ms(1);
       //***********************************************************************
       char voltaje[3];
        voltaje[0]=adcVal/100+48;
        voltaje[1]=(adcVal/10)%10+48;
        voltaje[2]=adcVal%10+48;
        Lcd_Clear();
        Lcd_Set_Cursor(1,1);
        Lcd_Write_String("P1:");
        Lcd_Set_Cursor(2,1);
        Lcd_Write_Char(voltaje[0]);
        Lcd_Write_Char(voltaje[1]);
        Lcd_Write_Char(voltaje[2]);
        __delay_ms(100);
       //***********************************************************************
       
       spiWrite(2);
       contador = spiRead();       //aqui leo contador
       //***********************************************************************
       char count[3];
        count[0]=contador/100+48;
        count[1]=(contador/10)%10+48;
        count[2]=contador%10+48;
        Lcd_Set_Cursor(1,7);
        Lcd_Write_String("Con:");
        Lcd_Set_Cursor(2,7);
        Lcd_Write_Char(count[0]);
        Lcd_Write_Char(count[1]);
        Lcd_Write_Char(count[2]);
        //__delay_ms(500);
       //***********************************************************************
       __delay_ms(100);
       __delay_ms(1);
       PORTCbits.RC1 = 1;       //Slave Deselect 
       
       
       
       PORTCbits.RC2 = 0;       //Slave Select
       __delay_ms(1);
       
       spiWrite(1);
       slave2 = spiRead();       //ALMACENAR EN UNA VARIABLE
       //***********************************************************************
       char VOS[3];
        VOS[0]=slave2/100+48;
        VOS[1]=(slave2/10)%10+48;
        VOS[2]=slave2%10+48;
        Lcd_Set_Cursor(1,14);
        Lcd_Write_String("P2:");
        Lcd_Set_Cursor(2,14);
        Lcd_Write_Char(VOS[0]);
        Lcd_Write_Char(VOS[1]);
        Lcd_Write_Char(VOS[2]);
        __delay_ms(500);
       //***********************************************************************
       
       __delay_ms(1);
       PORTCbits.RC2 = 1;       //Slave Deselect 
       
       //__delay_ms(250);
       //PORTB++;
    }
    return;
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    TRISC1 = 0;
    TRISC2 = 0;
    TRISA = 0;
    TRISB = 0;
    TRISD = 0;
    PORTB = 0;
    PORTD = 0;
    TRISE = 0x00;
    PORTCbits.RC2 = 1;
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    Lcd_Init();
}