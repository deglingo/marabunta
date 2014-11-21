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
void mbs_game_setup ( MbsGame *game );
void mbs_game_start ( MbsGame *game );
void mbs_game_handle_request ( MbsGame *game,
                               MbObject *player,
                               MbState *state );
void mbs_game_register_resource ( MbsGame *game,
                                  const gchar *name,
                                  MbResourceFlags flags );



#endif
