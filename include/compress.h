#ifndef HCOMPRESS_H
#define HCOMPRESS_H

#ifdef HCOMPRESS_EXPORT_CLASS
#define HCOMPRESS_CLASS __declspec(dllexport)
#else
#define HCOMPRESS_CLASS __declspec(dllimport)
#endif // HCOMPRESS_EXPPORT_CLASS

struct _CLzmaEncProps;

// an in-memory compress class
class HCOMPRESS_CLASS HCompress
{
public:

    // nested enum
    typedef enum COMPRESS_PATTERN
    {
        ZLIB,
        LZMA,
        SNAPPY
    }PATTERN;

public:

    // ctor
    HCompress(PATTERN pattern = ZLIB);
    HCompress(const HCompress& that);
    HCompress& operator =(const HCompress& that);

    // dector
    ~HCompress();

public:

    // compress src ----> dest
    // commonly, we set destLen = srcLen to ensure enough destBuffer.
    // specially, when pattern = 'LZMA', please set destLen = srcLen + 5, because LZMA 
    //     need add 5 bytes special data to destBuffer.
    bool compress(void* src, unsigned long srcLen, void* dest, unsigned long& destLen);

    // uncompress src ----> dest
    bool uncompress(void* src, unsigned long srcLen, void* dest, unsigned long& destLen);

    // set lzma compress props
    // compLevel: 0 ~ 9 ,  dictSize: 1 << 12 ~ 1 << 24,  threadNum: 1 ~ 2;
    // default: 1,         default: 1 << 12,             default: 2;
    void setLzmaProps(int compLevel, int dictSize, int threadNum);

private:

    // zlib comp
    bool compress_zlib(void* src, unsigned long srcLen, void* dest, unsigned long& destLen);

    // zlib uncomp
    bool uncompress_zlib(void* src, unsigned long srcLen, void* dest, unsigned long& destLen);

    // lzma comp
    bool compress_lzma(void* src, unsigned long srcLen, void* dest, unsigned long& destLen);

    // lzma uncomp
    bool uncompress_lzma(void* src, unsigned long srcLen, void* dest, unsigned long& destLen);

    // Snappy comp
    bool compress_snappy(void* src, unsigned long srcLen, void* dest, unsigned long& destLen);

    // Snappy uncomp
    bool uncompress_snappy(void* src, unsigned long srcLen, void* dest, unsigned long& destLen);

protected:

    PATTERN _pttrn;
    _CLzmaEncProps* _lzmaProps;
};

#endif // HCOMPRESS_H