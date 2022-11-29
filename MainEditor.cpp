#include "MainEditor.h"

MainEditor::MainEditor(Window* window) : helper(window), caret(window)
{
    tools = new EditorTools(window);
    vScroll = new Scrollbar(window, ScrollType::S_Vertical, &scrollVal, SDL_Rect{0.95*window->getWidth(), 0.2*window->getHeight(), 0.025*window->getWidth(), 0.8*window->getHeight()});
    this->window = window;

    arr = nullptr;
    binaryText = nullptr;
    scrollVal = 0;
    path = nullptr;
}

void MainEditor::Show(){

    if (binaryText){
        for (int i = 0; i < size; i++){
            if (binaryText[i])
                binaryText[i]->Render(0,-scrollVal);
        }
    }
    caret.Show(0,-scrollVal);

    tools->Show();
    vScroll->Show();
}
void MainEditor::EventHandler(SDL_Event e){
    if (tools->EventHandler(e)){
        if (path) LoadTexture();
    }
    vScroll->EventHandler(e);

    if (!binaryText) return;
    for (int i = 0; i < size; i++){
        if (binaryText[i] && e.type == SDL_MOUSEBUTTONDOWN)
            if (e.button.x >= binaryText[i]->GetPosX() && e.button.x <= binaryText[i]->GetPosX()+binaryText[i]->getWidth() &&
                e.button.y + scrollVal >= binaryText[i]->GetPosY() && e.button.y + scrollVal <= binaryText[i]->GetPosY()+binaryText[i]->getHeight())
                {
                    caret.SetParent(binaryText[i]->GetRect(), tools->GetCodType() == CodeType::x2 ? 8 : tools->GetCodType() == CodeType::x8 ? 3 : 2, i, true);
                    caret.SetXPos(e.button.x);
                }
    }

    // delete
    if (caret.IsActive()){
        if (e.key.keysym.sym == SDLK_BACKSPACE && e.type == SDL_KEYDOWN){
            int ind = caret.GetInd(), byte = caret.GetOffcetInd();
            char newC;
            SDL_Color color = {255,255,255,255};
            TTF_Font* font = TTF_OpenFont("Fonts/FreeMono.ttf", 32);
            switch(tools->GetCodType()){
                case CodeType::x2:
                    newC = helper.Deletex2(arr[ind], byte);
                    binaryText[ind]->Load(helper.Getx2Code(newC), color, font);
                    break;
                case CodeType::x8:
                    newC = helper.Deletex8(arr[ind], byte);
                    binaryText[ind]->Load(helper.Getx8Code(newC), color, font);
                    break;
                case CodeType::x16:
                    newC = helper.Deletex16(arr[ind], byte);
                    binaryText[ind]->Load(helper.Getx16Code(newC), color, font);
                    break;
            }
            arr[ind] = newC;

            TTF_CloseFont(font);
            font = nullptr;
        }
    }

    // Add
    if (caret.IsActive()){
        if (e.type == SDL_KEYDOWN && (e.key.keysym.sym >= SDLK_0 && e.key.keysym.sym <= SDLK_9 || e.key.keysym.sym >= SDLK_a && e.key.keysym.sym <= SDLK_f)){
            int ind = caret.GetInd(), byte = caret.GetOffcetInd();
            char newC;
            SDL_Color color = {255,255,255,255};
            TTF_Font* font = TTF_OpenFont("Fonts/FreeMono.ttf", 32);
            switch(tools->GetCodType()){
                case CodeType::x2:
                    if (e.key.keysym.sym < SDLK_0 || e.key.keysym.sym > SDLK_1) break;
                    newC = helper.Addx2(arr[ind], byte,e.key.keysym.sym );
                    binaryText[ind]->Load(helper.Getx2Code(newC), color, font);
                    if (arr[ind] != newC) tools->SetTextColor(false);
                    arr[ind] = newC;
                    e.key.keysym.sym = SDLK_RIGHT;
                    break;
                case CodeType::x8:
                    if (e.key.keysym.sym < SDLK_0 || e.key.keysym.sym > SDLK_7) break;
                    newC = helper.Addx8(arr[ind], byte, e.key.keysym.sym );
                    binaryText[ind]->Load(helper.Getx8Code(newC), color, font);
                    if (arr[ind] != newC) tools->SetTextColor(false);
                    arr[ind] = newC;
                    e.key.keysym.sym = SDLK_RIGHT;
                    break;
                case CodeType::x16:
                    if ((e.key.keysym.sym < SDLK_0 || e.key.keysym.sym > SDLK_9) && (e.key.keysym.sym < SDLK_a || e.key.keysym.sym > SDLK_f)) break;
                    newC = helper.Addx16(arr[ind], byte, e.key.keysym.sym );
                    binaryText[ind]->Load(helper.Getx16Code(newC), color, font);
                    if (arr[ind] != newC) tools->SetTextColor(false);
                    arr[ind] = newC;
                    e.key.keysym.sym = SDLK_RIGHT;
                    break;
            }

            TTF_CloseFont(font);
            font = nullptr;
        }
    }

    char t;
    if (t = caret.EventHandler(e)){
        if (caret.GetInd()+t < size && caret.GetInd()+t >= 0)
        {
            int max = tools->GetCodType() == CodeType::x2 ? 8 : tools->GetCodType() == CodeType::x8 ? 3 : 2;
            caret.SetParent(binaryText[caret.GetInd()+t]->GetRect(),max,caret.GetInd()+t, t > 0);
        }
    }
}

MainEditor::~MainEditor()
{
    if (arr){
        delete[] arr;
        arr = nullptr;
    }
    if (tools){
        delete(tools);
        tools = nullptr;
    }
}

void MainEditor::LoadFile(char* path){
    this->path = path;
    if (binaryText){
        for (int i = 0; i < size; i++){
            if (binaryText[i]){
                delete(binaryText[i]);
                binaryText[i] = nullptr;
            }
        }
        binaryText = nullptr;
    }

    SDL_RWops* file = SDL_RWFromFile(path, "rw+");
    if (!file){
        SDL_Log("Coudnt open file %s", path);
        return;
    }
    size = SDL_RWsize(file);

    if (arr){
        delete[]arr;
        arr = nullptr;
    }

    arr = new char[size];
    SDL_RWread(file, arr, sizeof(char), size);
    SDL_RWclose(file);

    LoadTexture();

    int i = 0;
    for (; path[i] != '/'; i++);
    tools->SetText(path+i+1);
}

void MainEditor::LoadTexture(){
    if (binaryText){
        for (int i = 0; i < size; i++){
            if (binaryText[i]){
                delete(binaryText[i]);
                binaryText[i] = nullptr;
            }
        }
        binaryText = nullptr;
    }

    binaryText = new Text*[size];
    int h;

    switch(tools->GetCodType()){
        case CodeType::x2:
            h = helper.Createx2Texture(arr,size,binaryText);
            break;
        case CodeType::x8:
            h = helper.Createx8Texture(arr,size,binaryText);
            break;
        case CodeType::x16:
            h = helper.Createx16Texture(arr,size,binaryText);
            break;
    }

    vScroll->SetScrollRange(0,h-0.8*window->getHeight());

    caret.Reset();
    scrollVal = 0;
}

void MainEditor::AddByte(){
    if (!path) return;
    tools->SetTextColor(false);

    char* newArr = new char[size+1];
    Text** newText = new Text*[size+1];

    for (int i = 0; i < size; i++){
        newArr[i] = arr[i];
        newText[i] = binaryText[i];
    }

    newArr[size] = 0;
    newText[size] = new Text(window->GetRenderer());

    SDL_Color color = {255,255,255,255};
    TTF_Font* font = TTF_OpenFont("Fonts/FreeMono.ttf",32);
    int h = 0;
    switch(tools->GetCodType()){
        case CodeType::x2:
            newText[size]->Load("00000000", color, font);
            newText[size]->SetRenderPosition(size%3==0 ? 0.2*window->getWidth() : size%3==1 ? 0.4*window->getWidth() : 0.6*window->getWidth(),h = 0.25*window->getHeight() + size/3 * 30);
            break;
        case CodeType::x8:
            newText[size]->Load("000", color, font);
            newText[size]->SetRenderPosition(200 + ((size%7) )*120, h = 0.25*window->getHeight() + (size/7 + 1)*30);
            break;
        case CodeType::x16:
            newText[size]->Load("00", color, font);
            newText[size]->SetRenderPosition(200 + ((size%9) )*100, h = 0.25*window->getHeight() + (size/9 + 1)*30);
            break;
    }
    TTF_CloseFont(font);

    size++;
    delete[]arr;
    delete[]binaryText;

    arr = newArr;
    binaryText = newText;

    vScroll->SetScrollRange(0,h-0.8*window->getHeight());
    vScroll->Shift(0, window->getHeight());

    caret.Reset();
}
void MainEditor::DeleteByte(){
    if (!path || size == 0) return;
    tools->SetTextColor(false);

    size--;
    delete(binaryText[size]);

    int h = 0;
    switch(tools->GetCodType()){
        case CodeType::x2:
            h = 0.25*window->getHeight() + size/3 * 30;
            break;
        case CodeType::x8:
            h = 0.25*window->getHeight() + (size/7 + 1)*30;
            break;
        case CodeType::x16:
            h = 0.25*window->getHeight() + (size/9 + 1)*30;
            break;
    }

    vScroll->SetScrollRange(0,h-0.8*window->getHeight());
    vScroll->Shift(0, window->getHeight());
    caret.Reset();
}

void MainEditor::SaveFile(){
    if (!path) return;
    tools->SetTextColor(true);

    remove(path);

    SDL_Log("Path %s", path);
    SDL_RWops* file = SDL_RWFromFile(path, "w+");
    SDL_RWwrite(file, arr, sizeof(char), size);
    SDL_RWclose(file);
}
