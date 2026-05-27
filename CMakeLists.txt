cmake_minimum_required(VERSION 3.16)
project(mediaplayer_lab9 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Multimedia Sql Test)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appmediaplayer_lab9
    main.cpp
    databasemanager.cpp
    databasemanager.h
    playlistmanager.cpp
    playlistmanager.h
    playermodel.cpp
    playermodel.h
    playerviewmodel.cpp
    playerviewmodel.h
)

qt_add_qml_module(appmediaplayer_lab9
    URI mediaplayer_lab9
    VERSION 1.0
    QML_FILES
        Main.qml
        PlayerPage.qml
        PlaylistPage.qml
        SettingsPage.qml
)

set_target_properties(appmediaplayer_lab9 PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appmediaplayer_lab9 PRIVATE
    Qt6::Quick
    Qt6::Multimedia
    Qt6::Sql
)

qt_add_executable(tst_player
    tests/tst_player.cpp
    databasemanager.cpp
    playlistmanager.cpp
    playermodel.cpp
)

set_target_properties(tst_player PROPERTIES
    AUTOMOC ON
)

target_link_libraries(tst_player PRIVATE
    Qt6::Test
    Qt6::Core
    Qt6::Sql
    Qt6::Multimedia
)

enable_testing()
add_test(NAME PlayerTest COMMAND tst_player)

install(TARGETS appmediaplayer_lab9
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)