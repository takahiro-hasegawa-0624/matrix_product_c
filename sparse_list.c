//疎行列形式で計算
#include <stdio.h>
#include <stdlib.h>

#define NEW(p,n) {p = malloc((n)*sizeof(p[0])); if ((p)==NULL) {printf("not enough memory\n"); exit(1);};}

typedef struct slobj_ {
  struct slobj_ *next;        // 後の要素へのポインタ
  int j;                               // j 列目
  double v;                       // (i, j) 要素
}* slobj;

typedef struct {
  slobj head;// 先頭要素のポインタ
  slobj tail;// 末尾要素のポインタ
}* slist;

typedef struct {
    int n, m; // 行数，列数
    slist* A; // 行ベクトルを表すリストの配列
}* smatrix;

slobj slobj_new(int x, double y)
{
  slobj p;
  NEW(p,1); // 構造体の要素のメモリを確保する
  p->j = x; // j列目
  p->v = y; //要素
  p->next = NULL; // 必ず初期化しておく

  return p;
}

slist slist_new(void)
{
  slist L;
  NEW(L,1); // 構造体の要素のメモリを確保する
  L->head = NULL; 
  L->tail = NULL; 

  return L;
}

void slist_insert_tail(slist L, slobj p) // リストLの末尾に要素pを追加する
{
  if (L->head == NULL){
    L->head = p;
    L->tail = p;
  } else{
    L->tail->next = p;
    L->tail = p;
  }

  return;
}

smatrix smatrix_new(int x, int y) // x行y列の行列を作成する
{
    smatrix S;
    int i;
    NEW(S,1); // 構造体の要素のメモリを確保する
    NEW(S->A,x); // 構造体の要素のメモリを確保する。行数xのリストのメモリ。

    S->n = x; // 行数
    S->m = y; // 列数

    for (i = 0; i < x; i++)
    {
        S->A[i] = slist_new(); //列方向のリスト
    }
    
    return S;
}

smatrix smatrix_read(void)
{
    int n,m,x,i;
    double y;
    slobj p;
    slist L;
    smatrix S;

    scanf("%d", &n); // 行数
    scanf("%d", &m); // 列数

    S = smatrix_new(n, m);

    for (i = 0; i < n; i++){

        while (1){
            scanf("%d", &x);
            if (x == -1)
            {
                break;
            }
            
            scanf("%lf", &y);

            slist_insert_tail(S->A[i], slobj_new(x, y));
        }
    }

    return S;
}

void smatrix_print(smatrix S)
{
    int i;
    slobj p;

    printf("%d ", S->n);
    printf("%d \n", S->m);

    for (i = 0; i < S->n; i++)
    {
        p = S->A[i]->head;
        while (p != NULL)
        {
            printf("%d ", p->j);
            printf("%f ", p->v);
            p = p->next;
        }
        printf("-1 \n");
    }

    return;
}

smatrix smatrix_transpose(smatrix S) // 行列の転置をとる
{
    int i,j;
    slobj p;
    smatrix T;

    T = smatrix_new(S->m, S->n); // 転置行列の格納先を用意

    for (i = 0; i < S->n; i++){ //Sのi行目
        p = S->A[i]->head;
        while (p != NULL)
        {
            slist_insert_tail(T->A[p->j - 1], slobj_new(i+1, p->v)); //転置をとる
            p = p->next;
        }
    }

    return T;
}

double common_element_inner_product(slist L1, slist L2) // リストL1、L2の内積
{
  slobj p1,p2;
  double sum;

  sum = 0;

  p1 = L1->head;
  p2 = L2->head;
  while ((p1 != NULL) && (p2 != NULL)) {
      if (p1->j == p2->j) { // 列が等しい場合要素の積をとる
          sum = sum + p1->v * p2->v;
          p1 = p1->next;
          p2 = p2->next;
      } else{ // 列が異なる場合、小さい方の要素の次の要素をとり、ループ
        if(p1->j > p2->j){
            p2 = p2->next;
        } else{
            p1 = p1->next;
        }
      }
  }

  return sum;
}

smatrix smatrix_product(smatrix S1, smatrix S2) // 行列積
{
    int i,j;
    double temp;
    smatrix S2_transpose, T;

    if (S1->m == S2->n) //行列積が取れる場合
    {
        S2_transpose = smatrix_transpose(S2); //転置行列
        T = smatrix_new(S1->n, S2->m); // 積をとった結果の行列を用意

        for (i = 0; i < S1->n; i++){ //S1のi行目
            for (j = 0; j < S2->m; j++){ //S2のj列目
                temp = 0;
                temp = common_element_inner_product(S1->A[i], S2_transpose->A[j]); //i行j列目の内積をとる
                if (temp != 0)
                {
                    slist_insert_tail(T->A[i], slobj_new(j+1, temp)); //内積結果を格納
                }
            }
        }
    } else{
        T = smatrix_new(0, 0); //行列積がとれない場合
    }

    return T;
}


int main(int argc, char *argv[])
{
    smatrix A,B;

    A = smatrix_read();

    B = smatrix_product(A,A);

    smatrix_print(A);

    return 0;
}