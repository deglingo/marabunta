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
  guint offset = 0;
  while ((block = mb_state_read(state, &offset)))
    {
      switch (block->type)
        {
        case MB_STATE_RESET:
          {
            MbStateReset *st_reset = (MbStateReset *) block;
            mbc_world_proxy_set_size(app->game_proxy->world,
                                     st_reset->world_width,
                                     st_reset->world_height);
          }
          break;
        case MB_STATE_COLONY:
          {
            MbStateColony *st_col = (MbStateColony *) block;
            mbc_sector_proxy_create_colony(app->game_proxy->world->sectors[st_col->y][st_col->x],
                                           st_col->owner);
          }
          break;
        case MB_STATE_FRAME:
          {
            MbStateFrame *st_frame = (MbStateFrame *) block;
            mbc_game_proxy_set_sim_time(app->game_proxy, st_frame->sim_time);
          }
          break;
        case MB_STATE_POP:
          {
            MbStatePop *st_pop = (MbStatePop *) block;
            MbcColonyProxy *col = app->game_proxy->world->sectors[st_pop->y][st_pop->x]->colony;
            mbc_colony_proxy_set_pop(col, st_pop->pop);
          }
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

