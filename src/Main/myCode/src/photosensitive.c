#include "../inc/photosensitive.h"

#include "../inc/myMath.h"
#include "../inc/light.h"



// 识别后的n路灰度值
uint8_t photosensitive_val[REGISTER_NUM] = {0};

void photosensitive_init(void)
{

}

void photosensitive_identify(void)
{
    set_rgb_brightness(1000, 1000, 1000);

    for(uint8_t i = 0; i < SENSORE_NUM; i++) //获取原始值
    {
        photosensitive_val[i] = VAL4096_2_255(get_adc_val(i));
    }
}
