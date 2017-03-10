#include "../../src/Yabl.h"
#include <gtest/gtest.h>

TEST(YablTest, initialUpState) {
  Button button;
  
  EXPECT_FALSE(button.down());
  EXPECT_FALSE(button.activity());
}
