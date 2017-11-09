//
// Created by Hugo on 04/11/2017.
//

#include <string.h>
#include <stdio.h>
#include "../minunit.h"
#include "../utils.c"
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

static char *mapsAreCorrectlyParsedAndPrinted() {
    Node *root = YAMLParseFile(testMapsPath);
    YAMLSaveNode(root, outputPath);
    YAMLFreeNode(root);
    mu_assert("mapsAreCorrectlyParsedAndPrinted", filesEquals(testMapsPath, outputPath));
    return 0;
}

static char *sequenceAreCorrectlyParsedAndPrinted() {
    Node *root = YAMLParseFile(testSequencesPath);
    YAMLSaveNode(root, outputPath);
    YAMLFreeNode(root);
    mu_assert("sequenceAreCorrectlyParsedAndPrinted", filesEquals(testSequencesPath, outputPath));
    return 0;
}

static char *mixAreCorrectlyParsedAndPrinted() {
    Node *root = YAMLParseFile(mixSequencePath);
    YAMLSaveNode(root, outputPath);
    YAMLFreeNode(root);
    mu_assert("mixAreCorrectlyParsedAndPrinted", filesEquals(mixSequencePath, outputPath));
    return 0;
}

char *end2end_all_tests() {
    mu_run_test(mapsAreCorrectlyParsedAndPrinted);
    mu_run_test(sequenceAreCorrectlyParsedAndPrinted);
    mu_run_test(mixAreCorrectlyParsedAndPrinted);
    return 0;
}