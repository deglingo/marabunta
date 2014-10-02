/* mbsgame.h -
 */

#ifndef _MBSGAME_H_
#define _MBSGAME_H_

#include "server/mbsbase.h"
#include "server/mbsgame-def.h"

/* [FIXME] MbsApp */
struct _MainData;



/* MbsGame:
 */
struct _MbsGame
{
  MBS_GAME_INSTANCE_HEADER;

  struct _MainData *app;
  LptTree *tree;
  GHashTable *lpt_clients; /* map < guint clid, LptClient * > */
  GHashTable *lpt_rclients;
  LptNode *n_sim_time;
  GTimer *timer;
  gdouble fps;
};



/* MbsGameClass:
 */
struct _MbsGameClass
{
  MBS_GAME_CLASS_HEADER;

  LptNSpec *nspec_dir;
  LptNSpec *nspec_uint;
};



MbsGame *mbs_game_new ( struct _MainData *app );
void mbs_game_add_player ( MbsGame *game,
                           guint id,
                           const gchar *name );
void mbs_game_start ( MbsGame *game );
void mbs_game_lpt_event ( MbsGame *game,
                          guint clid,
                          LObject *event );



#endif
