/* mbtkfoodpage.c -
 */

#include "mbtkfoodpage.h"
#include "mbtkfoodpage.inl"



/* mbtk_food_page_new:
 */
AltkWidget *mbtk_food_page_new ( void )
{
  return ALTK_WIDGET(l_object_new_give(MBTK_CLASS_FOOD_PAGE,
                                       "title", l_string_new("Food"),
                                       NULL));
}
