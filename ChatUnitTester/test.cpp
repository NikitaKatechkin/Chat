#include "pch.h"

#include <ChatClient/Frame.h>

const COORD emptyFrameSize = COORD{ 0, 0 };
const COORD validFrameSize = COORD{ 10, 10 };

bool operator==(const COORD& left, const COORD& right)
{
	return ((left.X == right.X) && (left.Y == right.Y));
}

TEST(FrameTestCase, CreationTest) 
{
	EXPECT_NO_THROW(new Frame(validFrameSize));
	EXPECT_NO_THROW(new Frame(emptyFrameSize));
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

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}