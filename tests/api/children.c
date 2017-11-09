//
// Created by Hugo on 09/11/2017.
//

#include "../minunit.h"
#include "../../src/code/yaml/api/data.c"
#include "../../src/code/yaml/api/type.c"
#include "../../src/code/yaml/api/children.c"

static char *getSize() {
    int size = 150;
    Node *node = getEmptyNode();
    node->childrenNumber = size;
    mu_assert("getSize", YAMLGetSize(node) == size);
    return 0;
}

static char *getChildren1() {
    Node *node = getEmptyNode();
    mu_assert("getChildren1", YAMLGetChildren(node) == node->children);
    return 0;
}

static char *getChildren2() {
    Node *node = NULL;
    mu_assert("getChildren2", YAMLGetChildren(node) == NULL);
    return 0;
}

static char *getChildAtIndex1() {
    Node *node = YAMLGetMapNode("node");
    Node *child1 = YAMLGetValueNode("child", "value");
    YAMLAddChild(node, child1);
    mu_assert("getChildAtIndex1", areNodeEquals(child1, YAMLGetChildAtIndex(node, 0)));
    return 0;
}

static char *getChildAtIndex2() {
    Node *node = YAMLGetMapNode("node");
    Node *child1 = YAMLGetValueNode("child", "value");
    YAMLAddChild(node, child1);
    mu_assert("getChildAtIndex2", YAMLGetChildAtIndex(node, -1) == NULL);
    return 0;
}

static char *getChildAtIndex3() {
    Node *node = YAMLGetMapNode("node");
    Node *child2 = YAMLGetChildAtIndex(node, 0);
    mu_assert("getChildAtIndex3", child2 == NULL);
    return 0;
}

static char *addValueChild1() {
    char key[] = "key";
    char value[] = "value";
    Node *node = YAMLGetMapNode("node");
    mu_assert("addValueChild1", YAMLAddValueChild(node, key, value) == 1 && strcmp(node->children[0].key, key) == 0 && strcmp(node->children[0].value, value) == 0);
    return 0;
}

static char *addValueChild2() {
    Node *node = YAMLGetMapNode("node");
    mu_assert("addValueChild2", YAMLAddValueChild(node, NULL, NULL) == 0);
    return 0;
}

static char *removeChildAtIndex() {
    // TODO
    return 0;
}

char *children_all_tests() {
    mu_run_test(getSize);
    mu_run_test(getChildren1);
    mu_run_test(getChildren2);
    mu_run_test(getChildAtIndex1);
    mu_run_test(getChildAtIndex2);
    mu_run_test(getChildAtIndex3);
    mu_run_test(addValueChild1);
    mu_run_test(addValueChild2);
    return 0;
}