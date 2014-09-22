/* mbalmain.c -
 */

#include "client-al/mbalbase.h"
#include "client-al/mbalapp.h"



/* [FIXME] globals */
static GQueue *msg_queue = NULL;
static guint msg_offset = 0;



/* AlSource:
 */
typedef struct _AlSource AlSource;

struct _AlSource
{
  GSource g_source;
  ALLEGRO_EVENT_QUEUE *queue;
  ALLEGRO_EVENT event;
  unsigned int pending : 1;
};



static gboolean _al_source_prepare ( GSource *src,
                                     gint *timeout )
{
  AlSource *al = (AlSource *) src;
  *timeout = 0;
  if (al->pending)
    return TRUE;
  if (al_get_next_event(al->queue, &al->event)) {
    al->pending = 1;
    return TRUE;
  } else {
    return FALSE;
  }
}



static gboolean _al_source_check ( GSource *src )
{
  AlSource *al = (AlSource *) src;
  return al->pending;
}



static gboolean _al_source_dispatch ( GSource *src,
                                      GSourceFunc callback,
                                      gpointer data )
{
  AlSource *al = (AlSource *) src;
  if (!al->pending) {
    fprintf(stderr, "[fixme] no event to dispatch!\n");
    return TRUE;
  }
  fprintf(stderr, "[todo] dispatch event: %d\n", al->event.type);
  /* [FIXME] */
  if (al->event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
    exit(0);
  al->pending = 0;
  /* return FALSE if the source should be removed */ 
  return TRUE;
}



static void _al_source_finalize ( GSource *src )
{
}



static GSource *al_source_new ( void )
{
  static GSourceFuncs funcs = {
    _al_source_prepare,
    _al_source_check,
    _al_source_dispatch,
    _al_source_finalize,
  };
  AlSource *source;
  source = (AlSource *) g_source_new(&funcs, sizeof(AlSource));
  source->queue = al_create_event_queue();
  return (GSource *) source;
}



/* _create_dialog:
 */
AltkWidget *_create_dialog ( AltkDisplay *display )
{
  AltkWidget *dlg, *lbl;
  dlg = altk_dialog_new(display);
  lbl = altk_label_new("Hello!");
  altk_container_add(ALTK_CONTAINER(dlg), lbl);
  return dlg;
}



/* _send:
 */
static void _send ( MbcClient *client,
                    LString *msg )
{
  g_queue_push_head(msg_queue, l_object_ref(msg));
  mbc_client_add_watch(client, G_IO_OUT);
}



static void _on_client_ready ( MbcClient *client,
                               GIOCondition condition,
                               LStream *stream,
                               gpointer data )
{
  if (condition & G_IO_IN)
    CL_DEBUG("[TODO] client input...");
  if (condition & G_IO_OUT)
    {
      LString *msg;
      CL_DEBUG("process write...");
      if ((msg = g_queue_peek_head(msg_queue)))
        {
          /* [FIXME] send everything in loop until EAGAIN ? */
          guint size = msg->len - msg_offset;
          gint64 w;
          GError *err = NULL;
          LStreamStatus s;
          s = l_stream_write(stream, msg->str + msg_offset, size, &w, &err);
          switch (s) {
          case L_STREAM_STATUS_OK:
            CL_DEBUG("sent %d bytes...", (gint) w);
            msg_offset += w;
            if (msg_offset >= msg->len) {
              l_object_unref(g_queue_pop_head(msg_queue));
              msg_offset = 0;
            }
            break;
          default:
            CL_DEBUG("[TODO] write status = %d", s);
          }
        }
      if (g_queue_is_empty(msg_queue))
        mbc_client_remove_watch(client, G_IO_OUT);
    }
}



/* main:
 */
int main ( int argc,
           char **argv )
{
  MbalApp *app;
  AltkDisplay *display;
  GSource *al_source;
  MbcClient *cli;
  AltkWidget *dlg;
  LString *msg;
  al_init();
  altk_init();
  app = mbal_app_new();
  msg_queue = g_queue_new();
  display = altk_display_new();
  dlg = _create_dialog(display);
  al_source = al_source_new();
  g_source_attach(al_source, NULL);
  /* al_register_event_source(((AlSource *) al_source)->queue, */
  /*                          al_get_display_event_source(display)); */
  cli = mbc_client_new(_on_client_ready, NULL);
  CL_DEBUG("connecting localhost:6666");
  mbc_client_connect(cli, "localhost", 6666);
  msg = l_string_new("DATA");
  _send(cli, msg);
  l_object_unref(msg);
  altk_widget_show_all(dlg);
  altk_display_open(display);
  altk_main();
  return 0;
}
