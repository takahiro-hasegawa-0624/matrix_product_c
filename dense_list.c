//密行列形式で計算
//3重ループ

#include<stdio.h>
#include<stdlib.h>

#define NEW(p,n) {p = malloc((n)*sizeof(p[0]));if ((p)==NULL) {printf("not enough memory\n"); exit(1);};}

typedef double* row_vector;//実数の配列をrow_vectorとする
typedef row_vector* matrix;//row_vectorの配列をmatrixとする

row_vector create_row_vector(int n)
{
  int i;
  row_vector V;
  NEW(V, n); // 構造体の要素のメモリを確保
  for (i=0; i<n; i++)
  {
      V[i] = 0.0; //各要素を0にする
  }
  return V;
}

matrix create_matrix(int n, int m) //n行m列の行列を作成
{
  int i;
  matrix M;
  NEW(M, n);
  for (i=0; i<n; i++)
  {
    M[i] = create_row_vector(m);
  } 
  return M;
}

matrix read_matrix(int n, int m, matrix M)
{
  int i,j;
  double k;

  for (i=0; i<n; i++)
  {
    for (j=0; j<m; j++)
    {
        scanf("%lf", &k);
        M[i][j] = k;
    }
  }
  return M;
}

void print_matrix(matrix M, int n, int m)
{
    int i,j;
    printf("%d %d\n", n, m);

    for (i=0; i<n; i++) {
        for (j=0; j<m; j++) {
            if (M[i][j] != 0)
            {
                printf("%d ", j+1);
                printf("%lf ", M[i][j]);
            }
            if (j == m-1)
            {
                printf("-1\n");
            }
        }
      printf("\n");
   }
   return;
}

matrix transpose_matrix(int n, int m, matrix M) //n行m列の行列Mの転置行列を作成
{
    int i,j;
    matrix M_;
    
    M_ = create_matrix(m, n);

    for (i=0 ; i<n ; i++)
    {
        for (j=0 ; j<m ; j++)
        {
            M_[i][j] = M[j][i];
        }
    }

    return M_;
}

matrix matrix_product(matrix A, matrix B, int An, int Am, int Bm) //An行Am列の行列AとBn行Bm列の行列Bの積
{
    int i,j,k;
    matrix C;
    
    C = create_matrix(An, Bm);

    for (i=0 ; i<An ; i++){
        for (j=0 ; j<Bm ; j++){
            for (k=0; k<Am; k++){
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return C;
}

matrix matrix_product_jk_exchange(matrix A, matrix B, int An, int Am, int Bm) //jkループ交換法
{
    int i,j,k;
    matrix C;
    
    C = create_matrix(An, Bm);

    for (i=0 ; i<An ; i++){
        for (k=0; k<Am; j++){
            for (j=0 ; j<Bm ; k++){
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return C;
}

matrix transposed_matrix_product(matrix A, matrix B, int An, int Am, int Bn) //転置行列との行方向の内積による行列積
{
    int i,j,k;
    matrix C;
    
    C = create_matrix(An, Bn);

    for (i=0 ; i<An ; i++){
        for (j=0; k<Am; j++){
            for (k=0 ; j<Bn ; k++){
                C[i][j] += A[i][k] * B[j][k];
            }
        }
    }

    return C;
}

void free_matrix(matrix A, int n) //n行の行列Aのメモリ解放
{
    int i;

    for (i=0; i<n; i++){
        free(A[i]);
    }

    free(A);
    return;
}

int main(void)
{
  int n,m;
  matrix M, M_2;

  scanf("%d %d", &n, &m);
  M = create_matrix(n, m);

  read_matrix(n,m,M);

  M_2 = matrix_product(M, M, n, m, m);

  print_matrix(M_2, n, m);

  free_matrix(M, n);
  free_matrix(M_2, n);

  return 0;
}