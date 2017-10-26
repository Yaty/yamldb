cmake_minimum_required(VERSION 3.8)
project(CBDD1)

set(SRCS
        src/main.c
        src/gui/gui.c
        src/ui/ui.c)

set(HEADERS
        src/gui/gui.h
        src/ui/ui.h)

add_executable(cbdd ${SRCS} ${HEADERS})