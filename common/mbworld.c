/* mbworld.c -
 */

#include "common/private.h"
#include "common/mbworld.h"
#include "common/mbworld.inl"



/* mb_world_new:
 */
MbWorld *mb_world_new ( void )
{
  return MB_WORLD(l_object_new(MB_CLASS_WORLD, NULL));
}



/* mb_world_create:
 */
void mb_world_create ( MbWorld *world,
                       LptNode *root,
                       guint width,
                       guint height )
{
  LptNSpec *ns_dir, *ns_uint;
  l_trash_push();
  /* nspecs */
  ns_dir = L_TRASH_OBJECT
    (lpt_nspec_dir_new("DIR"));
  ns_uint = L_TRASH_OBJECT
    (lpt_nspec_int_new("UINT", 0, G_MAXINT, 0));
  /* root */
  world->n_root = l_object_ref(root);
  /* width */
  world->n_width = L_TRASH_OBJECT
    (lpt_node_new(ns_uint));
  lpt_node_set_value(world->n_width, L_TRASH_OBJECT(l_int_new(width)));
  lpt_node_add(world->n_root, world->n_width,
               L_TRASH_OBJECT(l_string_new("width")));
  /* height */
  world->n_height = L_TRASH_OBJECT
    (lpt_node_new(ns_uint));
  lpt_node_set_value(world->n_height, L_TRASH_OBJECT(l_int_new(height)));
  lpt_node_add(world->n_root, world->n_height,
               L_TRASH_OBJECT(l_string_new("height")));
  l_trash_pop();
}



/* mb_world_bind:
 */
void mb_world_bind ( MbWorld *world,
                     LptNode *root )
{
  ASSERT(root);
  l_trash_push();
  world->n_root = l_object_ref(root);
  world->n_width = 
    lpt_node_get_child(world->n_root,
                       L_TRASH_OBJECT(l_string_new("width")));
  ASSERT(world->n_width);
  world->n_height = 
    lpt_node_get_child(world->n_root,
                       L_TRASH_OBJECT(l_string_new("height")));
  ASSERT(world->n_height);
  l_trash_pop();
}
