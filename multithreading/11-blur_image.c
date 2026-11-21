#include <pthread.h>
#include "multithreading.h"

#define NB_THREADS 8UL

/**
 * blur_thread - thread entry, blurs one portion
 * @arg: pointer to a blur_portion_t
 *
 * Return: NULL
 */
static void *blur_thread(void *arg)
{
	blur_portion((blur_portion_t const *)arg);
	return (NULL);
}

/**
 * blur_image - blurs a whole image using horizontal bands, one per thread
 * @img_blur: destination image
 * @img: source image
 * @kernel: convolution kernel
 */
void blur_image(img_t *img_blur, img_t const *img, kernel_t const *kernel)
{
	pthread_t tid[NB_THREADS];
	blur_portion_t part[NB_THREADS];
	int started[NB_THREADS];
	size_t n, i, rows;

	if (!img_blur || !img || !kernel || !img->h)
		return;
	n = img->h < NB_THREADS ? img->h : NB_THREADS;
	rows = img->h / n;
	for (i = 0; i < n; i++)
	{
		part[i].img = img;
		part[i].img_blur = img_blur;
		part[i].kernel = kernel;
		part[i].x = 0;
		part[i].y = i * rows;
		part[i].w = img->w;
		part[i].h = (i == n - 1) ? img->h - i * rows : rows;
		started[i] = pthread_create(&tid[i], NULL, blur_thread,
					    &part[i]) == 0;
		if (!started[i])
			blur_portion(&part[i]);
	}
	for (i = 0; i < n; i++)
		if (started[i])
			pthread_join(tid[i], NULL);
}
