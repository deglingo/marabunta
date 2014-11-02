/* mbccolonyproxy.c -
 */

#include "client/mbccolonyproxy.h"
#include "client/mbccolonyproxy.inl"



/* Signals:
 */
enum
  {
    SIG_POP_NOTIFY,
    SIG_COUNT,
  };

static LSignalID signals[SIG_COUNT];



/* mbc_colony_proxy_class_init:
 */
static void mbc_colony_proxy_class_init ( LObjectClass *cls )
{
  signals[SIG_POP_NOTIFY] =
    l_signal_new(cls,
                 "pop_notify");
}



/* mbc_colony_proxy_new:
 */
MbcColonyProxy *mbc_colony_proxy_new ( gint owner )
{
  MbcColonyProxy *cp;
  cp = MBC_COLONY_PROXY(l_object_new(MBC_CLASS_COLONY_PROXY, NULL));
  cp->owner = owner;
  return cp;
}



/* mbc_colony_proxy_set_pop:
 */
void mbc_colony_proxy_set_pop ( MbcColonyProxy *proxy,
                                gint64 *pop )
{
  gint tp;
  for (tp = 0; tp < MB_POP_TYPE_COUNT; tp++)
    {
      proxy->pop[tp] = pop[tp];
    }
  l_signal_emit(L_OBJECT(proxy), signals[SIG_POP_NOTIFY], 0);
}
