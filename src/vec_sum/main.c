#define CL_SILENCE_DEPRECATION

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SOURCE_SIZE (0x100000)
#define SIZE 1024


void fill_data(float *A, float *B) {
	// Initialize values for array members.
	int i;
	for (i = 0; i < SIZE; i++) {
		A[i] = i * 1.234f;
		B[i] = i * 2.345f;
	}
}

void check_correctness(float *A, float *B, float *C) {
	// Test if correct answer
	int i;
	for (i = 0; i < SIZE; i++) {
		if (C[i] != (A[i] + B[i])) {
			printf("Something didn't work correctly! Failed test, ");
			break;
		}
	}
	if (i == SIZE) {
		printf("Everything seems to work fine for ");
	}
}

void calculate_on_cpu(float *A, float *B, float *C) {
	for (int i = 0; i < SIZE; i++) {
		C[i] = A[i] + B[i];
	}
}

void calculate_on_gpu(float *A, float *B, float *C) {
	// Load kernel from file 
	FILE *kernelFile = fopen("src/kernels/vec_sum.cl", "r");;
	char *kernelSource;
	size_t kernelSize;

	if (!kernelFile) {
		fprintf(stderr, "No file was found!\n");
		exit(-1);
	}

	kernelSource = (char*)malloc(MAX_SOURCE_SIZE);
	kernelSize = fread(kernelSource, 1, MAX_SOURCE_SIZE, kernelFile);
	fclose(kernelFile);

	// Getting platform and device information
	cl_platform_id platformId = NULL;
	cl_device_id deviceID = NULL;
	cl_uint errNumDevices;
	cl_uint errNumPlatforms;
	cl_int err = clGetPlatformIDs(1, &platformId, &errNumPlatforms);
	err = clGetDeviceIDs(platformId, CL_DEVICE_TYPE_DEFAULT, 1, &deviceID, &errNumDevices);

	// Creating context.
	cl_context context = clCreateContext(NULL, 1, &deviceID, NULL, NULL,  &err);

	// Creating command queue
	cl_command_queue commandQueue = clCreateCommandQueue(context, deviceID, 0, &err);

	// Memory buffers for each array
	cl_mem aMemObj = clCreateBuffer(context, CL_MEM_READ_ONLY, SIZE * sizeof(float), NULL, &err);
	cl_mem bMemObj = clCreateBuffer(context, CL_MEM_READ_ONLY, SIZE * sizeof(float), NULL, &err);
	cl_mem cMemObj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, SIZE * sizeof(float), NULL, &err);

	// Copy lists to memory buffers
	err = clEnqueueWriteBuffer(commandQueue, aMemObj, CL_TRUE, 0, SIZE * sizeof(float), A, 0, NULL, NULL);;
	err = clEnqueueWriteBuffer(commandQueue, bMemObj, CL_TRUE, 0, SIZE * sizeof(float), B, 0, NULL, NULL);

	// Create program from kernel source
	cl_program program = clCreateProgramWithSource(context, 1, (const char **)&kernelSource, (const size_t *)&kernelSize, &err);	

	// Build program
	err = clBuildProgram(program, 1, &deviceID, NULL, NULL, NULL);

	// Create kernel
	cl_kernel kernel = clCreateKernel(program, "addVec", &err);

	// Set arguments for kernel
	err = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&aMemObj);	
	err = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&bMemObj);	
	err = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&cMemObj);	

	// Execute the kernel
	size_t globalItemSize = SIZE;
	size_t localItemSize = 64; // globalItemSize has to be a multiple of localItemSize. 1024/64 = 16 
	err = clEnqueueNDRangeKernel(commandQueue, kernel, 1, NULL, &globalItemSize, &localItemSize, 0, NULL, NULL);	

	// Read from device back to host.
	err = clEnqueueReadBuffer(commandQueue, cMemObj, CL_TRUE, 0, SIZE * sizeof(float), C, 0, NULL, NULL);

	// Clean up, release memory.
	err = clFlush(commandQueue);
	err = clFinish(commandQueue);
	err = clReleaseCommandQueue(commandQueue);
	err = clReleaseKernel(kernel);
	err = clReleaseProgram(program);
	err = clReleaseMemObject(aMemObj);
	err = clReleaseMemObject(bMemObj);
	err = clReleaseMemObject(cMemObj);
	err = clReleaseContext(context);
}


int main(int argc, char ** argv) {

	clock_t start, end;

	// Allocate memories for input arrays and output array
	float A[SIZE]; float B[SIZE]; float C[SIZE];
	
	fill_data(A, B);
	start = clock();
	calculate_on_cpu(A, B, C);
	end = clock();
	check_correctness(A, B, C);
	printf("CPU, total time: %f sec\n", ((double) (end - start)) / CLOCKS_PER_SEC);

	fill_data(A, B);
	start = clock();
	calculate_on_gpu(A, B, C);
	end = clock();
	check_correctness(A, B, C);
	printf("GPU, total time: %f sec\n", ((double) (end - start)) / CLOCKS_PER_SEC);

	return 0;
}