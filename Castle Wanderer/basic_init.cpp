#include "basic_init.h"
#include "game.h"
#include <SDL_image.h>
#include <ctime>

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

TTF_Font* font = NULL;


SDL_Renderer* initSDL() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        logError(std::cerr, "Init", true);
        return nullptr;
    }
    else {
        //Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) //or "linear"
        {
            printf("Warning: Linear texture filtering not enabled!");
        }

        window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
        if (window == NULL) {
            logError(std::cerr, "Window", false);
            return nullptr;
        }
        else {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (renderer == NULL) {
                logError(std::cerr, "Render", false);
                return nullptr;
            }
            else {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                int Flag = IMG_INIT_PNG;
                if (!(IMG_Init(Flag) & Flag)) {
                    logError(std::cerr, "Init PNG", false);
                    return nullptr;
                }
                else {
                    //Initialize SDL_mixer
                    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
                    {
                        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
                    }
                }
            }
        }
    }
    //SDL_RenderSetLogicalSize(render, SCREEN_WIDTH, SCREEN_HEIGHT);
    return renderer;
}

TTF_Font* initTTF() {
    //Initialize SDL_ttf
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError();
        return nullptr;
    }
    else {
        //Open the font
        font = TTF_OpenFont(fontName.c_str(), fontSize);
        if (font == NULL)
        {
            std::cerr << "Failed to load lazy font! SDL_ttf Error: " << TTF_GetError() << std::endl;
            return nullptr;
        }
    }
    return font;
}

SDL_Texture* loadFromImage(std::string path)
{
    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        std::cerr << "Unable to load image! SDL_image Error:" << IMG_GetError() << std::endl;
    }
    else
    {
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, redColorKey, greenColorKey, blueColorKey));

        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == NULL)
        {
            std::cerr << "Unable to create texture! SDL Error: " << SDL_GetError() << std::endl;
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}

SDL_Texture* loadFromText(std::string textureText, SDL_Rect* renderText, SDL_Color textColor, Uint32 textWrapLength)
{
    SDL_Texture* text = nullptr;

    SDL_Surface* textSurface;
    if (textWrapLength > 0) {
        textSurface = TTF_RenderText_Blended_Wrapped(font, textureText.c_str(), textColor, textWrapLength);
    }
    else {
        textSurface = TTF_RenderText_Blended(font, textureText.c_str(), textColor);
    }


    if (textSurface == NULL)
    {
        std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
    }
    else
    {
        //Create texture from surface pixels
        text = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (text == NULL)
        {
            std::cerr << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << std::endl;
        }
        else
        {
            //Get image dimensions
            renderText->w = textSurface->w;
            renderText->h = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface(textSurface);
    }

    //Return success
    return text;
}

Mix_Music* loadFromMusic(std::string path) {
    Mix_Music* music = nullptr;
    music = Mix_LoadMUS(path.c_str());
    if (music == NULL)
    {
        std::cerr << "Failed to load music! SDL_mixer Error: " << Mix_GetError() << std::endl;
        std::cerr << "Music path:  " << path << std::endl;
    }
    return music;
}


void logError(std::ostream& out, const std::string& ms, bool fatal) {
    out << ms << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}

void freeTexture(SDL_Texture* texture) {
    if (texture != NULL)
    {
        SDL_DestroyTexture(texture);
        texture = NULL;
    }
}

void freeMusic(Mix_Music* music) {
    if (music != NULL)
    {
        Mix_FreeMusic(music);
        music = NULL;
    }
}

void close() {
    //Free global font
    TTF_CloseFont(font);
    font = NULL;

    //Destroy window	
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = NULL;
    renderer = NULL;

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
