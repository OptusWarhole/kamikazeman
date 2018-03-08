#include <server.h>

int player_dies(t_clients *clients, int player_id, int map[10][10])
{
  int   i;
  int   j;

  for (i = 0; i < 10; i++)
    for (j = 0; j < 10; j++)
      if (map[i][j] == player_id)
        map[i][j] = 0;
  if (player_state(clients, player_id) == -1)
    return (-1);
  return (0);
}

int clients_signal(int client, int id, int len, char *buff)
{
  char  c[len];

  my_memset(c, len);
  sprintf(c, "%d ", id);
  my_strcat(c, buff);
  if (write(client, c, len) == -1)
    {
      perror("write()");
      return (-1);
    }
  my_memset(c, len);
  return (0);
}

int	player_state(t_clients *clients, int player_id)
{
  int	i;

  for (i = 0; i < 4; i++)
    if ((clients[i].fd != player_id + 3) && (clients[i].fd != 0))
      clients_signal(clients[i].fd, player_id, 7, "DIE\n");
  if (my_close_sock(player_id + 3) == -1)
    return (-1);
  clients[player_id - 1].fd= 0;
  clients->done = clients->done + 1;
  if (clients->done == 3)
    for (i = 0; i < 4; i++)
      if (clients[i].fd != 0)
	if (write(clients[i].fd, "WIN\n", 5) == -1)
	  return (-1);
  return (0);
}
