/* mbcgame.h -
 */

#ifndef _MBCGAME_H_
#define _MBCGAME_H_

#include "client/mbcbase.h"
#include "client/mbcgame-def.h"

G_BEGIN_DECLS



/* MbcGame:
 */
struct _MbcGame
{
  MBC_GAME_INSTANCE_HEADER;

  MbStateHandler handler;
  gpointer handler_data;
  GDestroyNotify destroy_data;
};



/* MbcGameClass:
 */
struct _MbcGameClass
{
  MBC_GAME_CLASS_HEADER;
};


MbObject *mbc_game_new ( MbStateHandler handler,
                         gpointer handler_data,
                         GDestroyNotify destroy_data );
void mbc_game_update_state ( MbcGame *game,
                             MbState *state );
void mbc_game_send_state ( MbcGame *game,
                           MbState *state );



G_END_DECLS

#endif /* ifndef _MBCGAME_H_ */
