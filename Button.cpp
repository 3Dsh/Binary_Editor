#include "Button.h"

Button::Button(SDL_Renderer* renderer, SDL_Rect rect) : RendererObject(renderer){
    xPos = rect.x;
    yPos = rect.y;
    mWidth = rect.w;
    mHeight = rect.h;
    normalText = pressedText = highlightedText = singleText = curText = nullptr;
    normalImage = pressedImage = highlightedImage = singleImage = curImage = nullptr;
}

Button::Event Button::EventHandler(SDL_Event& e) {
    if (e.button.x > xPos && e.button.x <= xPos + mWidth &&
        e.button.y > yPos && e.button.y <= yPos + mHeight){
        if (e.type == SDL_MOUSEBUTTONUP) {
            SetHighlightedColor();
            return e.button.button == SDL_BUTTON_RIGHT ? Event::RB_Up : Event::LB_Up;
        }
        else if (e.button.button == SDL_BUTTON_LEFT){
            SetPressedColor();
            return e.type == SDL_MOUSEBUTTONDOWN ? Event::LB_Down : Event::MouseMotion;
        }
        else if (e.button.button == SDL_BUTTON_RIGHT){
            return e.type == SDL_MOUSEBUTTONDOWN ? Event::RB_Down : Event::MouseMotion;
        }
        else {
            SetHighlightedColor();
            return Event::MouseMotion;
        }
    }
    else {
        SetNormalColor();
        return Event::None;
    }
}

void Button::SetNormalColor() {
    if (!singleText)
        curText = normalText;
    if (!singleImage)
        curImage = normalImage;
}

void Button::SetHighlightedColor() {
    if (!singleText)
        curText = highlightedText;
    if (!singleImage)
        curImage = highlightedImage;
}

void Button::SetPressedColor() {
    if (!singleText)
        curText = pressedText;
    if (!singleImage)
        curImage = pressedImage;
}

void Button::LoadBackImages(char *normalImagePath, char *pressedImagePath, char *highlightedImagePath) {
    normalImage = new Image(renderer, normalImagePath);
    normalImage->SetRenderRect(SDL_Rect{xPos,yPos,mWidth,mHeight});

    pressedImage = new Image(renderer, pressedImagePath);
    pressedImage->SetRenderRect(SDL_Rect{xPos,yPos,mWidth,mHeight});

    highlightedImage = new Image(renderer, highlightedImagePath);
    highlightedImage->SetRenderRect(SDL_Rect{xPos,yPos,mWidth,mHeight});

    curImage = normalImage;
}

void Button::LoadBackImages(char* imagePath){
    singleImage = new Image(renderer, imagePath);
    singleImage->SetRenderRect(SDL_Rect{xPos,yPos,mWidth,mHeight});

    curImage = singleImage;
}

void Button::CropImages(float x0Prec, float y0Prec, float x1Prec, float y1Prec){
    normalImage->Crop(x0Prec, y0Prec, x1Prec, y1Prec);
    pressedImage->Crop(x0Prec, y0Prec, x1Prec, y1Prec);
    highlightedImage->Crop(x0Prec, y0Prec, x1Prec, y1Prec);
}

void Button::LoadText(SDL_Color color, const char* text, TTF_Font *font, bool scroll){
    float oW, oH, oMin;
    float wMax = 0.8 * mWidth;
    float hMax = 0.8 * mHeight;

    singleText = new Text(renderer);
    singleText->Load(text,color,font);

    SDL_Rect rect = SDL_Rect{xPos,yPos,singleText->getWidth(),singleText->getHeight()};
    oW = (float)wMax / singleText->getWidth();
    oH = (float)hMax / singleText->getHeight();
    oMin = oW < oH ? oW : oH;
    rect.w = oMin*singleText->getWidth();
    rect.h = oMin*singleText->getHeight();
    rect.x += (mWidth - rect.w)/2;
    rect.y += (mHeight - rect.h)/2;
    singleText->SetRenderRect(rect);

    if (scroll && rect.h < 0.3*mHeight){
        singleText->ChangeHeightProportionally(0.4*mHeight);
        singleText->SetScrollRect(new SDL_Rect{0,0,wMax, singleText->getHeight()});
        singleText->SetRenderPosition(xPos + (mWidth - wMax)/2, yPos + (mHeight - singleText->getHeight())/2);
    }
    curText = singleText;
}

void Button::LoadText(SDL_Color normalTextColor, SDL_Color pressedTextColor, SDL_Color highlightedTextColor, const char *text, TTF_Font *font) {

    float oW, oH, oMin;
    float wMax = 0.8 * mWidth;
    float hMax = 0.8 * mHeight;

    if (normalText)
        delete(normalText);
    if (pressedText)
        delete(pressedText);
    if (highlightedText)
        delete(highlightedText);

    normalText = new Text(renderer);
    normalText->Load(text,normalTextColor,font);
    SDL_Rect rect = SDL_Rect{xPos,yPos,normalText->getWidth(),normalText->getHeight()};
    oW = (float)wMax / normalText->getWidth();
    oH = (float)hMax / normalText->getHeight();
    oMin = oW < oH ? oW : oH;
    rect.w = oMin*normalText->getWidth();
    rect.h = oMin*normalText->getHeight();
    rect.x += (mWidth - rect.w)/2;
    rect.y += (mHeight - rect.h)/2;
    normalText->SetRenderRect(rect);

    pressedText = new Text(renderer);
    pressedText->Load(text,pressedTextColor,font);
    rect = SDL_Rect{xPos,yPos,pressedText->getWidth(),pressedText->getHeight()};
    oW = (float)wMax / pressedText->getWidth();
    oH = (float)hMax / pressedText->getHeight();
    oMin = oW < oH ? oW : oH;
    rect.w = oMin*pressedText->getWidth();
    rect.h = oMin*pressedText->getHeight();
    rect.x += (mWidth - rect.w)/2;
    rect.y += (mHeight - rect.h)/2;
    pressedText->SetRenderRect(rect);

    highlightedText = new Text(renderer);
    highlightedText->Load(text,highlightedTextColor,font);
    rect = SDL_Rect{xPos,yPos,highlightedText->getWidth(),highlightedText->getHeight()};
    oW = (float)wMax / highlightedText->getWidth();
    oH = (float)hMax / highlightedText->getHeight();
    oMin = oW < oH ? oW : oH;
    rect.w = oMin*highlightedText->getWidth();
    rect.h = oMin*highlightedText->getHeight();
    rect.x += (mWidth - rect.w)/2;
    rect.y += (mHeight - rect.h)/2;
    highlightedText->SetRenderRect(rect);

    curText = normalText;
}

void Button::SetRenderPosition(int x, int y){
    SetRenderRect(SDL_Rect{x,y,mWidth,mHeight});
}

void Button::SetRenderRect(SDL_Rect rect){
    xPos = rect.x;
    yPos = rect.y;
    mWidth = rect.w;
    mHeight = rect.h;

    if (normalImage) normalImage->SetRenderRect(SDL_Rect{xPos,yPos,mWidth,mHeight});
    if (pressedImage) pressedImage->SetRenderRect(SDL_Rect{xPos,yPos,mWidth,mHeight});
    if (highlightedImage) highlightedImage->SetRenderRect(SDL_Rect{xPos,yPos,mWidth,mHeight});
    if (singleImage) singleImage->SetRenderRect(SDL_Rect{xPos,yPos,mWidth,mHeight});

    rect.x += 0.1*mWidth;
    rect.y += 0.1*mHeight;
    rect.h *= 0.8;
    rect.w *= 0.8;

    if (normalText) normalText->SetRenderRect(rect);
    if (pressedText) pressedText->SetRenderRect(rect);
    if (highlightedText) highlightedText->SetRenderRect(rect);
    if (singleText) singleText->SetRenderRect(rect);
}

void Button::Render( int offcetX, int offcetY ) {

    if (curImage) curImage->Render(offcetX,offcetY);
    if (curText) curText->Render(offcetX, offcetY);
}

Button::~Button(){
    SDL_Log("Button dtor\t%x",this);
    if (normalText){
        delete(normalText);
        normalText = nullptr;
    }
    if (highlightedText){
        delete(highlightedText);
        highlightedText = nullptr;
    }
    if (pressedText){
        delete(pressedText);
        pressedText = nullptr;
    }
    if (singleText){
        delete(singleText);
        singleText = nullptr;
    }

    if (normalImage){
        delete(normalImage);
        normalImage = nullptr;
    }
    if (highlightedImage){
        delete(highlightedImage);
        highlightedImage = nullptr;
    }
    if (pressedImage){
        delete(pressedImage);
        pressedImage = nullptr;
    }
    if (singleImage){
        delete(singleImage);
        singleImage = nullptr;
    }

    curImage = nullptr;
    curText = nullptr;
}
