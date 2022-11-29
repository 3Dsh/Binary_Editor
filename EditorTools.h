#ifndef EDITORTOOLS_H
#define EDITORTOOLS_H

#include "Window.h"
#include "GameState.h"
#include "Button.h"
#include "Panel.h"
#include "InputDialogWindow.h"
#include <cmath>

enum CodeType{
    x2, x8, x16,
};

class EditorTools
{
    public:
        EditorTools(Window* window);
        ~EditorTools();

        void Show();
        bool EventHandler(SDL_Event e);
        void SetText(char* fileName);
        void SetTextColor(bool saved);

        CodeType GetCodType(){ return cType; }
    private:
        CodeType cType;
        Button* openButton, *x2Button, *x8Button, *x16Button;
        Button* saveButton, *createButton;
        Button* plusButton, *minusButton;
        Text* fileName;
        Panel* panel;
        Window* window;
};

class Caret{
    public:
        Caret(Window* window){
            caret = new Panel(window->GetRenderer());
            caret->Create(1,1,SDL_TEXTUREACCESS_TARGET);
            caret->SetColor(255,255,255,255);
            caret->SetRenderRect(SDL_Rect{0,0,0,0});

            area = SDL_Rect{0,0,0,0};
            count = 0;
            isActive = false;
        }
        ~Caret(){

        }

        void Show(int offcetX, int offcetY){
            caret->Render(0,offcetY);
        }

        void SetParent(SDL_Rect area, int count, int ind, bool leftSide){
            this->area = area;
            this->ind = ind;
            this->count = count;
            offcet = area.w / count;
            isActive = true;

            if (leftSide) caret->SetRenderRect(SDL_Rect{area.x,area.y,2,area.h});
            else caret->SetRenderRect(SDL_Rect{area.x + count*offcet,area.y,2,area.h});
        }

        int EventHandler(SDL_Event e){
            if (!count) return 0;
            if (e.type == SDL_KEYDOWN){
                if (e.key.keysym.sym == SDLK_RIGHT){
                    if (caret->GetPosX()+offcet <= area.x+area.w)
                        caret->SetRenderPosition(caret->GetPosX()+offcet,area.y);
                    else return 1;
                }
                else if (e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_BACKSPACE){
                    if (caret->GetPosX() - offcet >= area.x)
                        caret->SetRenderPosition(caret->GetPosX()-offcet,area.y);
                    else return -1;
                }
            }
            return 0;
        }

        void SetXPos(int x){
            int min = area.w, ind = 0;
            for (int i = 0; i < count; i++){
                if (abs(area.x + offcet*i - x) < min){
                    min = abs(area.x + offcet*i - x);
                    ind = i;
                }
            }
            caret->SetRenderPosition(caret->GetPosX() + offcet*ind,area.y);
        }

        void Reset(){
            caret->SetRenderRect(SDL_Rect{0,0,0,0});
            area = SDL_Rect{0,0,0,0};
            count = 0;
            isActive = false;
        }

        int GetOffcetInd(){
            return (caret->GetPosX() - area.x) / offcet;
        }

        bool IsActive(){
            return isActive;
        }

        int GetInd(){
            return ind;
        }

    private:
        bool isActive;
        Panel* caret;
        SDL_Rect area;
        int count, offcet, ind;
};

#endif // EDITORTOOLS_H
