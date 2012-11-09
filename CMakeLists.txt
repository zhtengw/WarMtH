cmake_minimum_required(VERSION 2.4)
project(warmth)

find_package(Qt4 REQUIRED)


add_subdirectory(src bin)
add_subdirectory(l10n)
add_subdirectory(images)

INSTALL(FILES AUTHORS Changelog COPYING INSTALL README TODO DESTINATION doc/warmth)