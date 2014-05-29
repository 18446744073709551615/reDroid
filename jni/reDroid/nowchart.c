#include "nowchart.h"

size_t wcslen(const widechar_t * s)
{
  const widechar_t *p;

  p = s;
  while (*p)
    p++;

  return p - s;
}

widechar_t * wcsncpy(widechar_t * s1, const widechar_t * s2, size_t n)
{
  widechar_t *p;
  const widechar_t *q;

  *s1 = '\0';
  p = s1;
  q = s2;
  while (n && *q)
    {
      *p++ = *q++;
      n--;
    }
  *p = '\0';

  return s1;
}

char * wcs2csncpy(char * s1, const widechar_t * s2, size_t n)
{
  char *p;
  const widechar_t *q;

  *s1 = '\0';
  p = s1;
  q = s2;
  while (n && *q)
    {
      *p++ = *q++;
      n--;
    }
  *p = '\0';

  return s1;
}

