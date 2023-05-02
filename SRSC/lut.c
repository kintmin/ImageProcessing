#include "ip.h"

void apply_LUT(image_ptr buffer, unsigned long number_of_pixels, int *operation(int));
void change_LUT(image_ptr buffer, unsigned long number_of_pixels, int *operation(int));
void set_LUT(unsigned char *LUT, int *operation(int));

int nullLUT(int value);
int customLUT(int value);
int gamma_collection(int value);
int nagative_transform(int value);
int streching(int value);
int compression(int value);
int posterizing(int value);
int thresholding(int value);
int bounded_thresholding(int value);
int iso_intensity_contouring(int value);
int solarizing(int value);

void apply_LUT(image_ptr buffer, unsigned long number_of_pixels, int *operation(int))
{
	unsigned char LUT[256];
	unsigned long i;

	set_LUT(LUT, operation);

	for (i = 0; i < number_of_pixels; i++)
		buffer[i] = LUT[buffer[i]];
}

void change_LUT(image_ptr buffer, unsigned long number_of_pixels, int *operation(int))
{
	unsigned char LUT[256];
	unsigned long i, j;

	set_LUT(LUT, operation);

	// 256 x 256 파일이라고 가정
	for (j = 0; j < 256; ++j) {
		for (i = 0; i < LUT[j]; ++i) {
			buffer[256 * (255 - i) + j] = 255;
		}
		for (; i < 256; ++i) {
			buffer[256 * (255 - i) + j] = 0;
		}
	}
}

void set_LUT(unsigned char *LUT, int *operation(int))
{
	unsigned long i;
	int temp;

	for (i = 0; i < 256; i++)
	{
		temp = operation(i);
		CLIP(temp, 0, 255);
		LUT[i] = temp;
	}
}

#pragma region operation 모음

int nullLUT(int value)
{
	return value;
}

int customLUT(int value)
{
	return value;
}

// contrast 증가/감소 효과
int gamma_collection(int value)
{
	float gamma = 2;
	return (pow(value, gamma) / pow(255, gamma)) * 255;
}

// 원본은 LUT가 y=x인데, 이건 LUT가 y =-x+255 라서 nagative
int nagative_transform(int value)
{
	return 255 - value;
}

// contrast 증가 효과
int streching(int value)
{
	int low = 30, high = 30;
	if (value < low) return 0;
	else if (value > 255 - high) return 255;

	float a = 255.0 / (255 - low - high);
	return a * (value - low);
}

// contrast 감소 효과
int compression(int value)
{
	int low = 30, high = 30;
	return (255 - low - high) / 255.0 * value + low;
}

// 색 level을 낮춤
int posterizing(int value)
{
	int step = 32;
	return value / step * step;
}

// level을 binary 수준으로 낮춤
int thresholding(int value)
{
	return value < 128 ? 0 : 255;
}

// 특정 범위만 level을 binary 수준으로 낮춤
int bounded_thresholding(int value)
{
	int low = 80, mid = 120, high = 160;
	if (value > low && value < mid) return low;
	else if (value >= mid && value < high) return high;
	else return value;
}

// 특정 level을 없애버림
int iso_intensity_contouring(int value)
{
	return value % 32 < 16 ? 0 : value;
}

// 특정 부분을 nagative하게 바꿈
int solarizing(int value)
{
	return  value < 180 ? value : 255 - value;
}

#pragma endregion
