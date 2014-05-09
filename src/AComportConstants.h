
#ifndef _ACOMPORTCONSTANTS_H_
#define _ACOMPORTCONSTANTS_H_


// Parity codes

#define acPARITY_NONE							 0	// No parity bits
#define acPARITY_ODD							 1	// Odd parity
#define acPARITY_EVEN							 2	// Even parity bits
#define acPARITY_MARK							 3	// Mark parity
#define acPARITY_SPACE							 4	// Space parity


// Stopbit codes

#define acSTOPBITS_1							 1	// 1 bit stop bit (default)
#define acSTOPBITS_2							 2	// 2 bit stop bit
#define acSTOPBITS_15							15	// 1.5 bit parity


// Error codes 

#define acERR_SUCCESS							 0  // Success. 
#define acERR_EVALUATIONEXPIRED					 1  // Evaluation period has expired 
#define acERR_INVALIDLICENSE					 2	// Registration code is invalid. 

#define acERR_GENERICERROR					   100	// Generic port Error
#define acERR_INVALIDPARAM					   101	// Invalid port parameter
#define acERR_INVALIDPORT					   102	// Invalid port. Most probably, port does not exist or is in use 
#define acERR_PORTALREADYOPENED				   103	// Unable to perform operation because port it already opened 
#define acERR_OPENPORTFAILED				   104	// Generic open port error 
#define acERR_INITIALIZEPORTFAILED			   105	// Unable to initialize port 
#define acERR_WRITEPORTFAILED				   106	// Unable to write to port 
#define acERR_WRITEPORTDIRECTFAILED			   107	// Unable to write directly to port 
#define acERR_READPORTFAILED				   108	// Unable to read from port 
#define acERR_GETCOMSTATEFAILED				   109	// Unable to retrieve device information 
#define acERR_SETCOMSTATEFAILED				   110	// Unable to change device settings 
#define acERR_PURGECOMFAILED			       111	// Unable to purge the port 
#define acERR_TIMEOUT						   112	// Port timeout


#endif	// _ACOMPORTCONSTANTS_H_