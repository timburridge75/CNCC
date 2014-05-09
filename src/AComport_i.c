/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Tue Apr 13 18:24:46 2004
 */
/* Compiler settings for G:\ActiveComport\VisualC\AComport.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID IID_IComPort = {0x38014DE4,0x7967,0x4F71,{0xA6,0xC1,0xCD,0xB6,0x2E,0x7D,0xD2,0x69}};


const IID LIBID_ACOMPORTLib = {0x68A2C188,0xA606,0x4841,{0xAE,0x8A,0xD5,0x8C,0x6F,0x6B,0xE5,0x83}};


const CLSID CLSID_ComPort = {0x07ECB42B,0x322A,0x40B9,{0xA8,0xA9,0x38,0x15,0xAF,0x3C,0x4F,0x60}};


#ifdef __cplusplus
}
#endif

