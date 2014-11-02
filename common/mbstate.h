/* mbstate.h -
 */

#ifndef _MBSTATE_H_
#define _MBSTATE_H_

#include "common/mbbase.h"
#include "common/mbstate-def.h"

G_BEGIN_DECLS



#define MB_STATE_BLOCKS_SIZE 256

typedef enum _MbStateType MbStateType;
typedef struct _MbStateBlock MbStateBlock;
typedef struct _MbStateReset MbStateReset;
typedef struct _MbStateFrame MbStateFrame;
typedef struct _MbStateColony MbStateColony;



/* MbStateType:
 */
enum _MbStateType
  {
    MB_STATE_RESET,
    MB_STATE_FRAME,
    MB_STATE_COLONY,
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



/* MbStateColony:
 */
struct _MbStateColony
{
  MbStateBlock block;
  guint x;
  guint y;
  gint owner;
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
MbStateBlock *mb_state_next ( MbState *state,
                              MbStateType type );
MbStateBlock *mb_state_read ( MbState *state,
                              guint *offset );



G_END_DECLS

#endif /* ifndef _MBSTATE_H_ */
