/* mbcclient.c -
 */

#include "client/mbcclient.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


struct message
{
  gchar *buf;
  gsize len;
  gsize ofs;
};


/* mbc_client_new:
 */
MbcClient *mbc_client_new ( void )
{
  MbcClient *cli;
  cli = g_new0(MbcClient, 1);
  cli->msg_queue = g_queue_new();
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



static gboolean _client_send ( MbcClient *cli )
{
  struct message *msg;
  GIOStatus s;
  gsize w;
  GError *err = NULL;
  if (!(msg = g_queue_peek_head(cli->msg_queue))) DIE("??");
  s = g_io_channel_write_chars(cli->chan, msg->buf + msg->ofs, msg->len - msg->ofs, &w, &err);
  switch (s) {
  case G_IO_STATUS_NORMAL:
	printf("sent %d bytes\n", w);
	msg->ofs += w;
	if (msg->ofs == msg->len) {
	  printf("msg complete\n");
	  if (g_io_channel_flush(cli->chan, NULL) != G_IO_STATUS_NORMAL)
		DIE("flush failed");
	  g_queue_pop_head(cli->msg_queue);
	  /* [FIXME] free msg */
	}
	break;
  default:
	DIE("[TODO] s=%d", s);
  }
  if (g_queue_is_empty(cli->msg_queue))
	return FALSE;
  else
	return TRUE;
}


static gboolean _on_client_ready ( GIOChannel *chan,
								   GIOCondition cond,
								   gpointer data )
{
  MbcClient *cli = data;
  if (cond & G_IO_IN)
	{
	  DIE("[TODO] client ready (in)");
	}
  if (cond & G_IO_OUT)
	{
	  if (_client_send(cli)) {
		return TRUE;
	  } else {
		printf("removing watchout\n");
		cli->watchout = 0;
		return FALSE;
	  }
	}
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
  cli->watchout = 0;
  return 0;
}



/* mbc_client_send:
 */
void mbc_client_send ( MbcClient *cli,
					   const gchar *text )
{
  struct message *msg = g_new0(struct message, 1);
  msg->buf = g_strdup(text);
  msg->len = strlen(msg->buf);
  msg->ofs = 0;
  g_queue_push_head(cli->msg_queue, msg);
  if (cli->watchout == 0)
	{
	  cli->watchout = g_io_add_watch(cli->chan, G_IO_OUT, _on_client_ready, cli);
	}
}
