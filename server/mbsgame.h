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

  gpointer private;
};



/* MbsGameClass:
 */
struct _MbsGameClass
{
  MBS_GAME_CLASS_HEADER;
};



MbObject *mbs_game_new ( void );



#endif
