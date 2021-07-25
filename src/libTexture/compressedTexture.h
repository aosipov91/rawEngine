//
// Compressed texture class
//

#ifndef	__COMPRESSED_TEXTURE__
#define	__COMPRESSED_TEXTURE__

#include	"texture.h"

class	CompressedTexture : public Texture
{
public:
	CompressedTexture ( int32_t theWidth, int32_t theHeight, int32_t theNumComponents, int32_t theFormat, int32_t theLevels, int32_t theSize );

	virtual	bool	upload ( int32_t target, bool mipmap = true );
};

#endif
