#include "texture.h"
#include "src/renderer/opengl/openGLExtensions.h"

namespace renderer {

Texture::Texture() { 
	glGenTextures(1, &mHandle);
}

Texture::Texture(const char* path) {
	glGenTextures(1, &mHandle);
	Load(path);
}

Texture::~Texture() { 
	glDeleteTextures(1, &mHandle);
}


void Texture::Load(const char* path) { 
	mHandle = util::loadDDS(path);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

}

void Texture::Set(unsigned int uniformIndex, unsigned int textureIndex) {
	glActiveTexture(GL_TEXTURE0 + textureIndex);
	glBindTexture(GL_TEXTURE_2D, mHandle);
	glUniform1i(uniformIndex, textureIndex); // Bind uniform X to sampler Y
}

void Texture::UnSet(unsigned int textureIndex) {
	glActiveTexture(GL_TEXTURE0 + textureIndex);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
}

unsigned int Texture::GetHandle() { 
	return mHandle;
}

}
