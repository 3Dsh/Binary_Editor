#include "MenuManager.h"

MenuManager::MenuManager(Window* win)
{
    window = win;

    editor = new MainEditor(window);
    choiseMenu = new ChoiseFileMenu(window, "TargetFiles");
}

void MenuManager::Update(){

}
void MenuManager::Draw(){
    switch(State::curState){
        case State::MainEditor:
        case State::SaveState:
        case State::CreateState:
        case State::AddByte:
        case State::DeleteByte:
            editor->Show();
            break;
        case State::ChoiseFileMenu:
            choiseMenu->Show();
            break;
    }
}

void MenuManager::EventHandler(SDL_Event e){
    char* path;
    switch(State::curState){
        case State::MainEditor:
            editor->EventHandler(e);
            break;
        case State::ChoiseFileMenu:
            if (path = choiseMenu->EventHandler(e)){
                State::curState = State::MainEditor;
                editor->LoadFile(path);
            }
            break;
        case State::SaveState:
            editor->SaveFile();
            choiseMenu->Reload();
            State::curState = State::MainEditor;
            break;
        case State::CreateState:
            path = OpenDialog();
            if (path){
                editor->LoadFile(path);
            }
            State::curState = State::MainEditor;
            break;
        case State::AddByte:
            editor->AddByte();
            State::curState = State::MainEditor;
            break;
        case State::DeleteByte:
            editor->DeleteByte();
            State::curState = State::MainEditor;
            break;
    }
}

char* MenuManager::OpenDialog(){
    DlgButton *b = new DlgButton[2];
    b[0] = DlgButton{0,"Сохранить"};
    b[1] = DlgButton{1,"Отмена"};

    InputDialogWindow inputDlg(window, b, 2, 0.5*window->getWidth());
    int id = inputDlg.Work("Введите название файла", 1);

    if (id == 1) return NULL;

    delete[]b;
    b = nullptr;

    const char* dir = "TargetFiles/";
    const char* lvlName = inputDlg.GetInputText();

    char* path = new char[SDL_utf8strlen(dir) + SDL_utf8strlen(lvlName) + 1];
    path[0] = '\0';
    strcat(path, dir);
    strcat(path,lvlName);

    SDL_Log("PPP %s", path);
    SDL_RWFromFile(path, "w+b");
    SDL_Log("PATH %s", path);

    return path;
}

MenuManager::~MenuManager()
{
    SDL_Log("MenuManager dtor\t%x",this);
    if (editor){
        delete(editor);
        editor = nullptr;
    }
    if (choiseMenu){
        delete(choiseMenu);
        choiseMenu = nullptr;
    }
}
