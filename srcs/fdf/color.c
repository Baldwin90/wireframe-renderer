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

#include <Math.h>
#include <stdio.h>
#include <libft.h>
#include <fdf.h>

float lerp_angle(float a, float b, float t)
{
	float num = (b - a) - floor((b - a) / 360.0f) * 360.0f;
	if (num > 180.0f)
	{
		num -= 360.0f;
	}
	return a + num * (t - (int) t);
}

float lerp(float a, float b, float t)
{
	return a + (b - a) * (t - (int) t);
}

void HSBLerp(float a[], float b[], float t, float *vals)
{
	float h,s;

	if(a[2]==0){
		h=b[0];
		s=b[2];
	}else if(b[2]==0){
		h=a[0];
		s=a[1];
	}else{
		if(a[1]==0){
			h=b[0];
		}else if(b[2]==0){
			h=a[0];
		}else{
			// works around bug with LerpAngle
			float angle = lerp_angle(a[0] * 360.0f, b[0] * 360.0f, t);
			while (angle < 0.0f)
				angle += 360.0f;
			while (angle > 360.0f)
				angle -= 360.0f;
			h=angle/360.0f;
		}
		s= lerp(a[1],b[1],t);
	}
	vals[0] = h;
	vals[1] = s;
	vals[2] = lerp(a[2], b[2], t);
}

int HSBtoRGB(float hsbvals[])
{
	float hue=hsbvals[0], saturation=hsbvals[1], brightness=hsbvals[2];
	int r = 0, g = 0, b = 0;
	if (saturation == 0) {
		r = g = b = (int) (brightness * 255.0f + 0.5f);
	} else {
		float h = (hue - (float)floor(hue)) * 6.0f;
		float f = h - (float)floor(h);
		float p = brightness * (1.0f - saturation);
		float q = brightness * (1.0f - saturation * f);
		float t = brightness * (1.0f - (saturation * (1.0f - f)));
		switch ((int) h) {
			case 0:
				r = (int) (brightness * 255.0f + 0.5f);
				g = (int) (t * 255.0f + 0.5f);
				b = (int) (p * 255.0f + 0.5f);
				break;
			case 1:
				r = (int) (q * 255.0f + 0.5f);
				g = (int) (brightness * 255.0f + 0.5f);
				b = (int) (p * 255.0f + 0.5f);
				break;
			case 2:
				r = (int) (p * 255.0f + 0.5f);
				g = (int) (brightness * 255.0f + 0.5f);
				b = (int) (t * 255.0f + 0.5f);
				break;
			case 3:
				r = (int) (p * 255.0f + 0.5f);
				g = (int) (q * 255.0f + 0.5f);
				b = (int) (brightness * 255.0f + 0.5f);
				break;
			case 4:
				r = (int) (t * 255.0f + 0.5f);
				g = (int) (p * 255.0f + 0.5f);
				b = (int) (brightness * 255.0f + 0.5f);
				break;
			case 5:
				r = (int) (brightness * 255.0f + 0.5f);
				g = (int) (p * 255.0f + 0.5f);
				b = (int) (q * 255.0f + 0.5f);
				break;
		}
	}
	return (r << 16) | (g << 8) | (b << 0);
}

void RGBtoHSB(int r, int g, int b, float *vals)
{
	float hue, saturation, brightness;
	int cmax = (r > g) ? r : g;
	if (b > cmax) cmax = b;
	int cmin = (r < g) ? r : g;
	if (b < cmin) cmin = b;

	brightness = ((float) cmax) / 255.0f;
	if (cmax != 0)
		saturation = ((float) (cmax - cmin)) / ((float) cmax);
	else
		saturation = 0;
	if (saturation == 0)
		hue = 0;
	else {
		float redc = ((float) (cmax - r)) / ((float) (cmax - cmin));
		float greenc = ((float) (cmax - g)) / ((float) (cmax - cmin));
		float bluec = ((float) (cmax - b)) / ((float) (cmax - cmin));
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
