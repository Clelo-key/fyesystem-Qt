# M_utils/CMakeLists.txt

add_library(M_Utils "")

# 使用 target_sources 按组件组织源文件

target_sources(M_Utils
PRIVATE
    sqlHelper/sqlHelper.cpp
    passwordEncoder/passwordEncoder.cpp
    regexValidate/regexValidate.cpp

PUBLIC
    sqlHelper/sqlHelper.h
    passwordEncoder/passwordEncoder.h
    regexValidate/regexValidate.h
    httpRequestManager/httpRequestManager.cpp
    httpRequestManager/httpRequestManager.h
)

# 链接 Qt 模块

target_link_libraries(M_Utils 
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Network
    sodium
)

# 关键：将当前目录设为公开头文件路径

target_include_directories(M_Utils PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)
