/* mbtkpoptable.h -
 */

#ifndef _MBTKPOPTABLE_H_
#define _MBTKPOPTABLE_H_

#include "client-al/mbtkbase.h"
#include "client-al/mbtkpoptable-def.h"

G_BEGIN_DECLS



/* MbtkPopTable:
 */
struct _MbtkPopTable
{
  MBTK_POP_TABLE_INSTANCE_HEADER;

  gpointer private;
};



/* MbtkPopTableClass:
 */
struct _MbtkPopTableClass
{
  MBTK_POP_TABLE_CLASS_HEADER;
};



AltkWidget *mbtk_pop_table_new ( void );
void mbtk_pop_table_set_colony ( MbtkPopTable *table,
                                 MbObject *colony );



G_END_DECLS

#endif /* ifndef _MBTKPOPTABLE_H_ */
