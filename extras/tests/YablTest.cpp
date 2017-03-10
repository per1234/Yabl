#include <Yabl.h>
#include <gtest/gtest.h>

using ::testing::Return;

TEST(YablTest, initialUpState) {
  Button button;
  
  EXPECT_CALL(button, read())
    .WillRepeatedly(Return(true));

  EXPECT_TRUE(button.inverted());
  EXPECT_TRUE(button.read());
  EXPECT_FALSE(button.down());
  EXPECT_FALSE(button.activity());
}

TEST(YablTest, initiaDownState) {
  Button button;
  
  EXPECT_CALL(button, read())
    .WillRepeatedly(Return(false));
  
  EXPECT_TRUE(button.inverted());
  EXPECT_FALSE(button.read());
  EXPECT_FALSE(button.down());
  EXPECT_FALSE(button.activity());
}
