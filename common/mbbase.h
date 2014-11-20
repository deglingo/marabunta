/* mbbase.h -
 */

#ifndef _MBBASE_H_
#define _MBBASE_H_

#include <los.h>



typedef guint MbObjectID;



/* MbRoomType:
 */
typedef enum _MbRoomType
{
  MB_ROOM_TYPE_ROYAL_CHAMBER,
  MB_ROOM_TYPE_FARM,
  MB_ROOM_TYPE_MINE,
  MB_ROOM_TYPE_COUNT,
}
  MbRoomType;



/* [fixme] */
#define MB_GAME_MAX_PLAYERS 4
#define MB_PLAYER_MAX_NAME 16
#define MB_TASK_MAX_NAME 16
#define MB_RESOURCE_MAX_NAME 16

/* [fixme] */
typedef gint MbPriorityValue;

gchar *mb_task_path_next ( const gchar **path,
                           gchar *dest );

/* maximum chars of mb_count_print() result */
#define MB_COUNT_CHARS 7

gchar *mb_count_print ( gint64 count,
                        gchar *dest );


#endif
