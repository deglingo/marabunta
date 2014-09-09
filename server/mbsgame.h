/* mbsgame.h -
 */

#ifndef _MBSGAME_H_
#define _MBSGAME_H_

#include "server/mbsbase.h"



typedef struct _MbsGame MbsGame;



/* MbsGame:
 */
struct _MbsGame
{
  LptTree *tree;
};



MbsGame *mbs_game_new ( void );
void mbs_game_add_player ( MbsGame *game,
                           guint id,
                           const gchar *name );



#endif
