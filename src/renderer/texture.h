#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "src/util/ddsLoader.h"

namespace renderer {

class Texture {
protected:
	unsigned int mHandle;
private:
	Texture(const Texture& other);
	Texture& operator=(const Texture& other);
public:
	Texture();
	Texture(const char* path);
	~Texture();

	void Load(const char* path);

	void Set(unsigned int uniformIndex, unsigned int textureIndex);
	void UnSet(unsigned int textureIndex);
	unsigned int GetHandle();
};

}
#endif
