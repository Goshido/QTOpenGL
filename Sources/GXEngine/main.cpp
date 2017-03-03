#include <QCoreApplication>
#include <QDebug>
#include <QDir>

#include <GXEngine/renderer.h>
#include <GXEngine/physics_simulator.h>
#include <ParticlesGame/particles_game.h>
#include <GXCommon/gx_log.h>
#include <GXCommon/gx_time.h>

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    QDir::setCurrent("../../../");

    gx_engine::Renderer* renderer = gx_engine::Renderer::instance();
    gx_engine::PhysicsSimulator* physicsSimulator = gx_engine::PhysicsSimulator::instance();
    particles_game::ParticlesGame game;
    renderer->setGame(&game);

    int retCode = app.exec();

    delete renderer;
    delete physicsSimulator;

    return retCode;
}
