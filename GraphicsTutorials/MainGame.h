//
//  MainGame.h
//  GraphicsTutorials
//
//  Created by Yvo Keuter on 9/2/15.
//  Copyright (c) 2015 blabla. All rights reserved.
//

#ifndef __GraphicsTutorials__MainGame__
#define __GraphicsTutorials__MainGame__

#include <SDL2/SDL.h>
#include <GameEngine/Sprite.h>
#include <GameEngine/GLSLProgram.h>
#include <GameEngine/GLTexture.h>
#include <GameEngine/InputManager.h>
#include <GameEngine/Timing.h>
#include <vector>
#include <GameEngine/Window.h>
#include <GameEngine/Camera2D.h>
#include <GameEngine/SpriteBatch.h>
#include "Bullet.h"

enum class GameState {PLAY, EXIT};

class MainGame {
public:
    MainGame();
    ~MainGame();
    void run();
    
private:
    void initSystems();
    void initShaders();
    void processInput();
    void gameLoop();
    void drawGame();

    GameEngine::Window _window;
    int _screenWidth;
    int _screenHeight;
    GameState _gameState;
    
    GameEngine::GLSLProgram _colorProgram;
    GameEngine::Camera2D _camera;
    
    GameEngine::SpriteBatch _spriteBatch;
    
    GameEngine::InputManager _inputManager;
    GameEngine::FpsLimiter _fpsLimiter;
    
    std::vector<Bullet> _bullets;
    
    float _time;
    float _fps;
    float _maxFPS;
};

#endif /* defined(__GraphicsTutorials__MainGame__) */
