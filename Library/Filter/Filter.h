////////////////////////////////////////////////////////////////////////////////
///	\file	Filter.h
///
///	\brief Kalman filter. You can find the original source code was developed by Marcus and
///	the original post can be found here: 
///	http://interactive-matter.eu/blog/2009/12/18/filtering-sensor-data-with-a-kalman-filter/
/// I've changed the original code so that it also returns the filter value. Apart from that
///	its virtually the same to the original source.
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
////////////////////////////////////////////////////////////////////////////////
#ifndef __FILTER_H__
#define __FILTER_H__

		////////////////////////////////////////////////////////////////////////////////
		///	\brief	Filter parameters
		////////////////////////////////////////////////////////////////////////////////
		typedef struct
		{
			float q; //process noise covariance. Controls the strength of the filter. The smaller this value is the stringer it is.
			float r; //measurement noise covariance. Controls the filter reponse time. The smaller this number is set the more resposive it is to larger changes but it will also allow more noise through.
			float x; //Initial value and also the output value.
			float p; //Internal use leave as zero. estimation error covariance
			float k; //Internal use leave as zero. kalman gain. Its Internal
			
		} FilterParameter;
		
		float Filter_Process(FilterParameter *state, float measurement);
		
#endif
