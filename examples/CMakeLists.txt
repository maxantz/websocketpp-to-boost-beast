cmake_minimum_required(VERSION 4.2)

project(WebSocketServerExample LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
	PRIVATE
		src/main.cpp
)


if(WIN32)
	target_link_libraries(${PROJECT_NAME}
		PRIVATE
			WebSocketServer
			wsock32
			ws2_32
	)
else()
	target_link_libraries(${PROJECT_NAME}
		PRIVATE
			WebSocketServer
	)
endif()
