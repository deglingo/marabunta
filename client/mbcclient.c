/* mbcclient.c -
 */

#include "client/mbcclient.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>



/* mbc_client_new:
 */
MbcClient *mbc_client_new ( void )
{
  MbcClient *cli;
  cli = g_new0(MbcClient, 1);
  return cli;
}



/* init_sockaddr:
 */
static void init_sockaddr ( struct sockaddr_in *name,
							const gchar *host,
							guint16 port )
{
  struct hostent *hostinfo;
  name->sin_family = AF_INET;
  name->sin_port = htons(port);
  if (!(hostinfo = gethostbyname(host)))
	DIE("unknown host: '%s'", host);
  name->sin_addr = *(struct in_addr *) hostinfo->h_addr;
}



/* mbc_client_connect:
 */
gint mbc_client_connect ( MbcClient *cli,
						  const gchar *host,
						  guint16 port )
{
  struct sockaddr_in server_name;
  /* create the socket */
  if ((cli->sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	DIE("could not create socket: %s", STRERROR);
  init_sockaddr(&server_name, host, port);
  /* connect */
  if (connect(cli->sock, (struct sockaddr *) &server_name, sizeof(server_name)) < 0)
	DIE("connect failed: %s", STRERROR);
  return 0;
}
