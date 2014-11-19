/* mbsplayer.h -
 */

#ifndef _MBSPLAYER_H_
#define _MBSPLAYER_H_

#include "server/mbsbase.h"
#include "server/mbsplayer-def.h"

G_BEGIN_DECLS



/* MbsPlayer:
 */
struct _MbsPlayer
{
  MBS_PLAYER_INSTANCE_HEADER;
};



/* MbsPlayerClass:
 */
struct _MbsPlayerClass
{
  MBS_PLAYER_CLASS_HEADER;
};



MbObject *mbs_player_new ( MbObject *game,
                           const gchar *name,
                           MbPlayerHandler handler,
                           gpointer data,
                           GDestroyNotify destroy_data );



G_END_DECLS

#endif /* ifndef _MBSPLAYER_H_ */
