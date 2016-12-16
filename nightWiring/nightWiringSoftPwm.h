/***************************************************
                nightWiring Project
        By Xiaohai Li  (haixiaolee@gmail.com)
***************************************************/

#ifdef __cplusplus
extern "C" {
#endif

extern int  softPwmCreate (int pin, int value, int range);
extern void softPwmWrite  (int pin, int value);
extern void softPwmStop   (int pin);

#ifdef __cplusplus
}
#endif
