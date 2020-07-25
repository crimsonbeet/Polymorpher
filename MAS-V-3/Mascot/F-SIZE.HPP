struct F_SIZE_JOB
{
	char Name[20];
	long Size;
	char Sign;
}	;
void	F_SIZE_SHAPES(F_SIZE_JOB&JOB);
void	F_SIZE_COLLARS(F_SIZE_JOB&JOB);
void
__sizemain__(char Name[20], long Size, char Sign);
