#ifndef MD2_HPP
#define MD2_HPP

typedef unsigned char uchar8_t;

typedef struct
{
	int magicnumber;
	int version;

	int skinwidth;
	int skinheight;

	int framesize;
	int skins_c;
	int vertices_c;
	int textcoords_c;
	int triangles_c;
	int glcmds_c;
	int frames_c;

	int skins_offset;
	int textcoords_offset;
	int triangles_offset;
	int frames_offset;
	int glcmds_offset;
	int eof_offset;

} MD2_Header;

typedef struct
{
	unsigned short vertex[3];
	unsigned short st[3];
} MD2_Triangle;



#endif