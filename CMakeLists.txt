cmake_minimum_required(VERSION 3.8)
project(CBDD1)

set(SRCS
        src/code/main.c
        src/code/yaml/utils/parser.c
        src/code/yaml/api/children.c
        src/code/yaml/api/data.c
        src/code/yaml/api/free.c
        src/code/yaml/api/output.c
        src/code/yaml/api/parse.c
        src/code/yaml/api/type.c
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
        src/header/yaml/node.h
        src/header/yaml/utils/parser.h
        src/header/yaml/api/children.h
        src/header/yaml/api/data.h
        src/header/yaml/api/free.h
        src/header/yaml/api/output.h
        src/header/yaml/api/parse.h
        src/header/yaml/api/type.h
        src/header/db_manager.h
        src/header/db_manipulation.h
        src/header/directory_functions.h
        src/header/file_manager.h
        src/header/general.h
        src/header/string_array_functions.h
        src/header/table_manipulation.h
        )

add_executable(cbdd ${SRCS} ${HEADERS})

# Tests

set(TESTS_SRCS

        src/code/string_array_functions.c
        src/code/yaml/utils/parser.c
        src/code/yaml/api/children.c
        src/code/yaml/api/data.c
        src/code/yaml/api/free.c
        src/code/yaml/api/output.c
        src/code/yaml/api/parse.c
        src/code/yaml/api/type.c

        tests/main.c
        tests/utils.c
        tests/src/code/parser/allParser.c
        tests/src/code/api/data.c
        tests/src/code/api/children.c
        tests/src/code/api/allApi.c
        tests/src/code/api/free.c
        tests/src/code/api/output.c
        tests/src/code/api/type.c)

set(TESTS_HEADERS

        src/header/yaml/api.h
        src/header/yaml/node.h
        src/header/yaml/utils/parser.h
        src/header/yaml/api/children.h
        src/header/yaml/api/data.h
        src/header/yaml/api/free.h
        src/header/yaml/api/output.h
        src/header/yaml/api/parse.h
        src/header/yaml/api/type.h

        src/header/string_array_functions.h
        tests/src/header/minunit.h
        tests/src/header/utils.h
        tests/src/header/parser/allParser.h
        tests/src/header/api/data.h
        tests/src/header/api/children.h
        tests/src/header/api/allApi.h
        tests/src/header/api/free.h
        tests/src/header/api/output.h
        tests/src/header/api/type.h)

add_executable(cbdd_tests ${TESTS_SRCS} ${TESTS_HEADERS})