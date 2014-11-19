/* mbcplayer.h -
 */

#ifndef _MBCPLAYER_H_
#define _MBCPLAYER_H_

#include "client/mbcbase.h"
#include "client/mbcplayer-def.h"

G_BEGIN_DECLS



/* MbcPlayer:
 */
struct _MbcPlayer
{
  MBC_PLAYER_INSTANCE_HEADER;
};



/* MbcPlayerClass:
 */
struct _MbcPlayerClass
{
  MBC_PLAYER_CLASS_HEADER;
};



MbObject *mbc_player_new ( MbObject *game,
                           MbObjectID id,
                           const gchar *name );



G_END_DECLS

#endif /* ifndef _MBCPLAYER_H_ */
