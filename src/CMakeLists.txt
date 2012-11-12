include_directories(${QT_INCLUDES} ${QT_QTNETWORK_INCLUDE_DIR})

INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(SRC_LIST 
    main.cpp 
    mainwindow.cpp 
    authmsgwindow.cpp 
    configwindow.cpp 
    aboutwindow.cpp)
#    qtsingleapplication.cpp
#    qtlocalpeer.cpp)
SET(HEAD_LIST 
    mainwindow.h 
    authmsgwindow.h 
    configwindow.h 
    aboutwindow.h)
#    qtsingleapplication.h
#    qtlocalpeer.h)

SET(warmth_RESOURCES ../images/images.qrc)

add_subdirectory(qtsingleapplication)

QT4_ADD_RESOURCES(RESOURCES_QRC ${warmth_RESOURCES})

QT4_WRAP_CPP(MOC_SRCS ${HEAD_LIST})
ADD_EXECUTABLE(warmth ${SRC_LIST} ${MOC_SRCS} ${RESOURCES_QRC})

TARGET_LINK_LIBRARIES(warmth ${QT_LIBRARIES} QtSingleApplication)

INSTALL(TARGETS warmth DESTINATION bin)
INSTALL(FILES warmth.desktop DESTINATION share/applications)

