//
// Created by Hugo on 10/11/2017.
//

#include <string.h>
#include "../../header/minunit.h"
#include "../../../../src/header/yaml/api.h"

static char *getMapNode() {
    Node *node = YAMLGetMapNode("key");
    mu_assert("getMapNode", YAMLGetType(node) == MAP);
    return 0;
}

static char *getSequenceNode() {
    Node *node = YAMLGetSequenceNode("key");
    mu_assert("getSequenceNode", YAMLGetType(node) == SEQUENCE);
    return 0;
}

static char *getSequenceValueNode() {
    Node *node = YAMLGetSequenceValueNode();
    mu_assert("getSequenceValueNode", YAMLGetType(node) == SEQUENCE_VALUE);
    return 0;
}

static char *getValueNode() {
    Node *node = YAMLGetValueNode("key", "value");
    mu_assert("getValueNode", YAMLGetType(node) == VALUE);
    return 0;
}

static char *getValue() {
    Node *node = YAMLGetValueNode("key", "value");
    mu_assert("getValue", strcmp(YAMLGetValue(node), "value") == 0);
    return 0;
}

static char *getKey() {
    Node *node = YAMLGetValueNode("key", "value");
    mu_assert("getKey", strcmp(YAMLGetKey(node), "key") == 0);
    return 0;
}

static char *setValue1() {
    Node *node = YAMLGetValueNode("key", "value");
    mu_assert("setValue1", YAMLSetValue(node, "value2") && strcmp(YAMLGetValue(node), "value2") == 0);
    return 0;
}

static char *setValue2() {
    Node *node = YAMLGetValueNode("key", "value");
    mu_assert("setValue2", YAMLSetValue(node, NULL) == 0);
    return 0;
}

static char *setKey1() {
    Node *node = YAMLGetValueNode("key", "value");
    mu_assert("setKey1", YAMLSetKey(node, "key2") && strcmp(YAMLGetKey(node), "key2") == 0);
    return 0;
}

static char *setKey2() {
    Node *node = YAMLGetValueNode("key", "value");
    mu_assert("setKey2", YAMLSetKey(node, NULL) == 0);
    return 0;
}

char *dataAllTests() {
    mu_run_test(getMapNode);
    mu_run_test(getSequenceNode);
    mu_run_test(getSequenceValueNode);
    mu_run_test(getValueNode);
    mu_run_test(getValue);
    mu_run_test(getKey);
    mu_run_test(setValue1);
    mu_run_test(setValue2);
    mu_run_test(setKey1);
    mu_run_test(setKey2);
    return 0;
}