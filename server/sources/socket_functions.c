#include "server.h"

void  check_socket(int fd_socket, fd_set *readfds)
{
  int   stop_socket;
  socklen_t             sock_len;
  struct sockaddr_in    sock_client;

  if (FD_ISSET(fd_socket, readfds))
    {
      //recup client socket
      stop_socket = accept(fd_socket, (struct sockaddr*)&sock_client, &sock_len);
      write(stop_socket, "The server is actually full, please retry later\n", 35);
      close(stop_socket);
    }
}

void init_fd_set(int fd_socket, fd_set *readfds, t_clients *clients)
{
  int   i;

  FD_ZERO(readfds); //clear the setting buffer
  for (i = 0; i < 4; ++i)
  {
    if (clients[i].fd != 0)
	    FD_SET(clients[i].fd, readfds); //fixed size buffer
  }
  FD_SET(fd_socket, readfds);
}

int	my_select(fd_set *readfds, int n)
{
  int			ret_sel;

  ret_sel = select(n, readfds, NULL, NULL, NULL);
  if (ret_sel < 0)
    {
      perror("select()");
      return (-1);
    }
  return (0);
}