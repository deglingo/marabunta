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



/* process_game_update:
 */
static void _process_game_update ( MbcApp *app,
                                   MbsPlayerID *player,
                                   MbMessage *message )
{
  mbc_game_proxy_process_update(app->game_proxy, message);
}



/* player_message_handler:
 */
static void player_message_handler ( MbsPlayerID player,
                                     MbMessage *message,
                                     gpointer data )
{
  switch (message->key)
    {
    case MB_MESSAGE_KEY_GAME_UPDATE:
      _process_game_update(MBC_APP(data), player, message);
      break;
    default:
      CL_DEBUG("[TODO] msg key %d", message->key);
    }
}



/* mbc_app_setup_solo_game:
 */
void mbc_app_setup_solo_game ( MbcApp *app )
{
  app->game = mbs_game_new();
  app->player = mbs_game_add_player(app->game, "Player1", player_message_handler, app, NULL);
  app->game_proxy = mbc_game_proxy_new();
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

