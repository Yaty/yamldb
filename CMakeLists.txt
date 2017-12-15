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
        src/code/sql/query.c
        src/code/sql/select.c
        src/code/sql/utils.c
        src/code/sql/insert.c
        src/code/yaml/api/db.c
        src/code/sql/join.c
        src/code/utils/hashmap.c
        src/code/sql/operators.c
        src/code/sql/parser.c
        src/code/sql/conditions.c
        src/code/utils/type.c
        src/code/sql/order.c)

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
        src/header/sql/query.h
        src/header/sql/utils.h
        src/header/sql/select.h
        src/header/sql/insert.h
        src/header/yaml/api/db.h
        src/header/sql/join.h
        src/header/utils/hashmap.h
        src/header/sql/parser.h
        src/header/sql/operators.h
        src/header/sql/conditions.h
        src/header/utils/type.h
        src/header/sql/order.h)

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
        src/code/utils/type.c

        tests/main.c
        tests/utils.c
        tests/src/code/parser/allParser.c
        tests/src/code/api/data.c
        tests/src/code/api/children.c
        tests/src/code/api/allApi.c
        tests/src/code/api/free.c
        tests/src/code/api/type.c
        tests/src/code/utils/string.c
        tests/src/code/utils/type.c)

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
        src/header/utils/type.h

        tests/src/header/minunit.h
        tests/src/header/utils.h
        tests/src/header/parser/allParser.h
        tests/src/header/api/data.h
        tests/src/header/api/children.h
        tests/src/header/api/allApi.h
        tests/src/header/api/free.h
        tests/src/header/api/output.h
        tests/src/header/api/type.h
        tests/src/header/utils/string.h
        tests/src/header/utils/type.h)

add_executable(cbdd_tests ${TESTS_SRCS} ${TESTS_HEADERS})