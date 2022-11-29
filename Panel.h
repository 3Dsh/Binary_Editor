#ifndef PANEL_H
#define PANEL_H

#include "Texture.h"

class Panel : public Texture
{
    public:
        Panel(SDL_Renderer* renderer);
        ~Panel();

        bool Create(int width, int height, SDL_TextureAccess access);
        void Render(int xOffcet = 0, int yOffcet = 0) override;

        void SetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
};

#endif // PANEL_H
