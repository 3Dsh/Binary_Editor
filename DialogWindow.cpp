#include "DialogWindow.h"

DialogWindow::DialogWindow(Window *window, DlgButton *buttonsInfo, int size)
{
    titleH = messageH = buttonH = dlgW = 0;
    titleText = messageText = nullptr;
    dialog = nullptr;

    for (int i = 0; i < size; i++)
        this->buttonsInfo.Add(buttonsInfo[i]);

    this->window = window;
}

void DialogWindow::AnalyzeText(char* text, TTF_Font *font, int* maxW, int* rows){
    int rowCount = 0, maxWidth = 0;
    int i = 0, start = 0;
    do {
        if (text[i] == '\n' || text[i] == '\0'){
            if (i - start > 0){
                char* tmpText = new char[i-start+1];
                int k = 0;
                for (int j = start; j < i; j++)
                    tmpText[k++] = text[j];
                tmpText[k] = '\0';
                Text t(window->GetRenderer());
                t.Load(tmpText, SDL_Color{0,0,0,0}, font);
                if (t.getWidth() > maxWidth) maxWidth = t.getWidth();
                delete[]tmpText;
            }
            start = i+1;
            rowCount++;
        }
    } while (text[i++] != '\0');

    *maxW = maxWidth;
    *rows = rowCount;
}

bool DialogWindow::LoadTitle(char* title){
    SDL_Color color = SDL_Color{205,205,205,255};
    TTF_Font *font = TTF_OpenFont("Fonts/FreeMono.ttf", 32);
    titleH = 0.045*window->getHeight();

    titleText = new Text(window->GetRenderer());
    titleText->Load(title, color, font);
    titleText->ChangeHeightProportionally(titleH);

    if (titleText->getWidth() > window->getWidth() ||
        titleH + messageH + buttonH > window->getHeight())
    {
        delete (titleText);
        titleText = nullptr;
        return false;
    }

    if (titleText->getWidth() > dlgW)
        dlgW = titleText->getWidth();

    return true;
}

bool DialogWindow::LoadMessage(char* message){
    SDL_Color color = SDL_Color{205,205,205,255};
    TTF_Font *font = TTF_OpenFont("Fonts/FreeMono.ttf", 32);

    int maxW, rows;
    AnalyzeText(message, font, &maxW, &rows);

    messageText = new Text(window->GetRenderer());
    messageText->Load(message, color, font, maxW);
    messageText->ChangeHeightProportionally(0.045*window->getHeight()*rows);

    messageH = 0.075*window->getHeight()*rows;

    if (messageText->getWidth() > window->getWidth() ||
        titleH + messageH + buttonH > window->getHeight())
    {
        delete(messageText);
        messageText = nullptr;
        return false;
    }

    if (messageText->getWidth() > dlgW)
        dlgW = messageText->getWidth();

    return true;
}

bool DialogWindow::LoadButtons(){
    if (buttonsInfo.Size() == 0)
        return false;

    int maxLen = 6;
    for (int i = 0; i < buttonsInfo.Size(); i++){
        int tmp;
        if ((tmp = SDL_utf8strlen(buttonsInfo[i].name)) > maxLen) maxLen = tmp;
    }

    int maxW = 0.06*window->getWidth();
    int maxH = 0.05*window->getHeight();

    maxW = (maxLen)/100.*window->getWidth();
    maxH = 0.05*window->getHeight();

    int padding = 0.005*window->getWidth();

    SDL_Color normal = SDL_Color{200,200,200,255};
    SDL_Color pressed = SDL_Color{255,255,0,255};
    SDL_Color highlited = SDL_Color{0,255,255,255};

    TTF_Font *font = TTF_OpenFont("Fonts/FreeMono.ttf", 32);

    buttonH = maxH;

    SDL_Rect rect = SDL_Rect{window->getWidth()/2 - (maxW/2*buttonsInfo.Size() + (padding/2*(buttonsInfo.Size()-1))),
    window->getHeight()/2 - (titleH+messageH+buttonH)/2 + titleH+messageH, maxW, maxH};

    int allBtnW = maxW*buttonsInfo.Size() + (buttonsInfo.Size()-1)*padding;

    if (allBtnW > window->getWidth() ||
        titleH + messageH + buttonH > window->getHeight())
    {
        delete(messageText);
        messageText = nullptr;
        return false;
    }

    if (allBtnW > dlgW)
        dlgW = allBtnW;

    Button* tmp;
    for (int i = 0; i < buttonsInfo.Size(); i++){
        tmp = new Button(window->GetRenderer(), rect);
        tmp->LoadBackImages("Images/backDialogButtonNormal.bmp","Images/backDialogButtonPressed.bmp","Images/backDialogButtonNormal.bmp");
        tmp->LoadText(normal, pressed, highlited, buttonsInfo[i].name, font);
        buttons.Add(tmp);

        rect.x += padding + maxW;
    }


    return true;
}

int DialogWindow::Work(char* title, char* message){
    if (!LoadTitle(title) || !LoadMessage(message) || !LoadButtons())
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

    messageText->SetRenderPosition(0,titleH + (messageH - messageText->getHeight())/2);
    dialog->setRenderTarget(true);
    rect = messageText->GetRect();
    messageText->RenderCopy(window->GetRenderer(), NULL, &rect);
    dialog->setRenderTarget(false);

    return DialogLoop();

}

int DialogWindow::DialogLoop(){
    SDL_Event e;
    int id = -1;
    while (true){
        dialog->Render();

        SDL_PollEvent(&e);
        for (int i = 0; i < buttons.Size(); i++){
            buttons[i]->Render();
            if (buttons[i]->EventHandler(e) == Button::Event::LB_Up){
                id = buttonsInfo[i].id;
                goto exit;
            }
        }
        SDL_RenderPresent(window->GetRenderer());
    }
    exit:
    return id;
}

DialogWindow::~DialogWindow()
{
    SDL_Log("DialogWindow dtor\t%x",this);
    buttonsInfo.Clear();
    buttons.Clear();
    if (titleText){
        delete(titleText);
        titleText = nullptr;
    }
    if (messageText){
        delete(messageText);
        messageText = nullptr;
    }
    if (dialog){
        delete(dialog);
        dialog = nullptr;
    }
}
