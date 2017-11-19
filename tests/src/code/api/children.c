//
// Created by Hugo on 09/11/2017.
//

#include <stdlib.h>
#include <string.h>
#include "../../header/minunit.h"
#include "../../header/utils.h"
#include "../../../../src/header/yaml/api.h"

static char *getSize() {
    int size = 150;
    Node *node = YAMLGetMapNode("map");
    node->childrenNumber = size;
    mu_assert("getSize", YAMLGetSize(node) == size);
}

static char *getChildren1() {
    Node *node = YAMLGetMapNode("map");
    mu_assert("getChildren1", YAMLGetChildren(node) == node->children);
}

static char *getChildren2() {
    Node *node = NULL;
    mu_assert("getChildren2", YAMLGetChildren(node) == NULL);
}

static char *getChildAtIndex1() {
    Node *node = YAMLGetMapNode("node");
    Node *child1 = YAMLGetValueNode("child", "value");
    YAMLAddChild(node, child1);
    mu_assert("getChildAtIndex1", areNodeEquals(child1, YAMLGetChildAtIndex(node, 0)));
}

static char *getChildAtIndex2() {
    Node *node = YAMLGetMapNode("node");
    Node *child1 = YAMLGetValueNode("child", "value");
    YAMLAddChild(node, child1);
    mu_assert("getChildAtIndex2", YAMLGetChildAtIndex(node, -1) == NULL);
}

static char *getChildAtIndex3() {
    Node *node = YAMLGetMapNode("node");
    Node *child2 = YAMLGetChildAtIndex(node, 0);
    mu_assert("getChildAtIndex3", child2 == NULL);
}

static char *addValueChild1() {
    char key[] = "key";
    char value[] = "value";
    Node *node = YAMLGetMapNode("node");
    mu_assert("addValueChild1", YAMLAddValueChild(node, key, value) == 1 && strcmp(node->children[0].key, key) == 0 && strcmp(node->children[0].value, value) == 0);
}

static char *addValueChild2() {
    Node *node = YAMLGetMapNode("node");
    mu_assert("addValueChild2", YAMLAddValueChild(node, NULL, NULL) == 0);
}

static char *removeChildAtIndex1() {
    Node *node = YAMLGetMapNode("map");
    YAMLAddValueChild(node, "key", "value");
    mu_assert("removeChildAtIndex1" , YAMLRemoveChildAtIndex(node, 0));
}

static char *removeChildAtIndex2() {
    Node *node = YAMLGetMapNode("map");
    mu_assert("removeChildAtIndex2" , YAMLRemoveChildAtIndex(node, 0) == 0);
}

static char *removeChildAtIndex3() {
    Node *node = YAMLGetMapNode("map");
    YAMLAddValueChild(node, "key", "value");
    YAMLAddValueChild(node, "key2", "value");
    int res = YAMLRemoveChildAtIndex(node, 0);
    mu_assert("removeChildAtIndex3" , res && strcmp(YAMLGetKey(YAMLGetChildAtIndex(node, 0)), "key2") == 0 && YAMLGetSize(node) == 1);
}

static char *addChildAtIndex1() {
    Node *node = YAMLGetMapNode("map");
    Node *child = YAMLGetValueNode("key", "value");
    int res = YAMLAddChildAtIndex(node, child, 0);
    mu_assert("addChildAtIndex1", res && areNodeEquals(YAMLGetChildAtIndex(node, 0), child));
}

static char *addChildAtIndex2() {
    Node *node = YAMLGetMapNode("map");
    Node *child = YAMLGetValueNode("key", "value");
    int res = YAMLAddChildAtIndex(node, child, 50);
    mu_assert("addChildAtIndex2", res == 0);
}

static char *addChildAtIndex3() {
    Node *node = YAMLGetMapNode("map");
    Node *child = YAMLGetValueNode("key", "value");
    int res = YAMLAddChildAtIndex(node, child, -1);
    mu_assert("addChildAtIndex3", res == 0);
}

static char *removeChildren() {
    Node *node = YAMLGetMapNode("map");
    YAMLAddValueChild(node, "key", "value");
    mu_assert("removeChildren", YAMLRemoveChildren(node) && YAMLGetSize(node) == 0);
}

static char *setChildren() {
    Node *node = YAMLGetMapNode("map");
    Node *children = (Node*) malloc(sizeof(Node));
    children[0] = *YAMLGetValueNode("k", "v");
    int res = YAMLSetChildren(node, children, 1);
    mu_assert("setChildren", res && areNodeEquals(children, YAMLGetChildren(node)) && YAMLGetSize(node) == 1);
}

char *childrenAllTests() {
    mu_run_test(getSize);
    mu_run_test(getChildren1);
    mu_run_test(getChildren2);
    mu_run_test(getChildAtIndex1);
    mu_run_test(getChildAtIndex2);
    mu_run_test(getChildAtIndex3);
    mu_run_test(addValueChild1);
    mu_run_test(addValueChild2);
    mu_run_test(removeChildAtIndex1);
    mu_run_test(removeChildAtIndex2);
    mu_run_test(removeChildAtIndex3);
    mu_run_test(addChildAtIndex1);
    mu_run_test(addChildAtIndex2);
    mu_run_test(addChildAtIndex3);
    mu_run_test(removeChildren);
    mu_run_test(setChildren);
    return 0;
}