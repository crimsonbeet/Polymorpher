#include<stdio.h>
#include<stdlib.h>
#include<string.h>
extern double STSIZ;
class DxbOutPut
{
	public:
	FILE*DXB;
	char*nam;
	void write_header(void);
	void set_double(void);
	void write_pline(short closure);
	void write_vertex(long x, long y);
	void write_vertex(double x, double y);
	void write_width(double sw, double ew);
	void write_line(long, long, long, long);
	void write_layer(char*layer);
	void write_seqend(void);
	void write_eof(void);
	DxbOutPut(){DXB=0;nam=0;}
	~DxbOutPut(){write_eof();}
}	;
void DxbOutPut::write_header(void)
{
	char header[40];
	strncpy(header, "AutoCAD DXB 1.0\r\n\x1a", 40);
	fwrite(header, strlen(header)+1, 1, DXB);
	return;
}
void DxbOutPut::set_double(void)
{
	unsigned char id;
	short mode;
	id = 135; /*ITEM-IS-NUMBER-MODE*/
	mode = 1; /*DOUBLE-MODE-FOLLOWS*/
	fwrite((char*)&id, 1, 1, DXB);
	fwrite((char*)&mode, 2, 1, DXB);
}
void DxbOutPut::write_line(long from_x, long from_y, long to_x, long to_y)
{
	struct
	{
		unsigned char id;
		double from_x;
		double from_y;
		double to_x;
		double to_y;
	}	line;
	line.id = 1;
	line.from_x = from_x/1000.0;
	line.from_y = from_y/1000.0;
	line.to_x = to_x/1000.0;
	line.to_y = to_y/1000.0;
	fwrite((char*)&line, sizeof(line), 1, DXB);
}
void DxbOutPut::write_pline(short closure)
{
	struct
	{
		unsigned char id;
		short closure;
	} pline;
	pline.id = 19;
	pline.closure = closure;
	fwrite((char*)&pline, 3, 1, DXB);
}
void DxbOutPut::write_layer(char*layer)
{
	unsigned char id;
	id = 129;
	fwrite((char*)&id, 1, 1, DXB);
	fwrite(layer, strlen(layer)+1, 1, DXB);
}
void DxbOutPut::write_vertex(double x, double y)
{
	struct
	{
	unsigned char id;
	double dx, dy;
	} vertex;
	vertex.id = 20;
	vertex.dx = (x * STSIZ)/1000;
	vertex.dy = (y * STSIZ)/1000;
	fwrite((char*)&vertex, 17, 1, DXB);
}
void DxbOutPut::write_vertex(long x, long y)
{
	struct
	{
	unsigned char id;
	double dx, dy;
	} vertex;
	vertex.id = 20;
	vertex.dx = ((double)x * STSIZ)/1000;
	vertex.dy = ((double)y * STSIZ)/1000;
	fwrite((char*)&vertex, 17, 1, DXB);
}
void DxbOutPut::write_width(double sw, double ew)
{
	struct
	{
		unsigned char id;
		double sw;
		double ew;
	}	width;
	width.id = 134;
	width.sw = sw/1000;
	width.ew = ew/1000;
	fwrite((char*)&width, 1, sizeof(width), DXB);
}
void DxbOutPut::write_seqend(void)
{
	unsigned char id;
	id = 17;
	fwrite((char*)&id, 1, 1, DXB);
}
void DxbOutPut::write_eof(void)
{
	unsigned char id = 0;
	if(DXB)
	{
		fwrite((char*)&id, 1, 1, DXB);
		fclose(DXB);
	}
	DXB = 0;
}
