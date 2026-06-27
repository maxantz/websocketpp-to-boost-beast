cmake_minimum_required(VERSION 4.2)

project(WebSocketServer LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_sources(${PROJECT_NAME}
	PUBLIC
		FILE_SET headers
		TYPE HEADERS
		BASE_DIRS
			include
		FILES
			include/Mockables/IWebSocketConnection.h
			include/Mockables/IWebSocketServer.h
			include/BeastConnection.hpp
			include/BeastServer.hpp
)

add_subdirectory(examples)

enable_testing()

add_subdirectory(tests)