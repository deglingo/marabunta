/* mbgame.h -
 */

#ifndef _MBGAME_H_
#define _MBGAME_H_

#include "mbbase.h"
#include "mbgame-def.h"

G_BEGIN_DECLS



/* MbRoomTypeInfo:
 */
typedef struct _MbRoomTypeInfo
{
  MbRoomType type;
  gchar *nick;
  gchar *name;
  gdouble x;
  gdouble y;
  gdouble width;
  gdouble height;
  gchar *work_task;
}
  MbRoomTypeInfo;



/* MbGame:
 */
struct _MbGame
{
  MB_GAME_INSTANCE_HEADER;

  GTimer *timer;
  gdouble next_frame;
  guint frame_offset;
  guint sim_time;
  struct _MbWorld *world;
  /* room types */
  GPtrArray *room_types; /* list < MbRoomTypeInfo > */
  GHashTable *room_types_map; /* map < nick, MbRoomTypeInfo > */
};



/* MbGameClass:
 */
struct _MbGameClass
{
  MB_GAME_CLASS_HEADER;
};



MbGame *mb_game_new ( void );
void mb_game_setup ( MbGame *game );
void mb_game_start ( MbGame *game );
MbRoomType mb_game_register_room_type ( MbGame *game,
                                        const gchar *nick,
                                        const gchar *name,
                                        gdouble x,
                                        gdouble y,
                                        gdouble width,
                                        gdouble height,
                                        const gchar *work_task );
MbRoomType mb_game_lookup_room_type ( MbGame *game,
                                      const gchar *nick );
const MbRoomTypeInfo *mb_game_room_type_info ( MbGame *game,
                                               MbRoomType type );
gint mb_game_max_room_type ( MbGame *game );
void mb_game_request_build_room ( MbGame *game,
                                  struct _MbColony *colony,
                                  MbRoomType type );



G_END_DECLS

#endif /* ifndef _MBGAME_H_ */
