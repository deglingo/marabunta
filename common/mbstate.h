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
typedef struct _MbStateReset MbStateReset;
typedef struct _MbStateFrame MbStateFrame;
typedef struct _MbStateSector MbStateSector;
typedef struct _MbStateColony MbStateColony;
typedef struct _MbStateNewPriority MbStateNewPriority;
typedef struct _MbStateNewTask MbStateNewTask;
typedef struct _MbStateNewRoom MbStateNewRoom;
typedef struct _MbStateNewResource MbStateNewResource;
typedef struct _MbStatePop MbStatePop;
typedef struct _MbStateTask MbStateTask;
typedef struct _MbStatePriority MbStatePriority;
/* new ones */
typedef struct _MbStateGameSetup MbStateGameSetup;
typedef struct _MbStateSectorSetup MbStateSectorSetup;
typedef struct _MbStateColonySetup MbStateColonySetup;
typedef struct _MbStateRoomSetup MbStateRoomSetup;
typedef struct _MbStateGameUpdate MbStateGameUpdate;
typedef struct _MbStateColonyUpdate MbStateColonyUpdate;
/* inner blocks */
typedef struct _MbStatePlayer MbStatePlayer;



/* MbStateType:
 */
enum _MbStateType
  {
    MB_STATE_RESET,
    MB_STATE_FRAME,
    MB_STATE_SECTOR,
    MB_STATE_COLONY,
    MB_STATE_NEW_TASK,
    MB_STATE_NEW_ROOM,
    MB_STATE_NEW_RESOURCE,
    MB_STATE_POP,
    MB_STATE_NEW_PRIORITY,
    MB_STATE_TASK,
    MB_STATE_PRIORITY,
    /* new ones */
    MB_STATE_GAME_SETUP,
    MB_STATE_SECTOR_SETUP,
    MB_STATE_COLONY_SETUP,
    MB_STATE_ROOM_SETUP,
    MB_STATE_GAME_UPDATE,
    MB_STATE_COLONY_UPDATE,
    MB_STATE_COUNT,
  };



/* MbStateBlock:
 */
struct _MbStateBlock
{
  MbStateType type;
};



/* MbStateReset:
 */
struct _MbStateReset
{
  MbStateBlock block;
  guint game_id;
  guint world_id;
  guint world_width;
  guint world_height;
};



/* MbStateFrame:
 */
struct _MbStateFrame
{
  MbStateBlock block;
  guint sim_time;
};



/* MbStateSector:
 */
struct _MbStateSector
{
  MbStateBlock block;
  guint sector_id;
  guint x;
  guint y;
};



/* MbStateColony:
 */
struct _MbStateColony
{
  MbStateBlock block;
  guint colony_id;
  guint sector_id;
  gint owner;
};



/* MbStateNewPriority:
 */
struct _MbStateNewPriority
{
  MbStateBlock block;
  guint priority_id;
  MbPriorityValue value;
};



/* MbStateNewTask:
 */
struct _MbStateNewTask
{
  MbStateBlock block;
  guint task_id;
  guint colony_id;
  guint parent_id;
  gboolean isgroup;
  gchar name[MBS_TASK_MAX_NAME+1];
  guint priority_id;
  gint64 workers;
};



/* MbStateNewRoom:
 */
struct _MbStateNewRoom
{
  MbStateBlock block;
  guint room_id;
  MbRoomType type;
  guint colony_id;
};



/* MbStateNewResource:
 */
struct _MbStateNewResource
{
  MbStateBlock block;
  guint resource_id;
  gchar name[MB_RESOURCE_MAX_NAME+1];
};



/* MbStatePop:
 */
struct _MbStatePop
{
  MbStateBlock block;
  guint colony_id;
  gint64 pop[MB_POP_TYPE_COUNT];
};



/* MbStateTask:
 */
struct _MbStateTask
{
  MbStateBlock block;
  guint task_id;
  gint64 workers;
};



/* MbStatePriority:
 */
struct _MbStatePriority
{
  MbStateBlock block;
  guint priority_id;
  MbPriorityValue value;
};



/* MbIStatePlayer:
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



/* MbStateColonyUpdate:
 */
struct _MbStateColonyUpdate
{
  MbStateBlock block;
  MbObjectID colony_id;
  gint64 pop[MB_POP_TYPE_COUNT];
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
