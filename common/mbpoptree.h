/* mbpoptree.h -
 */

#ifndef _MBPOPTREE_H_
#define _MBPOPTREE_H_

#include "common/mbbase.h"

/* [FIXME] */
struct _MbsTask;



typedef enum _MbPopType MbPopType;
typedef enum _MbMaturity MbMaturity;
typedef enum _MbCast MbCast;
typedef struct _MbPopUnit MbPopUnit;
typedef struct _MbPopTree MbPopTree;

typedef void (* MbPopTreeTraverseFunc) ( MbPopUnit *unit,
                                         gpointer data );



/* MbPopType:
 */
enum _MbPopType
  {
    MB_POP_EGG = 0,
    MB_POP_LARVAE_QUEEN,
    MB_POP_LARVAE_WORKER,
    MB_POP_LARVAE_SOLDIER,
    MB_POP_ADULT_QUEEN,
    MB_POP_ADULT_WORKER,
    MB_POP_ADULT_SOLDIER,
    MB_POP_TYPE_COUNT,
  };



/* MbMaturity:
 */
enum _MbMaturity
  {
    MB_MATURITY_EGG,
    MB_MATURITY_LARVAE,
    MB_MATURITY_ADULT,
  };



/* MbCast:
 */
enum _MbCast
  {
    MB_CAST_EGG, /* ?? */
    MB_CAST_QUEEN,
    MB_CAST_WORKER,
    MB_CAST_SOLDIER,
  };



extern const MbMaturity MB_MATURITY_TABLE[];
extern const MbCast MB_CAST_TABLE[];


#define MB_POP_MATURITY(tp) (MB_MATURITY_TABLE[tp])
#define MB_POP_CAST(tp)     (MB_CAST_TABLE[tp])



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
