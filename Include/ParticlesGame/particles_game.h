#ifndef PARTICLES_GAME_H
#define PARTICLES_GAME_H

#include <GXEngine/game.h>
#include <GXEngine/perspective_camera.h>

namespace particles_game {

class ParticlesGame : public gx_engine::Game {
public:
    explicit ParticlesGame();
    ~ParticlesGame() override;

    void onInitRenderer() override;
    void onFrame(float deltaTime) override;
    void onResize(int width, int height) override;
    void onDestroyRenderer() override;

private:
    gx_engine::PerspectiveCamera* _camera;
    float _angle;
};

} // namespace particles_game

#endif PARTICLES_GAME_H
