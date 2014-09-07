/* mbsserver.c -
 */

#include "server/srvprivate.h"
#include "server/mbsserver.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>



/* Client:
 */
typedef struct _Client Client;

struct _Client
{
  MbsServer *server;
  guint clid;
  GIOChannel *chan;
  guint watchid;
  gchar *buffer;
  gsize buffer_size; /* total size of the buffer */
  gsize msg_size;    /* size used in the buffer */
};



/* mbs_server_new:
 */
MbsServer *mbs_server_new ( MbsServerHandler handler,
							gpointer handler_data )
{
  MbsServer *s;
  s = g_new0(MbsServer, 1);
  s->port = 6666;
  s->handler = handler;
  s->handler_data = handler_data;
  s->client_counter = 0;
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
	CL_ERROR("socket failed: %s", STRERROR);
  /* name the socket */
  name.sin_family = AF_INET;
  name.sin_port = htons(port);
  name.sin_addr.s_addr = htonl(INADDR_ANY);
  if (bind(sock, (struct sockaddr *) &name, sizeof(name)) < 0)
	CL_ERROR("bind failed: %s", STRERROR);
  return sock;
}



static void _client_process_read ( Client *client,
                                   gchar *buffer,
                                   gsize size )
{
  /* grow the client buffer if needed */
  gsize new_size = client->msg_size + size;
  if (new_size > client->buffer_size) {
    while (new_size > client->buffer_size)
      client->buffer_size = (client->buffer_size ? (client->buffer_size * 2) : 1024);
    client->buffer = g_realloc(client->buffer, client->buffer_size);
  }
  /* copy the data */
  memcpy(client->buffer + client->msg_size, buffer, size);
  client->msg_size = new_size;
  /* [TODO] process... */
  CL_DEBUG("processing %d bytes message...", client->msg_size);
}



static gboolean _on_client_ready ( GIOChannel *chan,
								   GIOCondition cond,
								   gpointer data )
{
  Client *cli = data;
  gchar buf[65536];
  gsize bytes_read;
  GError *err = NULL;
  GIOStatus r;
  /* printf("reading from client %d\n", cli->clid); */
  r = g_io_channel_read_chars(chan, buf, 65536, &bytes_read, &err);
  switch (r) {
  case G_IO_STATUS_NORMAL:
	CL_DEBUG("got %d bytes from client %d", bytes_read, cli->clid);
    _client_process_read(cli, buf, bytes_read);
	break;
  case G_IO_STATUS_EOF:
	CL_DEBUG("got EOF from client %d", cli->clid);
	/* [FIXME] shutdown, cleanup... */
	return FALSE;
	break;
  case G_IO_STATUS_ERROR:
	CL_ERROR("io read failed");
	break;
  case G_IO_STATUS_AGAIN:
	CL_ERROR("[TODO] EAGAIN");
	break;
  default:
	CL_ERROR("??");
  }
  return TRUE;
}



/* _on_accept:
 */
static gboolean _on_accept ( GIOChannel *chan,
							 GIOCondition cond,
							 gpointer data )
{
  MbsServer *server = data;
  struct sockaddr_in client_name;
  size_t size = sizeof(client_name);
  gint sock;
  Client *client;
  MbsServerEvent event;
  if ((sock = accept(server->listen_sock, (struct sockaddr *) &
client_name, &size)) < 0)
	CL_ERROR("accept failed: %s", STRERROR);
  client = g_new0(Client, 1);
  client->server = server;
  client->clid = ++(server->client_counter);
  client->chan = g_io_channel_unix_new(sock);
  if (g_io_channel_set_flags(client->chan, G_IO_FLAG_NONBLOCK, NULL) != G_IO_STATUS_NORMAL)
	CL_ERROR("set flags failed");
  g_io_channel_set_encoding(client->chan, NULL, NULL);
  client->watchid = g_io_add_watch(client->chan, G_IO_IN, _on_client_ready, client);
  event.type = MBS_SERVER_EVENT_ACCEPT;
  event.accept.clid = client->clid;
  CL_DEBUG("client %d connected: %s:%hd",
           client->clid,
           inet_ntoa(client_name.sin_addr),
           ntohs(client_name.sin_port));
  server->handler(&event, server->handler_data);
  return TRUE;
}



/* mbs_server_start:
 */
void mbs_server_start ( MbsServer *server )
{
  CL_DEBUG("starting server on port %d", server->port);
  server->listen_sock = make_socket(server->port);
  if (listen(server->listen_sock, 1) < 0)
	CL_ERROR("listen failed: %s", STRERROR);
  server->listen_chan = g_io_channel_unix_new(server->listen_sock);
  g_io_add_watch(server->listen_chan, G_IO_IN, _on_accept, server);
}
