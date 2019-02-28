#pragma once

#include <cmath>
#include <ledAuxiliarFunctions.hpp>


namespace Conversions
{
    #define FIELD_HEIGHT 9000
    #define FIELD_WIDHT 6000

    int decimal_octal(int n) /* Function to convert decimal to octal */
    {
        int rem, i=1, octal=0;
        while (n!=0)
        {
            rem=n%8;
            n/=8;
            octal+=rem*i;
            i*=10;
        }
        return octal;
    }

    int octal_decimal(int n) /* Function to convert octal to decimal */
    {
        int decimal=0, i=0, rem;
        while (n!=0)
        {
            rem = n%10;
            n/=10;
            decimal += rem*pow(8,i);
            ++i;
        }
        return decimal;
    }

    void MoveCenterToCorner(int *x, int *y)
    {
        *x += FIELD_HEIGHT/2;
        *y += FIELD_WIDHT/2;
    }
    void MoveCornerToCenter(int *x, int *y)
    {
        *x -= FIELD_HEIGHT/2;
        *y -= FIELD_WIDHT/2;
    }

    //###################FIELD#######################
    //      ________________________________  x = 9000
    //     |           Transmiter           |
    //     |  Quadrant 8   |  Quadrant 7    |
    //     |               |                |
    //     |_______________|________________| x = 7750
    //     |               |                |
    //     |  Quadrant 6   |  Quadrant 5    |
    //     |               |                |
    //     |_______________|________________| x = 4500
    //     |               |                |
    //     |  Quadrant 4   |  Quadrant 3    |
    //     |               |                |
    //     |_______________|________________| x = 2250
    //     |               |                | ^
    //     |  Quadrant 2   |  Quadrant 1    | | X
    //     |               |                | |
    //     |___________Reciever_____________| |
    // y = 6000         y = 3000    <--------- (0,0)
    //                                 Y
    //################################################

    void ConvertToQuadrant(int *x, int *y, int *quadrant)
    {
        if(*y < FIELD_WIDHT/2)
        {
            if(*x < FIELD_HEIGHT/4)
            {
                *quadrant = 1;
            }
            else if(*x < FIELD_HEIGHT/2)
            {
                *quadrant = 3;
                *x -= FIELD_HEIGHT/4;
            }
            else if(*x < FIELD_HEIGHT*3/4)
            {
                *quadrant = 5;
                *x -= FIELD_HEIGHT/2;
            }
            else
            {
                *quadrant = 7;
                *x -= FIELD_HEIGHT*3/4;
            }
        }
        else
        {
            *y -= FIELD_WIDHT/2;
            if(*x < FIELD_HEIGHT/4)
            {
                *quadrant = 2;
            }
            else if(*x < FIELD_HEIGHT/2)
            {
                *quadrant = 4;
                *x -= FIELD_HEIGHT/4;
            }
            else if(*x < FIELD_HEIGHT*3/4)
            {
                *quadrant = 6;
                *x -= FIELD_HEIGHT/2;
            }
            else
            {
                *quadrant = 8;
                *x -= FIELD_HEIGHT*3/4;
            }
        }
    }

    void ConvertFromQuadrant(int *x, int *y, int quadrant)
    {
        switch (quadrant)
        {
        case 1:
            break;
        case 2:
            *y += FIELD_WIDHT/2;
            break;
        case 3:
            *x += FIELD_HEIGHT/4;
            break;
        case 4:
            *x += FIELD_HEIGHT/4;
            *y += FIELD_WIDHT/2;
            break;
        case 5:
            *x += FIELD_HEIGHT/2;
            break;
        case 6:
            *x += FIELD_HEIGHT/2;
            *y += FIELD_WIDHT/2;
            break;
        case 7:
            *x += FIELD_HEIGHT*3/4;
            break;
        case 8:
            *x += FIELD_HEIGHT*3/4;
            *y += FIELD_WIDHT/2;
            break;
        default:
            break;
        }
    }

    enum Color { OFF = 0, RED , GREEN, BLUE};

    void transmitLED(int digit)
    {
        
        Command command;
        
        
        switch(digit)
        {
            case 1:
                command.leds = LEDAux::changeColor(RED, RED, RED);
                break;
            case 2:
                command.leds = LEDAux::changeColor(RED, RED, GREEN);
                break;
            case 3:
                command.leds = LEDAux::changeColor(RED, GREEN, RED);
                break;
            case 4: 
                command.leds = LEDAux::changeColor(RED, GREEN, GREEN);
                break;
            case 5:
                command.leds = LEDAux::changeColor(GREEN, RED, RED);
                break;
            case 6:
                command.leds = LEDAux::changeColor(GREEN, RED, GREEN);
                break;
            case 7:
                command.leds = LEDAux::changeColor(GREEN, GREEN, RED);
                break;
            case 8:
                command.leds = LEDAux::changeColor(GREEN, GREEN, GREEN);
                break;

        }
        MotionAux::SendCommand(command);

    }

    void turnOFF()
    {   
        Command command;
        command.leds = LEDAux::changeColor(OFF, OFF, OFF);
        MotionAux::SendCommand(command);
    }
}


