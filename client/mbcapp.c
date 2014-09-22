/* mbcapp.c -
 */

#include "client/cliprivate.h"
#include "client/mbcapp.h"
#include "client/mbcapp.inl"



static void _on_client_ready ( MbcClient *client,
                               GIOCondition condition,
                               LStream *stream,
                               gpointer data );



/* mbc_app_init:
 */
static void mbc_app_init ( LObject *object )
{
  MbcApp *app = MBC_APP(object);
  app->client = mbc_client_new(_on_client_ready, app);
  app->msg_queue = g_queue_new();
  app->msg_offset = 0;
}



/* _on_client_ready:
 */
static void _on_client_ready ( MbcClient *client,
                               GIOCondition condition,
                               LStream *stream,
                               gpointer data )
{
  MbcApp *app = MBC_APP(data);
  if (condition & G_IO_IN)
    CL_DEBUG("[TODO] client input...");
  if (condition & G_IO_OUT)
    {
      LString *msg;
      CL_DEBUG("process write...");
      if ((msg = g_queue_peek_head(app->msg_queue)))
        {
          /* [FIXME] send everything in loop until EAGAIN ? */
          guint size = msg->len - app->msg_offset;
          gint64 w;
          GError *err = NULL;
          LStreamStatus s;
          s = l_stream_write(stream, msg->str + app->msg_offset, size, &w, &err);
          switch (s) {
          case L_STREAM_STATUS_OK:
            CL_DEBUG("sent %d bytes...", (gint) w);
            app->msg_offset += w;
            if (app->msg_offset >= msg->len) {
              l_object_unref(g_queue_pop_head(app->msg_queue));
              app->msg_offset = 0;
            }
            break;
          default:
            CL_DEBUG("[TODO] write status = %d", s);
          }
        }
      if (g_queue_is_empty(app->msg_queue))
        mbc_client_remove_watch(client, G_IO_OUT);
    }
}



/* _send:
 */
static void _send ( MbcApp *app,
                    LString *msg )
{
  g_queue_push_head(app->msg_queue, l_object_ref(msg));
  mbc_client_add_watch(app->client, G_IO_OUT);
}



/* mbc_app_connect:
 */
void mbc_app_connect ( MbcApp *app )
{
  CL_DEBUG("connecting to server...");
  mbc_client_connect(app->client, "localhost", 6666);
}



/* mbc_app_join_game:
 */
void mbc_app_join_game ( MbcApp *app )
{
  LString *msg = l_string_new("DATA");
  _send(app, msg);
  l_object_unref(msg);
}
