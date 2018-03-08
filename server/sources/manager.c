#include <server.h>

int	start(int fd_socket, t_clients *clients,
				   int map[10][10], t_bombs *bombs)
{
  fd_set		readfds;
  int			err;
  int			hfd;

  err = 0;
  hfd = clients[3].fd + 1;
  clients->done = 0;
  bombs->to_explode = 0;
  while (err != -1 && err != -2)
  {
    init_fd_set(fd_socket, &readfds, clients);
    if ((err = my_select(&readfds, hfd)) != -1) // if ready
  	{
  	  if ((err = manage(&readfds, clients, map, bombs, fd_socket)) == -1)
  	    return (-1);
  	}
    else
	    return (-1);
  }
  free(bombs);
  if (load_server(fd_socket, clients) == -1)
    return (-1);
  return (0);
}

int commands(char *buff, t_clients *clients, int map[10][10], t_bombs *bombs)
{
  t_map coord;
  int   ret;

  ret = 0;
  coord.x = 0;
  coord.y = 0;
  init_coord(clients->id, buff, &coord, map);
  if ((my_strcmp(buff, "UP\n") == 0)
      || (my_strcmp(buff, "DOWN\n") == 0)) 
    ret = move_to(coord, clients, buff, map, 0);
  else if ((my_strcmp(buff, "LEFT\n") == 0)
     || (my_strcmp(buff, "RIGHT\n") == 0))
    ret = move_to(coord, clients, buff, map, 1);
  else if (my_strcmp(buff, "DROP\n") == 0)
    ret = drop_bomb(clients, buff, &coord, &bombs);
  else if (my_strcmp(buff, "EXPLODE\n") == 0)
    ret = explode_bomb(clients, bombs, map);
  else
    ret = write(clients->id, "KO\n", 3);
  put_bomb_map(clients, map, &bombs);
  display_map(map);
  return (ret);
}

void init_coord(int client, char *buff, t_map *coord, int map[10][10])
{
  find_player(coord, client, map);
  if (my_strcmp(buff, "UP\n") == 0 || my_strcmp(buff, "LEFT\n") == 0)
  {
    coord->min = 0;
    coord->max = 10;
    if (my_strcmp(buff, "UP\n") == 0)
      coord->new_move = coord->y - 1;
    else
      coord->new_move = coord->x - 1;
  }
  else
  {
    coord->min = -1;
    coord->max = 9;
    if (my_strcmp(buff, "DOWN\n") == 0)
      coord->new_move = coord->y + 1;
    else
      coord->new_move = coord->x + 1;
  }
}

int send_info(t_clients *clients, char *buff, int flag)
{
  int i;

  if (flag == 1)
    if (write(clients->id, "OK\n", 3) == -1)
      {
  perror("write()");
  return (-1);
      }
  for (i = 0; i < 4; i++)
    if ((clients[i].fd != clients->id) && (clients[i].fd != 0))
      if (clients_signal(clients[i].fd, clients->id - 3, 10, buff) == -1)
  return (-1);
  return (0);
}

int	manage(fd_set *readfds, t_clients *clients, int map[10][10],
	       t_bombs *bombs, int fd_socket)
{
  char	buff[16];
  int	i;

  check_socket(fd_socket, readfds);
  for (i = 0; i < 4; ++i)
    if (clients[i].fd != 0)
      if (FD_ISSET(clients[i].fd, readfds))
	    {
	      my_memset(buff, 16);
	      if (read(clients[i].fd, buff, 16) <= 0)
	      {
	        if (player_dies(clients, i + 1, map) == -1)
		        return (-1);
	      }
	      else
	      {
	        clients->id = clients[i].fd;
	        if (commands(buff, clients, map, bombs) == -1)
		        return (-1);
	      }
	    }
  for (i = 0; i < 4; i++)
    if (clients[i].fd != 0)
      return (0);
  return (-2);
}
