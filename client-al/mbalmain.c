/* mbalmain.c -
 */

#include "client-al/mbalbase.h"
#include "client-al/mbalapp.h"



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



static void _on_game_started ( MbalApp *app,
                               gpointer data )
{
  CL_DEBUG("GAME STARTED!");
}



/* main:
 */
int main ( int argc,
           char **argv )
{
  MbalApp *app;
  AltkDisplay *display;
  GSource *al_source;
  AltkWidget *dlg;
  al_init();
  altk_init();
  app = mbal_app_new();
  l_signal_connect(L_OBJECT(app), "game_started", (LSignalHandler) _on_game_started, NULL, NULL);
  display = altk_display_new();
  dlg = _create_dialog(display);
  al_source = al_source_new();
  g_source_attach(al_source, NULL);
  /* al_register_event_source(((AlSource *) al_source)->queue, */
  /*                          al_get_display_event_source(display)); */
  mbc_app_connect(MBC_APP(app));
  mbc_app_join_game(MBC_APP(app));
  altk_widget_show_all(dlg);
  altk_display_open(display);
  altk_main();
  return 0;
}
