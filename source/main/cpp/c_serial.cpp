#include "rdno_core/c_serial.h"
#include "rdno_core/c_timer.h"

#ifdef TARGET_ESP32

#    include "Arduino.h"

namespace ncore
{
    namespace nserial
    {
        // Begin sets the data rate in bits per second (baud) for serial data transmission.
        // @see: https://www.arduino.cc/reference/en/language/functions/communication/serial/begin/
        void begin(nbaud::Enum baud)
        {
            Serial.begin(baud);
            while (!Serial)
            {
                ntimer::delay(10);  // Wait for 10 milliseconds before retrying
            }
        }

        // Print prints data to the serial port as human-readable ASCII text.
        // @see: https://www.arduino.cc/reference/en/language/functions/communication/serial/print/
        void print(const char* val) { Serial.print(val); }

        void print_U8(const u8 val, bool hex)
        {
            if (hex)
            {
                if (val < 16)
                {
                    Serial.print("0");
                }
                Serial.print(val, HEX);
            }
            else
            {
                Serial.print((unsigned int)val);
            }
        }

        void print_IP(const IPAddress_t address)
        {
            Serial.print((unsigned int)address.m_address[0]);
            Serial.print(".");
            Serial.print((unsigned int)address.m_address[1]);
            Serial.print(".");
            Serial.print((unsigned int)address.m_address[2]);
            Serial.print(".");
            Serial.print((unsigned int)address.m_address[3]);
        }

        void print_Mac(const MACAddress_t address)
        {
            Serial.print((unsigned int)address.m_address[0]);
            Serial.print(":");
            Serial.print((unsigned int)address.m_address[1]);
            Serial.print(":");
            Serial.print((unsigned int)address.m_address[2]);
            Serial.print(":");
            Serial.print((unsigned int)address.m_address[3]);
            Serial.print(":");
            Serial.print((unsigned int)address.m_address[4]);
            Serial.print(":");
            Serial.print((unsigned int)address.m_address[5]);
        }

        // Println prints data to the serial port as human-readable ASCII text followed by a
        // carriage return character (ASCII 13, or '\r') and a newline character (ASCII 10, or '\n').
        // @see: https://www.arduino.cc/reference/en/language/functions/communication/serial/println/
        void println(const char* val) { Serial.println(val); }

        // Write writes data to the serial port.
        // @see: https://www.arduino.cc/reference/en/language/functions/communication
        void write(const byte* data, s32 length)
        {
            // Write the specified data to the given serial port
            Serial.write(data, length);
        }

    }  // namespace nserial

    namespace nserial1
    {
        // Begin sets the data rate in bits per second (baud) for serial data transmission.
        // @see: https://www.arduino.cc/reference/en/language/functions/communication/serial/begin/
        void begin(nbaud::Enum baud, nconfig::Enum config, u8 rxPin, u8 txPin)
        {
#    if SOC_UART_NUM > 1
            uint32_t configValue = SERIAL_8N1;
            switch (config)
            {
                case nconfig::MODE_8N1: configValue = SERIAL_8N1; break;
            }
            if (configValue != 0)
            {
                Serial1.begin(baud, configValue, rxPin, txPin);
            }
#    endif
        }

        s32 available()
        {
#    if SOC_UART_NUM > 1
            return Serial1.available();
#    endif
            return 0;  // Invalid port
        }

        // Print prints data to the serial port as human-readable ASCII text.
        // @see: https://www.arduino.cc/reference/en/language/functions/communication/serial/print/
        void print(const char* val)
        {
#    if SOC_UART_NUM > 1
            Serial1.print(val);
#    endif
        }

        // Println prints data to the serial port as human-readable ASCII text followed by a
        // carriage return character (ASCII 13, or '\r') and a newline character (ASCII 10, or '\n').
        // @see: https://www.arduino.cc/reference/en/language/functions/communication/serial/println/
        void println(const char* val)
        {
#    if SOC_UART_NUM > 1
            Serial1.println(val);
#    endif
        }

        // Write writes data to the serial port.
        // @see: https://www.arduino.cc/reference/en/language/functions/communication
        void write(const byte* data, s32 length)
        {
#    if SOC_UART_NUM > 1
            Serial1.write(data, length);
#    endif
        }

        s32 read_until(char terminator, char* outString, s32 outMaxLength)
        {
            // Read data from the specified serial port until the terminator character is found or the maximum length is reached
            s32 n = 0;
#    if SOC_UART_NUM > 1
            n = Serial1.readBytesUntil(terminator, outString, outMaxLength);
#    endif
            return n;
        }
    }  // namespace nserial1

    namespace nserial2
    {
        // Begin sets the data rate in bits per second (baud) for serial data transmission.
        // @see: https://www.arduino.cc/reference/en/language/functions/communication/serial/begin/
        void begin(nbaud::Enum baud, nconfig::Enum config, u8 rxPin, u8 txPin)
        {
#    if SOC_UART_NUM > 2
            uint32_t configValue = SERIAL_8N1;
            switch (config)
            {
                case nconfig::MODE_8N1: configValue = SERIAL_8N1; break;
            }
            if (configValue != 0)
            {
                Serial2.begin(baud, configValue, rxPin, txPin);
            }
#    endif
        }

        s32 available()
        {
#    if SOC_UART_NUM > 2
            return Serial2.available();
#    else
            return 0;
#    endif
        }

        // Print prints data to the serial port as human-readable ASCII text.
        // @see: https://www.arduino.cc/reference/en/language/functions/communication/serial/print/
        void print(const char* val)
        {
#    if SOC_UART_NUM > 2
            Serial2.print(val);
#    endif
        }

        // Println prints data to the serial port as human-readable ASCII text followed by a
        // carriage return character (ASCII 13, or '\r') and a newline character (ASCII 10, or '\n').
        // @see: https://www.arduino.cc/reference/en/language/functions/communication/serial/println/
        void println(const char* val)
        {
#    if SOC_UART_NUM > 2
            Serial2.println(val);
#    endif
        }

        // Write writes data to the serial port.
        // @see: https://www.arduino.cc/reference/en/language/functions/communication
        void write(const byte* data, s32 length)
        {
#    if SOC_UART_NUM > 2
            Serial2.write(data, length);
#    endif
        }

        s32 read_until(char terminator, char* outString, s32 outMaxLength)
        {
            // Read data from the specified serial port until the terminator character is found or the maximum length is reached
#    if SOC_UART_NUM > 2
            return Serial2.readBytesUntil(terminator, outString, outMaxLength);
#    else
            return 0;
#    endif
        }
    }  // namespace nserial2

}  // namespace ncore

#else

namespace ncore
{
    namespace nserial
    {
        // Begin sets the data rate in bits per second (baud) for serial data transmission.
        // @see: https://www.arduino.cc/reference/en/language/functions/communication/serial/begin/
        // func Begin(baud int)
        void begin(nbaud::Enum baud)
        {
            // No operation in simulation
            (void)baud;
        }

        // Print prints data to the serial port as human-readable ASCII text.
        // @see: https://www.arduino.cc/reference/en/language/functions/communication/serial/print/
        void print(const char* val)
        {
            // No operation in simulation
            (void)val;
        }

        // Println prints data to the serial port as human-readable ASCII text followed by a carriage return character (ASCII 13, or '\r') and a newline character (ASCII 10, or '\n').
        // @see: https://www.arduino.cc/reference/en/language/functions/communication/serial/println/
        void println(const char* val)
        {
            // No operation in simulation
            (void)val;
        }

        // Write writes data to the serial port.
        // @see: https://www.arduino.cc/reference/en/language/functions/communication
        void write(const byte* data, s32 length)
        {
            // No operation in simulation
            (void)data;
            (void)length;
        }

        s32 read_until(char terminator, char* outString, s32 outMaxLength)
        {
            // No operation in simulation
            (void)terminator;
            (void)outString;
            (void)outMaxLength;
            return 0;  // Return 0 bytes read
        }
    }  // namespace nserial
    
    namespace nserial1
    {
        // Begin sets the data rate in bits per second (baud) for serial data transmission.
        // @see: https://www.arduino.cc/reference/en/language/functions/communication/serial/begin/

        void begin(nbaud::Enum baud, nconfig::Enum config, u8 rxPin, u8 txPin)
        {
            (void)baud;
            (void)config;
            (void)rxPin;
            (void)txPin;
        }

        s32 available() { return 0; }

        // Print prints data to the serial port as human-readable ASCII text.
        // @see: https://www.arduino.cc/reference/en/language/functions/communication/serial/print/
        void print(const char* val)
        {
            // No operation in simulation
            (void)val;
        }

        // Println prints data to the serial port as human-readable ASCII text followed by a carriage return character (ASCII 13, or '\r') and a newline character (ASCII 10, or '\n').
        // @see: https://www.arduino.cc/reference/en/language/functions/communication/serial/println/
        void println(const char* val)
        {
            // No operation in simulation
            (void)val;
        }

        // Write writes data to the serial port.
        // @see: https://www.arduino.cc/reference/en/language/functions/communication
        void write(const byte* data, s32 length)
        {
            // No operation in simulation
            (void)data;
            (void)length;
        }

        s32 read_until(char terminator, char* outString, s32 outMaxLength)
        {
            // No operation in simulation
            (void)terminator;
            (void)outString;
            (void)outMaxLength;
            return 0;  // Return 0 bytes read
        }
    }  // namespace nserial1

    namespace nserial2
    {
        // Begin sets the data rate in bits per second (baud) for serial data transmission.
        // @see: https://www.arduino.cc/reference/en/language/functions/communication/serial/begin/

        void begin(nbaud::Enum baud, nconfig::Enum config, u8 rxPin, u8 txPin)
        {
            (void)baud;
            (void)config;
            (void)rxPin;
            (void)txPin;
        }

        s32 available() { return 0; }

        // Print prints data to the serial port as human-readable ASCII text.
        // @see: https://www.arduino.cc/reference/en/language/functions/communication/serial/print/
        void print(const char* val)
        {
            // No operation in simulation
            (void)val;
        }

        // Println prints data to the serial port as human-readable ASCII text followed by a carriage return character (ASCII 13, or '\r') and a newline character (ASCII 10, or '\n').
        // @see: https://www.arduino.cc/reference/en/language/functions/communication/serial/println/
        void println(const char* val)
        {
            // No operation in simulation
            (void)val;
        }

        // Write writes data to the serial port.
        // @see: https://www.arduino.cc/reference/en/language/functions/communication
        void write(const byte* data, s32 length)
        {
            // No operation in simulation
            (void)data;
            (void)length;
        }

        s32 read_until(char terminator, char* outString, s32 outMaxLength)
        {
            // No operation in simulation
            (void)terminator;
            (void)outString;
            (void)outMaxLength;
            return 0;  // Return 0 bytes read
        }
    }  // namespace nserial2

}  // namespace ncore

#endif
