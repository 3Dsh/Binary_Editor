#ifndef INPUTDIALOGWINDOW_H
#define INPUTDIALOGWINDOW_H

#include "DialogWindow.h"
#include "TextInput.h"

// диалоговое окно с полем для ввода
class InputDialogWindow : public DialogWindow
{
    public:
        InputDialogWindow(Window *window, DlgButton *buttonsInfo, int bLen, int width);
        ~InputDialogWindow();

        int Work(char* title, int cancelId);
        const char* GetInputText() {
            return inputField->GetText();}

    private:
        bool LoadTextField();
        int DialogLoop(int cancelId);

        TextInput *inputField;
};

#endif // INPUTDIALOGWINDOW_H
