/* mbgame.h -
 */

#ifndef _MBGAME_H_
#define _MBGAME_H_

#include "common/mbobject.h"
#include "common/mbgame-def.h"

G_BEGIN_DECLS



/* MbRoomTypeInfo:
 */
typedef struct _MbRoomTypeInfo
  {
    MbRoomType type;
    gint level;
    MbObject *technology;
  }
  MbRoomTypeInfo;



/* MbGame:
 */
struct _MbGame
{
  MB_GAME_INSTANCE_HEADER;

  GHashTable *object_map;
  GList *players;
  MbObject *world;
  guint frame_count;
  /* resources : map < gchar *name, MbObject *rsc > */
  GHashTable *resources;
  /* technologies : map < gchar *name, MbObject *tech > */
  GHashTable *technologies;
  /* room types */
  MbRoomTypeInfo *room_types[MB_ROOM_TYPE_COUNT][MB_ROOM_MAX_LEVEL];
};



/* MbGameClass:
 */
struct _MbGameClass
{
  MB_GAME_CLASS_HEADER;

  void (* add_player) ( MbGame *game,
                        MbObject *player );
};



#define MB_GAME_WORLD(g) (MB_GAME(g)->world)
#define MB_GAME_FRAME_COUNT(g) (MB_GAME(g)->frame_count)

void mb_game_started ( MbGame *game );
void mb_game_register_object ( MbGame *game,
                               MbObject *object );
MbObject *mb_game_lookup_object ( MbGame *game,
                                  MbObjectID id );
void mb_game_register_resource ( MbGame *game,
                                 MbObject *resource );
MbObject *mb_game_lookup_resource ( MbGame *game,
                                    const gchar *name );
void mb_game_register_technology ( MbGame *game,
                                   MbObject *technology );
MbObject *mb_game_lookup_technology ( MbGame *game,
                                      const gchar *name );
void mb_game_register_room_type ( MbGame *game,
                                  MbRoomType type,
                                  gint level,
                                  const gchar *tech );
void mb_game_add_player ( MbGame *game,
                          MbObject *player );
void mb_game_set_world ( MbGame *game,
                         MbObject *world );
void mb_game_set_frame_count ( MbGame *game,
                               guint frame_count );



G_END_DECLS

#endif /* ifndef _MBGAME_H_ */
