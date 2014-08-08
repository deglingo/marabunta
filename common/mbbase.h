/* mbbase.h -
 */

#ifndef _MBBASE_H_
#define _MBBASE_H_

#include <stdio.h>



/* TRACE:
 */
#define TRACE(msg...) do {						\
	fprintf(stdout, "%s:%s:%d: ",				\
			G_LOG_DOMAIN, __FILE__, __LINE__);	\
	fprintf(stdout, msg);						\
	fprintf(stdout, "\n");						\
  } while (0)



#endif
