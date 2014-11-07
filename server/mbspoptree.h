/* mbspoptree.h -
 */

#ifndef _MBSPOPTREE_H_
#define _MBSPOPTREE_H_

#include "server/mbsbase.h"

/* [FIXME] */
struct _MbsTask;



typedef struct _MbsPopUnit MbsPopUnit;
typedef struct _MbsPopTree MbsPopTree;

typedef void (* MbsPopTreeTraverseFunc) ( MbsPopUnit *unit,
                                          gpointer data );



/* MbsPopUnit:
 */
struct _MbsPopUnit
{
  MbPopType type;
  guint birthdate;
  gint64 count;
  /* [FIXME] */
  struct _MbsTask *task;
};



/* MbsPopTree:
 */
struct _MbsPopTree
{
  gint64 pop[MB_POP_TYPE_COUNT];
  /* [fixme] */
  /* MbsPopUnit *root; */
  GList *units;
};



MbsPopTree *mbs_pop_tree_new ( void );
void mbs_pop_tree_add ( MbsPopTree *tree,
                        MbPopType type,
                        guint birthdate,
                        gint64 count );
void mbs_pop_tree_traverse ( MbsPopTree *tree,
                             MbsPopTreeTraverseFunc func,
                             gpointer data );
void mbs_pop_tree_update ( MbsPopTree *tree,
                           MbsPopTree *adj );



#endif
