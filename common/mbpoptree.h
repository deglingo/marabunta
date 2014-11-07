/* mbpoptree.h -
 */

#ifndef _MBPOPTREE_H_
#define _MBPOPTREE_H_

#include "common/mbbase.h"
#include "common/mbpoptype.h"

/* [FIXME] */
struct _MbsTask;



typedef struct _MbPopUnit MbPopUnit;
typedef struct _MbPopTree MbPopTree;

typedef void (* MbPopTreeTraverseFunc) ( MbPopUnit *unit,
                                         gpointer data );



/* MbPopUnit:
 */
struct _MbPopUnit
{
  MbPopType type;
  guint birthdate;
  gint64 count;
  /* [FIXME] */
  struct _MbsTask *task;
};



/* MbPopTree:
 */
struct _MbPopTree
{
  gint64 pop[MB_POP_TYPE_COUNT];
  /* [fixme] */
  /* MbPopUnit *root; */
  GList *units;
};



MbPopTree *mb_pop_tree_new ( void );
void mb_pop_tree_add ( MbPopTree *tree,
                       MbPopType type,
                       guint birthdate,
                       gint64 count );
void mb_pop_tree_traverse ( MbPopTree *tree,
                            MbPopTreeTraverseFunc func,
                            gpointer data );
void mb_pop_tree_update ( MbPopTree *tree,
                          MbPopTree *adj );



#endif
