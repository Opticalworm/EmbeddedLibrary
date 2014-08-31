///////////////////////////////////////////////////////////////////////////
/// \file pid.h
/// \brief PID header
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
#ifndef     __PID_H__
#define     __PID_H__


    ///////////////////////////////////////////////////////////////////////////
    /// \brief defien PID data type
    ///////////////////////////////////////////////////////////////////////////
    typedef struct
    {
        float P; 				///< P gain
        float I;				///< I gain
        float D;				///< P gain
        float LastError;		///< Last calculated  Error. Internal use only
        float IntergralError; 	///< Intergral error. Internal use only
        
    }PIDType;
    
    void PID_Init(void);
    float PID_Process(PIDType *pidParameter, const float targetValue, const float actualValue);
    
    
#endif
