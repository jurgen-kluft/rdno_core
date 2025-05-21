#ifndef __RDNO_CORE_SERIAL_H__
#define __RDNO_CORE_SERIAL_H__
#include "rdno_core/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

namespace ncore
{
    namespace nserial
    {
        // @see: https://docs.arduino.cc/language-reference/en/functions/communication/serial
        namespace nbaud
        {
            enum Enum
            {
                Rate300    = 300,
                Rate600    = 600,
                Rate1200   = 1200,
                Rate2400   = 2400,
                Rate4800   = 4800,
                Rate9600   = 9600,
                Rate14400  = 14400,
                Rate19200  = 19200,
                Rate28800  = 28800,
                Rate38400  = 38400,
                Rate57600  = 57600,
                Rate115200 = 115200,
            };
        }

        // // Available gets the number of bytes (characters) available for reading from the serial port.
        // // @see: https://www.arduino.cc/reference/en/language/functions/communication/serial/available/
        s32 available();

        // // Begin sets the data rate in bits per second (baud) for serial data transmission.
        // // @see: https://www.arduino.cc/reference/en/language/functions/communication/serial/begin/
        // func Begin(baud int) 
        void begin(nbaud::Enum baud);

        // // Print prints data to the serial port as human-readable ASCII text.
        // // @see: https://www.arduino.cc/reference/en/language/functions/communication/serial/print/
        void print(const char* val);

        // // Println prints data to the serial port as human-readable ASCII text followed by a carriage return character (ASCII 13, or '\r') and a newline character (ASCII 10, or '\n').
        // // @see: https://www.arduino.cc/reference/en/language/functions/communication/serial/println/
        void println(const char* val);

    }  // namespace nserial
}  // namespace ncore

#endif  // __RDNO_CORE_SERIAL_H__
