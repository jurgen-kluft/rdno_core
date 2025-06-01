#include "rdno_core/c_serial.h"

#ifdef TARGET_ESP32

#include "Arduino.h"

namespace ncore
{
    namespace nserial
    {
        s32 Available()
        {
            return Serial.available();
        }

        // // Begin sets the data rate in bits per second (baud) for serial data transmission.
        // // @see: https://www.arduino.cc/reference/en/language/functions/communication/serial/begin/
        // func Begin(baud int) 
        void Begin(nbaud::Enum baud)
        {
            Serial.begin(baud);
        }

        // // Print prints data to the serial port as human-readable ASCII text.
        // // @see: https://www.arduino.cc/reference/en/language/functions/communication/serial/print/
        void Print(const char* val)
        {
            Serial.print(val);
        }

        // // Println prints data to the serial port as human-readable ASCII text followed by a carriage return character (ASCII 13, or '\r') and a newline character (ASCII 10, or '\n').
        // // @see: https://www.arduino.cc/reference/en/language/functions/communication/serial/println/
        void Println(const char* val)
        {
            Serial.println(val);
        }

    }  // namespace nserial
}  // namespace ncore

#else

namespace ncore
{
    namespace nserial
    {
        s32 Available(){ return 0; }

        // // Begin sets the data rate in bits per second (baud) for serial data transmission.
        // // @see: https://www.arduino.cc/reference/en/language/functions/communication/serial/begin/
        // func Begin(baud int) 
        void Begin(nbaud::Enum baud)
        {
            // No operation in simulation
            (void)baud;
        }

        // // Print prints data to the serial port as human-readable ASCII text.
        // // @see: https://www.arduino.cc/reference/en/language/functions/communication/serial/print/
        void Print(const char* val)
        {
            // No operation in simulation
            (void)val;
        }

        // // Println prints data to the serial port as human-readable ASCII text followed by a carriage return character (ASCII 13, or '\r') and a newline character (ASCII 10, or '\n').
        // // @see: https://www.arduino.cc/reference/en/language/functions/communication/serial/println/
        void Println(const char* val)
        {
            // No operation in simulation
            (void)val;
        }

    }  // namespace nserial
}  // namespace ncore

#endif
