#pragma once;

typedef struct {
  unsigned int 	 width;
  unsigned int 	 height;
  unsigned int 	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  char         	*comment;
  unsigned char	 *pixel_data;//[40 * 40 * 4 + 1];
}gimphead;