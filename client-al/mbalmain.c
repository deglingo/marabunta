/* mbalmain.c -
 */

#include "client-al/alprivate.h"
#include "client-al/mbalapp.h"



/* extern const char DIALOG_MAIN[]; */



/* /\* AlSource: */
/*  *\/ */
/* typedef struct _AlSource AlSource; */

/* struct _AlSource */
/* { */
/*   GSource g_source; */
/*   ALLEGRO_EVENT_QUEUE *queue; */
/*   ALLEGRO_EVENT event; */
/*   unsigned int pending : 1; */
/* }; */



/* static gboolean _al_source_prepare ( GSource *src, */
/*                                      gint *timeout ) */
/* { */
/*   AlSource *al = (AlSource *) src; */
/*   *timeout = 0; */
/*   if (al->pending) */
/*     return TRUE; */
/*   if (al_get_next_event(al->queue, &al->event)) { */
/*     al->pending = 1; */
/*     return TRUE; */
/*   } else { */
/*     return FALSE; */
/*   } */
/* } */



/* static gboolean _al_source_check ( GSource *src ) */
/* { */
/*   AlSource *al = (AlSource *) src; */
/*   return al->pending; */
/* } */



/* static gboolean _al_source_dispatch ( GSource *src, */
/*                                       GSourceFunc callback, */
/*                                       gpointer data ) */
/* { */
/*   AlSource *al = (AlSource *) src; */
/*   if (!al->pending) { */
/*     fprintf(stderr, "[fixme] no event to dispatch!\n"); */
/*     return TRUE; */
/*   } */
/*   fprintf(stderr, "[todo] dispatch event: %d\n", al->event.type); */
/*   /\* [FIXME] *\/ */
/*   if (al->event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) */
/*     exit(0); */
/*   al->pending = 0; */
/*   /\* return FALSE if the source should be removed *\/  */
/*   return TRUE; */
/* } */



/* static void _al_source_finalize ( GSource *src ) */
/* { */
/* } */



/* static GSource *al_source_new ( void ) */
/* { */
/*   static GSourceFuncs funcs = { */
/*     _al_source_prepare, */
/*     _al_source_check, */
/*     _al_source_dispatch, */
/*     _al_source_finalize, */
/*   }; */
/*   AlSource *source; */
/*   source = (AlSource *) g_source_new(&funcs, sizeof(AlSource)); */
/*   source->queue = al_create_event_queue(); */
/*   return (GSource *) source; */
/* } */



/* /\* _create_dialog: */
/*  *\/ */
/* AltkWidget *_create_dialog ( AltkDisplay *display ) */
/* { */
/*   GError *error = NULL; */
/*   LObject *dlg; */
/*   /\* [FIXME] *\/ */
/*   ALTK_CLASS_DIALOG; */
/*   ALTK_CLASS_LABEL; */
/*   /\* go *\/ */
/*   if (!(dlg = altk_builder_quick_parse(DIALOG_MAIN, -1, "dialog", &error))) */
/*     CL_ERROR("AltkBuilder error"); */
/*   ASSERT(ALTK_IS_DIALOG(dlg)); */
/*   altk_dialog_set_display(ALTK_DIALOG(dlg), display); */
/*   return ALTK_WIDGET(dlg); */
/* } */



/* static void _on_game_time_set ( LptNode *node, */
/*                                 AltkWidget *label ) */
/* { */
/*   LObject *value = lpt_node_get_value(node); */
/*   gchar *text; */
/*   ASSERT(L_IS_INT(value)); */
/*   text = g_strdup_printf("GAME TIME: %d", L_INT_VALUE(value)); */
/*   altk_label_set_text(ALTK_LABEL(label), text); */
/*   g_free(text); */
/* } */



/* static void _bind_node_label_handler ( LptNode *node, */
/*                                        AltkWidget *label ) */
/* { */
/*   CL_DEBUG("[TODO] bind_node_label..."); */
/* } */



/* static void bind_node_label ( LptNode *node, */
/*                               AltkWidget *widget ) */
/* { */
/*   l_signal_connect(L_OBJECT(node), */
/*                    "value_set", */
/*                    (LSignalHandler) _bind_node_label_handler, */
/*                    widget, */
/*                    NULL); */
/* } */



/* static void _on_game_started ( MbalApp *app, */
/*                                gpointer data ) */
/* { */
/*   LptTree *tree = MBC_APP(app)->tree; */
/*   AltkWidget *dlg = data; */
/*   bind_node_label(lpt_tree_get_node(tree, "/game/sim-time"), */
/*                   altk_widget_find(dlg, "game_time")); */
/*   /\* AltkWidget *game_time_label = altk_widget_find(dlg, "game-time"); *\/ */
/*   /\* LptNode *game_time_node = lpt_tree_get_node(tree, "/game/sim-time"); *\/ */
/*   /\* l_signal_connect(L_OBJECT(game_time_node), *\/ */
/*   /\*                  "value_set", *\/ */
/*   /\*                  (LSignalHandler) _on_game_time_set, *\/ */
/*   /\*                  game_time_label, *\/ */
/*   /\*                  NULL); *\/ */
/*   CL_DEBUG("GAME STARTED!"); */
/* } */



/* main:
 */
int main ( gint argc,
           gchar **argv )
{
  MbalApp *app;
  gint r;
  app = mbal_app_new();
  r = mbc_app_run(MBC_APP(app), argc, argv);
  l_object_unref(app);
  return r;
  /* AltkDisplay *display; */
  /* GSource *al_source; */
  /* AltkWidget *dlg; */
  /* al_init(); */
  /* altk_init(); */
  /* app = mbal_app_new(); */
  /* display = altk_display_new(); */
  /* dlg = _create_dialog(display); */
  /* al_source = al_source_new(); */
  /* g_source_attach(al_source, NULL); */
  /* /\* al_register_event_source(((AlSource *) al_source)->queue, *\/ */
  /* /\*                          al_get_display_event_source(display)); *\/ */
  /* l_signal_connect(L_OBJECT(app), "game_started", (LSignalHandler) _on_game_started, dlg, NULL); */
  /* mbc_app_connect(MBC_APP(app)); */
  /* mbc_app_join_game(MBC_APP(app)); */
  /* altk_widget_show_all(dlg); */
  /* altk_display_open(display); */
  /* altk_main(); */
  /* return 0; */
}
