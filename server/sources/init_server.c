#include <server.h>

int load_server(int fd_socket, t_clients *clients)
{
  if (client_process(fd_socket, &clients) == -1)
    return (-1);
  my_putstr("Players connected !\n\n", 0);
  if (init_map(fd_socket, clients) == -1)
    return (-1);
  return (0);
}

int	init_serv(int fd_socket, int port)
{
  struct sockaddr_in	my_sockaddr;

  my_sockaddr.sin_family = AF_INET; //address familly ip from internet 
  my_sockaddr.sin_addr.s_addr = INADDR_ANY; //allow to work without ip address
  my_sockaddr.sin_port = htons(port); //convert byte host to network byte
  if ((fd_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror("socket()");
      return (-1);
    }
  if (bind(fd_socket, (struct sockaddr*) &my_sockaddr,
	   sizeof(my_sockaddr)) == -1)
    {
      perror("bind()");
      return (-1);
    }
  return (fd_socket);
}

int	client_process(int fd_socket, t_clients **clients)
{
  int	i;
  char	c;
  char	buff[13];

  c = '1';
  my_memset(buff, 13);
  for (i = 0; i < 4; ++i)
    {
      if (((*clients)[i].fd = (load_client(fd_socket))) == -1)
	      return (-1);
      
      my_strcat(buff, &c);
      my_strcat(buff, " ID\n");

      if (write((*clients)[i].fd, buff, 13) == -1)
	      return (-1);
      my_memset(buff, 13);
      c = c + 1;
    }
  return (0);
}

int	load_client(int fd_socket)
{
  int	fd;
  socklen_t		sock_len;
  struct sockaddr_in	sock_client;

  fd = 0;
  sock_len = sizeof(sock_client);
  if (listen(fd_socket, 1) == -1)
    return (-1);
  my_putstr("Waiting a player...\n", 0);
  fd = accept(fd_socket, (struct sockaddr*)&sock_client, &sock_len);
  if (fd == -1)
    {
      perror("accept()");
      return (-1);
    }
  return (fd);
}
