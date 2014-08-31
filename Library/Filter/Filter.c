////////////////////////////////////////////////////////////////////////////////
///	\file	Filter.c
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
#include "Filter.h"

////////////////////////////////////////////////////////////////////////////////
///	\brief	Performs the filter routine
///
///	\param state filter profile and paramter
///	\param measurement new value to filter
///
///	\return return the out value. Note this is also stored in state->x
////////////////////////////////////////////////////////////////////////////////
float Filter_Process(FilterParameter *state, float measurement)
{
	//prediction update
	//omit x = x
	state->p = state->p + state->q;

	//measurement update
	state->k = state->p / (state->p + state->r);
	state->x = state->x + state->k * (measurement - state->x);
	state->p = (1 - state->k) * state->p;
  
	return state->x;
}
