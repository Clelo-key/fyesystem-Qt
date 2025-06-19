cmake_minimum_required(VERSION 3.30.5)
project(FyeQt VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
cmake_policy(SET CMP0118 NEW)
set(CMAKE_PREFIX_PATH "D:/Qt/6.9.0/msvc2022_64")
set(MYSQL_DIR "D:/MySQL/MySQL Server 8.0")

# 使用环境变量或让用户指定 Qt 路径
if(NOT DEFINED CMAKE_PREFIX_PATH)
    message(FATAL_ERROR "CMAKE_PREFIX_PATH is not defined. Please set it to your Qt installation directory.")
endif()
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH})

if(MSVC)
    add_compile_options(/Zc:__cplusplus)
endif()
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) 
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) 
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)  # Windows中CMake会将dll文件放在RUNTIME下

message(${CMAKE_BINARY_DIR})

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools Sql Gui Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools Sql Gui Network)

if(NOT DEFINED MYSQL_DIR)
    message(FATAL_ERROR "MYSQL_DIR is not defined. Please set it to your MySQL installation directory.")
endif()
set(feature_sql_mysql ON)
set(MySQL_INCLUDE_DIR "${MYSQL_DIR}/include")
set(MySQL_LIBRARY "${MYSQL_DIR}/lib/libmysql.lib") # 选择静态库进行链接

find_package(MySQL REQUIRED)

set(TS_FILES FyeQt_zh_CN.ts)
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(FyeQt
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        "asset/resource.qrc"
    )
    qt_create_translation(
      mainwindow.h mainwindow.cpp mainwindow.uiQM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(FyeQt SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(FyeQt
            ${PROJECT_SOURCES}
        )
    endif()
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()


add_subdirectory(vendor/libsodium-cmake)
add_subdirectory(M_utils)
add_subdirectory(M_components)
add_subdirectory(M_views)

target_link_libraries(FyeQt PUBLIC 
    M_Components
    M_Views
    M_Utils
    Qt${QT_VERSION_MAJOR}::Widgets
    ${MySQL_LIBRARY} # 链接 MySQL 库
)

target_precompile_headers(FyeQt PUBLIC ./mainwindow.h)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.FyeQt)
endif()
set_target_properties(FyeQt PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# 设置安装的位置前缀,在Windows是C:\Program Files。注意！这个设置必须在顶层的CMakeLists.txt当中指定
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/out/install)

include(GNUInstallDirs)
install(TARGETS FyeQt
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(FyeQt)
endif()

