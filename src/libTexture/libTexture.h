#ifndef LIB_TEXTURE_H_
#define LIB_TEXTURE_H_

#include "typeDefs.h"

class	FileSystem;
class	Data;
class	Texture;
class	TextureLoader;

unsigned	createTexture2D ( bool mipmap, const char * fileName );
bool		loadTexture ( int target, bool mipmap, const char * fileName );

#endif
