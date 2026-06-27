#ifndef __MOCKED_CONNECTION_H__
#define __MOCKED_CONNECTION_H__

#include <Mockables/IWebSocketConnection.h>
#include <gmock/gmock.h>

class MockedConnection : public ws::IWebSocketConnection
{
public:
    MOCK_METHOD(void, send, (std::string_view), (override));
    MOCK_METHOD(void, close, (), (override));
};

#endif	// __MOCKED_CONNECTION_H__
