/////////////////////////////////////////////////////////////////////////
///	\file	onewire.c
///	\brief portable one wire layer that use MCU serial peripheral to implement
///			the signal layer.
///
///	\section OneWireLayer One wire interfac layer
///
///	Author: Ronald Alexander Nobrega De Sousa (Opticalworm)
///	Website: www.HashDefineElectronics.com
///
///	Licences:
///
///		Copyright (c) 2014 Ronald Alexander Nobrega De Sousa
///
///		Permission is hereby granted, free of charge, to any person obtaining a copy
///		of this software and associated documentation files (the "Software"), to deal
///		in the Software without restriction, including without limitation the rights
///		to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
///		copies of the Software, and to permit persons to whom the Software is
///		furnished to do so, subject to the following conditions:
///
///		The above copyright notice and this permission notice shall be included in
///		all copies or substantial portions of the Software.
///	
///		THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
///		IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
///		FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
///		AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
///		LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
///		OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
///		THE SOFTWARE.
/////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include "common.h"
#include "uart.h"

/////////////////////////////////////////////////////////////////////////
///	\brief	This is the data used to reset the 1 wire node
/////////////////////////////////////////////////////////////////////////
static const uint_fast8_t ResetData = 0xF0;

/////////////////////////////////////////////////////////////////////////
///	\brief	Define the true state
/////////////////////////////////////////////////////////////////////////
static const uint_fast8_t OneWireTrue = 0XFF;

/////////////////////////////////////////////////////////////////////////
///	\brief	Define the false state
/////////////////////////////////////////////////////////////////////////
static const uint_fast8_t OneWireFalse = 0X00;

/////////////////////////////////////////////////////////////////////////
///	\brief	Baudrate for 9600
/////////////////////////////////////////////////////////////////////////
static const uint_fast16_t Baudrate9600 = 9600;

/////////////////////////////////////////////////////////////////////////
///	\brief	Baudrate for 115200
/////////////////////////////////////////////////////////////////////////
static const uint_fast16_t Baudrate115200 = 115200;

/////////////////////////////////////////////////////////////////////////
///	\brief	Defines how many bytes in the bit state
/////////////////////////////////////////////////////////////////////////
static const uint_fast8_t OneWireBitLength = 0x08;

/////////////////////////////////////////////////////////////////////////
///	\brief	Bit mask
/////////////////////////////////////////////////////////////////////////
static const uint_fast8_t OneWireBitMask = 0x01;

/////////////////////////////////////////////////////////////////////////
///	\brief	Read one byte from device
///
///	\param data pointer to return the read byte
///	\return false on success else true
/////////////////////////////////////////////////////////////////////////
int_fast8_t OneWire_Read(uint8_t *data)
{
    uint8_t Index = OneWireBitLength;
    uint8_t ReturnValue = 0;
    uint8_t Temp;
    
		Uart_Setbaud(Baudrate115200);
	
    while(Index)
    {
        //write a true
        Uart_WriteByte(OneWireTrue);

        //Dummy Read
        if(Uart_ReadByte(&Temp))
        {
					// Error while waiting for data to be received
					return TRUE;
				}
		
        ReturnValue = ReturnValue >> 1; //ShiftData
				
        if( Temp == OneWireTrue)
        {
            ReturnValue += 0x80;
        }
		
        //Count Away
        Index--;
    }

    *data = ReturnValue;
	
    return FALSE;
}

/////////////////////////////////////////////////////////////////////////
///	\brief	Writes one byte
///
/// \param source byte to write
/////////////////////////////////////////////////////////////////////////
int_fast8_t OneWire_Write(const uint8_t source)
{
	uint8_t Index = OneWireBitLength;
	uint8_t Temp = source;
	uint8_t Dummy = 0;
	
	Uart_Setbaud(Baudrate115200);
	
    while(Index) // Read one byte
    {
				//Wait here until there space to send the next bit of data
				while(!Uart_WriteBusy()) ;
			
        if(Temp & OneWireBitMask)
        {
            //write a true
            Uart_WriteByte(OneWireTrue);
        }
        else
        {
            //write a false
            Uart_WriteByte(OneWireFalse);
        }
        
        //Dummy Read
        if(Uart_ReadByte(&Dummy))
        {
					// Error no data was received
					return TRUE;
				}

        Temp = Temp >> 1; //Shift over to next byte
        Index--; //decrement count
    }
    
    return FALSE;    
}

/////////////////////////////////////////////////////////////////////////
///	\brief	Reset the connected 1 wire device
/////////////////////////////////////////////////////////////////////////
uint8_t OneWire_Reset(void)
{
	uint8_t Data = 0;
	Uart_Setbaud(Baudrate9600);
	
	Uart_WriteByte(ResetData); //Send the reset command
	
	//wait for data to be received
	if(Uart_ReadByte(&Data))
	{
		return TRUE; // Error while waiting for data to be received
	}
	
	// check if device is in the network
	if(Data == ResetData)
	{
		return TRUE; //Error device not found
	}
	
	return FALSE; // Device is available
}

/////////////////////////////////////////////////////////////////////////
///	\brief	initialize one wire interface
/////////////////////////////////////////////////////////////////////////
void OneWire_Init(void)
{
	Uart_Init(Baudrate9600);
	OneWire_Reset();
}

/////////////////////////////////////////////////////////////////////////
///	\brief	Calculates 1-wire CRC with the from the given array.
/// 	If source array contains the CRC then expect the calculated CRC 
///		to be zero.
///
///	\param source pointer to the byte array
///	\param length how many byte to process
///
///	\return returned the calculated CRC
///
///	\note the original source was made 
///		by clawson @ http://www.avrfreaks.net/index.php?name=PNphpBB2&file=printview&t=85318
/////////////////////////////////////////////////////////////////////////
uint8_t OneWire_CalculateCRC(uint8_t * source, uint8_t length) 
{ 
	uint8_t BitIndex;
	uint8_t CRC = 0;

	while(length)
	{
		CRC = CRC ^ *source;
		
		for (BitIndex = 0; BitIndex < 8; BitIndex++) 
		{ 
			if (CRC & 0x01) 
			{
				CRC = (CRC >> 1) ^ 0x8C; 
			}
			else
			{
				CRC >>= 1; 
			}
		} 
		
		source++;
		length--;
	}
	return CRC; 
}
