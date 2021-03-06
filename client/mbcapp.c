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



/* static void _player_handler ( MbPlayer *player, */
/*                               MbState *state, */
/*                               gpointer app ) */
/* { */
/*   mbc_game_update_state(MBC_GAME(MBC_APP(app)->game_proxy), state); */
/* } */



/* static void _solo_state_handler ( MbState *state, */
/*                                   gpointer data ) */
/* { */
/*   MbcApp *app = data; */
/*   mbs_game_handle_request(MBS_GAME(app->game), app->player, state); */
/* } */



/* mbc_app_setup_solo_game:
 */
void mbc_app_setup_solo_game ( MbcApp *app )
{
  /* setup the proxy */
  /* app->game_proxy = mbc_game_new(_solo_state_handler, app, NULL); */
  /* setup the real game */  
  app->game = mbs_game_new();
  app->player = mbs_player_new(app->game, "Player1", NULL /* _player_handler */, app, NULL);
  mb_game_add_player(MB_GAME(app->game), app->player);
  mbs_game_setup(MBS_GAME(app->game));
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
