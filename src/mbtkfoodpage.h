/* mbtkfoodpage.h -
 */

#ifndef _MBTKFOODPAGE_H_
#define _MBTKFOODPAGE_H_

#include "mbtkinfopage.h"
#include "mbtkfoodpage-def.h"

G_BEGIN_DECLS



/* MbtkFoodPage:
 */
struct _MbtkFoodPage
{
  MBTK_FOOD_PAGE_INSTANCE_HEADER;
};



/* MbtkFoodPageClass:
 */
struct _MbtkFoodPageClass
{
  MBTK_FOOD_PAGE_CLASS_HEADER;
};



AltkWidget *mbtk_food_page_new ( void );



G_END_DECLS

#endif /* ifndef _MBTKFOODPAGE_H_ */
