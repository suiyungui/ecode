#include "matrix.h"

void Matrix_Init(matrix_t* martix, int rows, int cols)
{
    ASSERT(rows > 0 && cols > 0);
    martix->rows = rows;
    martix->cols = cols;
    memset(martix->data, 0, MAX_SIZE * MAX_SIZE * sizeof(matrix_type));
}



void Matrix_Identity(matrix_t* matrix, int size)
{
    ASSERT(size > 0);
    matrix->rows = size;
    matrix->cols = size;
    memset(matrix->data, 0, sizeof(matrix->data));
    for(int i = 0; i < size; i++)
    {
        matrix->data[i][i] = 1.0f;
    }
}



void Matrix_From_Array(matrix_t* mat, const matrix_type* array,const int rows,const int cols)
{
    ASSERT(NULL != array);
    Matrix_Init(mat, rows, cols);
    for(int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            mat->data[i][j] = array[i * cols + j];
        }
    }
}




matrix_t Matrix_Transpose(const matrix_t* src)
{
    // ����Ŀ�����Ĵ�С
    matrix_t dest;
    Matrix_Init(&dest, src->cols, src->rows);
    // ����ת�ò���
    for(int i = 0; i < src->rows; i++)
    {
        for(int j = 0; j < src->cols; j++)
        {
            dest.data[j][i] = src->data[i][j];
        }
    }
    return dest;
}




// ����˷�
matrix_t multiply_matrices(const matrix_t* A, const matrix_t* B)
{
    ASSERT(A->cols == B->rows);

    // ��ʼ���������
    matrix_t dest;
    Matrix_Init(&dest, A->rows, B->cols);

    // ���о���˷�����
    for(int i = 0; i < A->rows; i++)
    {
        for(int j = 0; j < B->cols; j++)
        {
            for(int k = 0; k < A->cols; k++)
            {
                dest.data[i][j] += A->data[i][k] * B->data[k][j];
            }
        }
    }

    return dest;  // �ɹ�
}



// ����ӷ�
matrix_t add_matrices(const matrix_t* A, const matrix_t* B)
{
    // ʹ�� assert ��ȷ�����������ά����ͬ
    ASSERT(A->rows == B->rows && A->cols == B->cols);

    matrix_t result;
    Matrix_Init(&result, A->rows, A->cols);

    // ���о���ӷ�����
    for(int i = 0; i < A->rows; i++)
    {
        for(int j = 0; j < A->cols; j++)
        {
            result.data[i][j] = A->data[i][j] + B->data[i][j];
        }
    }

    return result;  // ���ؼӷ��������
}




// �������
matrix_t subtract_matrices(const matrix_t* A, const matrix_t* B)
{
    // ʹ�� assert ��ȷ�����������ά����ͬ
    ASSERT(A->rows == B->rows && A->cols == B->cols);

    // ��ʼ���������
    matrix_t result;
    Matrix_Init(&result, A->rows, A->cols);

    // ���о����������
    for(int i = 0; i < A->rows; i++)
    {
        for(int j = 0; j < A->cols; j++)
        {
            result.data[i][j] = A->data[i][j] - B->data[i][j];
        }
    }

    return result;  // ���ؼ����������
}




// ��˹��Ԫ����������
int inverse_matrix(matrix_t* A, matrix_t* invA)
{
    ASSERT(A->rows == A->cols);  // ��֧�ַ���

    const matrix_type THRESHOLD = 1e-6;  // ���ݾ������������ֵ

    Matrix_Init(invA, A->rows, A->cols);

    int n = A->rows;

    matrix_type augmented[MAX_SIZE][2 * MAX_SIZE];

    // ����������� [A | I]
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            augmented[i][j] = A->data[i][j];
            augmented[i][j + n] = (float)((i == j) ? 1 : 0);  // ���õ�λ���󲿷�
        }
    }

    // ��˹��Ԫ����
    for(int i = 0; i < n; i++)
    {
        // �ҵ��� i �е���Ԫ��
        int max_row = i;
        for(int j = i + 1; j < n; j++)
        {
            if(fabs(augmented[j][i]) > fabs(augmented[max_row][i]))
            {
                max_row = j;
            }
        }

        // �����Ԫ��Ϊ 0��˵�����󲻿���
        if(fabs(augmented[max_row][i]) < THRESHOLD)
        {
            return 1;  // ������
        }

        // ������ǰ�к������
        if(max_row != i)
        {
            for(int j = 0; j < 2 * n; j++)
            {
                matrix_type temp = augmented[i][j];
                augmented[i][j] = augmented[max_row][j];
                augmented[max_row][j] = temp;
            }
        }

        // �Ե�ǰ�н��й�һ����ʹ����Ԫ��Ϊ 1
        matrix_type pivot = augmented[i][i];
        for(int j = 0; j < 2 * n; j++)
        {
            augmented[i][j] /= pivot;
        }

        // ��ȥ��ǰ�������е�Ԫ��
        for(int j = 0; j < n; j++)
        {
            if(j != i)
            {
                matrix_type factor = augmented[j][i];
                for(int k = 0; k < 2 * n; k++)
                {
                    augmented[j][k] -= factor * augmented[i][k];
                }
            }
        }
    }

    // ��ȡ����󲿷� [I | A^-1]
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            invA->data[i][j] = augmented[i][j + n];
        }
    }

    return 0;  // ���棬���� 0
}





static inline float invSqrt(float x)
{
    float xhalf = 0.5f * x;

    int i = *(int*)&x;

    i = 0x5f375a86 - (i >> 1);

    x = *(float*)&i;

    x = x * (1.5f - xhalf * x * x);

    return x;
}




void normalize_vector(matrix_t *v)
{
    ASSERT(1 == v->cols || 1 == v->rows);

    matrix_type norm = 0;

    if(1 == v->rows)
    {
        for(int i = 0; i < v->cols; ++i)
        {
            norm += (v->data[0][i] * v->data[0][i]);
        }
    }
    if(1 == v->cols)
    {
        for(int i = 0; i < v->rows; ++i)
        {
            norm += (v->data[i][0] * v->data[i][0]);
        }
    }

    norm = invSqrt((float)norm);
    if(1 == v->rows)
    {
        for(int i = 0; i < v->cols; ++i)
        {
            v->data[0][i] *= norm;
        }
    }
    if(1 == v->cols)
    {
        for(int i = 0; i < v->rows; ++i)
        {
            v->data[i][0] *= norm;
        }
    }
}





void print_matrix(const matrix_t* matrix)
{
    for(int i = 0; i < matrix->rows; i++)
    {
        for(int j = 0; j < matrix->cols; j++)
        {
            printf("%2f ", matrix->data[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}



