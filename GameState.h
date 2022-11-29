#ifndef GAMESTATE_H
#define GAMESTATE_H

class State{
    public:

        enum States{
            MainEditor,
            ChoiseFileMenu,
            SaveState,
            CreateState,
            AddByte,
            DeleteByte,
        };

        static States curState; // текущее состояние
};

#endif // GAMESTATE_H
