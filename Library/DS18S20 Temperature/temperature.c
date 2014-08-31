/////////////////////////////////////////////////////////////////////////
///	\file	temperature.c
///	\brief interface code for 1 wire Temperature sensor (DS18S20). This was written
///		to talk to a single sensor and not multiple. 
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
#include "common.h"
#include "temperature.h"
#include "onewire.h"

/////////////////////////////////////////////////////////////////////////
///	\brief	DS18S20 internal ROM and function commands.
///	for a more extensive information on the DS18S20 commands then see the
///	device datasheet.
/////////////////////////////////////////////////////////////////////////
enum DS18S20Enum{
	// ROM Commands
	DS18S20_SEARCH_ROM = 0xF0, 			///< ROM command. When searching for connecting devices.
	DS18S20_READ_ROM = 0x33,			///< ROM Command. Reads device 64bit id
	DS18S20_MATCH_ROM = 0x55,			///< ROM Command. The match ROM command followed by a 64-bit ROM code sequence allows the bus master to address a specific slave device on a multidrop or single-drop bus
	DS18S20_SKIP_ROM = 0xCC,			///< ROM Command. The master can use this command to address all devices on the bus simultaneously without sending out any ROM code information
	DS18S20_ALARM_SEARCH = 0xEC,		///< ROM Command. The operation of this command is identical to the operation of the Search ROM command except that only slaves with a set alarm flag will respond
	// Function command
	DS18S20_CONVERT_T = 0x44,			///< Function command. initiates temperature conversion
	DS18S20_WRITE_SCRATCHPAD = 0x4E,	///< Function command. Read the entire scratchpad including the CRC byte.
	DS18S20_READ_SCRATCHPAD = 0xBE,		///< Function command. Write data into scratchpad byte 2, 3 (Th and Tl)
	DS18S20_COPY_SCRATCHPAD = 0x48,		///< Function command. Copies Th and Tl data from the scratchpad to EEPROM
	DS18S20_RECAL_E_2 = 0xB8,			///< Function command. Recalls Th and Tl data from EEPROM to the scratchpad
	DS18S20_READ_POWER_SUPPLY = 0xB4	///< Function command. Signals DS18S20 power supply mode to the master.
	
};

/////////////////////////////////////////////////////////////////////////
///	\brief	Initialize the temperature sensor
/////////////////////////////////////////////////////////////////////////
void Temperature_Init(void)
{
	OneWire_Init();
}

/////////////////////////////////////////////////////////////////////////
///	\brief	sensor serial number
///
///		format: <BYTE 0> device family code
///				<BYTE 1 - 6> Serial Num. Byte 1  is MSB
///				<BYTE 7> CheckSum.
///	
///	\param destination pointer to return the device 8 byte serial number. 
///	\return FALSE on success else TRUE error
/////////////////////////////////////////////////////////////////////////
TemperatureRespoceEnum Temperature_GetSerialNumber(uint8_t * destination)
{
	uint8_t Index = 0;
	
	if(OneWire_Reset() || OneWire_Write(DS18S20_READ_ROM))
	{ 
		
		return TMP_Error;
	}
	// Read sensor scratch pad
	for(Index = 0; Index < SERIAL_LENGTH; Index++)
	{
		 //get the array of data from sensor
		if(OneWire_Read(&destination[Index]))
		{ 
			return TMP_Error;
		}		
	}
	
	// verify CRC. if it return zero then we have the correct
	// CRC else its a mistmatch
	if(!OneWire_CalculateCRC(&destination[0], SERIAL_LENGTH))
	{
		return TMP_Success;
	}
	
	return TMP_Error;
}


/////////////////////////////////////////////////////////////////////////
///	\brief	Non-Blocking. Request all connected devices to start
///		converting temperature.
///
///	\return FALSE on success else TRUE error
/////////////////////////////////////////////////////////////////////////
TemperatureRespoceEnum Temperature_RequestConvertion(void)
{
	// Reset device, Skip ROM identification and request temperature conversion
	if(OneWire_Reset() || OneWire_Write(DS18S20_SKIP_ROM) || OneWire_Write(DS18S20_CONVERT_T))
	{ 
		
		return TMP_Error;
	}
	
	return TMP_Success;
}
	
/////////////////////////////////////////////////////////////////////////
///	\brief	a non-blocking method to Get Temperature from sensor.
///		You will need to call Temperature_RequestConvertion() to invoke
///		tempreature convertion.
///
///	\param temperature pointer to return the sensor temperature
///	\return TMP_Busy = Stil processing, TMP_Success or TMP_Error
///
///	\note the DS18S20 take 750ms to convert temperature.
///	\sa TemperatureRespoceEnum Temperature_RequestConvertion
/////////////////////////////////////////////////////////////////////////
TemperatureRespoceEnum Temperature_NonBlockingRead(float *temperature)
{
	uint8_t ReadData[9];
	uint8_t *ReadDataPointer;
	TypeCon dataTemp;
	uint8_t CalculatedCRC = 0;
  uint8_t Index = 0;


	// The sensor return zero if its still processing data otherwise we can continue
	if(OneWire_Read(&Index))
	{
		return TMP_Error; // Error. the device didn't respond. 
	}
	
	if(!Index)
	{
		return TMP_Busy;
	}
		

	// Reset device, Skip ROM identification and request temperature conversion
	if(OneWire_Reset() || OneWire_Write(DS18S20_SKIP_ROM) || OneWire_Write(DS18S20_READ_SCRATCHPAD))
	{ 
		
		return TMP_Error; //error
	}
		
	// Reset pointer
	ReadDataPointer = &ReadData[0];
	  
	// Read sensor scratch pad
	for(Index = 0; Index < 9; Index++)
	{
		 //get the array of data from sensor
		if(OneWire_Read(ReadDataPointer))
		{ 
			return TMP_Error; //error
		}		
		// Increment pointer
		ReadDataPointer++;
	}
	  
	// calculate the CRC value
	CalculatedCRC  = OneWire_CalculateCRC(&ReadData[0], 8);
	
	// Convert data to obtain the higher resolution we need
	ReadData[0] = ReadData[0] >> 1;
			
	//is it negative?
	if(ReadData[1])
	{
		ReadData[0] -= 0x80;
	}

	dataTemp._uint8[0] = ReadData[0];

	// Calculate temperature
	*temperature = ReadData[7] - ReadData[6];
	*temperature /= ReadData[7];
	*temperature = (dataTemp._int8[0] - 0.25) + *temperature;
	
	// Do the CRC match?
	if(CalculatedCRC == ReadData[8])
	{
		return TMP_Success;
	}

	return TMP_Error;
	
}

/////////////////////////////////////////////////////////////////////////
///	\brief	This function will request and get temperature from sensor.
///		this is a blocking function.
///
///	\param temperature pointer to return the sensor temperature
///	\return TMP_Success or TMP_Error
///
///	\note the DS18S20 take 750ms to convert temperature.
/////////////////////////////////////////////////////////////////////////
TemperatureRespoceEnum Temperature_BlockingRead(float *temperature)
{	
	
	TemperatureRespoceEnum ReturnState;
	
	if(Temperature_RequestConvertion()) // Request temperature from the sensors
	{
		return TMP_Error;
	}

	for( ;; ) // Wait until the sensor has converted the temperature
	{
		ReturnState = Temperature_NonBlockingRead(temperature);
		
		if(TMP_Busy != ReturnState)
		{
			return ReturnState;
		}
		
	}
}
