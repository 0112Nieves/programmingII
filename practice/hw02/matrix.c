#include "matrix.h"
#include <math.h>
sMatrix * matrix_init( uint8_t m, uint8_t n){
    sMatrix* matrix = (sMatrix*)malloc(sizeof(sMatrix));
    matrix->rows = m;
    matrix->cols = n;
    matrix->data = (int32_t**)malloc(matrix->rows * sizeof(int32_t *));
    for (uint8_t i = 0; i < matrix->rows; i++) {
        matrix->data[i] = (int32_t*)malloc(matrix->cols * sizeof(int32_t));
    }
    return matrix;
}
int32_t matrix_set( sMatrix *pM, uint8_t m, uint8_t n, int32_t value ){
    if(pM == NULL) return -1;
    if(m >= pM->rows || n >= pM->cols) return -1; 
    pM->data[m][n] = value;
    return 0;
}
void matrix_print( const sMatrix *pM ){
    for(int i=0; i<pM->rows; i++){
        for(int j=0; j<pM->cols; j++){
            printf("%d ", pM->data[i][j]);
        }
      printf("\n");      
    }
}
int32_t matrix_add( sMatrix *pA, const sMatrix *pB, const sMatrix *pC ){
    if(pA == NULL || pB == NULL || pC == NULL) return -1;
    if(pB->rows != pC->rows || pB->cols != pC->cols) return -1;
    for(int32_t i = 0; i < pA->rows; i++){
        for(int32_t j = 0; j < pA->cols; j++){
            pA->data[i][j] = pB->data[i][j]+pC->data[i][j];
        }
    }
    return 0;
}
int32_t matrix_multiply( sMatrix *pA, const sMatrix *pB, const sMatrix *pC ){
    if(pA == NULL || pB == NULL || pC == NULL) return -1;
    if(pB->cols != pC->rows) return -1;
    for(int i = 0; i < pA->cols; i++){
        for(int j = 0; j < pA->rows; j++){
            pA->data[i][j] = pB->data[i][j]*pC->data[j][i];
        }
    }
    return 0;
}
int32_t matrix_transpose( sMatrix *pA){
    if(pA == NULL) return -1;
    int originRow = pA->rows;
    int originColumn = pA->cols;
    pA->rows = originColumn;
    pA->cols = originRow;
    int32_t **transpose = (int32_t **) malloc(pA->rows * sizeof(int32_t *));
    for(int i = 0; i < pA->rows; i++){
        transpose[i] = malloc(pA->cols * sizeof(int32_t));
    }
    if(transpose == NULL || *transpose == NULL) return -1;
    for(int i = 0; i < originRow; i++){
        for(int j = 0; j < originColumn; j++){
            transpose[j][i] = pA->data[i][j];
        }
    }
    for(int i = 0; i < originRow; i++) free(pA->data[i]);
    free(pA->data);
    pA->data = transpose;
    return 0;
}
int32_t matrix_det(const sMatrix *pA, int32_t *pAns){
    if(pA == NULL || pAns == NULL) return -1;
    if(pA->rows != pA->cols) return -1;
    int n = pA->rows;
    if(pA->rows == 2){
        *pAns = pA->data[0][0]*pA->data[1][1]-pA->data[0][1]*pA->data[1][0];
        return 0;
    }
    int det = 0, sign = 1;
    sMatrix submatrix;
    for(int i = 0; i < n; i++){
        submatrix.rows = n-1;
        submatrix.cols = n-1;
        submatrix.data = (int32_t **)malloc(submatrix.rows*sizeof(int32_t*));
        for(int j = 0; j < submatrix.rows; j++){
            submatrix.data[j] = (int32_t *)malloc(submatrix.cols*sizeof(int32_t));
        }
        for(int j = 1; j < n; j++){
            int subCol = 0;
            for(int k = 0; k < n; k++){
                if(k == i) continue;
                submatrix.data[j-1][subCol] = pA->data[j][k];
                subCol++;
            }
        }
        int subDet;
        int ret = matrix_det(&submatrix, &subDet);
        for(int j = 0; j < submatrix.rows; j++){
            free(submatrix.data[j]);
        }
        free(submatrix.data);
        if(ret == -1) return -1;
        det += sign * pA->data[0][i] * subDet;
        sign *= -1;
    }
    *pAns = det;
    return 0;
}
int32_t matrix_free( sMatrix *pA){
    if(pA == NULL) return -1;
    for(int i = 0; i < pA->rows; i++){
        free(pA->data[i]);
    }
    free(pA->data);
    free(pA);
    return 0;
}
