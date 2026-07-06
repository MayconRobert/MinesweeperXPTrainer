#include <stdio.h>
#include <windows.h>

#include "utils.h"

HWND returnWindow()
{
    HWND window = FindWindow(NULL, "Minesweeper");
    if (window == NULL)
    {
        printf("Expected Minesweeper executable is not running\n");
        return NULL;
    }
    return window;
}
HANDLE attachProcess()
{
    DWORD processID = 0;
    HWND window = returnWindow();
    if (window == NULL)
    {
        printf("Error finding Minesweeper Window\n");
        exit(1);
    }

    GetWindowThreadProcessId(window, &processID);
    return OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_READ | PROCESS_VM_OPERATION, FALSE, processID);
}

int menu()
{
    DWORD choice = 0;
    printf("\nChoose a cheat\n");
    printf("1 - Pause/Resume Timer\n");
    printf("2 - Show Mines\n");
    printf("3 - Block/Unblock Mines\n");
    printf("Any other option to exit\n");
    scanf("%d", &choice);

    return choice;
}

void close()
{
    printf("\nThank you!\n");
    exit(0);
}