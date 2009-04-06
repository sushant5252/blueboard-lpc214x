#ifndef __MESSAGE_QUEUE_
#define __MESSAGE_QUEUE_

#include "type.h"

U8 message_queue_init();
U8 get_message_queue(U8 **ppmsg);
U8 post_imessage_queue(U8 *msg);

#endif // __MESSAGE_QUEUE_



