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
  gint sock;
  GPollFD pollfd;
  LStream *stream;
  GSource *source;
};



typedef struct _RWSource
{
  GSource g_source;
}
  RWSource;



static gboolean rw_source_prepare ( GSource *source,
                                    gint *timeout );
static gboolean rw_source_dispatch ( GSource *source,
                                     GSourceFunc callback,
                                     gpointer data );



static GSourceFuncs RW_SOURCE_FUNCS =
  {
    rw_source_prepare,
    NULL,
    rw_source_dispatch,
    NULL,
  };



GSource *rw_source_new ( void )
{
  RWSource *source = (RWSource *) g_source_new(&RW_SOURCE_FUNCS, sizeof(RWSource));
  return (GSource *) source;
}



static gboolean rw_source_prepare ( GSource *source,
                                    gint *timeout )
{
  *timeout = -1;
  return TRUE;
}



static gboolean rw_source_dispatch ( GSource *source,
                                     GSourceFunc callback,
                                     gpointer data )
{
  ASSERT(callback);
  return callback(data);
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
  s->client_counter = 0;
  s->client_map = g_hash_table_new(NULL, NULL);
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
  client->clid = ++(server->client_counter);
  if (!(client->stream = l_file_fdopen(client->sock, "rw", NULL)))
    CL_ERROR("[TODO] could not create client stream");
  g_hash_table_insert(server->client_map, GUINT_TO_POINTER(client->clid), client);
  /* create and send the event */
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



static gboolean _on_client_watch ( Client *client )
{
  MbsServerEvent event;
  event.type = MBS_SERVER_EVENT_READY;
  event.ready.clid = client->clid;
  event.ready.stream = client->stream;
  event.ready.condition = client->pollfd.revents;
  client->server->handler(&event, client->server->handler_data);
  return G_SOURCE_CONTINUE;
}



/* mbs_server_add_watch:
 */
void mbs_server_add_watch ( MbsServer *server,
                            guint clid,
                            GIOCondition condition )
{
  Client *client;
  if (!(client = g_hash_table_lookup(server->client_map, GUINT_TO_POINTER(clid))))
    CL_ERROR("unknown client: %d", clid);
  if (client->source)
    {
      /* [FIXME] glib sources seems to say that it is not necessary to
         remove a pollfd before modifying it */
      g_source_remove_poll(client->source, &client->pollfd);
      client->pollfd.events |= condition;
    }
  else
    {
      client->source = rw_source_new();
      g_source_set_callback(client->source,
                            (GSourceFunc) _on_client_watch,
                            client, NULL);
      client->pollfd.fd = client->sock;
      client->pollfd.events = condition;
      g_source_attach(client->source, NULL);
    }
  g_source_add_poll(client->source, &client->pollfd);
}
