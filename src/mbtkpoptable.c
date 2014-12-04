/* mbtkpoptable.c -
 */

#include "mbtkpoptable.h"
#include "mbtkpoptable.inl"



/* mbtk_pop_table_new:
 */
AltkWidget *mbtk_pop_table_new ( void )
{
  return ALTK_WIDGET(l_object_new(MBTK_CLASS_POP_TABLE, NULL));
}
