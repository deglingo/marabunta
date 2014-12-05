/* mbpoptree.h -
 */

#ifndef _MBPOPTREE_H_
#define _MBPOPTREE_H_

#include "mbbase.h"
#include "mbpoptype.h"



/* debug */
/* #define MB_DEBUG_POP_TREE */
/* #define MB_DEBUG_POP_TREE_HARD */

#ifdef MB_DEBUG_POP_TREE
#define MB_POP_TREE_CHECK(tree) mb_pop_tree_check((tree))
#else
#define MB_POP_TREE_CHECK(tree) do {} while (0)
#endif

#ifdef MB_DEBUG_POP_TREE_HARD
#define MB_POP_TREE_CHECK_HARD(tree) mb_pop_tree_check((tree))
#else
#define MB_POP_TREE_CHECK_HARD(tree) do {} while (0)
#endif



typedef struct _MbPopUnit MbPopUnit;
typedef struct _MbPopTree MbPopTree;

typedef void (* MbPopTreeTraverseFunc) ( MbPopUnit *unit,
                                         gpointer data );



/* MbPopUnit:
 */
struct _MbPopUnit
{
  /* note: unit_new() does not clear the memory, be sure to update it
     if you add values here */
  MbPopType type;
  guint birthdate;
  gint64 count;
  struct _MbTask *task;
};



MbPopTree *mb_pop_tree_new ( gint64 *pop );
void mb_pop_tree_check ( MbPopTree *tree );
void mb_pop_tree_clear ( MbPopTree *tree );
void mb_pop_tree_add ( MbPopTree *tree,
                       MbPopType type,
                       guint birthdate,
                       gint64 count );
void mb_pop_tree_traverse ( MbPopTree *tree,
                            MbPopTreeTraverseFunc func,
                            gpointer data );
void mb_pop_tree_update ( MbPopTree *tree,
                          MbPopTree *adj );
void mb_pop_tree_get_pop ( MbPopTree *tree,
                           gint64 *pop );

void mb_pop_unit_affect_task ( MbPopUnit *unit,
                               struct _MbTask *task );



#endif
