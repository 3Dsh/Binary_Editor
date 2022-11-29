#include "EditorTools.h"

EditorTools::EditorTools(Window* window)
{
    this->window = window;

    int size = 0.05*window->getWidth();
    int y = 0.05*window->getHeight();
    openButton = new Button(window->GetRenderer(), SDL_Rect{0.05*window->getWidth(), y,size, size});

    int padding = 0.02*window->getWidth();
    x2Button = new Button(window->GetRenderer(), SDL_Rect{0.2*window->getWidth(),y,size,size});
    x8Button = new Button(window->GetRenderer(), SDL_Rect{x2Button->GetPosX()+x2Button->getWidth() + padding,y,size,size});
    x16Button = new Button(window->GetRenderer(), SDL_Rect{x8Button->GetPosX()+x8Button->getWidth() + padding,y,size,size});

    saveButton = new Button(window->GetRenderer(), SDL_Rect{0.9*window->getWidth(),y,size,size});
    createButton = new Button(window->GetRenderer(),SDL_Rect{0.48*window->getWidth(),y,size,size});

    plusButton = new Button(window->GetRenderer(),SDL_Rect{0.55*window->getWidth(),10,size*3,size});
    minusButton = new Button(window->GetRenderer(),SDL_Rect{0.55*window->getWidth(),65,size*3,size});

    panel = new Panel(window->GetRenderer());
    panel->Create(1,1,SDL_TEXTUREACCESS_TARGET);
    panel->SetRenderRect(SDL_Rect{0,0,window->getWidth(), 0.2*window->getHeight()});
    panel->SetColor(200,200,200,255);

    openButton->LoadBackImages("Images/openNormal.bmp","Images/openPressed.bmp","Images/openHighlited.bmp");
    x2Button->LoadBackImages("Images/round.bmp");
    x8Button->LoadBackImages("Images/round.bmp");
    x16Button->LoadBackImages("Images/round.bmp");
    saveButton->LoadBackImages("Images/saveNormal.bmp","Images/savePressed.bmp","Images/saveHighlited.bmp");
    createButton->LoadBackImages("Images/createNormal.bmp","Images/createPressed.bmp","Images/createHighlited.bmp");
    plusButton->LoadBackImages("Images/addNormal.bmp","Images/addPressed.bmp","Images/addHighlited.bmp");
    minusButton->LoadBackImages("Images/deleteNormal.bmp","Images/deletePressed.bmp","Images/deleteHighlited.bmp");

    SDL_Color normalColor = {143,135,255,255};
    SDL_Color pressedColor = {204,122,255,255};
    SDL_Color highlitedColor = {152,111,232,255};
    TTF_Font* font = TTF_OpenFont("Fonts/FreeMono.ttf", 32);

    x2Button->LoadText(normalColor, pressedColor, highlitedColor, "x2", font);
    x8Button->LoadText(normalColor, pressedColor, highlitedColor, "x8", font);
    x16Button->LoadText(normalColor, pressedColor, highlitedColor, "x16", font);

    TTF_CloseFont(font);
    font = nullptr;
    fileName = nullptr;
    cType = CodeType::x2;
}

EditorTools::~EditorTools()
{
    //dtor
}

void EditorTools::Show(){
    panel->Render();
    openButton->Render();
    x2Button->Render();
    x8Button->Render();
    x16Button->Render();
    saveButton->Render();
    createButton->Render();
    plusButton->Render();
    minusButton->Render();

    if (fileName) fileName->Render();
}

void EditorTools::SetTextColor(bool saved){
    if (!fileName) return;
    SDL_Color color;
    if (saved) color = {144,255,118,255};
    else color = {255,116,71,255};

    int len = strlen(fileName->GetText());
    char* text = new char[len+1];
    strcpy(text,fileName->GetText());

    TTF_Font *font = TTF_OpenFont("Fonts/FreeMono.ttf",32);
    fileName->Load(text, color, font);
    fileName->ChangeHeightProportionally(panel->getHeight()/2);
    TTF_CloseFont(font);
    font = nullptr;
}

void EditorTools::SetText(char* text){
    SDL_Rect* fileNameRect;
    TTF_Font* font = TTF_OpenFont("Fonts/FreeMono.ttf",32);
    fileName = new Text(window->GetRenderer());
    fileName->Load(text, SDL_Color{144,255,118,255}, font);
    fileName->ChangeHeightProportionally(panel->getHeight()/2);
    if (fileName->getWidth() > 0.15*window->getWidth()) fileNameRect = new SDL_Rect{0,0,0.18*window->getWidth(), fileName->getHeight()};
    else fileNameRect = nullptr;
    fileName->SetScrollRect(fileNameRect);
    fileName->SetRenderPosition(0.7*panel->getWidth(), 30);
    TTF_CloseFont(font);
}

bool EditorTools::EventHandler(SDL_Event e){
    if (openButton->EventHandler(e) == Button::Event::LB_Up) State::curState = State::ChoiseFileMenu;

    if (x2Button->EventHandler(e) == Button::Event::LB_Up) {
        if (cType == CodeType::x2) return false;
        cType = CodeType::x2;
        return true;
    }
    if (x8Button->EventHandler(e) == Button::Event::LB_Up){
        if (cType == CodeType::x8) return false;
        cType = CodeType::x8;
        return true;
    }
    if (x16Button->EventHandler(e) == Button::Event::LB_Up){
        if (cType == CodeType::x16) return false;
        cType = CodeType::x16;
        return true;
    }
    if (saveButton->EventHandler(e) == Button::Event::LB_Up){
        State::curState = State::SaveState;
    }
    if (createButton->EventHandler(e) == Button::Event::LB_Up){
        State::curState = State::CreateState;
    }
    if (plusButton->EventHandler(e) == Button::Event::LB_Up){
        State::curState = State::AddByte;
    }
    if (minusButton->EventHandler(e) == Button::Event::LB_Up){
        State::curState = State::DeleteByte;
    }

    return false;
}

