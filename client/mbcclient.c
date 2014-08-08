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



static gboolean _on_client_ready ( GIOChannel *chan,
								   GIOCondition cond,
								   gpointer data )
{
  DIE("[TODO] client ready");
  return TRUE;
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
  /* create the IO channel */
  cli->chan = g_io_channel_unix_new(cli->sock);
  g_io_channel_set_encoding(cli->chan, NULL, NULL);
  cli->watchid = g_io_add_watch(cli->chan, G_IO_IN, _on_client_ready, cli);
  return 0;
}



/* mbc_client_send:
 */
void mbc_client_send ( MbcClient *cli,
					   const gchar *text )
{
  gsize w;
  GIOStatus r;
  GError *err = NULL;
  r = g_io_channel_write_chars(cli->chan, text, -1, &w, &err);
  if (r != G_IO_STATUS_NORMAL)
	DIE("[TODO] write failed");
  g_io_channel_flush(cli->chan, NULL);
}
