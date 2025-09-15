#include "rdno_core/c_serial.h"
#include "rdno_core/c_timer.h"
#include "rdno_core/c_str.h"

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
            const u64 startAttemptTime = ntimer::millis();
            while (!Serial && (ntimer::millis() - startAttemptTime) < 2000)
            {
                ntimer::delay(100);
            }
        }

        // Print prints data to the serial port as human-readable ASCII text.
        // @see: https://www.arduino.cc/reference/en/language/functions/communication/serial/print/
        void print(const char* val) { Serial.print(val); }

        void print(const s32 val)
        {
            char  strBuffer[16];
            str_t str = str_mutable(strBuffer, sizeof(strBuffer));
            to_str(str, val, 10);
            Serial.print(str.m_const);
        }

        void print(const u32 val, bool hex)
        {
            if (hex)
            {
                char  strBuffer[11];  // "0x" + 8 hex digits + null terminator
                str_t str = str_mutable(strBuffer, sizeof(strBuffer));
                str_append(str, "0x");
                to_str(str, val, 16);
                Serial.print(str.m_const);
            }
            else
            {
                char  strBuffer[12];  // Max 10 digits for u32 + null terminator
                str_t str = str_mutable(strBuffer, sizeof(strBuffer));
                to_str(str, val, 10);
                Serial.print(str.m_const);
            }
        }

        void print(const IPAddress_t& address)
        {
            char  strBuffer[20];
            str_t str = str_mutable(strBuffer, sizeof(strBuffer));
            to_str(str, address);
            Serial.print(str.m_const);
        }

        void print(const MACAddress_t& mac)
        {
            char  strBuffer[20];
            str_t str = str_mutable(strBuffer, sizeof(strBuffer));
            to_str(str, mac);
            Serial.print(str.m_const);
        }

        // Println prints data to the serial port as human-readable ASCII text followed by a
        // carriage return character (ASCII 13, or '\r') and a newline character (ASCII 10, or '\n').
        // @see: https://www.arduino.cc/reference/en/language/functions/communication/serial/println/
        void println(const char* val) { Serial.println(val); }

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

        s32 read_bytes(byte* outData, s32 outMaxLength)
        {
            s32 n = 0;
#    if SOC_UART_NUM > 2
            n = (s32)Serial2.readBytes(outData, outMaxLength);
#    endif
            return n;
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

        // Frame Header    Length   Detection Result   Target Distance   The energy values for each distance gate    Frame Tail
        // 4 bytes,        2 bytes, 1 byte,            2 bytes,          32 bytes,                                   4 bytes
        //
        //   Frame Header (F4 F3 F2 F1)
        //   Detection Result (00 absent, 01 present)
        //   Length, total number of bytes for detection result, target distance, and energy values for each distance gate
        //   Target Distance, indicating the distance of the target phase from the radar in the scene
        //   Energy values, 16 (total number of distance gates) * 2 bytes, size of energy value for each distance gate from 0 to 15
        //   Frame Tail (F8 F7 F6 F5)

        const byte hmmdTestBuffer1[] = {0xF4, 0xF3, 0xF2, 0xF1, 0x2B, 0x00, 0x01, 0x3C, 0x00, 0x10, 0x00, 0x20, 0x00, 0x30, 0x00, 0x40, 0x00, 0x50, 0x00, 0x60, 0x00, 0x70, 0x00,
                                        0x80, 0x00, 0x90, 0x00, 0xA0, 0x00, 0xB0, 0x00, 0xC0, 0x00, 0xD0, 0x00, 0xE0, 0x00, 0xF0, 0x00, 0x00, 0x00, 0xF8, 0xF7, 0xF6, 0xF5};
        const byte hmmdTestBuffer2[] = {0xF4, 0xF3, 0xF2, 0xF1, 0x2B, 0x00, 0x00, 0x4B, 0x00, 0x10, 0x00, 0x20, 0x00, 0x30, 0x00, 0x40, 0x00, 0x50, 0x00, 0x60, 0x00, 0x70, 0x00,
                                        0x80, 0x00, 0x90, 0x00, 0xA0, 0x00, 0xB0, 0x00, 0xC0, 0x00, 0xD0, 0x00, 0xE0, 0x00, 0xF0, 0x00, 0x00, 0x00, 0xF8, 0xF7, 0xF6, 0xF5};

        const byte* hmmdTestBufferPtr    = hmmdTestBuffer1;
        s32         hmmdTestBufferCursor = 0;

        s32 available()
        {
            if (hmmdTestBufferCursor == -1)
            {
                hmmdTestBufferPtr    = hmmdTestBufferPtr == hmmdTestBuffer1 ? hmmdTestBuffer2 : hmmdTestBuffer1;
                hmmdTestBufferCursor = 0;
                return 0;
            }

            return hmmdTestBufferCursor >= 0 ? 1 : 0;
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

        s32 read_bytes(byte* outData, s32 outMaxLength)
        {
            if (hmmdTestBufferCursor == -1)
            {
                return 0;
            }

            outData[0] = hmmdTestBufferPtr[hmmdTestBufferCursor++];
            if (hmmdTestBufferCursor >= sizeof(hmmdTestBuffer1))
            {
                hmmdTestBufferCursor = -1;
            }
            return 1;
        }
    }  // namespace nserial2

}  // namespace ncore

#endif
