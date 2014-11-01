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



/* _process_game_state:
 */
static void _process_game_state ( MbcApp *app,
                                  MbsPlayerID player,
                                  MbState *state )
{
  MbStateBlock *block;
  guint b;
  for (b = 0, block = state->blocks; b < state->n_blocks; b++, block++)
    {
      switch (block->type)
        {
        case MB_STATE_RESET:
          break;
        case MB_STATE_WORLD_SIZE:
          mbc_world_proxy_set_size(app->game_proxy->world, block->v0.v_int, block->v1.v_int);
          break;
        case MB_STATE_SIM_TIME:
          mbc_game_proxy_set_sim_time(app->game_proxy, block->v0.v_int);
          break;
        default:
          CL_ERROR("[TODO] block type: %d", block->type);
        }
    }
}



/* player_message_handler:
 */
static void player_message_handler ( MbsPlayerID player,
                                     MbMessage *message,
                                     gpointer data )
{
  MbcApp *app = data;
  CL_DEBUG("message(player=%d, key=%d", player, message->key);
  switch (message->key)
    {
    case MB_MESSAGE_KEY_GAME_SETUP:
      CL_DEBUG("game_setup");
      mbc_game_proxy_reset(app->game_proxy);
      _process_game_state(MBC_APP(data), player, MB_STATE(message->arg));
      mbc_game_proxy_started(app->game_proxy);
      break;
    case MB_MESSAGE_KEY_GAME_STATE:
      CL_DEBUG("game_state");
      ASSERT(MB_IS_STATE(message->arg));
      _process_game_state(MBC_APP(data), player, MB_STATE(message->arg));
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

