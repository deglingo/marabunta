/* mbpoptree.h -
 */

#ifndef _MBPOPTREE_H_
#define _MBPOPTREE_H_

#include "common/mbbase.h"



typedef enum _MbPopType MbPopType;
typedef struct _MbPopUnit MbPopUnit;
typedef struct _MbPopTree MbPopTree;



/* MbPopType:
 */
enum _MbPopType
  {
    MB_POP_EGG     = 0x00,
    MB_POP_LARVAE  = 0x01,
    MB_POP_ADULT   = 0x02,

    MB_POP_QUEEN   = 0x04,
    MB_POP_WORKER  = 0x05,
    MB_POP_SOLDIER = 0x06,
  };

#define MB_POP_MATURITY_MASK 0x03
#define MB_POP_CAST_MASK     0x0c

#define MB_POP_MATURITY(tp) ((tp) & MB_POP_MATURITY_MASK)
#define MB_POP_CAST(tp) ((tp) & MB_POP_CAST_MASK)



/* MbPopUnit:
 */
struct _MbPopUnit
{
  MbPopType type;
  guint birthdate;
  guint64 count;
};



/* MbPopTree:
 */
struct _MbPopTree
{
  MbPopUnit *root;
};



MbPopTree *mb_pop_tree_new ( void );



#endif
