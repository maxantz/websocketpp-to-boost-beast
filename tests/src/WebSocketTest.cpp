#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Mocked/MockedConnection.h"

TEST(WebSocketTest, SendMessage)
{
    auto conn = std::make_shared<MockedConnection>();

    EXPECT_CALL(*conn, send("hello"));

    conn->send("hello");
}
