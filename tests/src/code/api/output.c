//
// Created by Hugo on 10/11/2017.
//

#include "../../header/minunit.h"
#include "../../header/utils.h"
#include "../../../../src/header/yaml/api.h"

char *outputPath = "./resources/tests/parser/output.yml";
char *testMapsPath = "./resources/tests/parser/maps.yml";
char *testSequencesPath = "./resources/tests/parser/sequences.yml";
char *mixSequencePath = "./resources/tests/parser/mix.yml";

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

char *outputAllTests() {
    mu_run_test(mapsAreCorrectlyParsedAndPrinted);
    mu_run_test(sequenceAreCorrectlyParsedAndPrinted);
    mu_run_test(mixAreCorrectlyParsedAndPrinted);
    return 0;
}