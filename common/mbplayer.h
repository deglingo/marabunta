/* mbplayer.h -
 */

#ifndef _MBPLAYER_H_
#define _MBPLAYER_H_

#include "common/mbobject.h"
#include "common/mbstate.h"
#include "common/mbplayer-def.h"

G_BEGIN_DECLS



typedef void (* MbPlayerHandler) ( MbPlayer *player,
                                   MbState *state,
                                   gpointer data );



/* MbPlayer:
 */
struct _MbPlayer
{
  MB_PLAYER_INSTANCE_HEADER;

  LString *name;
  MbPlayerHandler handler;
  gpointer handler_data;
  GDestroyNotify destroy_data;
};



/* MbPlayerClass:
 */
struct _MbPlayerClass
{
  MB_PLAYER_CLASS_HEADER;
};



void mb_player_handle_state ( MbPlayer *player,
                              MbState *state );



G_END_DECLS

#endif /* ifndef _MBPLAYER_H_ */
