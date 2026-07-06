#include <locale.h>
#include "utils.h"
#include "cheats.h"

int main(int argc, char *argv[])
{
    while (1)
    {
        DWORD choice;
        setlocale(LC_ALL, "");
        system("cls");
        choice = menu();

        switch (choice)
        {
        case 1:
            freezeTimer();
            break;
        case 2:
            showMines();
            break;
        case 3:
            blockMines();
            break;

        default:
            close();
            break;
        }
    }
}