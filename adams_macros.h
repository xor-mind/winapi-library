#ifndef ADAMS_MACROS_H
#define ADAMS_MACROS_H

// unicode/utf defines
#ifdef UNICODE
#define tcout std::wcout 
#endif
#ifndef UNICODE
#define tcout  
#endif

// output
#define tsay tcout << 
#define say  std::cout <<
#define stop << std::endl;
//#define and <<
#define chill std::cin.get();
#define endline std::cout << std::endl;
#define derp std::cin.ignore(10, '\n');

// code quantification
#define Twice(x) x; x
#define Thrice(x) Twice(x); x
#define Quice(x) Thrice(x); x
#define Eighty(x) for (int i = 0; i<80; ++i) { x; }
#define Noice(x,n) for (int i = 0; i < n; ++i) { x; }


// Y_TUPLE_SIZE return the argument count for __VA_ARGS__
#define NARG(...) Y_TUPLE_SIZE_II((Y_TUPLE_SIZE_PREFIX_ ## __VA_ARGS__ ## _Y_TUPLE_SIZE_POSTFIX,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0))
#define Y_TUPLE_SIZE_II(__args) Y_TUPLE_SIZE_I __args
#define Y_TUPLE_SIZE_PREFIX__Y_TUPLE_SIZE_POSTFIX ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,0
#define Y_TUPLE_SIZE_I(__p0,__p1,__p2,__p3,__p4,__p5,__p6,__p7,__p8,__p9,__p10,__p11,__p12,__p13,__p14,__p15,__p16,__p17,__p18,__p19,__p20,__p21,__p22,__p23,__p24,__p25,__p26,__p27,__p28,__p29,__p30,__p31,__n,...) __n



#endif