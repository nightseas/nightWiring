/***************************************************
                nightWiring Project
        By Xiaohai Li  (haixiaolee@gmail.com)
***************************************************/

#ifdef __cplusplus
extern "C" {
#endif


int spiDataTransfer (int fd, unsigned char *data, int len);
int spiSetupMode (const char *spidev, int speed, int mode);
int spiSetup (const char *spidev, int speed);

#ifdef __cplusplus
}
#endif
