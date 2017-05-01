#ifndef FDF_H
# define FDF_H

#define SCREENSIZE 938

#define ABS(x) ((x) < 0 ? -(x) : (x))

typedef struct	s_matrix
{
	int	x;
	int	y;
	float **data;
}				t_matrix;

typedef struct	s_point
{
	t_matrix	*matrix_3d;
	float		screen_x;
	float		screen_y;
	float		hsb[3];
}				t_point;

typedef struct	s_windata
{
	void		*mlx;
	void		*win;
	void		*img;
	char		*pixel;
	int			bpp;
	int			line_size;
	int			endian;
}				t_windata;

typedef struct	s_mapdata
{
	float		alpha;
	float		beta;
	float		gamma;
	float		increment;
	int			x_size;
	int			background;
	char		display_interlace;
	char		anti_alias;
	char		draw_stats;
	t_arraylist	*arr;
	t_matrix	*rot_matrix;
	t_windata	*window;
}				t_mapdata;

#define D t_mapdata

void			data_free(t_mapdata *data);
float			lerp_angle(float a, float b, float t);
float			lerp(float a, float b, float t);
void			HSBLerp(float a[], float b[], float t, float *vals);
int				HSBtoRGB(float hsbvals[]);
void			RGBtoHSB(int r, int g, int b, float *vals);
char			*ft_ftoa(float f);
void			draw_stats(t_mapdata *data);
int				draw_image(t_mapdata *data);
void			draw_pixel(t_windata *data, int x, int y, int color);
void			draw_line(D *dt, float a[], float b[], float c[], float d[]);
void			draw_background(t_mapdata *data);
void			mem_swap(void *a, void *b, size_t size);
void			draw_fdf(t_mapdata *data);
int 			key_hook(int key_code, t_mapdata *data);
void			create_mlx(t_mapdata *data);
void			map_fill(t_mapdata *data);
t_mapdata		*data_create(int color);
void			data_update_rotation(t_mapdata *data);
void			data_addpoint(t_mapdata *data, float coords[], float hsb[]);
void			data_set_value(t_mapdata *data, float *target, float value);
void			rotate_point_into_view(t_mapdata *data, t_point *point);
void			rotate_data_into_view(t_mapdata *data);
t_matrix		*matrix_create(int x, int y, ...);
t_matrix		*matrix_create_empty(int x, int y);
t_matrix		*matrix_dotproduct(t_matrix *a, t_matrix *b);
void			matrix_free(t_matrix *m);

#undef D

#endif