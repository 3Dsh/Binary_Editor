#ifndef DIALOGWINDOW_H
#define DIALOGWINDOW_H

#include "Window.h"
#include "Button.h"
#include "List.h"
#include "Text.h"
#include "Panel.h"

// информация о кнопке на диалоговом окне
struct DlgButton{
    int id;
    char* name;
};

// Диалоговое окно
class DialogWindow
{
    public:
        DialogWindow(Window* window, DlgButton *buttonsInfo, int size);
        ~DialogWindow();

        int Work(char* title, char* message);

    protected:
        bool LoadTitle(char* title);
        bool LoadMessage(char* message);
        bool LoadButtons();
        int DialogLoop();

        void AnalyzeText(char* text, TTF_Font *font, int* maxW, int* rows);

        int titleH, messageH, buttonH;
        int dlgW;

        List<DlgButton> buttonsInfo;
        List<Button*> buttons;
        Text *titleText, *messageText;

        Panel *dialog;

        Window *window;
};

#endif // DIALOGWINDOW_H
