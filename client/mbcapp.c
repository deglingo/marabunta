/* mbcapp.c -
 */

#include "client/cliprivate.h"
#include "client/mbcapp.h"
#include "client/mbcapp.inl"

#include <sys/resource.h>



/* /\* Signals: */
/*  *\/ */
/* enum */
/*   { */
/*     SIG_GAME_STARTED, */
/*     SIG_COUNT, */
/*   }; */

/* static LSignalID signals[SIG_COUNT] = { 0, }; */



static gint run ( MbApp *app );



/* mbc_app_class_init:
 */
static void mbc_app_class_init ( LObjectClass *cls )
{
  MB_APP_CLASS(cls)->run = run;
  /* [FIXME] */
  struct rlimit limit;
  limit.rlim_cur = RLIM_INFINITY;
  limit.rlim_max = RLIM_INFINITY;
  if (setrlimit(RLIMIT_CORE, &limit) < 0)
    {
      CL_DEBUG("ERROR: setrlimit() failed: %s", strerror(errno));
    }
  /* signals[SIG_GAME_STARTED] = */
  /*   l_signal_new(cls, */
  /*                "game_started"); */
}



/* mbc_app_init:
 */
static void mbc_app_init ( LObject *object )
{
}



/* mbc_app_setup_solo_game:
 */
void mbc_app_setup_solo_game ( MbcApp *app )
{
  app->game = mbs_game_new();
  app->player = mbs_player_new(app->game, "Player1");
  mb_game_add_player(MB_GAME(app->game), app->player);
}



/* run:
 */
static gint run ( MbApp *app )
{
  GMainLoop *loop;
  loop = g_main_loop_new(NULL, FALSE);
  g_main_loop_run(loop);
  return 0;
}
