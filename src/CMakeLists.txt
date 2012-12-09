include_directories(${QT_INCLUDES} ${QT_QTNETWORK_INCLUDE_DIR})
SET(CMAKE_C_COMPILER "/usr/bin/g++")
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(SRC_LIST 
    main.cpp 
    mainwindow.cpp 
    authmsgwindow.cpp 
    configwindow.cpp 
    aboutwindow.cpp)
#     core/mentohust.cpp
#     core/dlfunc.cpp
#     core/md5.cpp
#     core/mycheck.cpp
#     core/myconfig.cpp
#     core/myini.cpp
#     core/myfunc.cpp
#     core/mystate.cpp)
#    qtsingleapplication.cpp
#    qtlocalpeer.cpp)
SET(GUI_HEAD_LIST 
    mainwindow.h 
    authmsgwindow.h 
    configwindow.h 
    aboutwindow.h)
# SET(CORE_HEAD_LIST
#     core/mentohust.h
#     core/dlfunc.h
#     core/md5.h
#     core/mycheck.h
#     core/myconfig.h
#     core/myini.h
#     core/myfunc.h
#     core/mystate.h
#     core/types.h
#     core/i18n.h)
#    qtsingleapplication.h
#    qtlocalpeer.h)

set_source_files_properties(${SRC_LIST} PROPERTIES LANGUAGE CXX )
SET(warmth_RESOURCES ../images/images.qrc)

add_subdirectory(qtsingleapplication)
add_subdirectory(core)

QT4_ADD_RESOURCES(RESOURCES_QRC ${warmth_RESOURCES})

QT4_WRAP_CPP(MOC_SRCS ${GUI_HEAD_LIST})
ADD_EXECUTABLE(warmth ${SRC_LIST} ${MOC_SRCS}  ${RESOURCES_QRC})

TARGET_LINK_LIBRARIES(warmth ${QT_LIBRARIES} mentohust QtSingleApplication)

INSTALL(TARGETS warmth DESTINATION bin)
INSTALL(FILES warmth.desktop DESTINATION share/applications)

