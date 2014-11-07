/* mbpoptype.h -
 */

#ifndef _MBPOPTYPE_H_
#define _MBPOPTYPE_H_

#include "common/mbbase.h"



typedef enum _MbPopType MbPopType;
typedef enum _MbMaturity MbMaturity;
typedef enum _MbCast MbCast;



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



MbPopType mb_pop_type ( MbCast pop_cast,
                        MbMaturity pop_maturity );



#endif
