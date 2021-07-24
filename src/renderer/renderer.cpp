#include "renderer.h"
#include <cstdlib>

namespace renderer {

Renderer::Renderer()
	: nMeshes(0)
{}

Renderer::~Renderer()
{
	for (int i = 0; i < nMeshes; i++)
	{
		free(batch[i]);	
	}
}

void Renderer::drawIndexed(const struct simpleVertex* vertices, int nVertices, const unsigned int* indices, int nIndices)
{
	batch[nMeshes++] = create_mesh_simple_vertex(vertices, nVertices,indices, nIndices);
}

void Renderer::drawTextured(const float* vertices, int nVertices, const unsigned int*indices, int nIndices)
{
    batch[nMeshes++] = create_textured_mesh(vertices, nVertices,indices, nIndices);
}


}
