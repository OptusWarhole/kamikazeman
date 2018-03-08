#include  <server.h>

int		main()
{
  int		fd_socket;
  t_clients	*clients;
  int		port;
  int		rc;

  port = 4242;
  fd_socket = 0;
  if ((clients = malloc(4*sizeof(t_clients))) == NULL)
    {
      perror("malloc()");
      return (-1);
    }
  if ((fd_socket = init_serv(fd_socket, port)) == -1)
    return (-1);
  rc = load_server(fd_socket, clients);
  free(clients);
  return (rc);
}
