#include "character.h"
#include <SDL_image.h>

MyCharacter::MyCharacter()
{
    frame = stand;
    frameTime = 0;

    charTexture = loadFromImage("image/wizardSheet.png");
    setSpriteClips();

    charNameTexture = loadFromText("Weedsycorn", &charNameRect, white);
    charNameRect.x = 82;
    charNameRect.y = 17;

    healthTexture = loadFromImage("image/health.png");
    healthRect.x = 40;
    healthRect.y = 48;
    healthRect.h = 15;

    healthBarTexture = loadFromImage("image/wizard health bar.png");
    healthBarRect.x = 10;
    healthBarRect.y = 10;
    healthBarRect.w = 420;
    healthBarRect.h = 70;

    leftmostCharacterPos = 0;
    rightmostCharacterPos = 0;
    charPosX = rand() % SCREEN_WIDTH;
    charPosY = baseGround-1;
    ground = baseGround;

    charVelocity = 0;
    charVelocityJump = 0;
    charVelX = 0;
    charVelY = 0;

    charWidth = 0;
    charHeight = 0;

    toRight = false;
    toLeft = true;
    walking = false;
    jumped = false;
    atEdgeOfScreen = false;

    damageReceived = 0;
    die = false;
}

MyCharacter::~MyCharacter()
{
    //Deallocate
    free();
}



void MyCharacter::setSpriteClips() {
    //Set sprite clips

    characterSpriteClips[walkR1].x = 24;
    characterSpriteClips[walkR1].y = 218;
    characterSpriteClips[walkR1].w = 99;
    characterSpriteClips[walkR1].h = 195;

    characterSpriteClips[walkR2].x = 126;
    characterSpriteClips[walkR2].y = 218;
    characterSpriteClips[walkR2].w = 104;
    characterSpriteClips[walkR2].h = 195;

    characterSpriteClips[walkR3].x = 238;
    characterSpriteClips[walkR3].y = 218;
    characterSpriteClips[walkR3].w = 113;
    characterSpriteClips[walkR3].h = 195;

    characterSpriteClips[walkR4].x = 360;
    characterSpriteClips[walkR4].y = 218;
    characterSpriteClips[walkR4].w = 121;
    characterSpriteClips[walkR4].h = 195;

    characterSpriteClips[walkR5].x = 482;
    characterSpriteClips[walkR5].y = 218;
    characterSpriteClips[walkR5].w = 107;
    characterSpriteClips[walkR5].h = 195;

    characterSpriteClips[walkR6].x = 589;
    characterSpriteClips[walkR6].y = 218;
    characterSpriteClips[walkR6].w = 97;
    characterSpriteClips[walkR6].h = 195;

    characterSpriteClips[walkL6].x = 10;
    characterSpriteClips[walkL6].y = 420;
    characterSpriteClips[walkL6].w = 99;
    characterSpriteClips[walkL6].h = 195;

    characterSpriteClips[walkL5].x = 113;
    characterSpriteClips[walkL5].y = 420;
    characterSpriteClips[walkL5].w = 107;
    characterSpriteClips[walkL5].h = 195;

    characterSpriteClips[walkL4].x = 235;
    characterSpriteClips[walkL4].y = 420;
    characterSpriteClips[walkL4].w = 115;
    characterSpriteClips[walkL4].h = 195;

    characterSpriteClips[walkL3].x = 358;
    characterSpriteClips[walkL3].y = 420;
    characterSpriteClips[walkL3].w = 105;
    characterSpriteClips[walkL3].h = 195;

    characterSpriteClips[walkL2].x = 482;
    characterSpriteClips[walkL2].y = 420;
    characterSpriteClips[walkL2].w = 100;
    characterSpriteClips[walkL2].h = 195;

    characterSpriteClips[walkL1].x = 600;
    characterSpriteClips[walkL1].y = 420;
    characterSpriteClips[walkL1].w = 100;
    characterSpriteClips[walkL1].h = 195;

    characterSpriteClips[stand].x = 0;
    characterSpriteClips[stand].y = 0;
    characterSpriteClips[stand].w = 110;
    characterSpriteClips[stand].h = 195;

    characterSpriteClips[attack].x = 280;
    characterSpriteClips[attack].y = 622;
    characterSpriteClips[attack].w = 195;
    characterSpriteClips[attack].h = 195;

}

void MyCharacter::renderCharacter(SDL_Renderer* renderer, SDL_Rect* clip)
{
    if (clip != NULL)
    {
        charWidth = clip->w;
        charHeight = clip->h;
    }

    SDL_Rect renderQuad = { charPosX, charPosY, charWidth, charHeight };

    //Render character to screen
    if (frame == attack && toLeft == true) {
        SDL_RenderCopyEx(renderer, charTexture, clip, &renderQuad, 0, NULL, SDL_FLIP_HORIZONTAL);
    }
    else {
        SDL_RenderCopy(renderer, charTexture, clip, &renderQuad);
    }
}

void MyCharacter::renderHealthBar(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, charNameTexture, NULL, &charNameRect);

    healthRect.w = 380 - damageReceived * 380 / health;
    SDL_RenderCopy(renderer, healthTexture, NULL, &healthRect);

    SDL_RenderCopy(renderer, healthBarTexture, NULL, &healthBarRect);
}

void MyCharacter::renderCurrentAction(SDL_Renderer* renderer, unsigned int currentTime) {
    if (walking && toRight) {
        if (currentTime > frameTime + nextFrameTime) {
            frame++;
            frameTime = currentTime;
            if (frame > walkR6)
            {
                frame = walkR1;
            }
        }
    }
    else if (walking && toLeft) {
        if (currentTime > frameTime + nextFrameTime) {
            frame++;
            frameTime = currentTime;
            if (frame<walkL1 || frame>walkL6)
            {
                frame = walkL1;
            }
        }
    }
    else if (!walking && toLeft) {
        if (currentTime > frameTime + nextFrameTime) {
            frame = walkL6;
            frameTime = currentTime;
        }
    }
    else if (!walking && toRight) {
        if (currentTime > frameTime + nextFrameTime) {
            frame = walkR6;
            frameTime = currentTime;
        }
    }

    SDL_Rect* currentClip = &characterSpriteClips[frame];
    renderCharacter(renderer, currentClip);

    renderHealthBar(renderer);
}



void MyCharacter::handleEvent(SDL_Event& e, unsigned int currentTime)
{
    //If a key was pressed
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        //Adjust the velocity
        switch (e.key.keysym.sym) {

        case SDLK_d:
            toRight = true;
            toLeft = false;
            charVelX += charVelocity;
            break;

        case SDLK_a:
            toRight = false;
            toLeft = true;
            charVelX -= charVelocity;
            break;

        case SDLK_w:
            if (charVelY == 0) {
                charVelY = -charVelocity;
            }
            break;
        case SDLK_s:
            if (charVelY == 0) {
                charVelY = charVelocity;
            }
            break;

        case SDLK_SPACE:
            if (!jumped) {
                charVelY =- charVelocityJump;
                ground = charPosY;
                jumped = true;
            }
            damageReceived -= 21;
            if (damageReceived < 0) {
                damageReceived = 0;
            }
            break;

        case SDLK_f:

            //Change to attack frame
            frame = attack;
            frameTime = currentTime + nextFrameTime;

            //Move back a bit because of the spell's rebound
            if (toRight) {
                charPosX -=3;
            }
            else if (toLeft) {
                charPosX +=3;
            }
            break;
        }
    }

    //If a key was released
    else if (e.type == SDL_KEYUP && e.key.repeat == 0)
    {
        //Adjust the velocity
        switch (e.key.keysym.sym) {

        case SDLK_d:
            charVelX -= charVelocity;
            break;

        case SDLK_a:
            charVelX += charVelocity;
            break;

        case SDLK_w:
            if (charVelY == -charVelocity) {
                charVelY = 0;
            }
            break;

        case SDLK_s:
            if (charVelY == charVelocity) {
                charVelY = 0;
            }
            break;

        case SDLK_f:
            //Move back a bit because of the spell's rebound
            if (toRight) {
                charPosX ++;
            }
            else if (toLeft) {
                charPosX --;
            }
            break;
        }
    }
}

void MyCharacter::move()
{
    if (charVelX != 0) {
        charPosX += charVelX;
        walking = true;
    }

    if (charVelY != 0) {
        charPosY += charVelY;
        walking = true;
    }

    checkCharLimits();
}

void MyCharacter::checkCharLimits() {
    //If went too far to the right or left
    if (charPosX < leftmostCharacterPos)
    {
        charPosX += charVelocity;
        atEdgeOfScreen = true;
    }
    else if (charPosX > rightmostCharacterPos) {
        charPosX -= charVelocity;
        atEdgeOfScreen = true;
    }
    else atEdgeOfScreen = false;

    //If jumped too far up
    if (jumped && charPosY < ground - jumpHeight) {
        charVelY = charVelocityJump;
    }
    //If jumped and fell back to where standing
    if (jumped && charPosY > ground) {
        charVelY = 0;
        charPosY = ground;
        jumped = false;
    }

    //If walked too far up
    if (walking && !jumped && charPosY < walkLimit) {
        charPosY = walkLimit;
    }
    //If walked too far down
    if (charPosY > baseGround) {
        charPosY = baseGround - 1;
    }

    //If not walking anywhere
    if (charVelX == 0 && charVelY != charVelocity) {
        walking = false;
    }
}

void MyCharacter::setFurthestPoints(int furthestLeftPoint, int furthestRightPoint) {
    leftmostCharacterPos = furthestLeftPoint;
    rightmostCharacterPos = furthestRightPoint;
}



bool MyCharacter::isAtEdgeOfScreen() {
    return atEdgeOfScreen;
}

bool MyCharacter::isToRight() {
    return toRight;
}



void MyCharacter::receiveDamage(int damage) {
    damageReceived += damage;
}



int MyCharacter::getCharPosX() {
    return charPosX;
}
int MyCharacter::getCharPosY() {
    return charPosY;
}

void MyCharacter::setVelocity(int BGVelocity) {
    charVelocity = BGVelocity;
    charVelocityJump = charVelocity * 4;
}

int MyCharacter::getCharVelX() {
    return charVelX;
}
int MyCharacter::getCharVelY() {
    return charVelY;
}

int MyCharacter::getWidth() {
    return charWidth;
}
int MyCharacter::getHeight() {
    return charHeight;
}


void MyCharacter::free()
{
    freeTexture(charNameTexture);
    freeTexture(healthBarTexture);
    freeTexture(healthTexture);
    freeTexture(charTexture);
}