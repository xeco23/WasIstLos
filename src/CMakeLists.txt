message(STATUS "Configuring some source code files")
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/VERSION" ${WIL_VERSION})
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/Config.hpp)

find_package(PkgConfig REQUIRED)

find_package(Intl REQUIRED)
find_package(Gettext REQUIRED)
include_directories(${Intl_INCLUDE_DIRS})
link_directories(${INTL_LIBRARY_DIRS})

pkg_check_modules(GTKMM gtkmm-3.0 REQUIRED)
include_directories(${GTKMM_INCLUDE_DIRS})

pkg_check_modules(LIBHANDY libhandy-1 REQUIRED)
include_directories(${LIBHANDY_INCLUDE_DIRS})

pkg_check_modules(WEBKIT webkit2gtk-4.1)
if (NOT WEBKIT_FOUND)
    pkg_check_modules(WEBKIT webkit2gtk-4.0 REQUIRED)
endif()
include_directories(${WEBKIT_INCLUDE_DIRS})

pkg_check_modules(APPINDICATOR ayatana-appindicator3-0.1 REQUIRED)
include_directories(${APPINDICATOR_INCLUDE_DIRS})

pkg_check_modules(CANBERRA libcanberra REQUIRED)
include_directories(${CANBERRA_INCLUDE_DIRS})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

list(APPEND
        WIL_SRCS
        main.cpp
        Resources.c
        util/Helper.cpp
        util/Settings.cpp
        util/Sound.cpp
        ui/Application.cpp
        ui/MainWindow.cpp
        ui/PreferencesWindow.cpp
        ui/PhoneNumberDialog.cpp
        ui/TrayIcon.cpp
        ui/WebView.cpp
    )

add_executable(${CMAKE_PROJECT_NAME} ${WIL_SRCS})
target_link_libraries(${CMAKE_PROJECT_NAME}
        ${GTKMM_LIBRARIES}
        ${LIBHANDY_LIBRARIES}
        ${WEBKIT_LIBRARIES}
        ${APPINDICATOR_LIBRARIES}
        ${CANBERRA_LIBRARIES}
        ${Intl_LIBRARIES}
    )

install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
