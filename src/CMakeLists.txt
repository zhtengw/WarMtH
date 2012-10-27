include_directories(${QT_INCLUDES})

########### next target ###############

set(SRCS main.cpp mainwindow.cpp configwindow.cpp)

add_executable(gmentohust ${SRCS})

#install(TARGETS gmentohust DESTINATION ${BIN_INSTALL_DIR})

########### install files ###############

#install(FILES gmentohust.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})

#install_icons(${ICON_INSTALL_DIR})


