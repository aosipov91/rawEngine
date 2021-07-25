//
// Some definitions for DDS files
//

#ifndef	__DDS_TYPES__
#define	__DDS_TYPES__
#include <cstdint>
#pragma pack (push, 1)

struct DDS_PIXELFORMAT
{
        uint32_t dwSize;
        uint32_t dwFlags;
        uint32_t dwFourCC;
        uint32_t dwRGBBitCount;
        uint32_t dwRBitMask;
        uint32_t dwGBitMask;
        uint32_t dwBBitMask;
        uint32_t dwABitMask;
};

struct DDS_HEADER
{
        uint32_t dwSize;
        uint32_t dwFlags;
        uint32_t dwHeight;
        uint32_t dwWidth;
        uint32_t dwPitchOrLinearSize;
        uint32_t dwDepth;
        uint32_t dwMipMapCount;
        uint32_t dwReserved1[11];
        DDS_PIXELFORMAT ddspf;
        uint32_t dwCaps1;
        uint32_t dwCaps2;
        uint32_t dwReserved2[3];
};

#pragma	pack (pop)

enum
{
  										// bit flags for header
	DDS_CAPS	    = 0x00000001,
	DDS_HEIGHT	    = 0x00000002,
	DDS_WIDTH	    = 0x00000004,
	DDS_PITCH	    = 0x00000008,
	DDS_PIXELFORMAT = 0x00001000,
	DDS_MIPMAPCOUNT = 0x00020000,
	DDS_LINEARSIZE  = 0x00080000,
	DDS_DEPTH	    = 0x00800000,

  										// flags for pixel formats
	DDS_ALPHA_PIXELS = 0x00000001,
	DDS_ALPHA        = 0x00000002,
	DDS_FOURCC	     = 0x00000004,
	DDS_RGB	         = 0x00000040,
    DDS_RGBA         = 0x00000041,

										// flags for complex caps
	DDS_COMPLEX	   = 0x00000008,
	DDS_TEXTURE	   = 0x00001000,
	DDS_MIPMAP	   = 0x00400000,

										// flags for cubemaps
	DDS_CUBEMAP	          = 0x00000200,
	DDS_CUBEMAP_POSITIVEX = 0x00000400,
	DDS_CUBEMAP_NEGATIVEX = 0x00000800,
	DDS_CUBEMAP_POSITIVEY = 0x00001000,
	DDS_CUBEMAP_NEGATIVEY = 0x00002000,
	DDS_CUBEMAP_POSITIVEZ = 0x00004000,
	DDS_CUBEMAP_NEGATIVEZ = 0x00008000,
	DDS_VOLUME		      = 0x00200000
};

#endif
