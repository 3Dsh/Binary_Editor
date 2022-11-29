#include "ChoiseFileMenu.h"

ChoiseFileMenu::ChoiseFileMenu(Window* window, char* lDir) : countOnPage(0), curPage(0), pageCount(0)
{
    this->window = window;

    LoadPaths(levelDir = lDir);
    paths.Sort();

    backButton = nextPage = prevPage = nullptr;
    font = TTF_OpenFont("Fonts/FreeMono.ttf", 64);

    InitButtons();
}

void ChoiseFileMenu::LoadPaths(char* dirPath){

paths.Clear();

#ifdef _WIN32

    WIN32_FIND_DATA f;
    HANDLE h = FindFirstFile(dirPath, &f);
    if(h != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (f.cFileName[0] == '.') continue;
            paths.Add(f.cFileName);
        } while(FindNextFile(h, &f));
    }
    else
    {
        fprintf(stderr, "Error opening directory\n");
    }

#else
#ifdef __unix__

    DIR *dir = opendir(dirPath);
    int dirLen = strlen(dirPath);
    if(dir)
    {
        struct dirent *ent;
        while((ent = readdir(dir)) != NULL)
        {
            if (ent->d_name[0] == '.') continue;
            paths.Add(ent->d_name);
        }
    }
    else
    {
        fprintf(stderr, "Error opening directory\n");
    }

#else
#ifdef __TURBOC__

    struct ffblk ffblk;
    if(findfirst("*.*", &ffblk, 0) == 0)
    {
        do
        {
            paths.Add(ffblk.ff_name);
        } while(findnext(&ffblk) == 0);
    }
    else
    {
        fprintf(stderr, "Error opening directory\n");
    }

#else
#error Unsupported Implementation
#endif
#endif
#endif

    //paths.sort();
}

void ChoiseFileMenu::Reload(){
    LoadPaths(levelDir);
    paths.Sort();
    InitButtons();
}

void ChoiseFileMenu::InitButtons(){
    int countOnLine = 6;
    int padding = 0.05f*window->getWidth();
    int size = (window->getWidth() - padding*(countOnLine+1))/countOnLine;
    int lineCount = window->getHeight() / (padding+size);

    countOnPage = countOnLine*lineCount;
    pageCount = (paths.Size()-1) / countOnPage + 1;
    curPage = 1;

    SDL_Rect rect = SDL_Rect{padding,padding,size,size};
    SDL_Color color = SDL_Color{255,255,255,255};

    for (int i = 0; i < buttons.Size(); i++){
        delete(buttons[i]);
        buttons[i] = nullptr;
    }
    buttons.Clear();
    Button* tmp;
    int x = 0, y = 0;
    for (int i = 0; i < paths.Size(); i++){
        tmp = new Button(window->GetRenderer(), rect);
        tmp->LoadText(color,paths[i],font, true);
        tmp->LoadBackImages("Images/round.bmp");
        buttons.Add(tmp);

        x++;
        rect.x += rect.w + padding;
        if (x >= countOnLine){
            rect.x = padding;
            rect.y += rect.h + padding;
            x = 0;
            y++;
            if (y >= lineCount) {
                y = 0;
                rect.y = padding;
            }
        }
    }

    if (backButton)
        delete(backButton);
    if (nextPage)
        delete(nextPage);
    if (prevPage)
        delete(prevPage);

    backButton = new Button(window->GetRenderer(),SDL_Rect{0.015*window->getWidth(),0.015*window->getWidth(),0.03*window->getWidth(), 0.03*window->getWidth()});
    backButton->LoadBackImages("Images/back.bmp");

    int btnSize = 0.05*window->getHeight();
    nextPage = new Button(window->GetRenderer(),SDL_Rect{0.5*window->getWidth(), window->getHeight()-btnSize, btnSize, btnSize});
    nextPage->LoadBackImages("Images/next.bmp");

    prevPage = new Button(window->GetRenderer(),SDL_Rect{0.5*window->getWidth()-btnSize, window->getHeight()-btnSize, btnSize, btnSize});
    prevPage->LoadBackImages("Images/prev.bmp");
}

void ChoiseFileMenu::Show(){
    int i0 = (curPage-1)*countOnPage;
    int i1 = i0+countOnPage;
    if (i1 > paths.Size()) i1 = paths.Size();

    for (int i = i0; i < i1; i++){
        buttons[i]->Render();
    }

    backButton->Render();
    nextPage->Render();
    prevPage->Render();
}

char* ChoiseFileMenu::EventHandler(SDL_Event e){
    if (backButton->EventHandler(e) == Button::Event::LB_Up) {
        State::curState = State::MainEditor;
        curPage = 1;
    }
    if (nextPage->EventHandler(e) == Button::Event::LB_Up){
        if (curPage+1 <= pageCount) curPage++;
    }
    else if (prevPage->EventHandler(e) == Button::Event::LB_Up){
        if (curPage - 1 > 0) curPage--;
    }

    int i0 = (curPage-1)*countOnPage;
    int i1 = i0+countOnPage;
    if (i1 > paths.Size()) i1 = paths.Size();

    Button::Event ev;
    for (int i = i0; i < i1; i++){
        ev = buttons[i]->EventHandler(e);
        if (ev == Button::Event::LB_Up)
            return GetFullPath(paths[i]);
    }

    return NULL;
}

char* ChoiseFileMenu::GetFullPath(char* level){
    char* path = new char[strlen(levelDir) + strlen(level) + 2];
    path[0] = '\0';
    strcat(path, levelDir);
    strcat(path, "/");
    strcat(path,level);
    return path;
}

ChoiseFileMenu::~ChoiseFileMenu()
{
    SDL_Log("ChoiseLevel dtor\t%x",this);
    paths.Clear();
    buttons.Clear();
    if (backButton){
        delete(backButton);
        backButton = nullptr;
    }
    if (nextPage){
        delete(nextPage);
        nextPage = nullptr;
    }
    if (prevPage){
        delete(prevPage);
        prevPage = nullptr;
    }
}
