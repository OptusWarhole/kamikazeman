#include <unistd.h>

void	my_putstr(char *str, int fd)
{
  int	i;

  for (i = 0; str[i]; i++);
  write(fd, str, i);
}
