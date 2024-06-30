/**
 * Map testing program
 * Made for x86
 */

#include "map.h"
#include <stdio.h>
#include <string.h>

void test1() {
    printf("\n========================= TEST 1 =========================\n\n");

    initMap();
    char *path = "FFFRFLFLFBFFFRFLFLFRFRFFFFFFBFLFFLFBFLFLFBFLFLFFLFFLFBFLFLFBFLFBFFFFFFRFFRFLFLFBFLFBFFFLFLF";
    printf("Full path: %s\n", path);
    for (int i = 0; i < 91; i++)
        appendTurn(*path++);
    setMapFinished();

    printf("Result:    %s\n", getMapPath());
    printf("Expected:  FFFRFLFRFRFLFLFRFRFFFFRFFRF\n");
    strcmp(getMapPath(), "FFFRFLFRFRFLFLFRFRFFFFRFFRF") == 0 ? printf("Test passed\n") : printf("Test failed\n");
    deleteMap();

    printf("\n==========================================================\n\n");
}

void test2() {
    printf("\n========================= TEST 2 =========================\n\n");

    initMap();
    char *path = "FLFBFFFLFLFBFLFFLFLFBFLFFLFLFRFLFBFFFLFLFRFLFLFBFLFBFFFRFLFLF";
    printf("Full path: %s\n", path);
    for (int i = 0; i < 61; i++)
        appendTurn(*path++);
    setMapFinished();

    printf("Result:    %s\n", getMapPath());
    printf("Expected:  FRFLFFFFLFFFFLFLFRFRFLFFF\n");
    strcmp(getMapPath(), "FRFLFFFFLFFFFLFLFRFRFLFFF") == 0 ? printf("Test passed\n") : printf("Test failed\n");
    deleteMap();

    printf("\n==========================================================\n\n");
}

void test3() {
    printf("\n========================= TEST 3 =========================\n\n");

    initMap();
    char *path = "FLFLFBFLFLFFLFFLFBFLFLFBFLFBFFFFFFRFFRFLFLFBFLFBFFFLFLFBFLFFLFLFBFLFFLFLFRFLFBFFFLFLFRFLFLFBFLFBFFFRFLFLF";
    printf("Full path: %s\n", path);
    for (int i = 0; i < 105; i++)
        appendTurn(*path++);
    setMapFinished();

    printf("Result:    %s\n", getMapPath());
    printf("Expected:  FRFFLFFFFLFLFRFRFLFFF\n");
    strcmp(getMapPath(), "FRFFLFFFFLFLFRFRFLFFF") == 0 ? printf("Test passed\n") : printf("Test failed\n");
    deleteMap();

    printf("\n==========================================================\n\n");
}

void test4() {
    printf("\n========================= TEST 4 =========================\n\n");

    initMap();
    char *path = "LLFLLLBLLLLBLLBRFRRL";
    printf("Full path: %s\n", path);
    for (int i = 0; i < 20; i++)
        appendTurn(*path++);
    setMapFinished();

    printf("Result:    %s\n", getMapPath());
    printf("Expected:  LLFLLR\n");
    strcmp(getMapPath(), "LLFLLR") == 0 ? printf("Test passed\n") : printf("Test failed\n");
    deleteMap();

    printf("\n==========================================================\n\n");
}

void test5() {
    printf("\n========================= TEST 5 =========================\n\n");

    initMap();
    char *path = "LBLBLLLBLLLLBLRRBLLFLLLBLLLLBLLBRFRRL";
    printf("Full path: %s\n", path);
    for (int i = 0; i < 37; i++)
    {
        // printf("%s + %c\n", getMapPath(), *path);
        appendTurn(*path++);
    }
    setMapFinished();

    printf("Result:    %s\n", getMapPath());
    printf("Expected:  RLFLFLR\n");
    strcmp(getMapPath(), "RLFLFLR") == 0 ? printf("Test passed\n") : printf("Test failed\n");
    deleteMap();

    printf("\n==========================================================\n\n");
}

int main() {
    test1();
    test2();
    test3();
    test4();
    test5();
    return 0;
}
