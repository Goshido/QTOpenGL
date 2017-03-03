#include <ParticlesGame/particles_game.h>
#include <ParticlesGame/cook_torrance_renderer.h>
#include <ParticlesGame/cook_torrance_simple_material.h>
#include <ParticlesGame/static_mesh.h>
#include <ParticlesGame/directed_light.h>
#include <GXEngine/renderer.h>
#include <GXEngine/physics_simulator.h>
#include <GXEngine/box_shape.h>
#include <GXEngine/sphere_shape.h>
#include <GXCommon/gx_log.h>

#define FOVY_DEG 60.0f
#define Z_NEAR 0.1f
#define Z_FAR 1000.0f
#define DEFAULT_ASPECT 1.0f


namespace particles_game {

gx_engine::RigidBody* sphere = nullptr;
gx_engine::BoxShape* boxShape = nullptr;
gx_engine::RigidBody* box = nullptr;

StaticMesh* boxMesh = nullptr;
StaticMesh* sphereMesh = nullptr;
CookTorranceSimpleMaterial* redMaterial = nullptr;
CookTorranceSimpleMaterial* greenMaterial = nullptr;

DirectedLight* light = nullptr;

ParticlesGame::ParticlesGame()
    : _camera(nullptr)
    , _angle(0.0f)
{
    // NOTHING
}

ParticlesGame::~ParticlesGame()
{
    // NOTHING
}

void ParticlesGame::onInitRenderer()
{
    gx_engine::Renderer* renderer = gx_engine::Renderer::instance();

    float width = (float)renderer->getWidth();
    float height = (float)renderer->getHeight();

    _camera = new gx_engine::PerspectiveCamera(gx_common::gxDegToRad(FOVY_DEG), width / height, Z_NEAR, Z_FAR);
    gx_engine::Camera::setActiveCamera(_camera);
    _camera->setLocation(0.0f, 2.0f, -6.0f);


    gx_common::GXMat3 rotation;
    gx_common::gxSetMat3Identity(rotation);
    gx_common::GXVec3 pivotOffset(0.0f, 0.0f, 0.0f);

    gx_engine::SphereShape* sphereShape = new gx_engine::SphereShape();
    sphereShape->setRadius(1.0f);

    sphere = new gx_engine::RigidBody();
    sphere->addShape(sphereShape, pivotOffset, rotation);
    sphere->setLocationWorld(0.0f, 0.0f, 0.0f);

    gx_engine::BoxShape* boxShape = new gx_engine::BoxShape();
    boxShape->setWidth(1.0f);
    boxShape->setHeight(1.0f);
    boxShape->setDepth(1.0f);

    box = new gx_engine::RigidBody();
    box->addShape(boxShape, pivotOffset, rotation);
    box->setLocationWorld(0.0f, 0.0f, 0.0f);

    boxMesh = new StaticMesh("3D Models/ParticlesGame/cube.obj");
    sphereMesh = new StaticMesh("3D Models/ParticlesGame/sphere.obj");

    redMaterial = new CookTorranceSimpleMaterial();
    redMaterial->setAlbedo(255, 0, 0, 255);

    greenMaterial = new CookTorranceSimpleMaterial();
    greenMaterial->setAlbedo(115, 185, 0, 255);

    light = new DirectedLight();
    light->setIntensity(5.0f);
    light->setRotation(GX_MATH_HALFPI * 0.5f, 0, GX_MATH_HALFPI * 0.5f);
}

void __stdcall moveSphere(float angle)
{
    gx_common::GXVec3 location;
    location.x = location.z = 0.0f;
    location.y = sinf(angle) * 3.0f;

    sphere->setLocationWorld(location.x, location.y, location.z);
}

void ParticlesGame::onFrame(float deltaTime)
{
    _angle += deltaTime * 0.5f;
    moveSphere(_angle);

    sphereMesh->setLocation(sphere->getLocationWorld());
    boxMesh->setLocation(box->getLocationWorld());

    CookTorranceRenderer* renderer = CookTorranceRenderer::instance();

    gx_engine::Camera::setActiveCamera(_camera);

    renderer->beginGeometryPass();
    renderer->renderMesh(*sphereMesh, *redMaterial);
    renderer->renderMesh(*boxMesh, *greenMaterial);

    renderer->beginLightPass();
    renderer->lightUp(*light);

    renderer->beginTonePass();
    renderer->beginBlurPass();

    renderer->presentFrame(RenderTarget::Normal);

    _camera->updateLastFrameViewMatrix();

    gx_engine::PhysicsSimulator* simulator = gx_engine::PhysicsSimulator::instance();

    gx_engine::PhysicsShapeLink* sphereShapes = sphere->getShapes();
    gx_engine::PhysicsShapeLink* boxShapes = box->getShapes();

    if (simulator->runGJK(*sphereShapes, *boxShapes)) {
        gx_common::gxLogA("Intersected\n");
    } else {
        gx_common::gxLogA("Not Intersected\n");
    }
}

void ParticlesGame::onResize(int width, int height)
{
    _camera->setAspect(width / (float)height);
    CookTorranceRenderer::instance()->resize(width, height);
}

void ParticlesGame::onDestroyRenderer()
{
    delete _camera;
    delete CookTorranceRenderer::instance();
    delete gx_engine::PhysicsSimulator::instance();

    delete sphereMesh;
    delete boxMesh;

    delete redMaterial;
    delete greenMaterial;

    delete light;

    delete sphere;
    delete box;
}

} // namespace particles_game
