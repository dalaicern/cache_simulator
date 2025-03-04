/* 23b1num1587 Dalaitseren B*/

/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int m_row, m_col;
    int Rblock, Cblock; 
    int tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;

    if (N == 64) {
        for (Cblock = 0; Cblock < M; Cblock += 8) {
            for (Rblock = 0; Rblock < N; Rblock += 8) {
                for (m_row = Rblock; m_row < Rblock + 4; m_row++) {
                    tmp0 = A[m_row][Cblock];
                    tmp1 = A[m_row][Cblock + 1];
                    tmp2 = A[m_row][Cblock + 2];
                    tmp3 = A[m_row][Cblock + 3];
                    tmp4 = A[m_row][Cblock + 4];
                    tmp5 = A[m_row][Cblock + 5];
                    tmp6 = A[m_row][Cblock + 6];
                    tmp7 = A[m_row][Cblock + 7];
                    B[Cblock][m_row] = tmp0;
                    B[Cblock + 1][m_row] = tmp1;
                    B[Cblock + 2][m_row] = tmp2;
                    B[Cblock + 3][m_row] = tmp3;
                    B[Cblock][m_row + 4] = tmp4;
                    B[Cblock + 1][m_row + 4] = tmp5;
                    B[Cblock + 2][m_row + 4] = tmp6;
                    B[Cblock + 3][m_row + 4] = tmp7;
                }
                for (m_col = Cblock; m_col < Cblock + 4; m_col++) {

                    tmp4 = A[Rblock + 4][m_col];
                    tmp5 = A[Rblock + 5][m_col];
                    tmp6 = A[Rblock + 6][m_col];
                    tmp7 = A[Rblock + 7][m_col];
                    tmp0 = B[m_col][Rblock + 4];
                    tmp1 = B[m_col][Rblock + 5];
                    tmp2 = B[m_col][Rblock + 6];
                    tmp3 = B[m_col][Rblock + 7];

                    B[m_col][Rblock + 4] = tmp4;
                    B[m_col][Rblock + 5] = tmp5;
                    B[m_col][Rblock + 6] = tmp6;
                    B[m_col][Rblock + 7] = tmp7;

                    B[m_col + 4][Rblock] = tmp0;
                    B[m_col + 4][Rblock + 1] = tmp1;
                    B[m_col + 4][Rblock + 2] = tmp2;
                    B[m_col + 4][Rblock + 3] = tmp3;
                    B[m_col + 4][Rblock + 4] = A[Rblock + 4][m_col + 4];
                    B[m_col + 4][Rblock + 5] = A[Rblock + 5][m_col + 4];
                    B[m_col + 4][Rblock + 6] = A[Rblock + 6][m_col + 4];
                    B[m_col + 4][Rblock + 7] = A[Rblock + 7][m_col + 4];
                }
            }
        }
    }else if (N == 32) {
        for (Cblock = 0; Cblock < M; Cblock += 8) {
            for (Rblock = 0; Rblock < N; Rblock += 8) {

                for (m_row = Rblock; m_row < Rblock + 8; m_row++) {
                    for (m_col = Cblock; m_col < Cblock + 8; m_col++) {
                        if (m_row != m_col) {
                            B[m_col][m_row] = A[m_row][m_col];
                        } else {
                            tmp0 = A[m_row][m_col];
                            tmp1 = m_row;
                        }
                    }
                    if (Rblock == Cblock) {
                        B[tmp1][tmp1] = tmp0;
                    }
                }

            }
        }
    } else {
        for (Cblock = 0; Cblock < M; Cblock += 16) {
            for (Rblock = 0; Rblock < N; Rblock += 16) {
                for (m_row = Rblock; (m_row < N) && (m_row < Rblock + 16); m_row++) {
                    for (m_col = Cblock; (m_col < M) && (m_col < Cblock + 16); m_col++) {
                        if (m_row != m_col) {
                            B[m_col][m_row] = A[m_row][m_col];
                        } else {
                            tmp0 = A[m_row][m_col];
                            tmp1 = m_row;
                        }
                    }
                    if (Rblock == Cblock) {
                        B[tmp1][tmp1] = tmp0;
                    }
                }
            }
        }
    }

}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple m_row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int m_row, m_col, tmp;

    for (m_row = 0; m_row < N; m_row++) {
        for (m_col = 0; m_col < M; m_col++) {
            tmp = A[m_row][m_col];
            B[m_col][m_row] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int m_row, m_col;

    for (m_row = 0; m_row < N; m_row++) {
        for (m_col = 0; m_col < M; ++m_col) {
            if (A[m_row][m_col] != B[m_col][m_row]) {
                return 0;
            }
        }
    }
    return 1;
}

