/* mbstate.h -
 */

#ifndef _MBSTATE_H_
#define _MBSTATE_H_

#include "common/mbbase.h"
#include "common/mbpoptype.h"
#include "common/mbroom.h"
#include "common/mbstate-def.h"

G_BEGIN_DECLS



#define MB_STATE_BLOCKS_SIZE 1024

typedef enum _MbStateType MbStateType;
typedef struct _MbStateBlock MbStateBlock;
/* server */
typedef struct _MbStateGameSetup MbStateGameSetup;
typedef struct _MbStateResourceSetup MbStateResourceSetup;
typedef struct _MbStateSectorSetup MbStateSectorSetup;
typedef struct _MbStateColonySetup MbStateColonySetup;
typedef struct _MbStatePrioritySetup MbStatePrioritySetup;
typedef struct _MbStateTaskSetup MbStateTaskSetup;
typedef struct _MbStateRoomSetup MbStateRoomSetup;
typedef struct _MbStateGameUpdate MbStateGameUpdate;
typedef struct _MbStateColonyUpdate MbStateColonyUpdate;
typedef struct _MbStatePriorityUpdate MbStatePriorityUpdate;
typedef struct _MbStateTaskUpdate MbStateTaskUpdate;
/* client */
typedef struct _MbStateRequestPriorityValue MbStateRequestPriorityValue;
/* inner blocks */
typedef struct _MbStatePlayer MbStatePlayer;
typedef struct _MbStateStock MbStateStock;

typedef void (* MbStateHandler) ( MbState *state,
                                  gpointer data );



/* MbStateType:
 */
enum _MbStateType
  {
    /* server */
    MB_STATE_GAME_SETUP,
    MB_STATE_RESOURCE_SETUP,
    MB_STATE_SECTOR_SETUP,
    MB_STATE_COLONY_SETUP,
    MB_STATE_PRIORITY_SETUP,
    MB_STATE_TASK_SETUP,
    MB_STATE_ROOM_SETUP,
    MB_STATE_GAME_UPDATE,
    MB_STATE_COLONY_UPDATE,
    MB_STATE_PRIORITY_UPDATE,
    MB_STATE_TASK_UPDATE,
    /* client */
    MB_STATE_REQUEST_PRIORITY_VALUE,
    MB_STATE_COUNT,
  };



/* MbStateBlock:
 */
struct _MbStateBlock
{
  MbStateType type;
};



/* MbStatePlayer:
 */
struct _MbStatePlayer
{
  MbObjectID id;
  gchar name[MB_PLAYER_MAX_NAME+1];
};



/* MbStateGameSetup:
 */
struct _MbStateGameSetup
{
  MbStateBlock block;
  guint n_players;
  MbStatePlayer players[MB_GAME_MAX_PLAYERS];
  MbObjectID world_id;
  guint world_width;
  guint world_height;
};



/* MbStateResourceSetup:
 */
struct _MbStateResourceSetup
{
  MbStateBlock block;
  MbObjectID resource_id;
  gchar name[MB_RESOURCE_MAX_NAME+1];
  MbResourceFlags flags;
};



/* MbStateSectorSetup:
 */
struct _MbStateSectorSetup
{
  MbStateBlock block;
  MbObjectID sector_id;
  guint x;
  guint y;
};



/* MbStateColonySetup:
 */
struct _MbStateColonySetup
{
  MbStateBlock block;
  MbObjectID colony_id;
  MbObjectID sector_id;
  MbObjectID owner_id;
};



/* MbStatePrioritySetup:
 */
struct _MbStatePrioritySetup
{
  MbStateBlock block;
  MbObjectID priority_id;
  MbPriorityValue value;
};



/* MbStateTaskSetup:
 */
struct _MbStateTaskSetup
{
  MbStateBlock block;
  MbObjectID task_id;
  MbObjectID priority_id;
  MbObjectID colony_id;
  MbObjectID parent_id;
  MbObjectID resource_id;
  gchar name[MB_TASK_MAX_NAME+1];
  gboolean isgroup;
};



/* MbStateRoomSetup;
 */
struct _MbStateRoomSetup
{
  MbStateBlock block;
  MbObjectID room_id;
  MbObjectID colony_id;
  MbRoomType type;
};



/* MbStateGameUpdate:
 */
struct _MbStateGameUpdate
{
  MbStateBlock block;
  guint frame_count;
};



/* MbStateStock:
 */
struct _MbStateStock
{
  /* not a block */
  MbObjectID resource_id;
  gint64 qtty;
};



/* MbStateColonyUpdate:
 */
struct _MbStateColonyUpdate
{
  MbStateBlock block;
  MbObjectID colony_id;
  gint64 pop[MB_POP_TYPE_COUNT];
  gint stock_size;
  MbStateStock stock[MB_RESOURCE_MAX_TYPES];
};



/* MbStatePriorityUpdate:
 */
struct _MbStatePriorityUpdate
{
  MbStateBlock block;
  MbObjectID priority_id;
  MbPriorityValue value;
};



/* MbStateTaskUpdate:
 */
struct _MbStateTaskUpdate
{
  MbStateBlock block;
  MbObjectID task_id;
  gint64 workers;
};



/* MbStateRequestPriorityValue:
 */
struct _MbStateRequestPriorityValue
{
  MbStateBlock block;
  MbObjectID priority_id;
  MbPriorityValue value;
};



/* MbState:
 */
struct _MbState
{
  MB_STATE_INSTANCE_HEADER;

  gchar blocks[MB_STATE_BLOCKS_SIZE];
  guint blocks_count;
};



/* MbStateClass:
 */
struct _MbStateClass
{
  MB_STATE_CLASS_HEADER;
};



MbState *mb_state_new ( void );
gpointer mb_state_next ( MbState *state,
                              MbStateType type );
MbStateBlock *mb_state_read ( MbState *state,
                              guint *offset );



G_END_DECLS

#endif /* ifndef _MBSTATE_H_ */
