#include <string.h>
#include "tom_aray.hpp"
struct F_SIZE_JOB
{
	char Name[20];
	long Size;
	char Sign;
}	;
void	F_SIZE_SHAPES(F_SIZE_JOB&JOB);
void	F_SIZE_COLLARS(F_SIZE_JOB&JOB);
void
__sizemain__(char Name[20], long Size, char Sign)
{
	F_SIZE_JOB JOB;
	memcpy(JOB.Name, Name, 20);
	JOB.Size = Size;
	JOB.Sign = Sign;
	F_SIZE_SHAPES(JOB);
	F_SIZE_COLLARS(JOB);
}
