#include <stdint.h>   
#include <pic32mx.h>  
#include "flappy.h"

//TAKEN FROM THE WORK IN LABB 3

int getsw( void )
{
    return (PORTD >> 8 & 0xf);
}
 int getbtns(void) 
{
    return (PORTD >> 5 & 0x7);
}