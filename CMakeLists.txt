project(crawler-qt)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

# Compilation settings
set (CMAKE_CXX_COMPILER             "/usr/bin/clang++")
set (CMAKE_CXX_FLAGS "-std=c++11 -pedantic -pedantic-errors -Wall -Wextra -Werror -Wdeprecated -march=native -fsanitize=address -fno-omit-frame-pointer ${CMAKE_CXX_FLAGS}")

set (CMAKE_CXX_FLAGS_DEBUG          "-g -O0 -DDEBUG") # -DVERBOSE
set (CMAKE_CXX_FLAGS_RELEASE        "-Ofast -DNDEBUG")
set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Ofast -g")

set (CMAKE_AR      "/usr/bin/llvm-ar")
set (CMAKE_LINKER  "/usr/bin/llvm-ld")
set (CMAKE_NM      "/usr/bin/llvm-nm")
set (CMAKE_OBJDUMP "/usr/bin/llvm-objdump")

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR} ../crawler)
link_directories( ../../crawler/build )

set(crawler-qt_SRCS 
	crawler-qt.cpp 
	devpick.cpp
	main.cpp
	CrawlerThread.cpp
	DiskListWidgetItem.cpp
	NotificationWidget.cpp
)
qt4_automoc(${crawler-qt_SRCS})
add_executable(crawler-qt ${crawler-qt_SRCS})
target_link_libraries(crawler-qt ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} blkid crawl)
install(TARGETS crawler-qt RUNTIME DESTINATION bin)
