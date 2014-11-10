/* mbspoptree.h -
 */

#ifndef _MBSPOPTREE_H_
#define _MBSPOPTREE_H_

#include "server/mbsbase.h"
#include "server/mbstask.h"



typedef struct _MbsPopUnit MbsPopUnit;
typedef struct _MbsPopTree MbsPopTree;

typedef void (* MbsPopTreeTraverseFunc) ( MbsPopUnit *unit,
                                          gpointer data );



/* MbsPopUnit:
 */
struct _MbsPopUnit
{
  /* key */
  MbPopType type;
  guint birthdate;
  /* value */
  gint64 count;
  MbsTask *task;
  /* tree data */
  MbsPopUnit *left;
  MbsPopUnit *right;
  guint red : 1;
};



/* MbsPopTree:
 */
struct _MbsPopTree
{
  gint64 pop[MB_POP_TYPE_COUNT];
  MbsPopUnit *root;
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

void mbs_pop_unit_affect_task ( MbsPopUnit *unit,
                                MbsTask *task );



#endif
