// hw0201.c
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>
#include <setjmp.h>
#include "matrix.h"
 
#define ERRORF(fmt, ...) \
    do { \
        fprintf(stderr, fmt, ##__VA_ARGS__); \
    } while (0)
 
 
sigjmp_buf env;
 
void recvSig(int sig) {
    siglongjmp(env, 1);
}
 
int32_t matrixSetArray(sMatrix *pM, uint8_t m, uint8_t n, void *values) {
    int32_t arr[m][n];
    memcpy(arr, values, sizeof(arr));
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix_set(pM, i, j, arr[i][j]) != 0) {
                return -1;
            }
        }
    }
    return 0;
}
 
int32_t arrayDet(uint8_t n, void *values) {
    int32_t arr[n][n];
    memcpy(arr, values, sizeof(arr));
 
    if(n == 1) {
        return arr[0][0];
    }
 
    int32_t det = 0, sign = 1;
    int32_t subArr[n-1][n-1];
    for(int32_t k = 0; k < n; k++) {
        int32_t subI = 0;
        for(int32_t i = 1; i < n; i++) {
            int32_t subJ = 0;
            for(int32_t j = 0; j < n; j++) {
                if(j == k) {
                    continue;
                }
                subArr[subI][subJ] = arr[i][j];
                subJ++;
            }
            subI++;
        }
        det += sign * arr[0][k] * arrayDet(n-1, subArr);
        sign = -sign;
    }
    return det;
}
 
void arrayPrint(uint8_t m, uint8_t n, void *values) {
    int32_t arr[m][n];
    memcpy(arr, values, sizeof(arr));
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d", arr[i][j]);
            if (j != n - 1) {
                printf(" ");
            }
        }
        printf("\n");
    }
}
 
int32_t testInit(uint8_t m, uint8_t n) {
    sMatrix *pM = matrix_init(m, n);
    if(pM == NULL) {
        ERRORF("matrix_init() failed\n");
        return -1;
    }
    if(matrix_free(pM) != 0) {
        ERRORF("matrix_free() failed\n");
        return -1;
    }
    return 0;
}
 
int32_t testDet(uint8_t n, void *values) {
    printf("M:\n");
    arrayPrint(n, n, values);
    sMatrix *pM = matrix_init(n, n);
    if(pM == NULL) {
        ERRORF("matrix_init() failed\n");
        return -1;
    }
    if(matrixSetArray(pM, n, n, values) != 0) {
        ERRORF("matrixSetArray() failed\n");
        return -1;
    }
    int32_t det;
    if(matrix_det(pM, &det) != 0) {
        ERRORF("matrix_det() failed\n");
        return -1;
    }
    printf("det(M) = %d\n", det);
    int32_t arrDet = arrayDet(n, values);
    if(det != arrDet) {
        ERRORF("matrix_det() returned %d, but arrayDet() returned %d\n", det, arrDet);
        return -1;
    }
    if(matrix_free(pM) != 0) {
        ERRORF("matrix_free() failed\n");
        return -1;
    }
    return 0;
}
 
int32_t testPrint(uint8_t m, uint8_t n, void *values) {
    printf("M:\n");
    arrayPrint(n, n, values);
    sMatrix *pM = matrix_init(m, n);
    if(pM == NULL) {
        ERRORF("matrix_init() failed\n");
        return -1;
    }
    if(matrixSetArray(pM, m, n, values) != 0) {
        ERRORF("matrixSetArray() failed\n");
        return -1;
    }
    printf("Print M:\n");
    matrix_print(pM);
    if(matrix_free(pM) != 0) {
        ERRORF("matrix_free() failed\n");
        return -1;
    }
    return 0;
}
 
int32_t testTranspose(uint8_t m, uint8_t n, void *values) {
    printf("M:\n");
    arrayPrint(m, n, values);
    sMatrix *pM = matrix_init(m, n);
    if(pM == NULL) {
        ERRORF("matrix_init() failed\n");
        return -1;
    }
    if(matrixSetArray(pM, m, n, values) != 0) {
        ERRORF("matrixSetArray() failed\n");
        return -1;
    }
    if(matrix_transpose(pM) != 0) {
        ERRORF("matrix_transpose() failed\n");
        return -1;
    }
    printf("M^T:\n");
    matrix_print(pM);
    if(matrix_free(pM) != 0) {
        ERRORF("matrix_free() failed\n");
        return -1;
    }
    return 0;
}
 
int32_t testAdd(uint8_t m, uint8_t n, void *valuesA, void *valuesB) {
    printf("A:\n");
    arrayPrint(m, n, valuesA);
    sMatrix *pA = matrix_init(m, n);
    if(pA == NULL) {
        ERRORF("matrix_init() failed\n");
        return -1;
    }
    if(matrixSetArray(pA, m, n, valuesA) != 0) {
        ERRORF("matrixSetArray() failed\n");
        return -1;
    }
    printf("B:\n");
    arrayPrint(m, n, valuesB);
    sMatrix *pB = matrix_init(m, n);
    if(pB == NULL) {
        ERRORF("matrix_init() failed\n");
        return -1;
    }
    if(matrixSetArray(pB, m, n, valuesB) != 0) {
        ERRORF("matrixSetArray() failed\n");
        return -1;
    }
    sMatrix *pC = matrix_init(m, n);
    if(pC == NULL) {
        ERRORF("matrix_init() failed\n");
        return -1;
    }
    if(matrix_add(pC, pA, pB) != 0) {
        ERRORF("matrix_add() failed\n");
        return -1;
    }
    printf("C = A + B:\n");
    matrix_print(pC);
    if(matrix_free(pA) != 0) {
        ERRORF("matrix_free() failed\n");
        return -1;
    }
    if(matrix_free(pB) != 0) {
        ERRORF("matrix_free() failed\n");
        return -1;
    }
    if(matrix_free(pC) != 0) {
        ERRORF("matrix_free() failed\n");
        return -1;
    }
    return 0;
}
 
int32_t testMul(uint8_t m, uint8_t n, uint8_t p, void *valuesA, void *valuesB) {
    printf("A:\n");
    arrayPrint(m, n, valuesA);
    sMatrix *pA = matrix_init(m, n);
    if(pA == NULL) {
        ERRORF("matrix_init() failed\n");
        return -1;
    }
    if(matrixSetArray(pA, m, n, valuesA) != 0) {
        ERRORF("matrixSetArray() failed\n");
        return -1;
    }
    printf("B:\n");
    arrayPrint(n, p, valuesB);
    sMatrix *pB = matrix_init(n, p);
    if(pB == NULL) {
        ERRORF("matrix_init() failed\n");
        return -1;
    }
    if(matrixSetArray(pB, n, p, valuesB) != 0) {
        ERRORF("matrixSetArray() failed\n");
        return -1;
    }
    sMatrix *pC = matrix_init(m, p);
    if(pC == NULL) {
        ERRORF("matrix_init() failed\n");
        return -1;
    }
    if(matrix_multiply(pC, pA, pB) != 0) {
        ERRORF("matrix_mul() failed\n");
        return -1;
    }
    printf("C = A * B:\n");
    matrix_print(pC);
    if(matrix_free(pA) != 0) {
        ERRORF("matrix_free() failed\n");
        return -1;
    }
    if(matrix_free(pB) != 0) {
        ERRORF("matrix_free() failed\n");
        return -1;
    }
    if(matrix_free(pC) != 0) {
        ERRORF("matrix_free() failed\n");
        return -1;
    }
    return 0;
}
 
int32_t valuesA[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
int32_t valuesB[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
int32_t valuesC[3][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
int32_t valuesD[2][3] = {{1, 2, 3}, {4, 5, 6}};
int32_t ValuesE[3][2] = {{1, 2}, {3, 4}, {5, 6}};
int32_t valuesF[2][2] = {{22, 28}, {49, 64}};
 
int32_t testcase1() {
    return testInit(3, 3);
}
 
int32_t testcase2() {
    return testPrint(3, 3, valuesA);
}
 
int32_t testcase3() {
    return testDet(3, valuesA);
}
 
int32_t testcase4() {
    return testDet(2, valuesF);
}
 
int32_t testcase5() {
    return testTranspose(3, 3, valuesA);
}
 
int32_t testcase6() {
    return testTranspose(2, 3, valuesD);
}
 
int32_t testcase7() {
    return testAdd(3, 3, valuesA, valuesB);
}
 
int32_t testcase8() {
    return testAdd(3, 3, valuesA, valuesC);
}
 
int32_t testcase9() {
    return testMul(3, 3, 3, valuesA, valuesB);
}
 
int32_t testcase10() {
    return testMul(2, 3, 2, valuesD, ValuesE);
}
 
int32_t (*testcases[])() = {
    testcase1,
    testcase2,
    testcase3,
    testcase4,
    testcase5,
    testcase6,
    testcase7,
    testcase8,
    testcase9,
    testcase10,
};
 
int32_t casePoints[] = {
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
};
 
const int32_t cases = sizeof(testcases) / sizeof(testcases[0]);
 
int main() {
    int32_t pass[cases];
    memset(pass, 0, sizeof(pass));
    int32_t points = 0;
    for(int32_t i = 0; i < cases; i++) {
        memset(&env, 0, sizeof(env));
        int r = sigsetjmp(env, 1);
        switch(r) {
        case 0: {
            signal(SIGSEGV, recvSig);
            printf("Testcase %d:\n", i + 1);
            int32_t ret = testcases[i]();
            if(ret == 0) {
                points += casePoints[i];
                pass[i] = 1;
            }
            break;
        }
        default:
            fprintf(stderr, "Segmentation fault when running testcase %d\n", i + 1);
            break;
        }
        printf("\n※※※※※※※※※※\n");
    }
    printf("Pass: ");
    for(int32_t i = 0; i < cases; i++) {
        if(pass[i]) {
            printf("%d ", i+1);
        }
    }
    printf("\n");
    printf("Fail: ");
    for(int32_t i = 0; i < cases; i++) {
        if(!pass[i]) {
            printf("%d ", i+1);
        }
    }
    printf("\n");
    printf("Points: %d\n", points);
    return 0;
}
