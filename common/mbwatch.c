/* mbwatch.c -
 */

#include "common/mbwatch.h"



struct _MBWatch
{
  GSource *source;
  GPollFD pollfd;
  MBWatchFunc func;
  gpointer data;
  GDestroyNotify destroy_data;
};



typedef struct _WatchSource
{
  GSource g_source;
  MBWatch *watch;
}
  WatchSource;



static gboolean watch_source_prepare ( GSource *source,
                                    gint *timeout );
static gboolean watch_source_dispatch ( GSource *source,
                                     GSourceFunc callback,
                                     gpointer data );



static GSourceFuncs WATCH_SOURCE_FUNCS =
  {
    watch_source_prepare,
    NULL,
    watch_source_dispatch,
    NULL,
  };



static gboolean watch_source_prepare ( GSource *source,
                                    gint *timeout )
{
  *timeout = -1;
  return TRUE;
}



static gboolean watch_source_dispatch ( GSource *source,
                                        GSourceFunc callback,
                                        gpointer data )
{
  WatchSource *ws = (WatchSource *) source;
  ws->watch->func(ws->watch->pollfd.revents, ws->watch->data);
  return G_SOURCE_CONTINUE;
}



MBWatch *mb_watch_new ( gint fd,
                        MBWatchFunc func,
                        gpointer data,
                        GDestroyNotify destroy )
{
  MBWatch *watch;
  watch = g_new0(MBWatch, 1);
  watch->func = func;
  watch->data = data;
  watch->destroy_data = destroy;
  watch->pollfd.fd = fd;
  return watch;
}



void mb_watch_add_condition ( MBWatch *watch,
                              GIOCondition condition )
{
  /* [FIXME] glib sources seems to say that we can modify a GPollFD
     condition on the fly, but i'm not sure... */
  watch->pollfd.events |= condition;
  /* just in case, no need to install a source if condition == 0 */
  if (!watch->pollfd.events)
    return;
  /* install the source if necessary */
  if (!watch->source)
    {
      WatchSource *ws = (WatchSource *) g_source_new(&WATCH_SOURCE_FUNCS, sizeof(WatchSource));
      ws->watch = watch;
      watch->source = (GSource *) ws;
      g_source_add_poll(watch->source, &watch->pollfd);
      g_source_attach(watch->source, NULL);
    }
}



/* mb_watch_remove_condition:
 */
void mb_watch_remove_condition ( MBWatch *watch,
                                 GIOCondition condition )
{
  watch->pollfd.events &= ~condition;
  if (watch->source && !watch->pollfd.events)
    {
      g_source_remove(g_source_get_id(watch->source));
      watch->source = NULL;
    }
}
