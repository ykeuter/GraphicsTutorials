//
//  MainGame.cpp
//  GraphicsTutorials
//
//  Created by Yvo Keuter on 9/2/15.
//  Copyright (c) 2015 blabla. All rights reserved.
//

#include "MainGame.h"
#include <iostream>
#include <string>
#include <OpenGL/gl3.h>
#include <GameEngine/GameEngine.h>
#include <GameEngine/GameEngineErrors.h>
#include <GameEngine/ResourceManager.h>

MainGame::MainGame() : _screenWidth(1024), _screenHeight(768), _time(0.0f), _gameState(GameState::PLAY),     _maxFPS(60.0f)
{
    _camera.init(_screenWidth, _screenHeight);
}

MainGame::~MainGame() {
    
}

void MainGame::run() {
    initSystems();

    gameLoop();
}

void MainGame::initSystems() {
    GameEngine::init();
    
    _window.create("Game Engine", _screenWidth, _screenHeight, 0);
    _spriteBatch.init();
    
    initShaders();
    _fpsLimiter.init(_maxFPS);
}

void MainGame::processInput(){
    SDL_Event evnt;
    
    const float CAMERA_SPEED = 2.0f;
    const float SCALE_SPEED = 0.1f;
    
    //Will keep looping until there are no more events to process
    while (SDL_PollEvent(&evnt)) {
        switch (evnt.type) {
            case SDL_QUIT:
                _gameState = GameState::EXIT;
                break;
            case SDL_MOUSEMOTION:
                _inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
                break;
            case SDL_KEYDOWN:
                _inputManager.pressKey(evnt.key.keysym.sym);
                break;
            case SDL_KEYUP:
                _inputManager.releaseKey(evnt.key.keysym.sym);
                break;
            case SDL_MOUSEBUTTONDOWN:
                _inputManager.pressKey(evnt.button.button);
                break;
            case SDL_MOUSEBUTTONUP:
                _inputManager.releaseKey(evnt.button.button);
                break;
        }
    }
    
    if (_inputManager.isKeyDown(SDLK_w)) {
        _camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
    }
    if (_inputManager.isKeyDown(SDLK_s)) {
        _camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
    }
    if (_inputManager.isKeyDown(SDLK_a)) {
        _camera.setPosition(_camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
    }
    if (_inputManager.isKeyDown(SDLK_d)) {
        _camera.setPosition(_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
    }
    if (_inputManager.isKeyDown(SDLK_q)) {
        _camera.setScale(_camera.getScale() + SCALE_SPEED);
    }
    if (_inputManager.isKeyDown(SDLK_e)) {
        _camera.setScale(_camera.getScale() - SCALE_SPEED);
    }
    
    if (_inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {
        glm::vec2 mouseCoords = _inputManager.getMouseCoords();
        mouseCoords = _camera.convertScreenToWorld(mouseCoords);
        
        glm::vec2 playerPosition(0.0f);
        glm::vec2 direction = mouseCoords - playerPosition;
        direction = glm::normalize(direction);
        
        _bullets.emplace_back(playerPosition, direction, 5.00f, 1000);
    }
}

void MainGame::gameLoop() {
    while (_gameState != GameState::EXIT) {
        _fpsLimiter.begin();
        processInput();
        _time += .01;
        _camera.update();
        
        // Update all bullets
        for (int i = 0; i < _bullets.size();) {
            if (_bullets[i].update() == true) {
                _bullets[i] = _bullets.back();
                _bullets.pop_back();
            } else {
                i++;
            }
        }
        
        drawGame();
        _fps = _fpsLimiter.end();
        //print only once every 10 frames
        static int frameCounter = 0;
        frameCounter++;
        if (frameCounter == 10000) {
            std::cout << _fps << std::endl;
            frameCounter = 0;
        }
    }
}

void MainGame::initShaders() {
    _colorProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
    _colorProgram.addAttribute("vertexPosition");
    _colorProgram.addAttribute("vertexColor");
    _colorProgram.addAttribute("vertexUV");
    _colorProgram.linkShaders();
}

void MainGame::drawGame() {
    glClearDepth(1.0);
    //Clear the color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    _colorProgram.use();
    
    //We are using texture unit 0
    glActiveTexture(GL_TEXTURE0);

    
    //Get the uniform location
    GLint textureLocation = _colorProgram.getUniformLocation("mySampler");
    //Tell the shader that the texture is in texture unit 0
    glUniform1i(textureLocation, 0);
    
    //Set the camera matrix
    GLint pLocation = _colorProgram.getUniformLocation("P");
    glm::mat4 cameraMatrix = _camera.getCameraMatrix();
    
    glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));
    
    _spriteBatch.begin();
    
    glm::vec4 pos(0.0f, 0.0f, 50.0f, 50.0f);
    glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
    static GameEngine::GLTexture texture = GameEngine::ResourceManager::getTexture("Textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");
    GameEngine::ColorRGBA8 color;
    color.r = 255;
    color.g = 255;
    color.b = 255;
    color.a = 255;
    
    _spriteBatch.draw(pos, uv, texture.id, 0.0f, color);

    for (int i = 0; i < _bullets.size(); i++) {
        _bullets[i].draw(_spriteBatch);
    }
    
    _spriteBatch.end();
    
    _spriteBatch.renderBatch();
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    _colorProgram.unuse();
    
    //Swap our buffer and draw everything to the screen!
    _window.swapBuffer();
}