//
// Created by Hugo on 04/11/2017.
//

#include <string.h>
#include <stdio.h>
#include "../minunit.h"
#include "../../src/code/string_array_functions.c"
#include "../../src/code/yaml/utils/parser.c"
#include "../../src/code/yaml/api/parse.c"
#include "../../src/code/yaml/api/free.c"
#include "../../src/code/yaml/api/output.c"

int tests_run = 0;
char *outputPath = "../resources/parser/output.yml";
char *testMapsPath = "../resources/parser/maps.yml";
char *testSequencesPath = "../resources/parser/sequences.yml";
char *mixSequencePath = "../resources/parser/mix.yml";

static int filesEquals(char *path1, char *path2) {
    if (path1 && path2) {
        FILE *file1 = fopen(path1, "r");
        FILE *file2 = fopen(path2, "r");
        if (file1 && file2) {
            char char1 = getc(file1);
            char char2 = getc(file2);

            while ((char1 != EOF) && (char2 != EOF) && (char1 == char2)) {
                char1 = getc(file1);
                char2 = getc(file2);
            }

            fclose(file1);
            fclose(file2);

            return char1 == char2;
        }
    }
    return 0;
}

static char *mapsAreCorrectlyParsedAndPrinted() {
    Node *root = YAMLParseFile(testMapsPath);
    YAMLSaveNode(root, outputPath);
    YAMLFreeNode(root);
    mu_assert("error, maps output not identical", filesEquals(testMapsPath, outputPath));
    return 0;
}

static char *sequenceAreCorrectlyParsedAndPrinted() {
    printf("A\n");
    Node *root = YAMLParseFile(testSequencesPath);
    printf("B\n");
    YAMLSaveNode(root, outputPath);
    printf("C\n");
    YAMLFreeNode(root);
    printf("D\n");
    mu_assert("error, sequences output not identical", filesEquals(testSequencesPath, outputPath));
    return 0;
}

static char *mixAreCorrectlyParsedAndPrinted() {
    Node *root = YAMLParseFile(mixSequencePath);
    YAMLSaveNode(root, outputPath);
    YAMLFreeNode(root);
    mu_assert("error, mix output not identical", filesEquals(mixSequencePath, outputPath));
    return 0;
}

char *values_all_tests() {
    printf("1\n");
    mu_run_test(mapsAreCorrectlyParsedAndPrinted);
    printf("2\n");
    mu_run_test(sequenceAreCorrectlyParsedAndPrinted);
    printf("3\n");
    mu_run_test(mixAreCorrectlyParsedAndPrinted);
    printf("4\n");
    return 0;
}