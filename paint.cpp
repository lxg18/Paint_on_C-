#include <iostream>
#include <windows.h>
#define debug 0 // отадка

using namespace std;

void logs(int data); // логи (отладка)
int mouse_x, mouse_y;
HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE); // получаение дескриптора устройства стандартного ввода
HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE); // получаение дескриптора устройства стандартного вывода

void gotoxy(int x, int y); // функция перемещения курсона в консоли
void change_color(int color); // функция изменения цвета

/* глобальные переменные */
bool i = 0; // состояние режима заливки
bool paint = 0;
int color = 15, // цвет кисти
    eraser_color = 0, // цвет ластика
    new_color = 15; // изменённый цвет
COORD bufferSize = {80, 30}; // размер окна

int main() {
    /* меняем размер буфера ввода и окна */
    SetConsoleScreenBufferSize(hOut, bufferSize);
    HANDLE out_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD crd = bufferSize;
    SMALL_RECT src = {0, 0, crd.X-1, crd.Y-1};
    SetConsoleWindowInfo (out_handle, true, &src);
    SetConsoleScreenBufferSize (out_handle, crd);
    SetConsoleTitle("Paint C++"); // заголовок окна

    MessageBox(NULL, "ЛКМ - рисовать первым цветом\nПКМ - рисовать вторым цветом\nДля выбора цвета - крутим колесо\nЧтобы назначить первый цвет на второй - жмём колесо\nСправа вверху отображаются текущие цвета\n\nEsc - выход", "Инструкция", MB_OK);

    int size_y = 20, size_x = 2 * size_y; // размеры области экрана где разрешено двигаться курсору
    int clk, cur_x = 0, cur_y = 0; // код нажатой клавиши, координаты курсора
    char draw_char = ' '; // символ для рисования (его не будет видно, т.к. цвет текста равен цвету фона)
    bool Continue = TRUE; // флаг для выхода из цикла

    INPUT_RECORD InRec; // для записи событий из буфера консоли
    DWORD NumRead;
    if (debug) {
        cout << "Mouse X coor: " << endl;
        cout << "Mouse Y coor: " << flush;
    }

    while (Continue) {
        ReadConsoleInput(hIn, &InRec, 1, &NumRead); // чтение буфера консоли
        switch (InRec.EventType) { // проверяем события по типу
            case KEY_EVENT: // события клавиатуры
                if (InRec.Event.KeyEvent.uChar.AsciiChar == 27) { // выход если нажали Esc
                    Continue = FALSE; // флаг для выхода из цикла
                }
                break;
            case MOUSE_EVENT: // события мышки
                DWORD const BtnState = InRec.Event.MouseEvent.dwButtonState; // нажатия кнопок
                if (BtnState & FROM_LEFT_1ST_BUTTON_PRESSED) { // левая кнопка мыши нажата
                    paint = 1;
                    new_color = color;
                    if (debug) logs(1);
                    change_color(new_color);
                }
                else if (BtnState & RIGHTMOST_BUTTON_PRESSED) { // правая кнопка мыши нажата
                    paint = 1;
                    new_color = eraser_color;
                    if (debug) logs(2);
                    change_color(new_color);
                }
                else paint = 0;
                if (InRec.Event.MouseEvent.dwButtonState == 4) { // нажали колёсико
                    eraser_color = new_color; // установить текущий цвет, цветом ластика
                    if (debug) logs(3);
                    change_color(new_color);
                }
                if (int(InRec.Event.MouseEvent.dwButtonState) > 10) { // вращение колёсика вверх
                    new_color < 15 ? new_color++ : new_color = 0;
                    color = new_color;
                    if (debug) logs(4);
                    change_color(new_color);
                }
                if (int(InRec.Event.MouseEvent.dwButtonState) < -10) { // вращение колёсика вниз
                    new_color > 0 ? new_color-- : new_color = 15;
                    color = new_color;
                    if (debug) logs(5);
                    change_color(new_color);
                }
                /* чтение координат курсора мыши в консоли */
                mouse_x = InRec.Event.MouseEvent.dwMousePosition.X;
                mouse_y = InRec.Event.MouseEvent.dwMousePosition.Y;
                if (debug) logs(6);
                logs(7); // вывод текущих цветов кистей
                change_color(new_color);
                break;
        }
        /* перемещение курсора */
        gotoxy(mouse_x, mouse_y);
        /* красим если включена заливка */
        if (paint == true) cout << draw_char << flush;;
     }
    return 0;
}

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(hOut, coord);
}

void change_color(int color) {
     SetConsoleTextAttribute(hOut, color * 16 + color);
}

void logs(int data) {
     /* координаты для записи значений */
    COORD dwMousePosition;
    COORD MouseWhereX = {14, 0};
    COORD MouseWhereY = {14, 1};
    COORD MouseClickSt = {0, 2};
    COORD NewColSq = {bufferSize.X-2, 0};
    COORD EraserColSq = {bufferSize.X-2, 1};

    switch (data) {
           case 1:
                SetConsoleTextAttribute(hOut, 7); // чёрный фон, серые буквы
                SetConsoleCursorPosition(hOut, MouseClickSt); // перемещение курсора в координату MouseClickSt
                cout << "LEFT BUTTON" << "        " << flush;
                break;
           case 2:
                SetConsoleTextAttribute(hOut, 7); // чёрный фон, серые буквы
                SetConsoleCursorPosition(hOut, MouseClickSt); // перемещение курсора в координату MouseClickSt
                cout << "RIGHT BUTTON" << "      " << flush;
                break;
           case 3:
                SetConsoleTextAttribute(hOut, 7); // чёрный фон, серые буквы
                SetConsoleCursorPosition(hOut, MouseClickSt); // перемещение курсора в координату MouseClickSt
                cout << "WHEEL BUTTON" << "      " << flush;
                break;
           case 4:
                SetConsoleTextAttribute(hOut, 7); // чёрный фон, серые буквы
                SetConsoleCursorPosition(hOut, MouseClickSt); // перемещение курсора в координату MouseClickSt
                cout << "WHEEL UP" << "          " << flush;
                break;
           case 5:
                SetConsoleTextAttribute(hOut, 7); // чёрный фон, серые буквы
                SetConsoleCursorPosition(hOut, MouseClickSt); // перемещение курсора в координату MouseClickSt
                cout << "WHEEL DOWN" << "        " << flush;
                break;
           case 6:
                SetConsoleTextAttribute(hOut, 7); // чёрный фон, серые буквы
                SetConsoleCursorPosition(hOut, MouseWhereX); // перемещение курсора в координату MouseWhereX
                cout << mouse_x << "  " << flush; // вывод координаты X
                SetConsoleCursorPosition(hOut, MouseWhereY); // перемещение курсора в координату MouseWhereY
                cout << mouse_y << "  " << flush; // вывод координаты Y
                break;
           case 7: // текущие цвета кистей
                SetConsoleCursorPosition(hOut, NewColSq);
                SetConsoleTextAttribute(hOut, color);
                cout << "#";
                SetConsoleCursorPosition(hOut, EraserColSq);
                SetConsoleTextAttribute(hOut, eraser_color);
                cout << "#";
            break;
    }
}


