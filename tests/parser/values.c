//
// Created by Hugo on 04/11/2017.
//

#include <string.h>
#include <stdio.h>
#include "../minunit.h"
#include "../../src/code/yaml/parser.c"
#include "../../src/code/string_array_functions.c"
#include "../../src/code/yaml/api.c"

int tests_run = 0;
const char *outputPath = "../resources/output.yaml";
const char *testMapsPath = "../resources/parser_test_maps.yaml";
const char *testSequencesPath = "../resources/parser_test_sequences.yaml";
const char *mixSequencePath = "../resources/parser_test_mix.yaml";

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
    Node *root = YAMLParseFile(testSequencesPath);
    YAMLSaveNode(root, outputPath);
    YAMLFreeNode(root);
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
    mu_run_test(mapsAreCorrectlyParsedAndPrinted);
    mu_run_test(sequenceAreCorrectlyParsedAndPrinted);
    mu_run_test(mixAreCorrectlyParsedAndPrinted);
    return 0;
}