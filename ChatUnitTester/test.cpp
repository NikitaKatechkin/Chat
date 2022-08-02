#include "pch.h"

#include <ChatClient/Frame.h>
#include <ChatClient/BorderShape.h>

#include <ChatClient/EventHandler.h>

#include <ChatClient/InputWidget.h>
#include <ChatClient/MessageWidget.h>
#include <ChatClient/TextWidget.h>

#include <memory>

const COORD emptyFrameSize = COORD{ 0, 0 };
const COORD validFrameSize = COORD{ 10, 10 };

const HANDLE validInputHandle = GetStdHandle(STD_INPUT_HANDLE);
const HANDLE invalidInputHandle = INVALID_HANDLE_VALUE;

const HANDLE validOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
const HANDLE invalidOutputHandle = INVALID_HANDLE_VALUE;

bool operator==(const COORD& left, const COORD& right)
{
	return ((left.X == right.X) && (left.Y == right.Y));
}

//---------------------------------------------------------------

TEST(FrameTestCase, CreationTest) 
{
	EXPECT_NO_THROW(new Frame(validFrameSize));
	EXPECT_ANY_THROW(new Frame(emptyFrameSize));
}

TEST(FrameTestCase, CopyTest)
{
	Frame tmp_frame(validFrameSize);

	EXPECT_NO_THROW(new Frame(tmp_frame));
	EXPECT_NO_THROW(auto frame = tmp_frame);
}

TEST(FrameTestCase, SizeValidityTest)
{
	Frame tmp_frame(validFrameSize);
	EXPECT_EQ(tmp_frame.GetFrameSize(), validFrameSize);
}

TEST(FrameTestCase, CharLenghtValidityTest)
{
	Frame tmp_frame(validFrameSize);
	EXPECT_EQ(tmp_frame.GetFrameSize(), validFrameSize);
	EXPECT_EQ(tmp_frame.GetFrameCharLength(), validFrameSize.X * validFrameSize.Y);
}

TEST(FrameTestCase, SizeChangingValidityTest)
{
	Frame tmp_frame(validFrameSize);
	EXPECT_EQ(tmp_frame.GetFrameSize(), validFrameSize);

	const COORD newSize{12, 12};

	tmp_frame.SetFrameSize(newSize);
	EXPECT_EQ(tmp_frame.GetFrameSize(), newSize);
	EXPECT_EQ(tmp_frame.GetFrameCharLength(), newSize.X * newSize.Y);
}

TEST(FrameTestCase, ClearTest)
{
	Frame tmp_frame(validFrameSize);
	tmp_frame.ClearFrame();

	const COORD newSize{ 12, 12 };

	tmp_frame.SetFrameSize(newSize);
	tmp_frame.ClearFrame();
}

//---------------------------------------------------------------

TEST(TextWidgetTestCase, CreationTest)
{
    const COORD renderPoint{ 0, 0 };
    const COORD widgetSize{ 80, 30 };
    const Frame validFrame(widgetSize);

    TextWidget* widget = nullptr;

    EXPECT_NO_THROW(widget = new TextWidget(validFrame, renderPoint, widgetSize));
    delete widget;

    EXPECT_NO_THROW(widget = new TextWidget());
    delete widget;

    const COORD newWidgetSize{ widgetSize.X + 1, widgetSize.Y + 1 };
    EXPECT_ANY_THROW(widget = new TextWidget(validFrame, 
                                             renderPoint,
                                             newWidgetSize));
    //delete widget;
}


//---------------------------------------------------------------

Frame CreateMainScreen(const COORD& frameSize);
Frame CreateInfoFrame(const COORD& frameSize);
Frame CreateMessageFrame(const COORD& frameSize);
Frame CreateInputFrame(const COORD& frameSize);

int main(int argc, char** argv)
{
    /**
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
    **/

    ///**
    int exit_code = 0;

    HANDLE std_in = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE std_out = GetStdHandle(STD_OUTPUT_HANDLE);

    if (std_in != INVALID_HANDLE_VALUE)
    {
        DWORD savedOldConsoleMode;
        if (GetConsoleMode(std_in, &savedOldConsoleMode) == TRUE)
        {
            if (SetConsoleMode(std_in, (ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT)) == TRUE)
            {
                {
                    COORD consoleSize = COORD{ 80, 30 };
                    SMALL_RECT windowSize = { 0,
                                              0,
                                              consoleSize.X - 1,
                                              consoleSize.Y - 1 };

                    SetConsoleWindowInfo(std_out, TRUE, &windowSize);
                    SetConsoleScreenBufferSize(std_out, consoleSize);
                }

                const COORD widgetSize = COORD{ 80, 10 };

                std::shared_ptr<EventHandler> eventHandler = std::make_shared<EventHandler>(std_in, std_out);

                eventHandler->StartEventHandling();

                std::shared_ptr<TextWidget> widgetInfo = std::make_shared<TextWidget>(L"CHAT ROOM IP =",
                    COORD{ 0, 0 },
                    widgetSize);
                widgetInfo->DisplayWidget();

                std::shared_ptr<MessageWidget> widgetMessage = std::make_shared<MessageWidget>(COORD{ 0, 10 },
                    widgetSize);

                widgetMessage->DisplayWidget();

                for (int i = 0; i < 6; i++)
                {
                    widgetMessage->PrintMessage(L"Nick: Hi");
                }

                //widgetMessage->MoveBufferOneLineAbove();
                widgetMessage->PrintMessage(L"John: Hi");
                widgetMessage->PrintMessage(L"John: Hi");


                for (int i = 0; i < 3; i++)
                {
                    widgetMessage->PrintMessage(L"Jane: Hi");
                }


                std::shared_ptr<InputWidget> widgetInput = std::make_shared<InputWidget>(COORD{ 0, 20 },
                    widgetSize);

                widgetInput->DisplayWidget();

                eventHandler->Observe(widgetInput.get());

                HANDLE waitEvent = CreateEvent(nullptr, TRUE, FALSE, L"KeepWaitingEvent");

                if (waitEvent != NULL)
                {
                    WaitForSingleObject(waitEvent, INFINITE);
                    CloseHandle(waitEvent);
                }

                eventHandler->StopEventHandling();

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

    return exit_code;
    //**/
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

Frame CreateInfoFrame(const COORD& frameSize)
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

Frame CreateMessageFrame(const COORD& frameSize)
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

Frame CreateInputFrame(const COORD& frameSize)
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