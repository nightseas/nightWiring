/***************************************************
                nightWiring Project
        By Xiaohai Li  (haixiaolee@gmail.com)
***************************************************/

#ifdef __cplusplus
extern "C" {
#endif

extern void softServoWrite  (int pin, int value);
extern int softServoSetup   (int p0, int p1, int p2, int p3, int p4, int p5, int p6, int p7);

#ifdef __cplusplus
}
#endif
