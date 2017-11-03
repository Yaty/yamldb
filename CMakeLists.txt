cmake_minimum_required(VERSION 3.8)
project(CBDD1)

set(SRCS
        src/code/main.c
        src/code/yaml/api.c
        src/code/yaml/parser.c
        src/code/db_manager.c
        src/code/db_manipulation.c
        src/code/directory_functions.c
        src/code/file_manager.c
        src/code/general.c
        src/code/string_array_functions.c
        src/code/table_manipulation.c
        )

set(HEADERS
        src/header/yaml/api.h
        src/header/yaml/parser.h
        src/header/yaml/node.h
        src/header/db_manager.h
        src/header/db_manipulation.h
        src/header/directory_functions.h
        src/header/file_manager.h
        src/header/general.h
        src/header/string_array_functions.h
        src/header/table_manipulation.h
        )

add_executable(cbdd ${SRCS} ${HEADERS})