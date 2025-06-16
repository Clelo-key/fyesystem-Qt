# components/CMakeLists.txt

# Windows平台动态库需要设置WINDOWS_EXPORT_ALL_SYMBOLS
if(WIN32)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()
set(BUILD_SHARED_LIBS ON)


add_library(M_Components SHARED "")

# 使用 target_sources 按组件组织源文件
target_sources(M_Components
    PRIVATE
        myButton/myButton.cpp
        myMenu/myMenu.cpp
        myLabel/myLabel.cpp
        myTimer/myTimer.cpp
        myTest/myTest.cpp
        myMessageBox/myMessageBox.cpp
        myMessageItem/myMessageItem.cpp
    PUBLIC
        myButton/myButton.h
        myMenu/myMenu.h
        myLabel/myLabel.h
        myTimer/myTimer.h
        myTest/myTest.h
        myMessageBox/myMessageBox.h
        myMessageItem/MyMessageItem.h
)

target_link_libraries(M_Components PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Gui
)

# 将当前目录设为公开头文件路径
target_include_directories(M_Components PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

install(TARGETS M_Components
ARCHIVE
    DESTINATION lib
LIBRARY #动态库
    DESTINATION lib
RUNTIME #可执行文件
    DESTINATION bin
)

