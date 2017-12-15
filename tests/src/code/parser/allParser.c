//
// Created by Hugo on 04/11/2017.
//

#include <stdio.h>
#include "../../../../src/header/yaml/api.h"
#include "../../header/minunit.h"
#include "../../header/utils.h"


char *outputPath = "./resources/tests/parser/output.yml";
char *testMapsPath = "./resources/tests/parser/maps.yml";
char *testSequencesPath = "./resources/tests/parser/sequences.yml";
char *mixSequencePath = "./resources/tests/parser/mix.yml";

static char *mapsAreCorrectlyParsedAndPrinted() {
    Node *root = YAMLParseFile(testMapsPath);
    YAMLSaveNode(root, outputPath);
    mu_assert("mapsAreCorrectlyParsedAndPrinted", filesEquals(testMapsPath, outputPath));
}

static char *sequenceAreCorrectlyParsedAndPrinted() {
    Node *root = YAMLParseFile(testSequencesPath);
    YAMLSaveNode(root, outputPath);
    mu_assert("sequenceAreCorrectlyParsedAndPrinted", filesEquals(testSequencesPath, outputPath));
}

static char *mixAreCorrectlyParsedAndPrinted() {
    Node *root = YAMLParseFile(mixSequencePath);
    YAMLSaveNode(root, outputPath);
    mu_assert("mixAreCorrectlyParsedAndPrinted", filesEquals(mixSequencePath, outputPath));
}

static char *multipleParse() {
    Node *root = YAMLParseFile(mixSequencePath);
    int i;
    int iterations = 10;

    for (i = 0; i < iterations; i++) {
        YAMLSaveNode(root, outputPath);
        if (!filesEquals(mixSequencePath, outputPath)) {
            break;
        }
        root = YAMLParseFile(outputPath);
    }

    mu_assert("multipleParse", i == iterations);
}

static char *allTests() {
    mu_run_test(mapsAreCorrectlyParsedAndPrinted);
    mu_run_test(sequenceAreCorrectlyParsedAndPrinted);
    mu_run_test(mixAreCorrectlyParsedAndPrinted);
    mu_run_test(multipleParse);
    remove(outputPath);
    return 0;
}

int parserTestAll() {
    printf("Testing parser ... ");
    char *result = allTests();
    if (result != NULL) {
        printf("Error : %s\n", result);
    } else {
        printf("OK !\n");
    }

    return result == NULL;
}