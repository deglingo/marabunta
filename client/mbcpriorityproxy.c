/* mbcpriorityproxy.c -
 */

#include "client/cliprivate.h"
#include "client/mbcpriorityproxy.h"
#include "client/mbcgameproxy.h"
#include "client/mbcpriorityproxy.inl"



/* Properties:
 */
enum
  {
    PROP_0,
    PROP_VALUE,
    PROP_COUNT,
  };

static LParamSpec *pspecs[PROP_COUNT];



/* mbc_priority_proxy_class_init:
 */
static void mbc_priority_proxy_class_init ( LObjectClass *cls )
{
  pspecs[PROP_VALUE] = l_param_spec_int("value", 0);
  l_object_class_install_properties(cls, PROP_COUNT, pspecs);
}



/* mbc_priority_proxy_new:
 */
MbcProxy *mbc_priority_proxy_new ( MbcProxy *game,
                                   guint id,
                                   MbPriorityValue value )
{
  MbcPriorityProxy *prio;
  ASSERT(MBC_IS_GAME_PROXY(game));
  prio = MBC_PRIORITY_PROXY(mbc_game_proxy_create_object(MBC_GAME_PROXY(game),
                                                         MBC_CLASS_PRIORITY_PROXY,
                                                         id));
  prio->value = value;
  return MBC_PROXY(prio);
}



/* mbc_priority_prxy_set_value:
 */
void mbc_priority_proxy_set_value ( MbcPriorityProxy *priority,
                                    MbPriorityValue value )
{
  priority->value = value;
  l_object_notify(L_OBJECT(priority), pspecs[PROP_VALUE]);
}



/* mbc_priority_proxy_request_set_value:
 */
void mbc_priority_proxy_request_set_value ( MbcPriorityProxy *proxy,
                                            MbPriorityValue value )
{
  mbc_game_proxy_request_priority_value(MBC_GAME_PROXY(MBC_PROXY(proxy)->game),
                                        MBC_PROXY(proxy),
                                        value);
}



/* mbc_priority_proxy_request_inc_value:
 */
void mbc_priority_proxy_request_inc_value ( MbcPriorityProxy *proxy )
{
  mbc_priority_proxy_request_set_value(proxy, proxy->value + 1);
}



/* mbc_priority_proxy_request_dec_value:
 */
void mbc_priority_proxy_request_dec_value ( MbcPriorityProxy *proxy )
{
  mbc_priority_proxy_request_set_value(proxy, proxy->value - 1);
}
