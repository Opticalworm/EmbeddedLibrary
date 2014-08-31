///////////////////////////////////////////////////////////////////////////
/// \file pid.c
/// \brief interface layer for controlling the heaters
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
///////////////////////////////////////////////////////////////////////////
#include "mbed.h"
#include "pid.h"

///////////////////////////////////////////////////////////////////////////
/// \brief Defines the sampling constant used by the PID: ie. dt 
/// (See Heater interrupt)
///////////////////////////////////////////////////////////////////////////
const float SamplingRateMs = 0.01;
///////////////////////////////////////////////////////////////////////////
/// \brief Defines the maximun the PID can set the driving PWM
///////////////////////////////////////////////////////////////////////////
const float MaximunOuput = 10.0;

///////////////////////////////////////////////////////////////////////////
/// \brief set the limit to the output
///////////////////////////////////////////////////////////////////////////
const float LimitOuput = 5;

///////////////////////////////////////////////////////////////////////////
/// \brief Defines the minimum the PID can set the driving PWM
///////////////////////////////////////////////////////////////////////////
const float MinimumOuput = 0;

///////////////////////////////////////////////////////////////////////////
/// \brief Setup up the PID controller
///////////////////////////////////////////////////////////////////////////
void PID_Init(void)
{

}

///////////////////////////////////////////////////////////////////////////
/// \brief PID process controller. re-entry
/// \param pidParameter the pid parameter where the funciton stores is last
/// know error and also hold the PID charateristics
/// \param targetValue the target value to which the PID will aim for
/// \param actualValue the reading from the temperature sensor
///
/// \return return heater PWM value
///////////////////////////////////////////////////////////////////////////
float PID_Process(PIDType *pidParameter, const float targetValue, const float actualValue)
{
    float Result = 0;
    
    // Calculate the current error between our target value and 
    // whatthe sensor are reading
    float CurrentError = ((float)targetValue - actualValue);
    // Take the diferent between last error and current error
    float DerivedError = ((CurrentError - pidParameter->LastError)/SamplingRateMs);
    // Take the sum of the last error
    pidParameter->IntergralError += (CurrentError*SamplingRateMs);
    
    // Calculate the new adjust PWM
    Result =    (pidParameter->P * CurrentError) +
                (pidParameter->I * pidParameter->IntergralError) +
                (pidParameter->D * DerivedError);

    // Now update the last error
    pidParameter->LastError = CurrentError;
    
    // Lets make sure that the Result are within limits
    if(Result > LimitOuput)
    {
        Result = LimitOuput;
       //return  1;//MaximunOuput;
    }
    else if(Result <= MinimumOuput)
    {
        Result = MinimumOuput;
        
    }
    
    //Return the new PWM value
    return (Result/MaximunOuput);
}
