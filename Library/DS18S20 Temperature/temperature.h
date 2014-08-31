////////////////////////////////////////////////////////////////////////////////
/// \file	temperature.h
///	\brief this is an interface layer to a 1wire temperature sensor based
///	on the Maxim document AN214.
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
///
///
///	Get Temperature Code Example (Non-Blocking):
///	\code
///	#include "temperature.h"
///
///
///	void main(void)
///	{
///		TemperatureRespoceEnum SensorResult;
///		float Temperature = 0;
///
///		Temperature_Init();
///
///		Temperature_RequestConvertion(); // Invoke convertion before we start to monitor the sensor
///
///		for( ;; ) // program loop
///		{
///			SensorResult = Temperature_NonBlockingRead(&Tempreature)
///
///			if(TMP_Busy == SensorResult)
///				{
///					// Device is still converting the temperature
///				}
///			else
///				{
///					Temperature_RequestConvertion(); // You need to invoke another convertion
///				}
///	
///			if(TMP_Success == SensorResult)
///				{
///					printf("%0.4f\r\n",Tempreature);				
///				}
///
///			// Process something else
///			YourProcess();
///		}
///	}
///	\endcode
///
///
///	Get Temperature Code Example (Blocking):
///	\code
///	#include "temperature.h"
///
///
///	void main(void)
///	{
///		float Temperature = 0;
///
///		Temperature_Init();
///
///		for( ;; )
///		{
///			if(!Temperature_BlockingRead(&Tempreature))
///				{
///						printf("%0.4f\r\n",Tempreature);
///				}
///			else
///				{
///					printf("Error\r\n");
///				}
///		}
///	}
///	\endcode 
///
////////////////////////////////////////////////////////////////////////////////
#ifndef __TEMPERATURE_H__
#define __TEMPERATURE_H__
	
	
	/////////////////////////////////////////////////////////////////////////
	///	\brief	Defines how many bytes to read from the device for the 
	///	Unique ID
	/////////////////////////////////////////////////////////////////////////
	#define SERIAL_LENGTH 8

	////////////////////////////////////////////////////////////////////////////////
	///	\brief This are the function enum types
	////////////////////////////////////////////////////////////////////////////////
	typedef enum{
		TMP_Success = 0,			///< comminucation was a success
		TMP_Error,						///< error was found
		TMP_Busy							///< Device is still busy converting data
	} TemperatureRespoceEnum;
	
	
	void Temperature_Init(void);
	TemperatureRespoceEnum Temperature_GetSerialNumber(uint8_t * destination);
	TemperatureRespoceEnum Temperature_BlockingRead(float *destination);
	TemperatureRespoceEnum Temperature_RequestConvertion(void);
	TemperatureRespoceEnum Temperature_NonBlockingRead(float *temperature);

#endif
