// Part of dvi2bitmap.
// Copyright 1999, 2000 Council for the Central Laboratory of the Research Councils.
// See file LICENCE for conditions.
//
// $Id$

#ifndef DVI2BITMAP_HEADER_READ
#define DVI2BITMAP_HEADER_READ 1
#include <string>
using namespace std;

#include "DviError.h"

typedef unsigned char Byte;
const char path_separator = '/';

// For some reason which I don't understand (am I not including a
// required library?), GCC streambuf.h and iostream.h complain about NULL
// being defined wrongly (as void*), unless I define it to be zero here.
//
// Hmm no, this isn't quite the problem.  If I include, for example, <string>, 
// before I include <iostream>, the problem goes away...
//
// I think this must just be a small GCC STL buglet.
#ifndef NULL
#define NULL 0
#endif

#endif // #ifdef DVI2BITMAP_HEADER_READ

