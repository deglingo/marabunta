/* mbworld.h -
 */

#ifndef _MBWORLD_H_
#define _MBWORLD_H_

#include "common/mbbase.h"
#include "common/mbworld-def.h"



/* MbWorld:
 */
struct _MbWorld
{
  MB_WORLD_INSTANCE_HEADER;

  LptNode *n_root;
  LptNode *n_width;
  LptNode *n_height;
};



/* MbWorldClass:
 */
struct _MbWorldClass
{
  MB_WORLD_CLASS_HEADER;
};



MbWorld *mb_world_new ( void );
void mb_world_create ( MbWorld *world,
                       LptNode *node,
                       guint width,
                       guint height );
void mb_world_bind ( MbWorld *world,
                     LptNode *root );



#endif
