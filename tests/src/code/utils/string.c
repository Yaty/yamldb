//
// Created by Hugo on 12/11/2017.
//

#include <stdio.h>
#include <string.h>
#include "../../header/minunit.h"
#include "../../../../src/header/string_array_functions.h"

static char *trim1() {
    char *msg = strdup("    spacesbefore");
    mu_assert("trim1", strcmp(trim(msg), "spacesbefore") == 0);
    return 0;
}

static char *trim2() {
    char *msg = strdup("spacesafter        ");
    mu_assert("trim2", strcmp(trim(msg), "spacesafter") == 0);
    return 0;
}

static char *trim3() {
    char *msg = strdup("    spaces     ");
    mu_assert("trim3", strcmp(trim(msg), "spaces") == 0);
    return 0;
}

static char *trim4() {
    mu_assert("trim4", trim(NULL) == NULL);
    return 0;
}

static char *trim5() {
    mu_assert("trim5", strcmp(trim(""), "") == 0);
    return 0;
}

static char *substring1() {
    char *msg = strdup("   msg   ");
    char substr[15] = "";
    int res = substring(msg, substr, 3, 3);
    mu_assert("substring1", strcmp(substr, "msg") == 0 && res);
    return 0;
}

static char *substring2() {
    char *msg = strdup("   msg   ");
    char substr[15] = "";
    mu_assert("substring2", substring(msg, substr, 9, 1) == 0);
    return 0;
}

static char *substring3() {
    char *msg = strdup("   msg   ");
    char substr[15] = "";
    mu_assert("substring3", substring(msg, substr, -1, 1) == 0);
    return 0;
}

static char *substring4() {
    char substr[15] = "";
    mu_assert("substring4", substring(NULL, substr, 5, 2) == 0);
    return 0;
}

static char *substring5() {
    char *msg = strdup("   msg   ");
    char substr[15] = "";
    int res = substring(msg, substr, 0, 3);
    mu_assert("substring5", strcmp(substr, "   ") == 0 && res);
    return 0;
}

static char *substring6() {
    char *msg = strdup("   msg   ");
    char substr[15] = "";
    int res = substring(msg, substr, 6, 3);
    mu_assert("substring6", strcmp(substr, "   ") == 0 && res);
    return 0;
}

static char *substring7() {
    mu_assert("substring7", substring(NULL, NULL, 5, 2) == 0);
    return 0;
}

static char *concat1() {
    char *str1 = "foo";
    char *str2 = "bar";
    mu_assert("concat1", strcmp(concat(str1, str2), "foobar") == 0);
    return 0;
}


static char *concat2() {
    char *str1 = "foo";
    char *str2 = NULL;
    mu_assert("concat2", strcmp(concat(str1, str2), str1) == 0);
    return 0;
}


static char *concat3() {
    char *str1 = NULL;
    char *str2 = "bar";
    mu_assert("concat3", strcmp(concat(str1, str2), str2) == 0);
    return 0;
}

static char *concat4() {
    mu_assert("concat4", concat(NULL, NULL) == NULL);
    return 0;
}

static char *toUppercase1() {
    char *str1 = strdup("foo bar __ x 123 ''");
    mu_assert("toUppercase1", strcmp(toUpperCase(str1), "FOO BAR __ X 123 ''") == 0);
    return 0;
}

static char *toUppercase2() {
    mu_assert("toUppercase1", toUpperCase(NULL) == NULL);
    return 0;
}

static char *startsWith1() {
    mu_assert("startsWith1", startsWith("manger", "man"));
    return 0;
}

static char *startsWith2() {
    mu_assert("startsWith2", startsWith("manger", "abcdef") == 0);
    return 0;
}

static char *startsWith3() {
    mu_assert("startsWith3", startsWith("manger", "") == 0);
    return 0;
}

static char *startsWith4() {
    mu_assert("startsWith4", startsWith("manger", NULL) == 0);
    return 0;
}

static char *startsWith5() {
    mu_assert("startsWith5", startsWith(NULL, "abc") == 0);
    return 0;
}

static char *startsWith6() {
    mu_assert("startsWith6", startsWith("manger", "manger"));
    return 0;
}

static char *startsWith7() {
    mu_assert("startsWith7", startsWith(NULL, NULL) == 0);
    return 0;
}

static char *endsWith1() {
    mu_assert("endsWith1", endsWith("manger", "nger"));
    return 0;
}

static char *endsWith2() {
    mu_assert("endsWith2", endsWith("manger", "abcd") == 0);
    return 0;
}

static char *endsWith3() {
    mu_assert("endsWith3", endsWith("manger", "") == 0);
    return 0;
}

static char *endsWith4() {
    mu_assert("endsWith4", endsWith("manger", NULL) == 0);
    return 0;
}

static char *endsWith5() {
    mu_assert("endsWith5", endsWith(NULL, "nger") == 0);
    return 0;
}

static char *endsWith6() {
    mu_assert("endsWith6", endsWith("manger", "manger"));
    return 0;
}

static char *endsWith7() {
    mu_assert("endsWith6", endsWith(NULL, NULL) == 0);
    return 0;
}

static char *substringIndex1() {
    mu_assert("substringIndex1", getSubstringIndex("substring moi !", "string") == 3);
    return 0;
}

static char *substringIndex2() {
    mu_assert("substringIndex2", getSubstringIndex("substring moi !", "et bha non !") == -1);
    return 0;
}

static char *substringIndex3() {
    mu_assert("substringIndex3", getSubstringIndex("substring moi !", "s") == 0);
    return 0;
}

static char *substringIndex4() {
    mu_assert("substringIndex4", getSubstringIndex("substring moi !", "!") == 14);
    return 0;
}

static char *substringIndex5() {
    mu_assert("substringIndex5", getSubstringIndex("substring moi !", NULL) == -1);
    return 0;
}

static char *substringIndex6() {
    mu_assert("substringIndex6", getSubstringIndex(NULL, "ABC") == -1);
    return 0;
}

static char *substringIndex7() {
    mu_assert("substringIndex7", getSubstringIndex(NULL, NULL) == -1);
    return 0;
}

static char *charIndex1() {
    mu_assert("charIndex1", getCharIndex("abc", 'a') == 0);
    return 0;
}

static char *charIndex2() {
    mu_assert("charIndex2", getCharIndex("abc", 'b') == 1);
    return 0;
}

static char *charIndex3() {
    mu_assert("charIndex3", getCharIndex("abc", 'c') == 2);
    return 0;
}

static char *charIndex4() {
    mu_assert("charIndex4", getCharIndex("abc", 'd') == -1);
    return 0;
}

static char *charIndex5() {
    mu_assert("charIndex5", getCharIndex("abc", NULL) == -1);
    return 0;
}

static char *charIndex6() {
    mu_assert("charIndex6", getCharIndex(NULL, 'd') == -1);
    return 0;
}

static char *charIndex7() {
    mu_assert("charIndex7", getCharIndex(NULL, NULL) == -1);
    return 0;
}

static char *removeChars1() {
    char *str = strdup(" test ");
    int res = removeChars(str, 0, 3);
    mu_assert("removeChars1", strcmp(str, "st ") == 0 && res);
    return 0;
}

static char *removeChars2() {
    char *str = strdup(" test ");
    int res = removeChars(str, 0, strlen(str));
    mu_assert("removeChars2", strcmp(str, "") == 0 && res);
    return 0;
}

static char *removeChars3() {
    mu_assert("removeChars3", removeChars(strdup("abc"), -1, 2) == 0);
    return 0;
}

static char *removeChars4() {
    mu_assert("removeChars4", removeChars(strdup("abc"), 0, -1) == 0);
    return 0;
}

static char *removeChars5() {
    mu_assert("removeChars5", removeChars(strdup("abc"), -1, 4) == 0);
    return 0;
}

static char *removeChars6() {
    mu_assert("removeChars6", removeChars(strdup("abc"), 3, 1) == 0);
    return 0;
}

static char *removeChars7() {
    mu_assert("removeChars7", removeChars(NULL, 0, 2) == 0);
    return 0;
}

static char *allTests() {
    mu_run_test(trim1);
    mu_run_test(trim2);
    mu_run_test(trim3);
    mu_run_test(trim4);
    mu_run_test(trim5);
    mu_run_test(substring1);
    mu_run_test(substring2);
    mu_run_test(substring3);
    mu_run_test(substring4);
    mu_run_test(substring5);
    mu_run_test(substring6);
    mu_run_test(substring7);
    mu_run_test(concat1);
    mu_run_test(concat2);
    mu_run_test(concat3);
    mu_run_test(concat4);
    mu_run_test(toUppercase1);
    mu_run_test(toUppercase2);
    mu_run_test(startsWith1);
    mu_run_test(startsWith2);
    mu_run_test(startsWith3);
    mu_run_test(startsWith4);
    mu_run_test(startsWith5);
    mu_run_test(startsWith6);
    mu_run_test(startsWith7);
    mu_run_test(endsWith1);
    mu_run_test(endsWith2);
    mu_run_test(endsWith3);
    mu_run_test(endsWith4);
    mu_run_test(endsWith5);
    mu_run_test(endsWith6);
    mu_run_test(endsWith7);
    mu_run_test(substringIndex1);
    mu_run_test(substringIndex2);
    mu_run_test(substringIndex3);
    mu_run_test(substringIndex4);
    mu_run_test(substringIndex5);
    mu_run_test(substringIndex6);
    mu_run_test(substringIndex7);
    mu_run_test(charIndex1);
    mu_run_test(charIndex2);
    mu_run_test(charIndex3);
    mu_run_test(charIndex4);
    mu_run_test(charIndex5);
    mu_run_test(charIndex6);
    mu_run_test(charIndex7);
    mu_run_test(removeChars1);
    mu_run_test(removeChars2);
    mu_run_test(removeChars3);
    mu_run_test(removeChars4);
    mu_run_test(removeChars5);
    mu_run_test(removeChars6);
    mu_run_test(removeChars7);
    return 0;
}

int stringTestAll() {
    printf("Testing strings functions ... ");
    char *result = allTests();
    if (result != NULL) {
        printf("Error : %s\n", result);
    } else {
        printf("OK !\n");
    }

    return result == NULL;
}