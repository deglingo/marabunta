/* mbsserver.c -
 */

#include "server/srvprivate.h"
#include "server/mbsserver.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>



/* Client:
 */
typedef struct _Client Client;

struct _Client
{
  MbsServer *server;
  gint sock;
  MBWatch *watch;
  LStream *stream;
  /* user data */
  gpointer data;
  GDestroyNotify destroy_data;
};



/* mbs_client_set_data:
 */
void mbs_client_set_data ( MbsClientID client_,
                           gpointer data,
                           GDestroyNotify destroy )
{
  Client *client = client_;
  if (client->data && client->destroy_data)
    client->destroy_data(client->data);
  client->data = data;
  client->destroy_data = destroy;
}



/* mbs_client_get_data:
 */
gpointer mbs_client_get_data ( MbsClientID client )
{
  return ((Client *) client)->data;
}



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



static void _on_client_watch ( GIOCondition cond,
                               gpointer data )
{
  Client *client = (Client *) data;
  MbsServerEvent event;
  event.type = MBS_SERVER_EVENT_READY;
  event.ready.client = client;
  event.ready.condition = cond;
  client->server->handler(&event, client->server->handler_data);
  /* [FIXME] handle EOF */
  if ((cond & G_IO_IN) && l_stream_eof(client->stream)) {
    CL_DEBUG("[TODO] got EOF from client %p", client);
    mb_watch_remove_condition(client->watch, G_IO_IN | G_IO_OUT);
  }
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
  client->sock = sock;
  if (fcntl(client->sock, F_SETFL, O_NONBLOCK) < 0)
    CL_ERROR("SETFL(NONBLOCK) failed: %s", STRERROR);
  if (!(client->stream = l_file_fdopen(client->sock, "rw", NULL)))
    CL_ERROR("[TODO] could not create client stream");
  client->watch = mb_watch_new(client->sock, _on_client_watch, client, NULL);
  server->clients = g_list_append(server->clients, client);
  /* create and send the event */
  event.type = MBS_SERVER_EVENT_ACCEPT;
  event.accept.client = client;
  event.accept.stream = client->stream;
  CL_DEBUG("client %p connected: %s:%hd",
           client,
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



/* mbs_server_add_watch:
 */
void mbs_server_add_watch ( MbsServer *server,
                            MbsClientID client,
                            GIOCondition condition )
{
  ASSERT(g_list_find(server->clients, client));
  mb_watch_add_condition(((Client *) client)->watch, condition);
}



/* mbs_server_remove_watch:
 */
void mbs_server_remove_watch ( MbsServer *server,
                               MbsClientID client,
                               GIOCondition condition )
{
  ASSERT(g_list_find(server->clients, client));
  mb_watch_remove_condition(((Client *) client)->watch, condition);
}
