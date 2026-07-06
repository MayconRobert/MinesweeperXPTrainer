#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

#include "utils.h"
#include "cheats.h"

#define BEG_MATRIX 0x01005340
#define END_MATRIX 0x0100567F
#define GAME_STATUS 0x01005160

static BOOL areMinesBlocked(HANDLE process, LPCVOID baseAddress)
{
    DWORD minesBlocked;
    ReadProcessMemory(process, baseAddress, &minesBlocked, sizeof(minesBlocked), NULL);
    return minesBlocked == 0x3D8366EB ? TRUE : FALSE; //\x66\xEB: ShellCode for blocked mines
}

void blockMines()
{
    HANDLE process = attachProcess();

    if (process == NULL)
    {
        printf("Error attaching Minesweeper executable\n");
        exit(1);
    }

    LPVOID ADDR_TRIGGER_FAIL = (LPVOID)0x0100352D; // Trigger game over
    DWORD shellcodeSize = 2;

    unsigned char resumeMinesShellcode[] = "\x74\x66"; // je 01003595
    unsigned char blockMinesShellcode[] = "\xEB\x66";  // jmp 01003595

    LPBYTE lpShellcode = (LPBYTE)malloc(shellcodeSize);

    memcpy(lpShellcode, areMinesBlocked(process, ADDR_TRIGGER_FAIL) ? resumeMinesShellcode : blockMinesShellcode, shellcodeSize);
    WriteProcessMemory(process, ADDR_TRIGGER_FAIL, lpShellcode, shellcodeSize, NULL);
    CloseHandle(process);

    free(lpShellcode);
}

static void printMines(HANDLE process, LPVOID buffer)
{
    ULONG_PTR tileAddr = 0;
    unsigned char *tileValue;
    tileValue = (unsigned char *)buffer;

    // Flag needs to be declared as a Struct to not append 0x00 in the end of the String
    unsigned char flag[] = {0x8E}; // Flag

    LPBYTE lpShellcode = (LPBYTE)malloc(sizeof(flag));
    memcpy(lpShellcode, flag, sizeof(flag));

    for (tileAddr = BEG_MATRIX; tileAddr <= END_MATRIX; tileAddr++)
    {
        if (*tileValue == 0x8F) // If it's a bomb
        {
            // printf("Bomb Tile: %x | Memory: %x", *tile, tileAddr);
            WriteProcessMemory(process, (LPVOID)tileAddr, lpShellcode, sizeof(flag), NULL);
        }
        tileValue++;
    }

    HWND window = returnWindow();
    if (window == NULL)
    {
        printf("Error finding Minesweeper Window\n");
        exit(1);
    }

    RedrawWindow(window, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE); // Refresh Window to show Flags
    free(lpShellcode);
}

void showMines()
{
    HANDLE process = attachProcess();

    if (process == NULL)
    {
        printf("Error attaching Minesweeper executable\n");
        exit(1);
    }

    BYTE gameStatus = 0;
    ReadProcessMemory(process, (LPVOID)GAME_STATUS, &gameStatus, sizeof(gameStatus), NULL);

    /*
    gameStatus == 0 - Game is running
    gameStatus == 2 - Game lost
    gameStatus == 3 - Game won
    */
    if (gameStatus == 0)
    {
        DWORD matrixSize = END_MATRIX - BEG_MATRIX;
        LPVOID buffer = malloc(matrixSize);

        ReadProcessMemory(process, (LPVOID)BEG_MATRIX, buffer, matrixSize, NULL); // Read Game Matrix
        printMines(process, buffer);
        free(buffer);
    }

    CloseHandle(process);
}

static BOOL isTimerFrozen(HANDLE process, LPCVOID baseAddress)
{
    DWORD timerFrozen;
    ReadProcessMemory(process, baseAddress, &timerFrozen, sizeof(timerFrozen), NULL);
    return timerFrozen == 0x90909090 ? TRUE : FALSE;
}

void freezeTimer()
{
    HANDLE process = attachProcess();

    if (process == NULL)
    {
        printf("Error attaching Minesweeper executable\n");
        exit(1);
    }

    LPVOID ADDR_INC_TIMER = (LPVOID)0x01002FF5; // Increases Timer
    DWORD shellcodeSize = 6;

    unsigned char resumeTimerShellcode[] = "\xFF\x05\x9C\x57\x00\x01"; // inc [winmine.exe+579C]
    unsigned char stopTimerShellcode[] = "\x90\x90\x90\x90\x90\x90";   // NOP

    LPBYTE lpShellcode = (LPBYTE)malloc(shellcodeSize);

    memcpy(lpShellcode, isTimerFrozen(process, ADDR_INC_TIMER) ? resumeTimerShellcode : stopTimerShellcode, shellcodeSize);
    WriteProcessMemory(process, ADDR_INC_TIMER, lpShellcode, shellcodeSize, NULL);
    CloseHandle(process);

    free(lpShellcode);
}