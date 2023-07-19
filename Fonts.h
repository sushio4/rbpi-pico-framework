// File to store fonts for project
// https://github.com/gavinlyonsrepo/NOKIA_5110_RPI

#pragma once

// (1) default (FUll ASCII)
// (2) thick (NO LOWERCASE)
// (3) seven segment
// (4) wide (NO LOWERCASE)
// (5) tiny
// (6) homespun
// (7) big numbers (NUMBERS ONLY )
// (8) med numbers (NUMBERS ONLY )

extern const unsigned char * pFontDefaultptr;
extern const unsigned char * pFontThickptr;
extern const unsigned char * pFontSevenSegptr;
extern const unsigned char * pFontWideptr;
extern const  unsigned char * pFontTinyptr;
extern const unsigned char * pFontHomeSpunptr;
//extern const uint8_t (* pFontBigNumptr)[64];
//extern const uint8_t (* pFontMedNumptr)[32];

const uint8_t fontWidthArray[]  = {5, 7,    4,    8,    3,    7,    16,   16};
const uint8_t fontHeightArray[] = {8, 8,    8,    8,    8,    8,    32,   16};
const uint8_t fontOffsetArray[] = {0, 0x20, 0x20, 0x20, 0x20, 0x20, 0x30, 0x30};

const unsigned char* const fontPtrArray[] = {pFontDefaultptr, pFontThickptr, pFontSevenSegptr, pFontWideptr,
											pFontTinyptr, pFontHomeSpunptr};

typedef enum
{
	LCDFontType_Default = 1,
	LCDFontType_Thick = 2, // NO LOWERCASE
	LCDFontType_SevenSeg = 3,
	LCDFontType_Wide = 4, // NO LOWERCASE
	LCDFontType_Tiny = 5,
	LCDFontType_Homespun = 6,
	//LCDFontType_Bignum = 7, // NUMBERS + : . ,one size
	//LCDFontType_Mednum = 8   // NUMBERS + : . ,one size
}LCDFontType_e;

#define getFontWidth(X)  ((uint8_t)(X-1)<6?fontWidthArray[X-1]:5)
#define getFontHeight(X) ((uint8_t)(X-1)<6?fontHeightArray[X-1]:8)
#define getFontOffset(X) ((uint8_t)(X-1)<6?fontOffsetArray[X-1]:0)
#define getFontPtr(X) 	 ((uint8_t)(X-1)<6?fontPtrArray[X-1]:pFontDefaultptr)

/*
typedef enum
{
	LCDFontWidth_3 = 3,
	LCDFontWidth_5 = 5,
	LCDFontWidth_7 = 7,
	LCDFontWidth_4 = 4,
	LCDFontWidth_8 = 8,
	LCDFontWidth_16 = 16
}LCDFontWidth_e; // width of the font in bytes cols.

typedef enum
{
	LCDFontOffset_Extend = 0x00, //   extends ASCII
	LCDFontOffset_Space = 0x20,  // Starts at Space
	LCDFontOffset_Number = 0x30,  // Starts at number '0'
}LCDFontOffset_e; // font offset in the ASCII table

typedef enum
{
	LCDFontHeight_8 = 8,
	LCDFontHeight_16 = 16,
	LCDFontHeight_32 = 32
}LCDFontHeight_e; // height of the font in bits
*/