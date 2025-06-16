# M_Views/CMakeLists.txt

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_DIR ${CMAKE_CURRENT_BINARY_DIR}/M_Views_autogen/include)
file(MAKE_DIRECTORY ${CMAKE_AUTOUIC_DIR})  # //


# Windows平台动态库需要设置WINDOWS_EXPORT_ALL_SYMBOLS
if (WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif ()
set(BUILD_SHARED_LIBS ON)

add_library(M_Views SHARED "")

target_sources(M_Views
        PRIVATE
        login/mylogin.cpp
        login/mylogin.ui
        register/myRegister.cpp
        register/myRegister.ui
        loginWindow/myLoginWindow.cpp
        loginWindow/myLoginWindow.ui
        PUBLIC
        login/mylogin.h
        register/myRegister.h
        loginWindow/myLoginWindow.h
)

target_link_libraries(M_Views PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Sql
        M_Utils
        M_Components
)

# 加载Ui文件
target_include_directories(M_Views PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_AUTOUIC_DIR}
)

# 安装lib，bin，exe文件
install(TARGETS M_Views
        ARCHIVE # bin
        DESTINATION lib
        LIBRARY # lib
        DESTINATION bin
        RUNTIME # exe
        DESTINATION bin
)

