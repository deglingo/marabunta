/* mbsgame.h -
 */

#ifndef _MBSGAME_H_
#define _MBSGAME_H_

#include "server/mbsbase.h"
#include "server/mbsgame-def.h"



/* MbsGame:
 */
struct _MbsGame
{
  MBS_GAME_INSTANCE_HEADER;

  LptTree *tree;
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



MbsGame *mbs_game_new ( void );
void mbs_game_add_player ( MbsGame *game,
                           guint id,
                           const gchar *name );
void mbs_game_start ( MbsGame *game );



#endif
