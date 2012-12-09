SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
SET(mentohust_LIB_SRCS 
    mentohust.c
    dlfunc.c
    md5.c
    mycheck.c
    myconfig.c
    myini.c
    myfunc.c
    mystate.c)
    #qtsinglecoreapplication.c 
    #qtlockedfile.c)
SET(mentohust_LIB_HEADS 
    mentohust.h
    dlfunc.h
    md5.h
    mycheck.h
    myconfig.h
    myini.h
    myfunc.h
    mystate.h
    types.h
    i18n.h)
    #qtsinglecoreapplication.h
    #qtlockedfile.h)

add_library(mentohust STATIC ${mentohust_LIB_SRCS} ${mentohust_LIB_HEADS})
set_target_properties(mentohust PROPERTIES VERSION 0 SOVERSION 0.3)

install(TARGETS mentohust DESTINATION lib)
