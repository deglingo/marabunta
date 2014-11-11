/* mbspoptree.h -
 */

#ifndef _MBSPOPTREE_H_
#define _MBSPOPTREE_H_

#include "server/mbsbase.h"
#include "server/mbstask.h"



/* debug */
/* #define MBS_DEBUG_POP_TREE */
/* #define MBS_DEBUG_POP_TREE_HARD */

#ifdef MBS_DEBUG_POP_TREE
#define MBS_POP_TREE_CHECK(tree) mbs_pop_tree_check((tree))
#else
#define MBS_POP_TREE_CHECK(tree) do {} while (0)
#endif

#ifdef MBS_DEBUG_POP_TREE_HARD
#define MBS_POP_TREE_CHECK_HARD(tree) mbs_pop_tree_check((tree))
#else
#define MBS_POP_TREE_CHECK_HARD(tree) do {} while (0)
#endif



typedef struct _MbsPopUnit MbsPopUnit;
typedef struct _MbsPopTree MbsPopTree;

typedef void (* MbsPopTreeTraverseFunc) ( MbsPopUnit *unit,
                                          gpointer data );



/* MbsPopUnit:
 */
struct _MbsPopUnit
{
  /* note: unit_new() does not clear the memory, be sure to update it
     if you add values here */
  MbPopType type;
  guint birthdate;
  gint64 count;
  MbsTask *task;
};



MbsPopTree *mbs_pop_tree_new ( void );
void mbs_pop_tree_check ( MbsPopTree *tree );
void mbs_pop_tree_clear ( MbsPopTree *tree );
void mbs_pop_tree_add ( MbsPopTree *tree,
                        MbPopType type,
                        guint birthdate,
                        gint64 count );
void mbs_pop_tree_traverse ( MbsPopTree *tree,
                             MbsPopTreeTraverseFunc func,
                             gpointer data );
void mbs_pop_tree_update ( MbsPopTree *tree,
                           MbsPopTree *adj );
void mbs_pop_tree_get_pop ( MbsPopTree *tree,
                            gint64 *pop );

void mbs_pop_unit_affect_task ( MbsPopUnit *unit,
                                MbsTask *task );



#endif
