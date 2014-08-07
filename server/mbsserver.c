/* mbsserver.c -
 */

#include "server/mbsserver.h"

#include <sys/socket.h>
#include <netinet/in.h>



/* mbs_server_new:
 */
MbsServer *mbs_server_new ( void )
{
  MbsServer *s;
  s = g_new0(MbsServer, 1);
  s->port = 6666;
  return s;
}



/* make_socket:
 */
static gint make_socket ( guint16 port )
{
  gint sock;
  struct sockaddr_in name;
  /* create the socket */
  if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	DIE("socket failed: %s", STRERROR);
  /* name the socket */
  name.sin_family = AF_INET;
  name.sin_port = htons(port);
  name.sin_addr.s_addr = htonl(INADDR_ANY);
  if (bind(sock, (struct sockaddr *) &name, sizeof(name)) < 0)
	DIE("bind failed: %s", STRERROR);
  return sock;
}



/* mbs_server_start:
 */
void mbs_server_start ( MbsServer *server )
{
  server->listen_sock = make_socket(server->port);
  if (listen(server->listen_sock, 1) < 0)
	DIE("listen failed: %s", STRERROR);
}
