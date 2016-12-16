/***************************************************
                nightWiring Project
        By Xiaohai Li  (haixiaolee@gmail.com)
***************************************************/

#ifdef __cplusplus
extern "C" {
#endif

extern int i2cRead           (int fd);
extern int i2cReadReg8       (int fd, int reg);
extern int i2cReadReg16      (int fd, int reg);

extern int i2cWrite          (int fd, int data);
extern int i2cWriteReg8      (int fd, int reg, int data);
extern int i2cWriteReg16     (int fd, int reg, int data);

extern int i2cSetupInterface (const char *device, int devId);
extern int i2cSetup (const char *device, const int devId);

#ifdef __cplusplus
}
#endif
