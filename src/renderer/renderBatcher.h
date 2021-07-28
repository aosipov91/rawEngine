#ifndef RENDER_BATCHER_H_
#define RENDER_BATCHER_H_

#include "renderer.h"

class RenderBatcher final
{
public:
    RenderBatcher();
    ~RenderBatcher() = default;

protected:
    void add(renderer::RenderMaterial mat, renderer::TexturedVertex* vetices);
    void render();
};

#endif
