/* mbstate.h -
 */

#ifndef _MBSTATE_H_
#define _MBSTATE_H_

#include "common/mbbase.h"
#include "common/mbstate-def.h"

G_BEGIN_DECLS



#define MB_STATE_MAX_BLOCKS 256

typedef enum _MbStateType MbStateType;
typedef union _MbStateValue MbStateValue;
typedef struct _MbStateBlock MbStateBlock;



/* MbStateType:
 */
enum _MbStateType
  {
    MB_STATE_RESET,
    MB_STATE_WORLD_SIZE,
  };



/* MbStateValue:
 */
union _MbStateValue
{
  gint v_int;
};



/* MbStateBlock:
 */
struct _MbStateBlock
{
  MbStateType type;
  MbStateValue v0;
  MbStateValue v1;
};



/* MbState:
 */
struct _MbState
{
  MB_STATE_INSTANCE_HEADER;

  MbStateBlock blocks[MB_STATE_MAX_BLOCKS];
  guint n_blocks;
};



/* MbStateClass:
 */
struct _MbStateClass
{
  MB_STATE_CLASS_HEADER;
};



MbState *mb_state_new ( void );
MbStateBlock *mb_state_next ( MbState *state,
                              MbStateType type );



G_END_DECLS

#endif /* ifndef _MBSTATE_H_ */
