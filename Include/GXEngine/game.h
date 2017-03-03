#ifndef GAME_H
#define GAME_H

namespace gx_engine {

class Game {
public:
    explicit Game();
    virtual ~Game();

    virtual void onInitRenderer();
    virtual void onFrame(float deltaTime);
    virtual void onResize(int width, int height);
    virtual void onDestroyRenderer();
};

} // gx_engine

#endif GAME_H
