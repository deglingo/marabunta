/* mbgame.h -
 */

#ifndef _MBGAME_H_
#define _MBGAME_H_

#include "common/mbobject.h"
#include "common/mbgame-def.h"

G_BEGIN_DECLS



/* MbGame:
 */
struct _MbGame
{
  MB_GAME_INSTANCE_HEADER;

  GHashTable *object_map;
  GList *players;
  MbObject *world;
};



/* MbGameClass:
 */
struct _MbGameClass
{
  MB_GAME_CLASS_HEADER;
};



#define MB_GAME_WORLD(g) (MB_GAME(g)->world)

void mb_game_register_object ( MbGame *game,
                               MbObject *object );
MbObject *mb_game_lookup_object ( MbGame *game,
                                  MbObjectID id );
MbObject *mb_game_add_player ( MbGame *game,
                               MbObject *player );
void mb_game_set_world ( MbGame *game,
                         MbObject *world );



G_END_DECLS

#endif /* ifndef _MBGAME_H_ */
