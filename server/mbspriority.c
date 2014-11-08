/* mbspriority.c -
 */

#include "server/srvprivate.h"
#include "server/mbspriority.h"
#include "server/mbsgame.h"
#include "server/mbspriority.inl"



/* Properties:
 */
enum
  {
    PROP_0,
    PROP_VALUE,
    PROP_COUNT,
  };

static LParamSpec *pspecs[PROP_COUNT] = { NULL, };



/* mbs_priority_class_init:
 */
static void mbs_priority_class_init ( LObjectClass *cls )
{
  pspecs[PROP_VALUE] = l_param_spec_int("value", 0);
  l_object_class_install_properties(cls, PROP_COUNT, pspecs);
}



/* mbs_priority_new:
 */
MbsPriority *mbs_priority_new ( MbsGame *game,
                                MbPriorityValue value )
{
  MbsPriority *prio = MBS_PRIORITY_NEW(NULL);
  prio->game = game; /* [fixme] ref ? */
  prio->value = value;
  mbs_game_register_control(game, MBS_OBJECT(prio));
  return prio;
}



/* mbs_priority_set_value:
 */
void mbs_priority_set_value ( MbsPriority *priority,
                              MbPriorityValue value )
{
  priority->value = value;
  l_object_notify(L_OBJECT(priority), pspecs[PROP_VALUE]);
}



/* mbs_priority_get_score_factor:
 */
gint64 mbs_priority_get_score_factor ( MbsPriority *priority )
{
  return 10 - priority->value;
}
