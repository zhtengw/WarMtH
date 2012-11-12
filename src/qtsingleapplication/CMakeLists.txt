include_directories(${QT_INCLUDES} ${QT_QTNETWORK_INCLUDE_DIR})

INCLUDE(${QT_USE_FILE})
set(QT_USE_QTNETWORK TRUE)
ADD_DEFINITIONS(${QT_DEFINITIONS})

SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
SET(qtsingleapplication_LIB_SRCS 
    qtsingleapplication.cpp 
    qtlocalpeer.cpp)
    #qtsinglecoreapplication.cpp 
    #qtlockedfile.cpp)
SET(qtsingleapplication_LIB_HEADS 
    qtsingleapplication.h 
    qtlocalpeer.h)
    #qtsinglecoreapplication.h
    #qtlockedfile.h)
QT4_WRAP_CPP(LIB_MOC_SRCS ${qtsingleapplication_LIB_HEADS})

add_library(QtSingleApplication SHARED ${LIB_MOC_SRCS} ${qtsingleapplication_LIB_SRCS})
TARGET_LINK_LIBRARIES(QtSingleApplication ${QT_LIBRARIES} ${QT_QTNETWORK_LIBRARIES})

set_target_properties(QtSingleApplication PROPERTIES VERSION 2 SOVERSION 2.6)

install(TARGETS QtSingleApplication DESTINATION lib)
