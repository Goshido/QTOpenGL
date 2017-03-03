#ifndef MATERIAL_H
#define MATERIAL_H

#include "renderable.h"
#include "shader_program.h"

namespace gx_engine {

class Material {
public:
    explicit Material();
    virtual ~Material();

    virtual void bind(const Renderable& activeRenderable) = 0;
    virtual void unbind() = 0;

protected:
    QSharedPointer<ShaderProgram> _program;
};

} // namespace gx_engine

#endif MATERIAL_H
