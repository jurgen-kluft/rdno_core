#include "rcore/c_log.h"
#include "rcore/c_serial.h"
#include "rcore/c_timer.h"
#include "rcore/c_str.h"
#include "ccore/c_stream.h"

#ifdef TARGET_ARDUINO
#    include "Arduino.h"
#endif

#include "rcore/c_network.h"

#ifdef TARGET_ARDUINO

namespace ncore
{
    namespace nserial
    {
#    ifndef TARGET_FINAL
        // Begin sets the data rate in bits per second (baud) for serial data transmission.
        // @see: https://www.arduino.cc/reference/en/language/functions/communication/serial/begin/
        void begin(nbaud::Enum baud)
        {
            Serial.begin(baud);
            const u64 startTime = ntimer::millis();
            while (!Serial)
            {
                if (ntimer::millis() - startTime > 3000)
                    break;
                ntimer::delay(10);
            }
        }

        // Print prints data to the serial port as human-readable ASCII text.
        // @see: https://www.arduino.cc/reference/en/language/functions/communication/serial/print/
        void print(const char* val) { Serial.print(val); }

        void print(u8 a, u8 b, u8 c, u8 d, char s)
        {
            char strBuffer[32];
            snprintf(strBuffer, sizeof(strBuffer), "%u%c%u%c%u%c%u", va_t(a), va_t(s), va_t(b), va_t(s), va_t(c), va_t(s), va_t(d));
            Serial.print(strBuffer);
        }

        void print(u8 a, u8 b, u8 c, u8 d, u8 e, u8 f, char s)
        {
            char strBuffer[32];
            snprintf(strBuffer, sizeof(strBuffer), "%02X%c%02X%c%02X%c%02X%c%02X%c%02X", va_t(a), va_t(s), va_t(b), va_t(s), va_t(c), va_t(s), va_t(d), va_t(s), va_t(e), va_t(s), va_t(f));
            Serial.print(strBuffer);
        }

        // Println prints data to the serial port as human-readable ASCII text followed by a
        // carriage return character (ASCII 13, or '\r') and a newline character (ASCII 10, or '\n').
        // @see: https://www.arduino.cc/reference/en/language/functions/communication/serial/println/
        void println(const char* val) { Serial.println(val); }
#    else
        void begin(nbaud::Enum baud) { CC_UNUSED(baud); }
        void print(const char* val) { CC_UNUSED(val); }
        void print(const IPAddress_t& address) { CC_UNUSED(address); }
        void print(const MACAddress_t& mac) { CC_UNUSED(mac); }
        void println(const char* val) { CC_UNUSED(val); }
#    endif
    }  // namespace nserial

    namespace nserialx
    {

        HardwareSerial* HwSerial[4] = {&Serial,
#    if SOC_UART_HP_NUM > 1
                                       &Serial1,
#    else
                                       nullptr,
#    endif
#    if SOC_UART_HP_NUM > 2
                                       &Serial2,
#    else
                                       nullptr,
#    endif
#    if SOC_UART_HP_NUM > 3
                                       &Serial3
#    else
                                       nullptr
#    endif
        };

        class serial_reader_t : public reader_t
        {
            HardwareSerial* m_serial;

        public:
            inline serial_reader_t(serial_t serial)
                : m_serial(HwSerial[serial])
            {
            }

            virtual s64 v_read(u8* data, s64 len) final override
            {
                s32 availableBytes = m_serial->available();
                if (availableBytes <= 0)
                    return 0;
                if (len > availableBytes)
                    len = availableBytes;
                return (s64)m_serial->readBytes(data, len);
            }
        };

        class nil_serial_reader_t : public reader_t
        {
        public:
            virtual s64 v_read(u8* data, s64 len) final override
            {
                CC_UNUSED(data);
                CC_UNUSED(len);
                return 0;
            }
        };

        // Begin sets the data rate in bits per second (baud) for serial data transmission.
        // @see: https://www.arduino.cc/reference/en/language/functions/communication/serial/begin/
        void begin(serial_t x, nbaud::Enum baud, nconfig::Enum config, s8 rxPin, s8 txPin)
        {
            if (x > 3 || HwSerial[x] == nullptr)
                return;

#    ifdef TARGET_ESP8266
            SerialConfig configValue = SERIAL_8N1;
            switch (config)
            {
                case nconfig::MODE_8N1: configValue = SERIAL_8N1; break;
            }
            if (configValue != 0)
            {
                HardwareSerial* serial = HwSerial[x];
                serial->begin(baud, configValue, SERIAL_FULL, rxPin, txPin);
            }
#    else  // TARGET_ESP32
            u32 configValue = SERIAL_8N1;
            switch (config)
            {
                case nconfig::MODE_8N1: configValue = SERIAL_8N1; break;
            }
            HardwareSerial* serial = HwSerial[x];
            serial->begin(baud, configValue, rxPin, txPin);
#    endif
        }

        serial_reader_t SERIAL_READERS[] = {
          serial_reader_t(0),
          serial_reader_t(1),
          serial_reader_t(2),
          serial_reader_t(3),
        };
        nil_serial_reader_t NIL_SERIAL_READER;

        reader_t* reader(serial_t x)
        {
            if (x > 3 || HwSerial[x] == nullptr)
                return &NIL_SERIAL_READER;
            return &SERIAL_READERS[x];
        }

        s32 available(serial_t x)
        {
            if (x > 3 || HwSerial[x] == nullptr)
                return 0;
            HardwareSerial* serial = HwSerial[x];
            return serial->available();
        }

        // Print prints data to the serial port as human-readable ASCII text.
        // @see: https://www.arduino.cc/reference/en/language/functions/communication/serial/print/
        void print(serial_t x, const char* val)
        {
            if (x > 3 || HwSerial[x] == nullptr)
                return;
            HardwareSerial* serial = HwSerial[x];
            serial->print(val);
        }

        // Println prints data to the serial port as human-readable ASCII text followed by a
        // carriage return character (ASCII 13, or '\r') and a newline character (ASCII 10, or '\n').
        // @see: https://www.arduino.cc/reference/en/language/functions/communication/serial/println/
        void println(serial_t x, const char* val)
        {
            if (x > 3 || HwSerial[x] == nullptr)
                return;
            HardwareSerial* serial = HwSerial[x];
            serial->println(val);
        }

        // Write writes data to the serial port.
        // @see: https://www.arduino.cc/reference/en/language/functions/communication
        void write(serial_t x, const byte* data, s32 length)
        {
            if (x > 3 || HwSerial[x] == nullptr)
                return;
            HardwareSerial* serial = HwSerial[x];
            serial->write(data, length);
        }

        s32 read_until(serial_t x, char terminator, char* outString, s32 outMaxLength)
        {
            if (x > 3 || HwSerial[x] == nullptr)
                return 0;
            HardwareSerial* serial = HwSerial[x];
            // Read data from the specified serial port until the terminator character is found or the maximum length is reached
            s32 n = 0;
            n     = serial->readBytesUntil(terminator, outString, outMaxLength);
            return n;
        }

        s32 read_bytes(serial_t x, byte* outData, s32 outMaxLength)
        {
            if (x > 3 || HwSerial[x] == nullptr)
                return 0;
            HardwareSerial* serial = HwSerial[x];
            s32             n      = 0;
            n                      = (s32)serial->readBytes(outData, outMaxLength);
            return n;
        }

    }  // namespace nserialx
}  // namespace ncore

#else

namespace ncore
{
    namespace nserial
    {
        void begin(nbaud::Enum baud) { CC_UNUSED(baud); }
        void print(const char* val) { CC_UNUSED(val); }
        void print(u8 a, u8 b, u8 c, u8 d, char separator )
        {
            CC_UNUSED(a);
            CC_UNUSED(b);
            CC_UNUSED(c);
            CC_UNUSED(d);
            CC_UNUSED(separator);
        }
        void print(u8 a, u8 b, u8 c, u8 d, u8 e, u8 f, char separator)
        {
            CC_UNUSED(a);
            CC_UNUSED(b);
            CC_UNUSED(c);
            CC_UNUSED(d);
            CC_UNUSED(e);
            CC_UNUSED(f);
            CC_UNUSED(separator);
        }
        void println(const char* val) { CC_UNUSED(val); }
    }  // namespace nserial

    namespace nserialx
    {
        void begin(serial_t x, nbaud::Enum baud, nconfig::Enum config, s8 rxPin, s8 txPin)
        {
            CC_UNUSED(x);
            CC_UNUSED(baud);
            CC_UNUSED(config);
            CC_UNUSED(rxPin);
            CC_UNUSED(txPin);
        }
        s32 available(serial_t x)
        {
            CC_UNUSED(x);
            return 0;
        }
        void print(serial_t x, const char* val)
        {
            CC_UNUSED(x);
            CC_UNUSED(val);
        }
        void println(serial_t x, const char* val)
        {
            CC_UNUSED(x);
            CC_UNUSED(val);
        }
        void write(serial_t x, const byte* data, s32 length)
        {
            CC_UNUSED(x);
            CC_UNUSED(data);
            CC_UNUSED(length);
        }
        s32 read_until(serial_t x, char terminator, char* outString, s32 outMaxLength)
        {
            CC_UNUSED(x);
            CC_UNUSED(terminator);
            CC_UNUSED(outString);
            CC_UNUSED(outMaxLength);
            return 0;
        }
        s32 read_bytes(serial_t x, byte* outData, s32 outMaxLength)
        {
            CC_UNUSED(x);
            CC_UNUSED(outData);
            CC_UNUSED(outMaxLength);
            return 0;
        }
    }  // namespace nserialx

}  // namespace ncore

#endif
