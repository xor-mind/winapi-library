#ifndef PREREQUISITES_H
#define PREREQUISITES_H

#ifndef NULL 
#define NULL 0
#endif

// define the real number values to be used
// default to use 'float' unless precompiler option set
#if DOUBLE_PRECISION == 1
    typedef double Real;
#else
    typedef float Real;
#endif

typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned int   uint;
typedef unsigned long  ulong;

#endif