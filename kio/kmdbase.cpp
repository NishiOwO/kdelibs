/*
    Adaption for use in KDE libraries by:
    Copyright (C) 2000 Dawit Alemayehu <adawit@kde.org>

    This class is derived from C++ implementation of "RSA Data
    Security, Inc. MD5 Message-Digest Algorithm" by Mordechai T.
    Abzug, Copyright (c) 1995.

    "RSA Data Security, Inc. MD5 Message-Digest Algorithm"
    Copyright (C) 1991-1992, RSA Data Security, Inc. Created 1991.
    All rights reserved.

    Base64 specification and implementation:
    Copyright (c) 1991 Bell Communications Research, Inc. (Bellcore)

    Permission to use, copy, modify, and distribute this material
    for any purpose and without fee is hereby granted, provided
    that the above copyright notice and this permission notice
    appear in all copies, and that the name of Bellcore not be
    used in advertising or publicity pertaining to this
    material without the specific, prior written permission
    of an authorized representative of Bellcore.  BELLCORE
    MAKES NO REPRESENTATIONS ABOUT THE ACCURACY OR SUITABILITY
    OF THIS MATERIAL FOR ANY PURPOSE.  IT IS PROVIDED "AS IS",
    WITHOUT ANY EXPRESS OR IMPLIED WARRANTIES.
*/
#include <string.h>
#include <stdlib.h>

#include <qstring.h>

#include "kmdbase.h"

// Constants for MD5Transform routine.
// Although we could use C++ style constants, defines are actually better,
// since they let us easily evade scope clashes.
#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

static Q_UINT8 PADDING[64]=
{
  0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// ROTATE_LEFT rotates x left n bits.
inline Q_UINT32 rotate_left  (Q_UINT32 x, Q_UINT32 n)
{
    return (x << n) | (x >> (32-n))  ;
}

// F, G, H and I are basic MD5 functions.
inline Q_UINT32 F (Q_UINT32 x, Q_UINT32 y, Q_UINT32 z)
{
    return (x & y) | (~x & z);
}

inline Q_UINT32 G (Q_UINT32 x, Q_UINT32 y, Q_UINT32 z)
{
    return (x & z) | (y & ~z);
}

inline Q_UINT32 H (Q_UINT32 x, Q_UINT32 y, Q_UINT32 z)
{
    return x ^ y ^ z;
}

inline Q_UINT32 I (Q_UINT32 x, Q_UINT32 y, Q_UINT32 z)
{
    return y ^ (x | ~z);
}

// FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
// Rotation is separate from addition to prevent recomputation.
inline void FF ( Q_UINT32& a, Q_UINT32 b, Q_UINT32 c, Q_UINT32 d,
                 Q_UINT32 x, Q_UINT32  s, Q_UINT32 ac )
{
    a += F(b, c, d) + x + ac;
    a = rotate_left (a, s) +b;
}

inline void GG ( Q_UINT32& a, Q_UINT32 b, Q_UINT32 c, Q_UINT32 d,
                 Q_UINT32 x, Q_UINT32 s, Q_UINT32 ac)
{
    a += G(b, c, d) + x + ac;
    a = rotate_left (a, s) +b;
}

inline void HH ( Q_UINT32& a, Q_UINT32 b, Q_UINT32 c, Q_UINT32 d,
                 Q_UINT32 x, Q_UINT32 s, Q_UINT32 ac )
{
    a += H(b, c, d) + x + ac;
    a = rotate_left (a, s) +b;
}

inline void II ( Q_UINT32& a, Q_UINT32 b, Q_UINT32 c, Q_UINT32 d,
                 Q_UINT32 x, Q_UINT32 s, Q_UINT32 ac )
{
    a += I(b, c, d) + x + ac;
    a = rotate_left (a, s) +b;
}

void encode ( Q_UINT8 *output, Q_UINT32 *input, Q_UINT32 len )
{
    Q_UINT32 i, j;
    for (i = 0, j = 0; j < len; i++, j += 4)
    {
        output[j]   = static_cast<Q_UINT8>((input[i] & 0xff));
        output[j+1] = static_cast<Q_UINT8>(((input[i] >> 8) & 0xff));
        output[j+2] = static_cast<Q_UINT8>(((input[i] >> 16) & 0xff));
        output[j+3] = static_cast<Q_UINT8>(((input[i] >> 24) & 0xff));
    }
}

// Decodes input (Q_UINT8) into output (Q_UINT32). Assumes len is
// a multiple of 4.
void decode (Q_UINT32 *output, Q_UINT8 *input, Q_UINT32 len)
{
    Q_UINT32 i, j;
    for (i = 0, j = 0; j < len; i++, j += 4)
        output[i] = static_cast<Q_UINT32>(input[j]) |
                    (static_cast<Q_UINT32>(input[j+1]) << 8)  |
                    (static_cast<Q_UINT32>(input[j+2]) << 16) |
                    (static_cast<Q_UINT32>(input[j+3]) << 24);
}

/******************************** KBase64 ********************************/

QString KBase64::encodeString( const QString& string )
{
    QString out;
    int c1, c2, c3;
    unsigned int i;

    int inPos  = 0;
    int outPos = 0;
    const char* buf = string.latin1();
    unsigned int len = string.length();
    char basis_64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    /* Get three characters at a time and encode them. */
    for (i=0; i < len/3; ++i)
    {
        c1 = buf[inPos++] & 0xFF;
        c2 = buf[inPos++] & 0xFF;
        c3 = buf[inPos++] & 0xFF;
        out[outPos++] = basis_64[(c1 & 0xFC) >> 2];
        out[outPos++] = basis_64[((c1 & 0x03) << 4) | ((c2 & 0xF0) >> 4)];
        out[outPos++] = basis_64[((c2 & 0x0F) << 2) | ((c3 & 0xC0) >> 6)];
        out[outPos++] = basis_64[c3 & 0x3F];
    }

    /* Encode the remaining one or two characters. */
    switch (len % 3)
    {
        case 0:
            break;
        case 1:
            c1 = buf[inPos] & 0xFF;
            out[outPos++] = basis_64[(c1 & 0xFC) >> 2];
            out[outPos++] = basis_64[((c1 & 0x03) << 4)];
            out[outPos++] = '=';
            out[outPos++] = '=';
            break;
        case 2:
            c1 = buf[inPos++] & 0xFF;
            c2 = buf[inPos] & 0xFF;
            out[outPos++] = basis_64[(c1 & 0xFC) >> 2];
            out[outPos++] = basis_64[((c1 & 0x03) << 4) | ((c2 & 0xF0) >> 4)];
            out[outPos++] = basis_64[((c2 & 0x0F) << 2)];
            out[outPos++] = '=';
            break;
    }
    out[outPos] = 0;
    return out;
}

#define ERRCHECK 1
static Q_UINT8 dtable[256];

QString KBase64::decodeString( const QString& buf )
{
    QString ret;
    unsigned int i, z=0, len=buf.length();

    for (i = 0; i < 255; i++)
        dtable[i] = 0x80;

    for (i = 'A'; i <= 'Z'; i++)
        dtable[i] = 0 + (i - 'A');

    for (i = 'a'; i <= 'z'; i++)
        dtable[i] = 26 + (i - 'a');

    for (i = '0'; i <= '9'; i++)
        dtable[i] = 52 + (i - '0');

    dtable['+'] = 62;
    dtable['/'] = 63;
    dtable['='] = 0;

    /* CONSTANT CONDITION */
    while ( 1 )
    {
        Q_UINT8 a[4], b[4], o[3];
        for (i = 0; i < 4; i++)
        {
            int c;
            if (z >= len)
            {
                if (ERRCHECK && (i > 0))
                {
                    fprintf(stderr, "Input file incomplete.\n");
                    return QString::null;
                }
                return ret;
            }
            c = buf[z].latin1();
            z++;
            if (dtable[c] & 0x80)
            {
                if (ERRCHECK)
                {
                    fprintf(stderr, "Illegal character '%c' in input file.\n", c);
                    return QString::null;
                }
                /* Ignoring errors: discard invalid character. */
                i--;
                continue;
            }
            a[i] = static_cast<Q_UINT8>(c);
            b[i] = static_cast<Q_UINT8>(dtable[c]);
        }
        o[0] = (b[0] << 2) | (b[1] >> 4);
        o[1] = (b[1] << 4) | (b[2] >> 2);
        o[2] = (b[2] << 6) | b[3];
        i = (a[2] == '=' ? 1 : (a[3] == '=' ? 2 : 3));
        for (unsigned int w=0; w < i; w++)
            ret.append(o[w]);
        if (i < 3)
            return ret;
    }
}

/******************************** KMD5 ********************************/


KMD5::KMD5()
{
    init();
}

KMD5::KMD5( Q_UINT8 *input )
{
    init();
    update(input, qstrlen(reinterpret_cast<char *>(input)));
    finalize();
}

KMD5::KMD5( const QCString& input )
{
    init();
    update( input );
    finalize();
}

KMD5::KMD5( const QString& input )
{
    init();
    update( input );
    finalize();
}

KMD5::KMD5(FILE *f)
{
    init();
    update( f, true );
    finalize ();
}

void KMD5::update ( const QString& input )
{
    char* in = const_cast<char *>( input.latin1() );
    update( reinterpret_cast<Q_UINT8*>(in), qstrlen(in) );
}

void KMD5::update( const QCString& input )
{
    update ( reinterpret_cast<Q_UINT8*>(input.copy().data()), input.length() );
}

void KMD5::update( Q_UINT8 *input, int len )
{
    if ( len == -1 )
        len = qstrlen( reinterpret_cast<char*>(input) );

    Q_UINT32 input_index;
    Q_UINT32 buffer_index;
    Q_UINT32 buffer_space;                // how much space is left in buffer
    Q_UINT32 input_length = static_cast<Q_UINT32>( len );

    if (m_finalized)
    {
        m_error = ERR_ALREADY_FINALIZED;
        return;
    }

    // Compute number of bytes mod 64
    buffer_index = static_cast<Q_UINT32>((m_count[0] >> 3) & 0x3F);

    // Update number of bits
    if (  (m_count[0] += (input_length << 3))<(input_length << 3) )
        m_count[1]++;

    m_count[1] += (input_length >> 29);

    buffer_space = 64 - buffer_index;  // how much space is left in buffer

    // Transform as many times as possible.
    if (input_length >= buffer_space)
    {   // ie. we have enough to fill the buffer
        // fill the rest of the buffer and transform
        memcpy (m_buffer + buffer_index, input, buffer_space);
        transform (m_buffer);

        // now, transform each 64-byte piece of the input, bypassing the buffer
        for (input_index = buffer_space; input_index + 63 < input_length;
             input_index += 64)
            transform (input+input_index);

        buffer_index = 0;  // so we can buffer remaining
    }
    else
        input_index=0;     // so we can buffer the whole input

    // and here we do the buffering:
    memcpy(m_buffer+buffer_index, input+input_index, input_length-input_index);
}

void KMD5::update( FILE *file, bool closeFile )
{
    Q_UINT8 buffer[1024];
    int len;

    while ((len=fread(buffer, 1, 1024, file)))
        update(buffer, len);
    
	// Check if we got to this point because
	// we reached EOF or an error.
	if ( !feof( file ) )
	    m_error = ERR_CANNOT_READ_FILE;
    
	// Close the file iff the flag is set.
    if ( closeFile && fclose (file) )
	    m_error = ERR_CANNOT_CLOSE_FILE;
}

// KMD5 finalization.  Ends an KMD5 message-digest operation,
// writing the the message digest and zeroizing the context.
void KMD5::finalize ()
{

    Q_UINT8 bits[8];
    Q_UINT32 index, padLen;

    if (m_finalized)
    {
        m_error = ERR_ALREADY_FINALIZED;
        return;
    }

    // Save number of bits
    encode (bits, m_count, 8);

    // Pad out to 56 mod 64.
    index = static_cast<Q_UINT32>((m_count[0] >> 3) & 0x3f);
    padLen = (index < 56) ? (56 - index) : (120 - index);
    update (PADDING, padLen);

    // Append length (before padding)
    update (bits, 8);

    // Store state in digest
    encode (m_digest, m_state, 16);

    // Fill sensitive information with zero's
    memset ( (void *)m_buffer, 0, sizeof(*m_buffer));

    m_finalized = true;
}

void KMD5::reset()
{
    init();
}

bool KMD5::verify( FILE* f, const char * msg_digest, DigestType type )
{
    init();
    update( f );
    finalize();
    return isDigestMatch( msg_digest,  type );
}

bool KMD5::verify( const QCString& input, const char * msg_digest,
                   DigestType type )
{
    init();
    update( input );
    finalize();
    return isDigestMatch( msg_digest,  type );
}

bool KMD5::verify( const QString& input, const char * msg_digest,
                   DigestType type )
{
    init();
    update( input );
    finalize();
    return isDigestMatch( msg_digest, type );
}

Q_UINT8* KMD5::rawDigest()
{
    Q_UINT8* s = new Q_UINT8[16];
    rawDigest( (char*)s );
    if ( m_error == ERR_NONE )
        return s;
    else
        return ( (Q_UINT8*)"" );
}

void KMD5::rawDigest( HASH bin )
{
    if (!m_finalized)
    {
        m_error = ERR_ALREADY_FINALIZED;
        return;
    }
    memcpy( bin, m_digest, 16 );
}

char * KMD5::hexDigest()
{
    char *s= new char[33];
    hexDigest( s );
    if ( m_error == ERR_NONE )
        return s;
    else
        return 0;
}

void KMD5::hexDigest( HASHHEX hex )
{
    if (!m_finalized)
    {
        m_error = ERR_ALREADY_FINALIZED;
        return;
    }

    for (int i=0; i<16; i++)
        sprintf(hex+i*2, "%02x", m_digest[i]);
    hex[32]='\0';
}

void KMD5::init()
{
    m_finalized= false;
    m_error = ERR_NONE;

    m_count[0] = 0;
    m_count[1] = 0;

    m_state[0] = 0x67452301;
    m_state[1] = 0xefcdab89;
    m_state[2] = 0x98badcfe;
    m_state[3] = 0x10325476;

    memset ( (void *) m_buffer, 0, sizeof(*m_buffer));
    memset ( (void *) m_digest, 0, sizeof(*m_digest));
}

bool KMD5::isDigestMatch( const char * msg_digest, DigestType type )
{
    bool result = false;

    switch (type)
    {
        case HEX:
            if ( strcmp( hexDigest(), msg_digest ) == 0 )
                result = true;
            break;
        case BIN:
            if ( strcmp( reinterpret_cast<char *>(rawDigest()), msg_digest ) == 0 )
                result = true;
            break;
        default:
            break;
    }
    return result;
}

void KMD5::transform( Q_UINT8 block[64] )
{

    Q_UINT32 a = m_state[0], b = m_state[1], c = m_state[2], d = m_state[3], x[16];

    decode (x, block, 64);
    ASSERT(!m_finalized);  // not just a user error, since the method is private

    /* Round 1 */
    FF (a, b, c, d, x[ 0], S11, 0xd76aa478); /* 1 */
    FF (d, a, b, c, x[ 1], S12, 0xe8c7b756); /* 2 */
    FF (c, d, a, b, x[ 2], S13, 0x242070db); /* 3 */
    FF (b, c, d, a, x[ 3], S14, 0xc1bdceee); /* 4 */
    FF (a, b, c, d, x[ 4], S11, 0xf57c0faf); /* 5 */
    FF (d, a, b, c, x[ 5], S12, 0x4787c62a); /* 6 */
    FF (c, d, a, b, x[ 6], S13, 0xa8304613); /* 7 */
    FF (b, c, d, a, x[ 7], S14, 0xfd469501); /* 8 */
    FF (a, b, c, d, x[ 8], S11, 0x698098d8); /* 9 */
    FF (d, a, b, c, x[ 9], S12, 0x8b44f7af); /* 10 */
    FF (c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
    FF (b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
    FF (a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
    FF (d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
    FF (c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
    FF (b, c, d, a, x[15], S14, 0x49b40821); /* 16 */

    /* Round 2 */
    GG (a, b, c, d, x[ 1], S21, 0xf61e2562); /* 17 */
    GG (d, a, b, c, x[ 6], S22, 0xc040b340); /* 18 */
    GG (c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
    GG (b, c, d, a, x[ 0], S24, 0xe9b6c7aa); /* 20 */
    GG (a, b, c, d, x[ 5], S21, 0xd62f105d); /* 21 */
    GG (d, a, b, c, x[10], S22,  0x2441453); /* 22 */
    GG (c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
    GG (b, c, d, a, x[ 4], S24, 0xe7d3fbc8); /* 24 */
    GG (a, b, c, d, x[ 9], S21, 0x21e1cde6); /* 25 */
    GG (d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
    GG (c, d, a, b, x[ 3], S23, 0xf4d50d87); /* 27 */
    GG (b, c, d, a, x[ 8], S24, 0x455a14ed); /* 28 */
    GG (a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
    GG (d, a, b, c, x[ 2], S22, 0xfcefa3f8); /* 30 */
    GG (c, d, a, b, x[ 7], S23, 0x676f02d9); /* 31 */
    GG (b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

    /* Round 3 */
    HH (a, b, c, d, x[ 5], S31, 0xfffa3942); /* 33 */
    HH (d, a, b, c, x[ 8], S32, 0x8771f681); /* 34 */
    HH (c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
    HH (b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
    HH (a, b, c, d, x[ 1], S31, 0xa4beea44); /* 37 */
    HH (d, a, b, c, x[ 4], S32, 0x4bdecfa9); /* 38 */
    HH (c, d, a, b, x[ 7], S33, 0xf6bb4b60); /* 39 */
    HH (b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
    HH (a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
    HH (d, a, b, c, x[ 0], S32, 0xeaa127fa); /* 42 */
    HH (c, d, a, b, x[ 3], S33, 0xd4ef3085); /* 43 */
    HH (b, c, d, a, x[ 6], S34,  0x4881d05); /* 44 */
    HH (a, b, c, d, x[ 9], S31, 0xd9d4d039); /* 45 */
    HH (d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
    HH (c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
    HH (b, c, d, a, x[ 2], S34, 0xc4ac5665); /* 48 */

    /* Round 4 */
    II (a, b, c, d, x[ 0], S41, 0xf4292244); /* 49 */
    II (d, a, b, c, x[ 7], S42, 0x432aff97); /* 50 */
    II (c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
    II (b, c, d, a, x[ 5], S44, 0xfc93a039); /* 52 */
    II (a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
    II (d, a, b, c, x[ 3], S42, 0x8f0ccc92); /* 54 */
    II (c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
    II (b, c, d, a, x[ 1], S44, 0x85845dd1); /* 56 */
    II (a, b, c, d, x[ 8], S41, 0x6fa87e4f); /* 57 */
    II (d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
    II (c, d, a, b, x[ 6], S43, 0xa3014314); /* 59 */
    II (b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
    II (a, b, c, d, x[ 4], S41, 0xf7537e82); /* 61 */
    II (d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
    II (c, d, a, b, x[ 2], S43, 0x2ad7d2bb); /* 63 */
    II (b, c, d, a, x[ 9], S44, 0xeb86d391); /* 64 */

    m_state[0] += a;
    m_state[1] += b;
    m_state[2] += c;
    m_state[3] += d;

    memset ( (void *) x, 0, sizeof(x) );
}
