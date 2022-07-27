#include "ConsoleHandler.h"

ConsoleHandler::ConsoleHandler(const COORD& consoleSize):
    m_consoleOutput(GetStdHandle(STD_OUTPUT_HANDLE)),
    m_consoleInput(GetStdHandle(STD_INPUT_HANDLE)),
    m_eventHandler(new EventHandler(m_consoleInput, m_consoleOutput))
{
    //Check validity of std_out handle
    if (m_consoleOutput == INVALID_HANDLE_VALUE)
    {
        std::stringstream errorMessage;

        errorMessage << "Failed to CreateConsoleScreenBuffer with GLE = ";
        errorMessage << GetLastError() << "." << std::endl;

        throw std::exception(errorMessage.str().c_str());
    }

    //Check validity of std_in handle
    if (m_consoleInput == INVALID_HANDLE_VALUE)
    {
        std::stringstream errorMessage;

        errorMessage << "Failed to CreateConsoleScreenBuffer with GLE = ";
        errorMessage << GetLastError() << "." << std::endl;

        throw std::exception(errorMessage.str().c_str());
    }

    //Get console screen buffer info
    if (GetConsoleScreenBufferInfo(m_consoleOutput, &m_consoleBufferInfo) == FALSE)
    {
        std::stringstream errorMessage;

        errorMessage << "Failed to GetConsoleScreenBufferInfo with GLE = ";
        errorMessage << GetLastError() << "." << std::endl;

        throw std::exception(errorMessage.str().c_str());
    }

    //Set console size
    if (SetConsoleSize(consoleSize) == FALSE)
    {
        std::stringstream errorMessage;

        errorMessage << "Failed to SetConsoleSize with GLE = ";
        errorMessage << GetLastError() << "." << std::endl;

        throw std::exception(errorMessage.str().c_str());
    }

    Widget* widgetInfo = new Widget(COORD{ 0, 0 }, COORD{ 80, 10 });
    auto frameInfo = CreateInfoFrame(widgetInfo->GetWidgetSize());
    widgetInfo->DrawWidget(frameInfo.GetFrameBuffer(), frameInfo.GetFrameSize());

    Widget* widgetMessage = new Widget(COORD{ 0, 10 }, COORD{ 80, 10 });
    auto frameMessage = CreateMessageFrame(widgetMessage->GetWidgetSize());
    widgetMessage->DrawWidget(frameMessage.GetFrameBuffer(), 
                             frameMessage.GetFrameSize());

    InputWidget* widgetInput = new InputWidget(COORD{ 0, 20 }, COORD{ 80, 10 });
    auto frameInput = CreateInputFrame(widgetInput->GetWidgetSize());
    widgetInput->DrawWidget(frameInput.GetFrameBuffer(), frameInput.GetFrameSize());

    AddWidget(widgetInfo);
    AddWidget(widgetMessage);
    AddWidget(widgetInput);

    
    m_eventHandler->Observe(dynamic_cast<InputWidget*>(m_widgetList.back()));
    m_eventHandlingThread = std::thread(&ConsoleHandler::StartEventHandling, this);
}

ConsoleHandler::~ConsoleHandler()
{
    StopEventHandling();
    m_eventHandlingThread.join();

    for (size_t index = 0; index < m_widgetList.size(); index++)
    {
        delete m_widgetList[index];
    }
}

HANDLE ConsoleHandler::GetWinAPIConsoleInputHandler()
{
    return m_consoleInput;
}

HANDLE ConsoleHandler::GetWinAPIConsoleOutputHandler()
{
    return m_consoleOutput;
}

BOOL ConsoleHandler::SetConsoleSize(const COORD& consoleSize)
{
    auto oldMode = m_consoleBufferInfo;

    SMALL_RECT windowSize = { 0, 0, consoleSize.X - 1, consoleSize.Y - 1 };
    BOOL result = SetConsoleWindowInfo(m_consoleOutput,
        TRUE,
        &windowSize);

    if (result == TRUE)
    {
        result = SetConsoleScreenBufferSize(m_consoleOutput,
            consoleSize);

        if (result == TRUE)
        {
            result = Update();
        }
        else
        {
            SetConsoleWindowInfo(m_consoleOutput,
                TRUE,
                &oldMode.srWindow);
            SetConsoleScreenBufferSize(m_consoleOutput,
                oldMode.dwSize);
        }
    }
    else
    {
        SetConsoleWindowInfo(m_consoleOutput,
            TRUE,
            &oldMode.srWindow);
    }

    return result;
}

COORD ConsoleHandler::GetConsoleSize()
{
    return m_consoleBufferInfo.dwSize;
}

void ConsoleHandler::Clear()
{
    for (auto& widget : m_widgetList)
    {
        widget->ClearWidget();
    }
}

BOOL ConsoleHandler::Draw(const wchar_t* buffer, const COORD& bufferSize)
{
    return FALSE;
}

BOOL ConsoleHandler::Display()
{
    BOOL result = TRUE;

    for (auto& widget : m_widgetList)
    {
        result = widget->DisplayWidget();
        if (result == FALSE)
        {
            break;
        }
    }

    return result;
}

BOOL ConsoleHandler::Update()
{
    BOOL result = GetConsoleScreenBufferInfo(m_consoleOutput, &m_consoleBufferInfo);

    if (result == TRUE)
    {
        for (auto& widget : m_widgetList)
        {
            widget->Update();
        }
        
        Clear();
    }

    return result;
}

BOOL ConsoleHandler::AddWidget(Widget* widget)
{
    COORD widgetSize = widget->GetWidgetSize();
    COORD startRenderPoint = widget->GetRenderStartPoint();

    BOOL result = (startRenderPoint.Y + widgetSize.Y <= m_consoleBufferInfo.dwSize.Y);

    if ((m_widgetList.empty() == false) && (result == TRUE))
    {
        auto lastWidget = m_widgetList.back();

        COORD lastWidgetSize = lastWidget->GetWidgetSize();
        COORD lastStartRenderPoint = lastWidget->GetRenderStartPoint();

        result = (startRenderPoint.Y >= lastStartRenderPoint.Y + lastWidgetSize.Y);
    }

    if (result == TRUE)
    {
        m_widgetList.push_back(widget);
    }

    return result;
}

void ConsoleHandler::StartEventHandling()
{
    m_isEventHandlingOn = true;

    while (m_isEventHandlingOn == true)
    {
        m_eventHandler->CatchEvent();
        m_eventHandler->ProcessEvent();
    }
}

void ConsoleHandler::StopEventHandling()
{
    m_isEventHandlingOn = false;
}

Frame ConsoleHandler::CreateMainScreen(const COORD& frameSize)
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

Frame ConsoleHandler::CreateInfoFrame(const COORD& frameSize)
{
    Frame frame(frameSize);

    BorderShape border(frameSize);
    std::wstring text = L"PROGRAMM INFO ZONE HERE";

    frame.PasteShape(border.GetBuffer(), border.GetSize(), COORD{ 0, 0 });
    frame.PasteShape(text.c_str(),
        COORD{ static_cast<short>(text.length()), 1 },
        COORD{ 1, 1 });

    return frame;
}

Frame ConsoleHandler::CreateMessageFrame(const COORD& frameSize)
{
    Frame frame(frameSize);

    BorderShape border(frameSize);
    std::wstring text = L"LIVE CHAT INFO ZONE HERE";

    frame.PasteShape(border.GetBuffer(), border.GetSize(), COORD{ 0, 0 });
    frame.PasteShape(text.c_str(),
        COORD{ static_cast<short>(text.length()), 1 },
        COORD{ 1, 1 });

    return frame;
}

Frame ConsoleHandler::CreateInputFrame(const COORD& frameSize)
{
    Frame frame(frameSize);

    BorderShape border(frameSize);
    std::wstring text = L"INPUT ZONE HERE";

    frame.PasteShape(border.GetBuffer(), border.GetSize(), COORD{ 0, 0 });
    frame.PasteShape(text.c_str(),
        COORD{ static_cast<short>(text.length()), 1 },
        COORD{ 1, 1 });

    return frame;
}
