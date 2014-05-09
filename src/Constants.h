
#ifndef CONST_H_
#define CONST_H_

// parity
enum
{
	SP_NOPARITY        =    0,
	SP_ODDPARITY       =    1,
	SP_EVENPARITY      =    2,
	SP_MARKPARITY      =    3,
	SP_SPACEPARITY     =    4,
};

// stopbits
enum 
{
	SP_ONESTOPBIT      =    0,
	SP_ONE5STOPBITS    =    1,
	SP_TWOSTOPBITS     =    2,
};

// baudrate
enum
{
	SP_CBR_110          =   110,
	SP_CBR_300          =   300,
	SP_CBR_600          =   600,
	SP_CBR_1200         =   1200,
	SP_CBR_2400         =   2400,
	SP_CBR_4800         =   4800,
	SP_CBR_9600         =   9600,
	SP_CBR_14400        =   14400,
	SP_CBR_19200        =   19200,
	SP_CBR_38400        =   38400,
	SP_CBR_56000        =   56000,
	SP_CBR_57600        =   57600,
	SP_CBR_115200       =   115200,
	SP_CBR_128000       =   128000,
	SP_CBR_256000       =   256000,
};


// event
enum
{
	SP_EV_RXCHAR         =  0x0001,  // Any Character received
	SP_EV_RXFLAG         =  0x0002,  // Received certain character
	SP_EV_TXEMPTY        =  0x0004,  // Transmitt Queue Empty
	SP_EV_CTS            =  0x0008,  // CTS changed state
	SP_EV_DSR            =  0x0010,  // DSR changed state
	SP_EV_RLSD           =  0x0020,  // RLSD changed state
	SP_EV_BREAK          =  0x0040,  // BREAK received
	SP_EV_ERR            =  0x0080,  // Line status error occurred
	SP_EV_RING           =  0x0100,  // Ring signal detected
	SP_EV_PERR           =  0x0200,  // Printer error occured
	SP_EV_RX80FULL       =  0x0400,  // Receive buffer is 80 percent full
	SP_EV_EVENT1         =  0x0800,  // Provider specific event 1
	SP_EV_EVENT2         =  0x1000,  // Provider specific event 2
};


// error
enum
{
	SP_CE_FRAME            = 0x0008,  // Receive Framing error
	SP_CE_OVERRUN          = 0x0002,  // Receive Overrun Error
	SP_CE_RXPARITY         = 0x0004,  // Receive Parity Error
};

// flow cotrol 
enum 
{
	SERIAL_DTR_CONTROL	   = 0x01, 
	SERIAL_DTR_HANDSHAKE   = 0x02,
	SERIAL_CTS_HANDSHAKE   = 0x08,
	SERIAL_DCD_HANDSHAKE   = 0x20, 
	SERIAL_DSR_HANDSHAKE   = 0x10,
	SERIAL_DSR_SENSITIVITY = 0x40, 
	SERIAL_ERROR_ABORT     = 0x80000000 
};

enum 
{
	SERIAL_AUTO_TRANSMIT   = 0x01, 
	SERIAL_AUTO_RECEIVE    = 0x02, 
	SERIAL_ERROR_CHAR	   = 0x04, 
	SERIAL_NULL_STRIPPING  = 0x08, 
	SERIAL_BREAK_CHAR	   = 0x10, 
	SERIAL_RTS_CONTROL	   = 0x40, 
	SERIAL_RTS_HANDSHAKE   = 0x80 
};

#endif

