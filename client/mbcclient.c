/* mbcclient.c -
 */

#include "client/cliprivate.h"
#include "client/mbcclient.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>



/* mbc_client_new:
 */
MbcClient *mbc_client_new ( MbcClientHandler handler,
                            gpointer data )
{
  MbcClient *cli;
  cli = g_new0(MbcClient, 1);
  cli->handler = handler;
  cli->handler_data = data;
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
	CL_ERROR("unknown host: '%s'", host);
  name->sin_addr = *(struct in_addr *) hostinfo->h_addr;
}



static void _on_client_watch ( GIOCondition condition,
                               gpointer data )
{
  MbcClient *cli = (MbcClient *) data;
  cli->handler(cli, condition, cli->stream, cli->handler_data);
}



/* mbc_client_connect:
 */
gint mbc_client_connect ( MbcClient *cli,
						  const gchar *host,
						  guint16 port )
{
  CL_DEBUG("[TODO]");
  /* struct sockaddr_in server_name; */
  /* /\* create the socket *\/ */
  /* if ((cli->sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) */
  /*   CL_ERROR("could not create socket: %s", STRERROR); */
  /* init_sockaddr(&server_name, host, port); */
  /* /\* connect *\/ */
  /* if (connect(cli->sock, (struct sockaddr *) &server_name, sizeof(server_name)) < 0) */
  /*   CL_ERROR("connect failed: %s", STRERROR); */
  /* /\* set the non-blocking flag *\/ */
  /* if (fcntl(cli->sock, F_SETFL, O_NONBLOCK) < 0) */
  /*   CL_ERROR("SETFL(NONBLOCK) failed: %s", STRERROR); */
  /* /\* create the stream *\/ */
  /* cli->stream = l_file_fdopen(cli->sock, "rw", NULL); */
  /* ASSERT(cli->stream); */
  /* /\* create the watch *\/ */
  /* cli->watch = mb_watch_new(cli->sock, _on_client_watch, cli, NULL); */
  /* mb_watch_add_condition(cli->watch, G_IO_IN); */
  return 0;
}



/* mbc_client_send:
 */
void mbc_client_send ( MbcClient *cli,
                       MbMessage *msg )
{
  CL_DEBUG("[TODO] client_send...");
}



/* mbc_client_add_watch:
 */
void mbc_client_add_watch ( MbcClient *client,
                            GIOCondition condition )
{
  mb_watch_add_condition(client->watch, condition);
}



/* mbc_client_remove_watch:
 */
void mbc_client_remove_watch ( MbcClient *client,
                               GIOCondition condition )
{
  mb_watch_remove_condition(client->watch, condition);
}
