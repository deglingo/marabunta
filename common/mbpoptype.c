/* mbpoptype.c -
 */

#include "common/private.h"
#include "common/mbpoptype.h"



/* MB_MATURITY_TABLE:
 */
const MbMaturity MB_MATURITY_TABLE[MB_POP_TYPE_COUNT] =
  {
    [MB_POP_EGG] = MB_MATURITY_EGG,
    [MB_POP_LARVAE_QUEEN] = MB_MATURITY_LARVAE,
    [MB_POP_LARVAE_WORKER] = MB_MATURITY_LARVAE,
    [MB_POP_LARVAE_SOLDIER] = MB_MATURITY_LARVAE,
    [MB_POP_ADULT_QUEEN] = MB_MATURITY_ADULT,
    [MB_POP_ADULT_WORKER] = MB_MATURITY_ADULT,
    [MB_POP_ADULT_SOLDIER] = MB_MATURITY_ADULT,
  };



/* MB_CAST_TABLE:
 */
const MbCast MB_CAST_TABLE[MB_POP_TYPE_COUNT] =
  {
    [MB_POP_EGG] = MB_CAST_EGG, /* ?? */
    [MB_POP_LARVAE_QUEEN] = MB_CAST_QUEEN,
    [MB_POP_LARVAE_WORKER] = MB_CAST_WORKER,
    [MB_POP_LARVAE_SOLDIER] = MB_CAST_SOLDIER,
    [MB_POP_ADULT_QUEEN] = MB_CAST_QUEEN,
    [MB_POP_ADULT_WORKER] = MB_CAST_WORKER,
    [MB_POP_ADULT_SOLDIER] = MB_CAST_SOLDIER,
  };



/* mb_pop_type:
 */
MbPopType mb_pop_type ( MbCast pop_cast,
                        MbMaturity pop_maturity )
{
  gint tp;
  /* [fixme] maybe a switch is more efficient ? */
  for (tp = 0; tp < MB_POP_TYPE_COUNT; tp++) {
    if (MB_POP_CAST(tp) == pop_cast && MB_POP_MATURITY(tp) == pop_maturity)
      return tp;
  }
  CL_ERROR("invalid pop type: cast=%d, maturity=%d", pop_cast, pop_maturity);
  return -1;
}



/* mb_pop_total:
 */
gint64 mb_pop_total ( gint64 *pop,
                      MbPopFlags pop_flags )
{
  gint tp;
  gint64 total = 0;
  for (tp = 0; pop_flags; tp++, pop_flags >>= 1)
    {
      ASSERT(tp < MB_POP_TYPE_COUNT);
      if (pop_flags & 1)
        total += pop[tp];
    }
  return total;
}
