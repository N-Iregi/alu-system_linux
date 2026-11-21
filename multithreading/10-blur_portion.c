#include "multithreading.h"

/**
 * blur_pixel - blurs one pixel with the kernel
 * @p: portion description
 * @x: pixel column
 * @y: pixel row
 *
 * Neighbours outside the image are skipped, and the result is divided by
 * the sum of the weights actually used.
 */
static void blur_pixel(blur_portion_t const *p, size_t x, size_t y)
{
	float r = 0, g = 0, b = 0, sum = 0, k;
	size_t kx, ky, half = p->kernel->size / 2;
	long ix, iy;
	pixel_t const *src;
	pixel_t *dst;

	for (ky = 0; ky < p->kernel->size; ky++)
	{
		for (kx = 0; kx < p->kernel->size; kx++)
		{
			ix = (long)x + (long)kx - (long)half;
			iy = (long)y + (long)ky - (long)half;
			if (ix < 0 || iy < 0 || (size_t)ix >= p->img->w ||
			    (size_t)iy >= p->img->h)
				continue;
			k = p->kernel->matrix[ky][kx];
			src = &p->img->pixels[iy * p->img->w + ix];
			r += src->r * k;
			g += src->g * k;
			b += src->b * k;
			sum += k;
		}
	}
	if (sum == 0)
		return;
	dst = &p->img_blur->pixels[y * p->img->w + x];
	dst->r = (uint8_t)(r / sum);
	dst->g = (uint8_t)(g / sum);
	dst->b = (uint8_t)(b / sum);
}

/**
 * blur_portion - blurs a portion of an image with a Gaussian blur
 * @portion: description of the portion to blur
 */
void blur_portion(blur_portion_t const *portion)
{
	size_t x, y, x_end, y_end;

	if (!portion || !portion->img || !portion->img_blur ||
	    !portion->kernel)
		return;
	x_end = portion->x + portion->w;
	y_end = portion->y + portion->h;
	if (x_end > portion->img->w)
		x_end = portion->img->w;
	if (y_end > portion->img->h)
		y_end = portion->img->h;
	for (y = portion->y; y < y_end; y++)
		for (x = portion->x; x < x_end; x++)
			blur_pixel(portion, x, y);
}
