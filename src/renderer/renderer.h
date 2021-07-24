#ifndef RENDERER_H_
#define RENDERER_H_

#include "src/renderer/data/mesh.h"
namespace renderer {

class Renderer
{
public:
	Renderer();
	~Renderer();

	void drawIndexed(const struct simpleVertex* vertices, int nVertices, const unsigned int* indices, int nIndices);
	void drawTextured(const struct texturedVertex* vertices, int nVertices, const unsigned int*indices, int nIndices);

	Mesh* batch[1024]; // TODO: actually it should vector
	int nMeshes;
};

}

#endif
