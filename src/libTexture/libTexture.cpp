#include "libTexture.h"
#include "texture.h"
#include "data.h"
#include "ddsLoader.h"
#include "src/renderer/opengl/openGLExtensions.h"


unsigned    createTexture2D ( bool mipmap, const char * fileName )
{
    unsigned    textureId;

    glGenTextures   ( 1, &textureId );
    glBindTexture   ( GL_TEXTURE_2D, textureId );
    glPixelStorei   ( GL_UNPACK_ALIGNMENT, 1 );                         // set 1-byte alignment

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    if ( !loadTexture ( GL_TEXTURE_2D, mipmap, fileName ) )
        return 0;

    glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    return textureId;
}

bool    loadTexture ( int target, bool mipmap, const char * fileName )
{
    DdsLoader* mDdsLoader = new DdsLoader;
    Texture* texture = mDdsLoader->load(new Data(fileName));

    if ( texture == NULL )
        return false;

    texture -> upload ( target, mipmap );

    if ( mipmap )
        glTexParameteri ( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    else
        glTexParameteri ( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    delete texture;
    delete mDdsLoader;

    return true;
}
