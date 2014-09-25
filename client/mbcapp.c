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
}



/* _on_client_ready:
 */
static void _on_client_ready ( MbcClient *client,
                               GIOCondition condition,
                               LStream *stream,
                               gpointer data )
{
  MbcApp *app = MBC_APP(data);
  GError *error = NULL;
  if (condition & G_IO_IN)
    CL_DEBUG("[TODO] client input...");
  if (condition & G_IO_OUT)
    {
      gboolean r;
      r = l_packer_send(app->packer, &error);
      if (error)
        CL_ERROR("[TODO] write error");
      if (r)
        mbc_client_remove_watch(client, G_IO_OUT);
    }
}



/* _send:
 */
static void _send ( MbcApp *app,
                    LObject *msg )
{
  l_packer_add(app->packer, msg);
  mbc_client_add_watch(app->client, G_IO_OUT);
}



/* mbc_app_connect:
 */
void mbc_app_connect ( MbcApp *app )
{
  CL_DEBUG("connecting to server...");
  mbc_client_connect(app->client, "localhost", 6666);
  app->packer = l_packer_new(app->client->stream);
  app->unpacker = l_unpacker_new(app->client->stream);
}



/* mbc_app_join_game:
 */
void mbc_app_join_game ( MbcApp *app )
{
  LString *msg = l_string_new("DATA", -1);
  _send(app, L_OBJECT(msg));
  l_object_unref(msg);
}
