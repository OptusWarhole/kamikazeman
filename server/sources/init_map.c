#include <server.h>

void  display_map(int map[10][10])
{
  int i;
  int j;

  for (i = 0; i < 10; i++)
  {
    for (j = 0; j < 10; j++)
      my_putint(map[i][j], 0);
    my_putstr("\n", 0);
  }
  my_putstr("\n", 0);
}

void	trigger(int *pos, int *box, t_map coord, int map[10][10], int trig)
{
  if (trig == 0)
    {
      *pos = coord.y;
      *box = map[coord.new_move][coord.x];
    }
  else
    {
      *pos = coord.x;
      *box = map[coord.y][coord.new_move];
    }
}

int move_to(t_map coord, t_clients *clients, char *buff,
		  int map[10][10], int trig)
{
  int	pos;
  int	box;
  int ret;

  pos = 0;
  box = 0;
  ret = 0;

  trigger(&pos, &box, coord, map, trig);
  if ((pos > coord.min) && (pos < coord.max))
  {
    if (box >= 9)
    {
      ret = new_pos(coord, clients, buff, map, trig);
    }
    else if (box == 0)
	    ret = new_pos(coord, clients, buff, map, trig);
    else
	    ret = write(clients->id, "KO\n", 3);
  }
  else
    ret = write(clients->id, "KO\n", 3);
  return (ret);
}

int		init_map(int fd_socket, t_clients *clients)
{
  t_bombs	*bombs;
  int		map[10][10] = {
    {1, 0, 5, 5, 7, 7, 5, 5, 0, 4},
    {0, 7, 5, 5, 5, 5, 5, 5, 7, 0},
    {5, 5, 8, 7, 7, 5, 7, 8, 5, 5},
    {8, 5, 7, 7, 8, 5, 7, 7, 5, 8},
    {7, 5, 5, 5, 5, 5, 7, 5, 5, 7},
    {7, 5, 5, 7, 5, 5, 5, 5, 5, 7},
    {8, 5, 7, 7, 5, 8, 7, 7, 5, 8},
    {5, 5, 8, 7, 5, 7, 7, 8, 5, 5},
    {0, 7, 5, 5, 5, 5, 5, 5, 7, 0},
    {2, 0, 5, 5, 7, 7, 5, 5, 0, 3}};

  if ((bombs = malloc(4*sizeof(t_bombs))) == NULL)
  {
    perror("malloc()");
    return (-1);
  }
  init_bomb(clients);
  clients->nb_bomb = 0;
  display_map(map);
  my_putstr("Map initialized !\n\n", 0);
  if (start(fd_socket, clients, map, bombs) == -1)
    return (-1);
  return (0);
}

void  *find_player(t_map *coord, int player, int map[10][10])
{
  for (coord->y = 0; coord->y < 10; (coord->y)++)
    for (coord->x = 0; coord->x < 10; (coord->x)++)
      if (map[coord->y][coord->x] == (player - 3))
        return (NULL);
  return (NULL);
}

int my_close_sock(int client)
{
  int   err_cl;

  err_cl = 0;
  if (client != 0)
    err_cl = close(client);
  if (err_cl == -1)
    {
      perror("close()");
      return (err_cl);
    }
  return (err_cl);
}