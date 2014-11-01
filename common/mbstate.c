/* mbstate.c -
 */

#include "common/private.h"
#include "common/mbstate.h"
#include "common/mbstate.inl"

#include <string.h>



/* mb_state_new:
 */
MbState *mb_state_new ( void )
{
  return MB_STATE_NEW(NULL);
}



/* mb_state_next:
 */
MbStateBlock *mb_state_next ( MbState *state,
                              MbStateType type )
{
  MbStateBlock *block;
  ASSERT(state->n_blocks < MB_STATE_MAX_BLOCKS);
  block = &state->blocks[state->n_blocks++];
  /* [fixme] debug only ? */
  memset(block, 0, sizeof(MbStateBlock));
  block->type = type;
  return block;
}
