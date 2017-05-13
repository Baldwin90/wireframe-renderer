/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achi <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/30 15:49:40 by achi              #+#    #+#             */
/*   Updated: 2017/01/30 15:49:42 by achi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <stdio.h>
#include <libft.h>
#include <fdf.h>

#define FHSA float h;float s;float angle;
#define FHSB float hue; float saturation; float brightness;
#define FRGBC float redc; float greenc; float bluec;
#define FHFPQT float h; float f; float p; float q; float t;
#define IRGB int r = 0; int g = 0; int b = 0;
#define ICMAXMIN int cmax; int cmin;
#define RGB r = g = b
#define F255P5 255.0f + 0.5f
#define H0RGB (brightness * F255P5);g = (int)(t * F255P5);b = (int)(p * F255P5);
#define H1RGB (q * F255P5);g = (int)(brightness * F255P5);b = (int)(p * F255P5);
#define H2RGB (p * F255P5);g = (int)(brightness * F255P5);b = (int)(t * F255P5);
#define H3RGB (p * F255P5);g = (int)(q * F255P5);b = (int)(brightness * F255P5);
#define H4RGB (t * F255P5);g = (int)(p * F255P5);b = (int)(brightness * F255P5);
#define H5RGB (brightness * F255P5);g = (int)(p * F255P5);b = (int)(q * F255P5);

float	lerp_angle(float a, float b, float t)
{
	float	num;

	num = (b - a) - floor((b - a) / 360.0f) * 360.0f;
	if (num > 180.0f)
		num -= 360.0f;
	return (a + num * (t - (int)t));
}

float	lerp(float a, float b, float t)
{
	return (a + (b - a) * (t - (int)t));
}

void	hsb_lerp(float a[], float b[], float t, float *vals)
{
	FHSA;
	if (a[2] == 0)
	{
		h = b[0];
		s = b[2];
	}
	else if (b[2] == 0)
	{
		h = a[0];
		s = a[1];
	}
	else
	{
		if (a[1] == 0)
		{
			h = b[0];
		}
		else if (b[2] == 0)
		{
			h = a[0];
		}
		else
		{
			angle = lerp_angle(a[0] * 360.0f, b[0] * 360.0f, t);
			while (angle < 0.0f)
				angle += 360.0f;
			while (angle > 360.0f)
				angle -= 360.0f;
			h = angle / 360.0f;
		}
		s = lerp(a[1], b[1], t);
	}
	vals[0] = h;
	vals[1] = s;
	vals[2] = lerp(a[2], b[2], t);
}

int		hsb2rgb(float hsbvals[])
{
	FHSB;
	IRGB;
	FHFPQT;
	hue = hsbvals[0];
	saturation = hsbvals[1];
	brightness = hsbvals[2];
	if (saturation == 0)
		RGB = (int)(brightness * F255P5);
	else
	{
		h = (hue - (float)floor(hue)) * 6.0f;
		f = h - (float)floor(h);
		p = brightness * (1.0f - saturation);
		q = brightness * (1.0f - saturation * f);
		t = brightness * (1.0f - (saturation * (1.0f - f)));
		if ((int)h == 0) {r = (int)H0RGB;}
		else if ((int)h == 1) {r = (int)H1RGB;}
		else if ((int)h == 2) {r = (int)H2RGB;}
		else if ((int)h == 3) {r = (int)H3RGB;}
		else if ((int)h == 4) {r = (int)H4RGB;}
		else if ((int)h == 5) {r = (int)H5RGB;}
	}
	return ((r << 16) | (g << 8) | (b << 0));
}

void	rgb2hsb(int r, int g, int b, float *vals)
{
	FHSB;
	FRGBC;
	ICMAXMIN;
	cmax = (r > g) ? r : g;
	if (b > cmax)
		cmax = b;
	cmin = (r < g) ? r : g;
	if (b < cmin)
		cmin = b;
	brightness = ((float)cmax) / 255.0f;
	saturation = (cmax != 0 ? ((float)(cmax - cmin)) / ((float)cmax) : 0);
	if (saturation == 0)
		hue = 0;
	else
	{
		redc = ((float)(cmax - r)) / ((float)(cmax - cmin));
		greenc = ((float)(cmax - g)) / ((float)(cmax - cmin));
		bluec = ((float)(cmax - b)) / ((float)(cmax - cmin));
		if (r == cmax)
			hue = bluec - greenc;
		else if (g == cmax)
			hue = 2.0f + redc - bluec;
		else
			hue = 4.0f + greenc - redc;
		hue = hue / 6.0f;
		if (hue < 0)
			hue = hue + 1.0f;
	}
	vals[0] = hue;
	vals[1] = saturation;
	vals[2] = brightness;
}
