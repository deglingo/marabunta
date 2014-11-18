/* mbplayer.h -
 */

#ifndef _MBPLAYER_H_
#define _MBPLAYER_H_

#include "common/mbobject.h"
#include "common/mbplayer-def.h"

G_BEGIN_DECLS



/* MbPlayer:
 */
struct _MbPlayer
{
  MB_PLAYER_INSTANCE_HEADER;

  gchar *name;
};



/* MbPlayerClass:
 */
struct _MbPlayerClass
{
  MB_PLAYER_CLASS_HEADER;
};



MbObject *mb_player_new ( MbObject *game,
                          guint id,
                          const gchar *name );



G_END_DECLS

#endif /* ifndef _MBPLAYER_H_ */
