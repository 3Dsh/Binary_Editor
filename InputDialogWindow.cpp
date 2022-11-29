#include "InputDialogWindow.h"

InputDialogWindow::InputDialogWindow(Window *window, DlgButton *buttonsInfo, int bLen, int width) : DialogWindow(window, buttonsInfo, bLen)
{
    dlgW = width;

    inputField = NULL;
}

int InputDialogWindow::Work(char* title, int cancelId){
    if (!LoadTitle(title) || !LoadTextField() || !LoadButtons())
        return -1;

    dialog = new Panel(window->GetRenderer());
    dialog->Create(dlgW, titleH+messageH+buttonH, SDL_TEXTUREACCESS_TARGET);
    dialog->SetColor(55 ,55,55,255); // цвет диалогового окна
    dialog->setRenderTarget(true);
    SDL_SetRenderDrawColor(window->GetRenderer(),83,83,83,255); // цвет заголовка
    SDL_RenderFillRect(window->GetRenderer(), new SDL_Rect{dialog->GetPosX(),dialog->GetPosY(),dialog->getWidth(), titleH});
    dialog->setRenderTarget(false);
    dialog->SetRenderPosition(window->getWidth()/2 - dialog->getWidth()/2,
        window->getHeight()/2 - dialog->getHeight()/2);

    titleText->SetRenderPosition(dialog->getWidth()/2 - titleText->getWidth()/2,0);
    dialog->setRenderTarget(true);
    SDL_Rect rect = titleText->GetRect();
    titleText->RenderCopy(window->GetRenderer(), NULL, &rect);
    dialog->setRenderTarget(false);

    int offcetX = 0.03*dialog->getWidth();
    int offcetY = 0.3*messageH;
    inputField->SetRenderRect(SDL_Rect{dialog->GetPosX() + offcetX, dialog->GetPosY()+titleH+offcetY, dlgW - offcetX*2, messageH-offcetY*2});

    return DialogLoop(cancelId);
}

int InputDialogWindow::DialogLoop(int cancelId){
    SDL_Event e;
    int id = -1;

    SDL_StartTextInput();
    while (true){
        dialog->Render();

        while (SDL_PollEvent(&e)){
            inputField->EventHandler(e);
            for (int i = 0; i < buttons.Size(); i++){
                if (buttons[i]->EventHandler(e) == Button::Event::LB_Up){
                    if (cancelId == buttonsInfo[i].id) {
                        id = cancelId;
                        goto exit;
                    }

                    if (strlen(GetInputText()) == 0) continue;

                    id = buttonsInfo[i].id;
                    goto exit;
                }
            }
        }
        for (int i = 0; i < buttons.Size(); i++){
            buttons[i]->Render();
        }
        inputField->Show();
        SDL_RenderPresent(window->GetRenderer());
    }
    exit:
    SDL_StopTextInput();
    return id;
}

bool InputDialogWindow::LoadTextField(){
    inputField = new TextInput(window,"Fonts/FreeMono.ttf", 255);
    inputField->SetBackMode(TextInput::BackMode::Frame);
    inputField->SetBackColor(SDL_Color{220,220,220,255});
    inputField->SetTextColor(SDL_Color{255,255,255,255});

    messageH = 0.08*window->getHeight();
    return true;
}

InputDialogWindow::~InputDialogWindow()
{
    SDL_Log("InputDialogWindow dtor\t%x",this);
    if (inputField){
        delete(inputField);
        inputField = nullptr;
    }
}
