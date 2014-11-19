/* mbstate.c -
 */

#include "common/private.h"
#include "common/mbstate.h"
#include "common/mbstate.inl"

#include <string.h>



static const guint MB_STATE_BLOCK_TYPE_SIZE[MB_STATE_COUNT] =
  {
    [MB_STATE_RESET] = sizeof(MbStateReset),
    [MB_STATE_FRAME] = sizeof(MbStateFrame),
    [MB_STATE_SECTOR] = sizeof(MbStateSector),
    [MB_STATE_COLONY] = sizeof(MbStateColony),
    [MB_STATE_NEW_PRIORITY] = sizeof(MbStateNewPriority),
    [MB_STATE_NEW_TASK] = sizeof(MbStateNewTask),
    [MB_STATE_NEW_ROOM] = sizeof(MbStateNewRoom),
    [MB_STATE_NEW_RESOURCE] = sizeof(MbStateNewResource),
    [MB_STATE_POP] = sizeof(MbStatePop),
    [MB_STATE_TASK] = sizeof(MbStateTask),
    [MB_STATE_PRIORITY] = sizeof(MbStatePriority),

    [MB_STATE_GAME_SETUP] = sizeof(MbStateGameSetup),
    [MB_STATE_SECTOR_SETUP] = sizeof(MbStateSectorSetup),
    [MB_STATE_COLONY_SETUP] = sizeof(MbStateColonySetup),
    [MB_STATE_GAME_UPDATE] = sizeof(MbStateGameUpdate),
    [MB_STATE_COLONY_UPDATE] = sizeof(MbStateColonyUpdate),
  };



/* mb_state_new:
 */
MbState *mb_state_new ( void )
{
  return MB_STATE_NEW(NULL);
}



/* mb_state_next:
 */
gpointer mb_state_next ( MbState *state,
                              MbStateType type )
{
  MbStateBlock *block;
  guint block_size;
  ASSERT(type >= 0 && type < MB_STATE_COUNT);
  block_size = MB_STATE_BLOCK_TYPE_SIZE[type];
  ASSERT(block_size > sizeof(MbStateBlock));
  block = ((gpointer) state->blocks) + state->blocks_count;
  state->blocks_count += block_size;
  ASSERT(state->blocks_count <= MB_STATE_BLOCKS_SIZE);
  /* [fixme] debug only ? */
  memset(block, 0, block_size);
  block->type = type;
  return block;
}



/* mb_state_read:
 */
MbStateBlock *mb_state_read ( MbState *state,
                              guint *offset )
{
  if ((*offset) < state->blocks_count)
    {
      MbStateBlock *block = ((gpointer) state->blocks) + (*offset);
      *offset += MB_STATE_BLOCK_TYPE_SIZE[block->type];
      return block;
    }
  else
    {
      return NULL;
    }
}
