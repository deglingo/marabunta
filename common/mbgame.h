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
  guint frame_count;
  /* resources : map < gchar *name, MbObject *rsc > */
  GHashTable *resources;
  /* technologies : map < gchar *name, MbObject *tech > */
  GHashTable *technologies;
  /* room types */
  GPtrArray *room_types;
};



/* MbGameClass:
 */
struct _MbGameClass
{
  MB_GAME_CLASS_HEADER;

  void (* add_player) ( MbGame *game,
                        MbObject *player );

  void (* request_build_room) ( MbGame *game,
                                MbObject *player,
                                MbObject *colony,
                                MbRoomType type );
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
MbRoomType mb_game_next_room_type ( MbGame *game );
MbRoomType mb_game_last_room_type ( MbGame *game );
void mb_game_register_room_type ( MbGame *game,
                                  MbRoomType type,
                                  const gchar *nick,
                                  const gchar *name,
                                  const gchar *work_task );
const MbRoomTypeInfo *mb_game_get_room_type_info ( MbGame *game,
                                                   MbRoomType type );
MbRoomType mb_game_lookup_room_type ( MbGame *game,
                                      const gchar *nick );
void mb_game_add_player ( MbGame *game,
                          MbObject *player );
void mb_game_set_world ( MbGame *game,
                         MbObject *world );
void mb_game_set_frame_count ( MbGame *game,
                               guint frame_count );

/* user requests */
void mb_game_request_build_room ( MbGame *game,
                                  MbObject *player,
                                  MbObject *colony,
                                  MbRoomType type );



G_END_DECLS

#endif /* ifndef _MBGAME_H_ */
