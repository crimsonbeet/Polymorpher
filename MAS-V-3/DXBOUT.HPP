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
	DxbOutPut(){DXB=0;}
	~DxbOutPut(){write_eof();}
}	;
