/**
#include "ConsoleEventHandler.h"
#include "EventHandler.h"

int main()
{
    int exit_code = 0;

    HANDLE std_in = GetStdHandle(STD_INPUT_HANDLE);
    if (std_in != INVALID_HANDLE_VALUE)
    {
        DWORD savedOldConsoleMode;
        if (GetConsoleMode(std_in, &savedOldConsoleMode) == TRUE)
        {
            if (SetConsoleMode(std_in, (ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT)) == TRUE)
            {

                EventHandler handler(std_in);
                
                while (true)
                {
                    handler.CatchEvent();
                    handler.ProcessEvent();
                }

                SetConsoleMode(std_in, savedOldConsoleMode);

            }
            else
            {
                std::cout << "Failed to change console mode." << std::endl;
                exit_code = -1;
            }
        }
        else
        {
            std::cout << "Failed to save console mode." << std::endl;
            exit_code = -1;
        }
    }
    else
    {
        std::cout << "Failed to get console handle." << std::endl;
        exit_code = -1;
    }

    std::cout << "HERE" << std::endl;

    return exit_code;
}
**/

/**
#include "EventProcessor.h"
#include "EventCollector.h"

#include <iostream>
#include <conio.h>
#include <cstdio>
#include <Windows.h>
#include <algorithm>
#include <memory>


void writeSingleCharacter(wchar_t* buffer, 
                          const COORD& bufferSize, 
                          const COORD& insertTopLeft, 
                          const wchar_t& ch);

void writeString(wchar_t* buffer,
                 const COORD& bufferSize,
                 const COORD& insertTopLeft,
                 const std::wstring& s);

std::wstring* createBorder(const COORD& borderSize);

int main()
{
    // Initializing all resources

    HANDLE console = CreateConsoleScreenBuffer((GENERIC_READ | GENERIC_WRITE), 
                                                0, 
                                                nullptr, 
                                                CONSOLE_TEXTMODE_BUFFER, 
                                                nullptr);

    if (console == INVALID_HANDLE_VALUE)
    {
        std::cerr << "CreateConsoleScreenBuffer win error: ";
        std::cerr << GetLastError() << std::endl;

        return EXIT_FAILURE;
    }

    DWORD oldConsoleMode = 0;
    if (GetConsoleMode(console, &oldConsoleMode) == FALSE)
    {
        std::cerr << "Failed to GetConsoleMode with error: ";
        std::cerr << GetLastError() << std::endl;

        return EXIT_FAILURE;
    }

    if (SetConsoleActiveScreenBuffer(console) == FALSE)
    {
        std::cerr << "Failed to SetConsoleActiveScreenBuffer with error: ";
        std::cerr << GetLastError() << std::endl;

        return EXIT_FAILURE;
    }

    CONSOLE_SCREEN_BUFFER_INFO consoleScreenInfo;
    if (GetConsoleScreenBufferInfo(console, &consoleScreenInfo) == FALSE)
    {
        std::cerr << "Failed to GetConsoleScreenBufferInfo with error: ";
        std::cerr << GetLastError() << std::endl;

        return EXIT_FAILURE;
    }

    //Main program logic

    const COORD charBufferSize = consoleScreenInfo.dwSize;

    wchar_t* screen = new wchar_t[charBufferSize.X * charBufferSize.Y];
    std::fill_n(screen, charBufferSize.X * charBufferSize.Y, L' ');

    DWORD dwBytesWritten = 0;

    COORD borderSize{ consoleScreenInfo.dwSize.X, 6};
    std::wstring* border = createBorder(borderSize);

    short line_offset = 0;
    for (short i = 0; i < borderSize.Y; i++)
    {
        writeString(screen, 
                    charBufferSize, 
                    COORD{ 0, static_cast<short>(i + line_offset) }, 
                    border[i]);
    }
    writeString(screen, 
                charBufferSize, 
                COORD{ 2, static_cast<short>(1 + line_offset) }, 
                L"PROGRAMM INFO ZONE HERE");

    line_offset += borderSize.Y;

    borderSize = COORD{ consoleScreenInfo.dwSize.X, 
                        static_cast<short>(consoleScreenInfo.dwSize.Y - borderSize.Y * 2) };
    border = createBorder(borderSize);

    for (short i = 0; i < borderSize.Y; i++)
    {
        writeString(screen,
                    charBufferSize,
                    COORD{ 0, static_cast<short>(i + line_offset) },
                    border[i]);
    }
    writeString(screen,
                charBufferSize,
                COORD{ 2, static_cast<short>(1 + line_offset) },
                L"LIVE CHAT ZONE HERE");

    line_offset += borderSize.Y;

    borderSize = COORD{ consoleScreenInfo.dwSize.X, 6 };
    border = createBorder(borderSize);

    for (short i = 0; i < borderSize.Y; i++)
    {
        writeString(screen,
                    charBufferSize,
                    COORD{ 0, static_cast<short>(i + line_offset) },
                    border[i]);
    }
    writeString(screen,
                charBufferSize,
                COORD{ 2, static_cast<short>(1 + line_offset) },
                L"INPUT ZONE HERE");

    if (WriteConsoleOutputCharacter(console, 
                                    screen, 
                                    charBufferSize.X * charBufferSize.Y,
                                    {0,0}, 
                                    &dwBytesWritten) == 0)
    {
        std::cerr << "Failed to WriteConsoleOutputCharacter with error: ";
        std::cerr << GetLastError() << std::endl;

        return EXIT_FAILURE;
    }

    //Restoring console mode after all operations are done

    int wait = _getch();

    SetConsoleMode(console, oldConsoleMode);
    CloseHandle(console);
    delete[] border;

    return 0;
}

void writeSingleCharacter(wchar_t* buffer, const COORD& bufferSize, const COORD& insertTopLeft, const wchar_t& ch)
{
    buffer[insertTopLeft.Y * bufferSize.X + insertTopLeft.X] = ch;
}

void writeString(wchar_t* buffer, const COORD& bufferSize, const COORD& insertTopLeft, const std::wstring& s)
{
    swprintf(&buffer[insertTopLeft.Y * bufferSize.X + insertTopLeft.X], 
             s.size() + 1, 
             L"%s", 
             s.c_str());
}

std::wstring* createBorder(const COORD& borderSize)
{
    //Allocating reqiered memory
    std::wstring* border = new std::wstring[borderSize.Y];

    for (short i = 0; i < borderSize.Y; i++)
    {
        border[i].resize(borderSize.X);
    }

    //Filling first row of border
    std::fill_n(&border[0][0], borderSize.X, L'═');
    border[0][0] = L'╔';
    border[0][borderSize.X - 1] = L'╗';

    //Filling middle rows of border

    for (short i = 1; i < (borderSize.Y - 1); i++)
    {
        std::fill_n(&border[i][0], borderSize.X, L' ');

        border[i][0] = L'║';
        border[i][borderSize.X - 1] = L'║';
    }

    //Filling last row of border

    std::fill_n(&border[borderSize.Y - 1][0], borderSize.X, L'═');
    border[borderSize.Y - 1][0] = L'╚';
    border[borderSize.Y - 1][borderSize.X - 1] = L'╝';

    return border;
}
**/


#include "EventProcessor.h"
#include "EventCollector.h"
#include "ConsoleHandler.h"
#include "Frame.h"
#include "BorderShape.h"
#include "ConsoleEventHandler.h"

#include <iostream>
#include <conio.h>
#include <cstdio>
#include <Windows.h>
#include <algorithm>
#include <memory>

Frame CreateMainScreen(const COORD& frameSize);

int main()
{
    ConsoleHandler console;

    //Forming frame

    auto mainFrame = CreateMainScreen(console.GetConsoleSize());

    //Displaying frame in a loop
    
    //while (true)
    {
        console.ClearDisplay();

        console.UpdateDisplay(mainFrame.GetFrameBuffer(), mainFrame.GetFrameSize());

        console.DrawDisplay();
    }

    console.SetConsoleSize(COORD{40, 25});
    mainFrame = CreateMainScreen(console.GetConsoleSize());

    {
        console.ClearDisplay();

        console.UpdateDisplay(mainFrame.GetFrameBuffer(), mainFrame.GetFrameSize());

        console.DrawDisplay();
    }

    HANDLE waitEvent = CreateEvent(nullptr, TRUE, FALSE, L"KeepWaitingEvent");

    if (waitEvent != NULL)
    {
        WaitForSingleObject(waitEvent, INFINITE);
        CloseHandle(waitEvent);
    }

    return 0;
}

Frame CreateMainScreen(const COORD& frameSize)
{
    Frame frame(frameSize);

    COORD offset{ 0, 0 };
    BorderShape border(COORD{ frameSize.X, 10 });
    std::wstring text = L"PROGRAMM INFO ZONE HERE";

    frame.PasteShape(border.GetBuffer(), border.GetSize(), offset);
    frame.PasteShape(text.c_str(),
        COORD{ static_cast<short>(text.length()), 1 },
        COORD{ static_cast<short>(offset.X + 1), static_cast<short>(offset.Y + 1) });

    offset.Y += border.GetSize().Y;
    border = BorderShape(COORD{ border.GetSize().X,
                                static_cast<short>(frameSize.Y - border.GetSize().Y * 2) });
    text = L"LIVE CHAT INFO ZONE HERE";

    frame.PasteShape(border.GetBuffer(), border.GetSize(), offset);
    frame.PasteShape(text.c_str(),
        COORD{ static_cast<short>(text.length()), 1 },
        COORD{ static_cast<short>(offset.X + 1), static_cast<short>(offset.Y + 1) });

    offset.Y += border.GetSize().Y;
    border = BorderShape(COORD{ border.GetSize().X, 10 });
    text = L"INPUT ZONE HERE";

    frame.PasteShape(border.GetBuffer(), border.GetSize(), offset);
    frame.PasteShape(text.c_str(),
        COORD{ static_cast<short>(text.length()), 1 },
        COORD{ static_cast<short>(offset.X + 1), static_cast<short>(offset.Y + 1) });

    return frame;
}
