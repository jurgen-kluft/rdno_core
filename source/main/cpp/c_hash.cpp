#include "rdno_core/c_timer.h"
#include "rdno_core/c_hash.h"
#include "rdno_core/c_math.h"
#include "rdno_core/c_memory.h"

namespace ncore
{
    namespace nhash
    {
        // from: https://www.oryx-embedded.com/doc/sha1_8c_source.html

        // SHA-1 block size
#define SHA1_BLOCK_SIZE 64
// SHA-1 digest size
#define SHA1_DIGEST_SIZE 20

// Macro to access the workspace as a circular buffer
#define W(n) w[(n) & 0x0F]

// SHA-1 auxiliary functions
#define CH(x, y, z)     (((x) & (y)) | (~(x) & (z)))
#define PARITY(x, y, z) ((x) ^ (y) ^ (z))
#define MAJ(x, y, z)    (((x) & (y)) | ((x) & (z)) | ((y) & (z)))

        // SHA-1 padding
        static const u8 padding[64] = {0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

        // SHA-1 constants
        static const u32 k[4] = {0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xCA62C1D6};

// Load unaligned 32-bit integer (big-endian encoding)
#define LOAD32BE(p) (((u32)(((u8 *)(p))[0]) << 24) | ((u32)(((u8 *)(p))[1]) << 16) | ((u32)(((u8 *)(p))[2]) << 8) | ((u32)(((u8 *)(p))[3]) << 0))

// Store unaligned 32-bit integer (big-endian encoding)
#define STORE32BE(a, p) ((u8 *)(p))[0] = ((u32)(a) >> 24) & 0xFFU, ((u8 *)(p))[1] = ((u32)(a) >> 16) & 0xFFU, ((u8 *)(p))[2] = ((u32)(a) >> 8) & 0xFFU, ((u8 *)(p))[3] = ((u32)(a) >> 0) & 0xFFU

        /**
         * @brief Digest a message using SHA-1
         * @param[in] data Pointer to the message being hashed
         * @param[in] length Length of the message
         * @param[out] digest Pointer to the calculated digest
         * @return Error code
         **/

        void FinalRaw(context_t *context, u8 digest[SHA1_DIGEST_SIZE]);
        void ProcessBlock(context_t *context);

        bool Compute(u8 const *data, u32 length, u8 digest[20])
        {
            context_t context[1];

            // Check parameters
            if (data == NULL && length != 0)
                return false;  // ERROR_INVALID_PARAMETER

            if (digest == NULL)
                return false;  // ERROR_INVALID_PARAMETER

            // Initialize the SHA-1 context
            Init(context);
            // Digest the message
            Update(context, data, length);
            // Finalize the SHA-1 message digest
            Final(context, digest);

            return true;
        }

        /**
         * @brief Initialize SHA-1 message digest context
         * @param[in] context Pointer to the SHA-1 context to initialize
         **/

        void Init(context_t *context)
        {
            // Set initial hash value
            context->h[0] = 0x67452301;
            context->h[1] = 0xEFCDAB89;
            context->h[2] = 0x98BADCFE;
            context->h[3] = 0x10325476;
            context->h[4] = 0xC3D2E1F0;

            // Number of bytes in the buffer
            context->size = 0;
            // Total length of the message
            context->totalSize = 0;
        }

        /**
         * @brief Update the SHA-1 context with a portion of the message being hashed
         * @param[in] context Pointer to the SHA-1 context
         * @param[in] data Pointer to the buffer being hashed
         * @param[in] length Length of the buffer
         **/

        void Update(context_t *context, u8 const *data, u32 length)
        {
            u32 n;

            // Process the incoming data
            while (length > 0)
            {
                // The buffer can hold at most 64 bytes
                n = math::g_min(length, 64 - context->size);

                // Copy the data to the buffer
                g_memcpy(context->buffer + context->size, data, n);

                // Update the SHA-1 context
                context->size += n;
                context->totalSize += n;
                // Advance the data pointer
                data = (u8 *)data + n;
                // Remaining bytes to process
                length -= n;

                // Process message in 16-word blocks
                if (context->size == 64)
                {
                    // Transform the 16-word block
                    ProcessBlock(context);
                    // Empty the buffer
                    context->size = 0;
                }
            }
        }

        /**
         * @brief Finish the SHA-1 message digest
         * @param[in] context Pointer to the SHA-1 context
         * @param[out] digest Calculated digest
         **/

        void Final(context_t *context, u8 digest[20])
        {
            u32 i;
            u32 paddingSize;
            u32 totalSize;

            // Length of the original message (before padding)
            totalSize = context->totalSize * 8;

            // Pad the message so that its length is congruent to 56 modulo 64
            if (context->size < 56)
            {
                paddingSize = 56 - context->size;
            }
            else
            {
                paddingSize = 64 + 56 - context->size;
            }

            // Append padding
            Update(context, padding, paddingSize);

            // Append the length of the original message
            for (i = 0; i < 8; i++)
            {
                context->buffer[63 - i] = totalSize & 0xFF;
                totalSize >>= 8;
            }

            // Calculate the message digest
            ProcessBlock(context);

            // Copy the resulting digest
            for (i = 0; i < (SHA1_DIGEST_SIZE / 4); i++)
            {
                STORE32BE(context->h[i], digest + i * 4);
            }
        }

        /**
         * @brief Finish the SHA-1 message digest (no padding added)
         * @param[in] context Pointer to the SHA-1 context
         * @param[out] digest Calculated digest
         **/

        void FinalRaw(context_t *context, u8 digest[SHA1_DIGEST_SIZE])
        {
            uint_t i;

            // Copy the resulting digest
            for (i = 0; i < (SHA1_DIGEST_SIZE / 4); i++)
            {
                STORE32BE(context->h[i], digest + i * 4);
            }
        }

        /**
         * @brief Process message in 16-word blocks
         * @param[in] context Pointer to the SHA-1 context
         **/

#define ROL32(a, n) (((a) << (n)) | ((a) >> (32 - (n))))

        void ProcessBlock(context_t *context)
        {
            uint_t i;
            u32    temp;

            // Initialize the 5 working registers
            u32 a = context->h[0];
            u32 b = context->h[1];
            u32 c = context->h[2];
            u32 d = context->h[3];
            u32 e = context->h[4];

            // Process message in 16-word blocks
            u32 *w = context->w;

            // Convert from big-endian byte order to host byte order
            for (i = 0; i < 16; i++)
            {
                w[i] = LOAD32BE(context->buffer + i * 4);
            }

            // SHA-1 hash computation (alternate method)
            for (i = 0; i < 80; i++)
            {
                // Prepare the message schedule
                if (i >= 16)
                {
                    W(i) = ROL32(W(i + 13) ^ W(i + 8) ^ W(i + 2) ^ W(i), 1);
                }

                // Calculate T
                if (i < 20)
                {
                    temp = ROL32(a, 5) + CH(b, c, d) + e + W(i) + k[0];
                }
                else if (i < 40)
                {
                    temp = ROL32(a, 5) + PARITY(b, c, d) + e + W(i) + k[1];
                }
                else if (i < 60)
                {
                    temp = ROL32(a, 5) + MAJ(b, c, d) + e + W(i) + k[2];
                }
                else
                {
                    temp = ROL32(a, 5) + PARITY(b, c, d) + e + W(i) + k[3];
                }

                // Update working registers
                e = d;
                d = c;
                c = ROL32(b, 30);
                b = a;
                a = temp;
            }

            // Update the hash value
            context->h[0] += a;
            context->h[1] += b;
            context->h[2] += c;
            context->h[3] += d;
            context->h[4] += e;
        }

    }  // namespace nhash
}  // namespace ncore
