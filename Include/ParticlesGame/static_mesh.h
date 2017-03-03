#ifndef STATIC_MESH_H
#define STATIC_MESH_H

#include <GXEngine/renderable.h>
#include <GXEngine/mesh_geometry.h>

namespace particles_game {

class StaticMesh : public gx_engine::Renderable {
public:
    explicit StaticMesh(const QString& meshFile);
    ~StaticMesh() override;

    void onDraw() override;

protected:
    void onTransformChanged() override;

private:
    QSharedPointer<gx_engine::MeshGeometry> _mesh;
};

} //namespace particles_game

#endif STATIC_MESH_H
