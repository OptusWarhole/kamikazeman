char    *my_memset(char *s, int n)
{
  int   i;

  for (i = 0; i < n; ++i)
    s[i] = '\0';
  return (s);
}
