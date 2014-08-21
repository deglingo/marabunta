/* mbalmain.c -
 */

#include <stdio.h>
#include <allegro5/allegro.h>

#include "client/mbcclient.h"



/* AlSource:
 */
typedef struct _AlSource AlSource;

struct _AlSource
{
  GSource g_source;
  ALLEGRO_EVENT_QUEUE *al_queue;
};



static gboolean _al_source_prepare ( GSource *src,
                                     gint *timeout )
{
  *timeout = -1;
  return FALSE;
}



static gboolean _al_source_check ( GSource *src )
{
  return FALSE;
}



static gboolean _al_source_dispatch ( GSource *src,
                                      GSourceFunc callback,
                                      gpointer data )
{
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
  /* source->al_queue = al_create_event_queue(); */
  return (GSource *) source;
}



/* main:
 */
int main ( int argc,
           char **argv )
{
  ALLEGRO_DISPLAY *display;
  GSource *al_source;
  GMainLoop *loop;
  MbcClient *cli;
  al_init();
  al_source = al_source_new();
  g_source_attach(al_source, NULL);
  if (!(display = al_create_display(640, 480))) {
    fprintf(stderr, "al_create_display() failed\n");
    exit(1);
  }
  cli = mbc_client_new();
  printf("marabunta-al: hello!\n");
  mbc_client_connect(cli, "localhost", 6666);
  mbc_client_send(cli, "hola!");
  loop = g_main_loop_new(NULL, FALSE);
  g_main_loop_run(loop);
  return 0;
}
