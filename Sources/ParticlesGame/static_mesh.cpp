#include <ParticlesGame/static_mesh.h>
#include <GXEngine/camera.h>

namespace particles_game {

StaticMesh::StaticMesh(const QString& meshFile)
{
    _mesh = gx_engine::MeshGeometry::loadFromObj(meshFile);
}

StaticMesh::~StaticMesh()
{
    // NOTHING
}

void StaticMesh::onDraw()
{
    if (gx_engine::Camera::getActiveCamera()->isObjectVisible(_mesh->getBoundsWorld())) {
        _mesh->render();
    }

    updateLastFrameModelMatrix();
}

void StaticMesh::onTransformChanged()
{
    _mesh->updateBoundsWorld(_currentModelMatrix);
}

} //namespace particles_game
