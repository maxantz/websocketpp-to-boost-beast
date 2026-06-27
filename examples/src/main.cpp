#include "BeastServer.hpp"
#include <iostream>

int main()
{
    ws::BeastServer server;

    server.setMessageHandler(
        [&server](auto conn, std::string msg)
        {
            std::cout << msg << std::endl;

            conn->send("echo: " + msg);
        });

    server.listen(9002);
    server.run();
}
