#include <stdio.h>
#include <cuda.h>

__global__ void spmv_csr(int n, int *row_ptr, int *col_idx, float *data, float *x, float *y)
{
    int row = blockIdx.x * blockDim.x + threadIdx.x;
    if (row < n)
    {
        float sum = 0.0f;
        for (int j = row_ptr[row]; j < row_ptr[row + 1]; j++)
        {
            sum += data[j] * x[col_idx[j]];
        }
        y[row] = sum;
    }
}

int main()
{
    int n = 3;
    int nnz = 5;

    float h_data[] = {1, 2, 3, 4, 5};
    int h_col_idx[] = {0, 2, 1, 0, 2};
    int h_row_ptr[] = {0, 2, 3, 5};
    float h_x[] = {1, 2, 3};
    float h_y[3];

    float *d_data, *d_x, *d_y;
    int *d_col_idx, *d_row_ptr;

    cudaMalloc((void **)&d_data, nnz * sizeof(float));
    cudaMalloc((void **)&d_col_idx, nnz * sizeof(int));
    cudaMalloc((void **)&d_row_ptr, (n + 1) * sizeof(int));
    cudaMalloc((void **)&d_x, n * sizeof(float));
    cudaMalloc((void **)&d_y, n * sizeof(float));

    cudaMemcpy(d_data, h_data, nnz * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemcpy(d_col_idx, h_col_idx, nnz * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_row_ptr, h_row_ptr, (n + 1) * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_x, h_x, n * sizeof(float), cudaMemcpyHostToDevice);

    spmv_csr<<<1, n>>>(n, d_row_ptr, d_col_idx, d_data, d_x, d_y);

    cudaMemcpy(h_y, d_y, n * sizeof(float), cudaMemcpyDeviceToHost);

    for (int i = 0; i < n; i++)
        printf("%f ", h_y[i]);

    cudaFree(d_data);
    cudaFree(d_col_idx);
    cudaFree(d_row_ptr);
    cudaFree(d_x);
    cudaFree(d_y);

    return 0;
}