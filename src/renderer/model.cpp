#include "model.h"
#include <cstdlib>
void model_init(struct Model *model)
{
    model->indices = nullptr;
    model->f_vertices = nullptr;
    model->t_vertices = nullptr;
    model->textured = 0;
    model->textured_vertices = nullptr;
    model->diffuseMap = 0;
    model->specularMap = 0;
    model->normalMap = 0;

}

void model_unload(struct Model *model)
{
    free(model->indices);
    free(model->f_vertices);
    free(model->t_vertices);
    free(model->textured_vertices);
}
