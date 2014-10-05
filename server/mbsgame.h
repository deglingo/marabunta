/* mbsgame.h -
 */

#ifndef _MBSGAME_H_
#define _MBSGAME_H_

#include "server/mbsbase.h"
#include "server/mbsgame-def.h"



typedef gpointer MbsPlayerID;

typedef void (* MbsGameTreeHandler) ( LptTree *tree,
                                      LObject *message,
                                      gpointer player_data );



/* MbsGame:
 */
struct _MbsGame
{
  MBS_GAME_INSTANCE_HEADER;

  GList *players;
  LptTree *tree;
  MbsGameTreeHandler tree_handler;
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



MbsGame *mbs_game_new ( MbsGameTreeHandler tree_handler );                        
MbsPlayerID mbs_game_add_player ( MbsGame *game,
                                  const gchar *name,
                                  gpointer data,
                                  GDestroyNotify destroy_data );
void mbs_game_start ( MbsGame *game );
void mbs_game_lpt_event ( MbsGame *game,
                          MbsPlayerID player,
                          LObject *event );



#endif
