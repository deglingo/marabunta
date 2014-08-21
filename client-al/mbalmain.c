/* mbalmain.c -
 */

#include <stdio.h>
#include <allegro5/allegro.h>

#include "client/mbcclient.h"



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



/* main:
 */
int main ( int argc,
           char **argv )
{
  ALLEGRO_DISPLAY *display;
  GSource *al_source;
  GMainLoop *loop;
  MbcClient *cli;
  GSourceFuncs al_source_funcs = {
    _al_source_prepare,
    _al_source_check,
    _al_source_dispatch,
    _al_source_finalize,
  };
  al_init();
  al_source = g_source_new(&al_source_funcs, sizeof(GSource));
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
