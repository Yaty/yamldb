//
// Created by Hugo on 10/11/2017.
//

#include <stdio.h>
#include "../../header/minunit.h"
#include "../../../../src/header/yaml/api.h"

static char *isCollection1() {
    Node *node = YAMLGetSequenceNode("key");
    mu_assert("isCollection1", YAMLIsCollection(node));
    return 0;
}

static char *isCollection2() {
    Node *node = YAMLGetMapNode("key");
    mu_assert("isCollection2", YAMLIsCollection(node));
    return 0;
}

static char *isCollection3() {
    Node *node = YAMLGetSequenceValueNode();
    mu_assert("isCollection3", YAMLIsCollection(node));
    return 0;
}

static char *isCollection4() {
    Node *node = YAMLGetValueNode("key", "value");
    mu_assert("isCollection4", !YAMLIsCollection(node));
    return 0;
}

static char *isSequence() {
    Node *node = YAMLGetSequenceNode("key");
    mu_assert("isSequence", YAMLIsSequence(node));
    return 0;
}

static char *isMap() {
    Node *node = YAMLGetMapNode("key");
    mu_assert("isMap", YAMLIsMap(node));
    return 0;
}

static char *isSequenceValue() {
    Node *node = YAMLGetSequenceValueNode();
    mu_assert("isSequenceValue", YAMLIsSequenceValue(node));
    return 0;
}

static char *isValue() {
    Node *node = YAMLGetValueNode("key", "value");
    mu_assert("isValue", YAMLIsValue(node));
    return 0;
}

static char *isUndefined() {
    Node *node = YAMLGetValueNode("key", "value");
    node->type = UNDEFINED;
    mu_assert("isUndefined", YAMLIsUndefined(node));
    return 0;
}

static char *getType() {
    Node *node = YAMLGetMapNode("key");
    mu_assert("getType", YAMLGetType(node) == MAP);
    return 0;
}

static char *setType() {
    Node *node = YAMLGetMapNode("key");
    mu_assert("setType", YAMLSetType(node, SEQUENCE) && YAMLGetType(node) == SEQUENCE);
    return 0;
}

char *typeAllTests() {
    mu_run_test(isCollection1);
    mu_run_test(isCollection2);
    mu_run_test(isCollection3);
    mu_run_test(isCollection4);
    mu_run_test(isSequence);
    mu_run_test(isMap);
    mu_run_test(isSequenceValue);
    mu_run_test(isValue);
    mu_run_test(isUndefined);
    mu_run_test(getType);
    mu_run_test(setType);
}