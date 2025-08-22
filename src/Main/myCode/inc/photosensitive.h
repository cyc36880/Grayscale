#ifndef __PHOTOSENSITIVE_H__
#define __PHOTOSENSITIVE_H__


#ifdef __cplusplus
extern "C" {
#endif

#include "../myMain.h"


extern uint8_t photosensitive_val[REGISTER_NUM];

void photosensitive_init(void);
void photosensitive_identify(void);

#ifdef __cplusplus
}
#endif


#endif /* __PHOTOSENSITIVE_H__ */

