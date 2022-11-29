#include "EditorHelper.h"

EditorHelper::EditorHelper(Window* window)
{
    this->window = window;
    font = TTF_OpenFont("Fonts/FreeMono.ttf", 32);
}

EditorHelper::~EditorHelper()
{
    if (font){
        TTF_CloseFont(font);
        font = nullptr;
    }
}

int EditorHelper::Createx2Texture(char* arr, int size, Text** binaryArr){
    Uint32 start = SDL_GetTicks();

    SDL_Color color = SDL_Color{255,255,255,255};
    int i = 0;
    for (; i < size; i++){
        binaryArr[i] = new Text(window->GetRenderer());
        binaryArr[i]->Load(Getx2Code(arr[i]), color, font);
        binaryArr[i]->SetRenderPosition(i%3==0 ? 0.2*window->getWidth() : i%3==1 ? 0.4*window->getWidth() : 0.6*window->getWidth(), 0.25*window->getHeight() + i/3 * 30);
    }

    SDL_Log("end %d", SDL_GetTicks() - start);
    return 0.25*window->getHeight() + --i/3 * 30;

    /*char* x2Code = new char[size*8+size];
    x2Code[0] = '\0';
    for (int i = 0; i < size; i++){
        binaryArr[i] = nullptr;
        strcat(x2Code, Getx2Code(arr[i]));
        strcat(x2Code, i%2==0 ? " " : "\n");
    }

    binaryArr[0] = new Text(renderer);
    x2Code[size*8+size-1] = '\0';
    binaryArr[0]->Load(x2Code, color, font, 1000);
    binaryArr[0]->SetRenderPosition(100,100);*/
}
int EditorHelper::Createx8Texture(char* arr, int size, Text** binaryArr){
    Uint32 start = SDL_GetTicks();

    SDL_Color color = SDL_Color{255,255,255,255};
    int i = 0;
    int x = 0,y = 0.25*window->getHeight();
    for (; i < size; i++){
        if (i % 7 == 0){
            x = 200; y += 30;
        }
        binaryArr[i] = new Text(window->GetRenderer());
        binaryArr[i]->Load(Getx8Code(arr[i]), color, font);
        binaryArr[i]->SetRenderPosition(x,y);

        x += 120;
    }

    return y;
}
int EditorHelper::Createx16Texture(char* arr, int size, Text** binaryArr){
    Uint32 start = SDL_GetTicks();

    SDL_Color color = SDL_Color{255,255,255,255};
    int i = 0;
    int x = 0,y = 0.25*window->getHeight();
    for (; i < size; i++){
        if (i % 9 == 0){
            x = 200; y += 30;
        }
        binaryArr[i] = new Text(window->GetRenderer());
        binaryArr[i]->Load(Getx16Code(arr[i]), color, font);
        binaryArr[i]->SetRenderPosition(x,y);

        x += 100;
    }

    return y;
}

char EditorHelper::Deletex2(char c, char num){
    int mask = (255 >> num) | (255 << (9-num));
    return c&mask;
}
char EditorHelper::Deletex8(char c, char num){
    if (num < 1 || num > 3) return c;
    int start, end;
    if (num == 1){
        start = 1; end = 2;
    }
    else if (num == 2){
        start = 3; end = 5;
    }
    else start = 6, end = 8;
    SDL_Log("%d %d", start, end);
    for (int i = start; i <= end; i++){
        c = Deletex2(c,i);
    }
    return c;
}
char EditorHelper::Deletex16(char c, char num){
    if (num < 1 || num > 2) return c;
    int start = (num-1)*4+1;
    for (int i = 0; i < 4; i++){
        c = Deletex2(c,start+i);
    }
    return c;
}

char* EditorHelper::Getx2Code(char c){
    char* x2 = new char[9];

    char mask = 0x1;

    x2[8] = '\0';
    for (int i = 7; i >= 0; i--){
        x2[i] = c & mask ? '1' : '0';
        mask *= 2;
    }

    return x2;
}

char* EditorHelper::Getx8Code(char c){
    char* x8 = new char[4];
    x8[3] = '\0';

    int n = 0;
    int mask = 128;
    char* x2 = Getx2Code(c);
    for (int i = 0; i < 8; i++){
        n += (x2[i]-'0')*mask;
        mask = mask >> 1;
    }

    for (int i = 2; i >=0; i--){
        x8[i] = '0' + n%8;
        n/=8;
    }
    return x8;
}

char* EditorHelper::Getx16Code(char c){
    char* x16 = new char[3];
    x16[2] = '\0';

    int n = 0;
    int mask = 128;
    char* x2 = Getx2Code(c);
    for (int i = 0; i < 8; i++){
        n += (x2[i]-'0')*mask;
        mask = mask >> 1;
    }

    if (n < 16) sprintf(x16, "0%x", n);
    else sprintf(x16, "%x", n);
    return x16;
}

char EditorHelper::Addx2(char c, char num, char sym){
    if (sym == '1') {
        return c | (256 >> (num+1));
    }
    else return Deletex2(c, num+1);
}
char EditorHelper::Addx8(char c, char num, char sym){
    char* x8 = Getx8Code(c);
    if (!(num == 0 && sym > '3')) x8[num] = sym;
    char rVal = (x8[0] - '0')* 64 + (x8[1] - '0') * 8 + (x8[2] - '0');
    int sd = 0 | rVal;
    return rVal;
}
char EditorHelper::Addx16(char c, char num, char sym){
    if (num < 0 || num > 1) return c;
    char* x16 = Getx16Code(c);
    x16[num] = sym;

    int rVal = (x16[0] >= 'a' && x16[0] <= 'f' ? 10+x16[0]-'a' : x16[0] - '0')*16 + (x16[1] >= 'a' && x16[1] <= 'f' ? 10+x16[1]-'a' : x16[1] - '0');
    return rVal;
}
