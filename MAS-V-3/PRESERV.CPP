#include <dos.h>
#include <mem.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define done(i) exit((i))

void preserv (void)
{
  unsigned char far *ptr;
  union
	{
	 char c[4];
	 unsigned long l;
	} sum;

  char date[] = "STORED_D";
  char cod[]  = "STORED_C";
  char firm[] = "STORED_F";
  char d[9], c[9], f[9];
  unsigned int i;
  unsigned long Sum;
  d[0] = 83; d[1] = 84; d[2] = 79; d[3] = 82; d[4] = 69; d[5] = 68;
  d[6] = 95; d[7] = 68; d[8] = 0;

  c[0] = 83; c[1] = 84; c[2] = 79; c[3] = 82; c[4] = 69; c[5] = 68;
  c[6] = 95; c[7] = 67; c[8] = 0;

  f[0] = 83; f[1] = 84; f[2] = 79; f[3] = 82; f[4] = 69; f[5] = 68;
  f[6] = 95; f[7] = 70; f[8] = 0;

  if (strcmp (date, d) == 0 ||
      strcmp (cod, c)  == 0 ||
      strcmp (firm, f) == 0)
    done(1000);

  ptr = (unsigned char*)MK_FP (0xF000, 0xFFF5);
  for (Sum=0,i=0; i<8; ptr++,i++)
    Sum += *ptr;
  memcpy (sum.c, date, 4);
  if (Sum != sum.l)
    done (1000);

  ptr = (unsigned char*)MK_FP (0xF000, 0xFFFE);
  Sum = *ptr;
  memcpy (sum.c, cod, 4);
  if (Sum != sum.l)
    done (1000);
  ptr = (unsigned char*)MK_FP (0xFE00, 0x009);
  for (Sum=0; *ptr; ptr++)
    Sum += *ptr;
  ptr = (unsigned char*)MK_FP (0xF000, 0x00B);
  for (; *ptr; ptr++)
    Sum += *ptr;
  memcpy (sum.c, firm, 4);
  if (Sum != sum.l)
    done (1000);
}
