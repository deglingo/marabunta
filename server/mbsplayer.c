/* mbsplayer.c -
 */

#include "server/mbsplayer.h"
#include "server/mbsplayer.inl"



/* mbs_player_new:
 */
MbObject *mbs_player_new ( MbObject *game,
                           const gchar *name,
                           MbPlayerHandler handler,
                           gpointer handler_data,
                           GDestroyNotify destroy_data )
{
  MbPlayer *p = MB_PLAYER(l_object_new_give(MBS_CLASS_PLAYER,
                                            "game", l_object_ref(game),
                                            "name", l_string_new(name),
                                            NULL));
  /* [FIXME] handler property */
  p->handler = handler;
  p->handler_data = handler_data;
  p->destroy_data = destroy_data;
  return MB_OBJECT(p);
}
