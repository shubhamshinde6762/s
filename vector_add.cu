#include <iostream>
#include <vector>
using namespace std;

void addVectors(const float* a, const float* b, float* c, int n) {
    for (int i = 0; i < n; ++i) {
        c[i] = a[i] + b[i];
    }
}

__global__ void addVectorGPU(const float* a, const float* b, float* c, int n)
{
    int i = blockDim.x * blockIdx.x + threadIdx.x;

    if (i < n)
    {
        c[i] = a[i] + b[i];
    }
}

int main() {
    int n = 1 << 20;

    size_t size = n * sizeof(float);

    
}