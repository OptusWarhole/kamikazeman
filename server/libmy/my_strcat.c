char	*my_strcat(char *str1, char *str2)
{
  int	i;
  int	c;

  i = 0;
  c = 0;
  while (str1[i] != '\0')
    i = i + 1;
  while (str2[c] != '\0')
    {
      str1[i] = str2[c];
      c = c + 1;
      i = i + 1;
    }
  return (str1);
}
