#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include "ip.h"

#pragma region extern function
extern image_ptr creat_pnm(int rows, int cols, int type);
extern image_ptr read_pnm(char *filename, int *rows, int *cols, int *type);
extern void write_pnm(image_ptr ptr, char *filename, int rows, int cols, int magic_number);

extern void apply_LUT(image_ptr buffer, unsigned long number_of_pixels, int *operation(int));
extern void change_LUT(image_ptr buffer, unsigned long number_of_pixels, int *operation(int));
extern int solarizing(int value);

extern void histogram_equalize(image_ptr buffer, unsigned long number_of_pixels);
extern void change_histogram(image_ptr buffer, unsigned long number_of_pixels);
extern void change_CDF(image_ptr buffer, unsigned long number_of_pixels);

extern void convolve(image_ptr source, int cols, int rows, int kwidth, int kheight, float *kernel, int bias, char *filename);
extern void makeczp(image_ptr buffer, int rows, int cols, int V, int H);
extern void nearest_neighbor_interpolation(image_ptr buffer, char *fileout, int rows, int cols, float x_scale, float y_scale, int type);
extern void bilinear_interpolation(image_ptr buffer, char *fileout, int rows, int cols, float x_scale, float y_scale, int type);

extern void frame_processing(char *file1, char *file2, char *fileout);

void ConvertBMPtoPGN(char *filename, char *fileout);
#pragma endregion

#pragma region main function
void median_filt(image_ptr source, int cols, int rows, char *filename, int side);
void mean_filt(image_ptr source, int cols, int rows, char *filename, int side);

void change_pgm();
void processing_LUT();
void creat_LUT();

void processing_HE();
void creat_histogram();
void creat_CDF();

void processing_convolution();
void creat_zone_plate();

void processing_nearest_neighor_interpolation();
void processing_frame();
#pragma endregion

int main(int argc, char *argv[])
{
	char filein[100];
	char fileout[100];
	image_ptr buffer = NULL;
	BITMAPHEADER header;
	unsigned long bytes_per_pixel;
	unsigned long number_of_pixels;

	printf("Input name of input file (bmp)\n");
	gets(filein);

	printf("\nInput name of output file (pgm)\n");
	gets(fileout);
	printf("\n");

	ConvertBMPtoPGN(filein, fileout);

	return 0;
}

void creat_LUT()
{
	char fileout[100];
	image_ptr buffer = NULL;
	unsigned long bytes_per_pixel;
	unsigned long number_of_pixels;

	printf("Input name of output file\n");
	gets(fileout);
	printf("\n");

	buffer = creat_pnm(256, 256, 5);
	bytes_per_pixel = 1;
	number_of_pixels = 256 * 256;

	change_LUT(buffer, number_of_pixels, solarizing);
	write_pnm(buffer, fileout, 256, 256, 5);
	IP_FREE(buffer);
}

void processing_LUT()
{
	char filein[100];
	char fileout[100];
	int rows, cols, type;
	image_ptr buffer = NULL;
	unsigned long bytes_per_pixel;
	unsigned long number_of_pixels;

	printf("Input name of input file\n");
	gets(filein);

	printf("\nInput name of output file\n");
	gets(fileout);
	printf("\n");

	buffer = read_pnm(filein, &rows, &cols, &type);

	if (type == PPM)
		bytes_per_pixel = 3;
	else
		bytes_per_pixel = 1;
	number_of_pixels = (bytes_per_pixel) * (rows) * (cols);

	apply_LUT(buffer, number_of_pixels, solarizing);
	write_pnm(buffer, fileout, rows, cols, type);

	IP_FREE(buffer);
}

void change_pgm()
{
	char filein[100];
	char fileout[100];
	int rows, cols, type;
	image_ptr buffer = NULL;
	unsigned long bytes_per_pixel;
	unsigned long number_of_pixels;
	int i, j;

	printf("Input name of input file\n");
	gets(filein);

	printf("\nInput name of output file\n");
	gets(fileout);
	printf("\n");

	buffer = read_pnm(filein, &rows, &cols, &type);

	if (type == PPM)
		bytes_per_pixel = 3;
	else
		bytes_per_pixel = 1;
	number_of_pixels = (bytes_per_pixel) * (rows) * (cols);

	for (i = 0; i < rows; ++i) {
		for (j = 0; j < cols; ++j) {
			//buffer[cols * i + j] = (float)j / (cols) * 255.0;
			//buffer[cols * i + j] = (float)i / rows * 255.0;
			//buffer[cols * i + j] = (float)(i + j);
			//CLIP(buffer[cols * i + j], 0, 255);
			if (i == 90 || i == 140 || j == 90 || j == 140) buffer[cols * i + j] = 255;
		}
	}

	write_pnm(buffer, fileout, rows, cols, type);

	IP_FREE(buffer);
}

void processing_HE()
{
	char filein[100];
	char fileout[100];
	int rows, cols, type;
	image_ptr buffer = NULL;
	unsigned long bytes_per_pixel;
	unsigned long number_of_pixels;

	// 콘솔 입력
	printf("Input name of input file\n");
	gets(filein);

	printf("\nInput name of output file\n");
	gets(fileout);
	printf("\n");

	// 이진파일을 char로 읽어온다.
	buffer = read_pnm(filein, &rows, &cols, &type);

	// 이미지 타입에 따른 크기 설정
	if (type == PPM)
		bytes_per_pixel = 3;
	else
		bytes_per_pixel = 1;
	number_of_pixels = (bytes_per_pixel) * (rows) * (cols);

	// 이미지를 he가 적용된 이미지로 변경하고 저장한다.
	histogram_equalize(buffer, number_of_pixels);
	write_pnm(buffer, fileout, rows, cols, type);

	IP_FREE(buffer);
}

void creat_histogram()
{
	char filein[100];
	char fileout[100];
	int rows, cols, type;
	image_ptr buffer = NULL;
	unsigned long bytes_per_pixel;
	unsigned long number_of_pixels;

	// 콘솔 입력
	printf("Input name of input file\n");
	gets(filein);

	printf("\nInput name of output file\n");
	gets(fileout);
	printf("\n");

	// 이진파일을 char로 읽어온다.
	buffer = read_pnm(filein, &rows, &cols, &type);

	// 이미지 타입에 따른 크기 설정
	if (type == PPM)
		bytes_per_pixel = 3;
	else
		bytes_per_pixel = 1;
	number_of_pixels = (bytes_per_pixel) * (rows) * (cols);

	// 이미지를 histogram으로 변경하고 저장한다.
	change_histogram(buffer, number_of_pixels);
	write_pnm(buffer, fileout, 256, 256, 5);	// 크기가 256 x 256 인 histogram 생성

	IP_FREE(buffer);
}

void processing_convolution()
{
	char filein[100];
	char fileout[100];
	int rows, cols, type;
	image_ptr buffer = NULL;
	unsigned long bytes_per_pixel;
	unsigned long number_of_pixels;

	float averageFilter[3][3] = { {1.0 / 9, 1.0 / 9, 1.0 / 9}, {1.0 / 9, 1.0 / 9, 1.0 / 9}, {1.0 / 9, 1.0 / 9, 1.0 / 9} };
	float highpassFilter[3][3] = { {-1.0 / 9, -1.0 / 9, -1.0 / 9}, {-1.0 / 9, 8.0 / 9, -1.0 / 9}, {-1.0 / 9, -1.0 / 9, -1.0 / 9} };
	float sharpeningFilter[3][3] = { {-1, -1, -1}, {-1, 9, -1}, {-1, -1, -1} };

	float prewittGx[3][3] = { {-1, -1, -1}, {0, 0, 0}, {1, 1, 1} };
	float prewittGy[3][3] = { {-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1} };

	float sobelGx[3][3] = { {-1, -2, -1}, {0, 0, 0}, {1, 2, 1} };
	float sobelGy[3][3] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
	
	// 9시방향부터 시계방향
	float compassPrewitt[8][3][3] =
	{
		{{ -1, 1, 1 }, {-1, -2, 1}, {-1, 1, 1}},
		{{ -1, -1, 1 }, {-1, -2, 1}, {1, 1, 1}},
		{{-1, -1, -1}, {1, -2, 1}, {1, 1, 1}},
		{{1, -1, -1}, {1, -2, -1}, {1, 1, 1} },
		{{1, 1, -1}, {1, -2, -1}, {1, 1, -1}},
		{{1, 1, 1}, {1, -2, -1}, {1, -1, -1}},
		{{1, 1, 1}, {1, -2, 1}, {-1, -1, -1}},
		{{1, 1, 1}, {-1, -2, 1}, {-1, -1, 1}}
	};

	float compassKirsh[8][3][3] =
	{
		{{-3, -3, 5}, {-3, 0, 5}, {-3, -3, 5}},
		{{-3, -3, -3}, {-3, 0, 5}, {-3, 5, 5}},
		{{-3, -3, -3}, {-3, 0, -3}, {5, 5, 5}},
		{{-3, -3, -3}, {5, 0, -3}, {5, 5, -3}},
		{{5, -3, -3}, {5, 0, -3}, {5, -3, -3}},
		{{5, 5, -3}, {5, 0, -3}, {-3, -3, -3}},
		{{5, 5, 5}, {-3, 0, -3}, {-3, -3, -3}},
		{{-3, 5, 5}, {-3, 0, 5}, {-3, -3, -3}}
	};

	float compassRobinson3[8][3][3] =
	{
		{{-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1}},
		{{-1, -1, 0}, {-1, 0, 1}, {0, 1, 1}},
		{{-1, -1, -1}, {0, 0, 0}, {1, 1, 1}},
		{{0, -1, -1}, {1, 0, -1}, {1, 1, 0}},
		{{1, 0, -1}, {1, 0, -1}, {1, 0, -1}},
		{{1, 1, 0}, {1, 0, -1}, {0, -1, -1}},
		{{1, 1, 1}, {0, 0, 0}, {-1, -1, -1}},
		{{0, 1, 1}, {-1, 0, 1}, {-1, -1, 0}}
	};

	float compassRobinson5[8][3][3] =
	{
		{{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}},
		{{-2, -1, 0}, {-1, 0, 1}, {0, 1, 2}},
		{{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}},
		{{0, -1, -2}, {1, 0, -1}, {2, 1, 0}},
		{{1, 0, -1}, {2, 0, -2}, {1, 0, -1}},
		{{2, 1, 0}, {1, 0, -1}, {0, -1, -2}},
		{{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}},
		{{0, 1, 2}, {-1, 0, 1}, {-2, -1, 0}}
	};

	float LoG[5][5] = {
		{0, 0, -1, 0, 0},
		{0, -1, -2, -1, 0},
		{-1, -2, 16, -2, -1},
		{0, -1, -2, -1, 0},
		{0, 0, -1, 0, 0}
	};

	printf("input name of input file\n");
	gets(filein);

	printf("\ninput name of output file\n");
	gets(fileout);
	printf("\n");

	buffer = read_pnm(filein, &rows, &cols, &type);

	if (type == PPM)
		bytes_per_pixel = 3;
	else
		bytes_per_pixel = 1;
	number_of_pixels = (bytes_per_pixel) * (rows) * (cols);

	//convolve(buffer, cols, rows, 5, 5, LoG, 0, fileout);
	median_filt(buffer, cols, rows, fileout, 3);
	//mean_filt(buffer, cols, rows, fileout, 3);

	IP_FREE(buffer);
}

void creat_zone_plate()
{
	char fileout[100];
	image_ptr buffer = NULL;
	unsigned long bytes_per_pixel;
	unsigned long number_of_pixels;

	printf("Input name of output file\n");
	gets(fileout);
	printf("\n");

	buffer = creat_pnm(256, 256, 5);
	bytes_per_pixel = 1;
	number_of_pixels = 256 * 256;

	makeczp(buffer, 256, 256, 100, 100);
	write_pnm(buffer, fileout, 256, 256, 5);
	IP_FREE(buffer);
}

void creat_CDF()
{
	char filein[100];
	char fileout[100];
	int rows, cols, type;
	image_ptr buffer = NULL;
	unsigned long bytes_per_pixel;
	unsigned long number_of_pixels;

	// 콘솔 입력
	printf("Input name of input file\n");
	gets(filein);

	printf("\nInput name of output file\n");
	gets(fileout);
	printf("\n");

	// 이진파일을 char로 읽어온다.
	buffer = read_pnm(filein, &rows, &cols, &type);

	// 이미지 타입에 따른 크기 설정
	if (type == PPM)
		bytes_per_pixel = 3;
	else
		bytes_per_pixel = 1;
	number_of_pixels = (bytes_per_pixel) * (rows) * (cols);

	// 이미지를 histogram의 CDF로 변경하고 저장한다.
	change_CDF(buffer, number_of_pixels);
	write_pnm(buffer, fileout, 256, 256, 5);	// 크기가 256 x 256 인 CDF 생성

	IP_FREE(buffer);
}

void processing_nearest_neighor_interpolation() {
	char filein[100];
	char fileout[100];
	int rows, cols, type;
	image_ptr buffer = NULL;
	unsigned long bytes_per_pixel;
	unsigned long number_of_pixels;

	// 콘솔 입력
	printf("Input name of input file\n");
	gets(filein);

	printf("\nInput name of output file\n");
	gets(fileout);
	printf("\n");

	// 이진파일을 char로 읽어온다.
	buffer = read_pnm(filein, &rows, &cols, &type);

	// 이미지 타입에 따른 크기 설정
	if (type == PPM)
		bytes_per_pixel = 3;
	else
		bytes_per_pixel = 1;
	number_of_pixels = (bytes_per_pixel) * (rows) * (cols);

	nearest_neighbor_interpolation(buffer, fileout, rows, cols, 2, 2, type);

}

void processing_frame()
{
	char file1[100];
	char file2[100];
	char fileout[100];
	int rows, cols, type;
	image_ptr buffer = NULL;
	unsigned long bytes_per_pixel;
	unsigned long number_of_pixels;

	// 콘솔 입력
	printf("Input name of first input file\n");
	gets(file1);
	printf("Input name of second input file\n");
	gets(file2);
	printf("\nInput name of output file\n");
	gets(fileout);
	printf("\n");

	frame_processing(file1, file2, fileout);
}