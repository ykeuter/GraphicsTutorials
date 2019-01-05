//
//  Bullet.h
//  GraphicsTutorials
//
//  Created by Yvo Keuter on 9/9/15.
//  Copyright (c) 2015 blabla. All rights reserved.
//

#ifndef __GraphicsTutorials__Bullet__
#define __GraphicsTutorials__Bullet__

#include <glm/glm.hpp>
#include <GameEngine/SpriteBatch.h>

class Bullet
{
public:
    Bullet(glm::vec2 pos, glm::vec2 dir, float speed, int lifeTime);
    ~Bullet();
    
    void draw(GameEngine::SpriteBatch& spriteBatch);
    // Returns true when we are out of life
    bool update();
    
private:
    int _lifeTime;
    float _speed;
    glm::vec2 _direction;
    glm::vec2 _position;
};

#endif /* defined(__GraphicsTutorials__Bullet__) */
