## 功能说明
1. 本工程基于安富莱的`STM32V6`开发板；
2. 本工程通过`CubeMx`配置并生成初始化代码；
3. `MCU`可以通过`FatFS`同时访问`SDcard`和`extFlash`;
4. 本工程的外部`SPI Flash`驱动使用了[SFUD](https://github.com/armink/SFUD.git)，移植说明查看[这里](https://github.com/ShadowThree/STM32V6_SPI_FLASH_database)；
5. 可通过宏定义`UDISK_SDCARD`配置是将`SDcard`还是`SPI Flash`模拟成`U盘`；
6. 不管是`FATFS`还是`USB`，对`SDcard`的操作都是通过`DMA`方式完成的；

## 注意
0. 非常重要！！！
> 使用`USB`相关功能时，必须使用`ARM Compiler V5`编译才能正常工作；使用`ARM Compiler V6`可以编译成功，但是挂载失败！
1. 由于`ST`公司的`HAL`库有点问题，`SDIO`初始化代码需作如下修改(这里不需要调用`SD`卡初始化函数，后面的`FatFS`会调用)：
```c
void MX_SDIO_SD_Init(void)
{
  /* USER CODE BEGIN SDIO_Init 0 */
  /* USER CODE END SDIO_Init 0 */
  /* USER CODE BEGIN SDIO_Init 1 */
  /* USER CODE END SDIO_Init 1 */
  hsd.Instance = SDIO;
  hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
  hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
  hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
  hsd.Init.BusWide = SDIO_BUS_WIDE_4B;      /** @fixme: MUST modify SDIO_BUS_WIDE_4B to SDIO_BUS_WIDE_1B */
  hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd.Init.ClockDiv = 0;
  /* USER CODE BEGIN SDIO_Init 2 */
  hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
  /* USER CODE END SDIO_Init 2 */
}
```
2. 为了避免歧义，可以将`ThirdUtils\sfud\sfud\inc\sfud_cfg.h`文件删除(因为移植`SFUD`时复制并修改了这个文件)；
3. 当前的[fatfs_file_handler.c](./ThirdUtils/fatfs_file_handler/fatfs_file_handler.c)文件中，`FH_mkfs()`和`FH_get_space()`两个函数对`SPI Flash`执行失败；
4. 和`SDcard`相关的`FATFS`功能，都是通过`CubeMX`生成，主要在[sd_diskio.c](./FATFS/Target/sd_diskio.c)中；
5. 和`SPI Flash`相关的`FATFS`功能，框架是通过`CubeMX`生成，需要自己实现初始化及读写功能，主要在[user_diskio.c](./FATFS/Target/user_diskio.c)中；
6. `USB`初始化函数`MX_USB_DEVICE_Init()`调用之前，一定要先初始化`SDIO`接口，不然`USB`初始化将会失败；但是`CubeMX`生成的初始化代码中，并未调用初始化`SDIO`相关函数，所以要么在`MX_SDIO_Init()`中初始化`SDIO`，要么在`CubeMX`中设置不调用`MX_USB_DEVICE_Init()`，由用户初始化`SDIO`后再主动调用。初始化`SDIO`可以通过`FH_mkfs()`或者`FH_mount()`实现，它们都会调用[sd_diskio.c](./FATFS/Target/sd_diskio.c)中的`SD_initialize()`函数；
7. 当使能`SDIO`的`DMA`读写时，`SDIO`的全局中断也一定要使能，不然`DMA`读写会出问题！
8. 如果需要也可以给`SPI Flash`添加`DMA`读写，主要修改[sfud_port.c](./ThirdUtils/sfud_port/sfud_port.c)中的`spi_write_read()`函数即可；
9. `CubeMX`的`USB_DEVICE`好像不支持多实例（即同时将`SDcard`和`SPI Flash`模拟成`U盘`），所以本工程通过一个宏定义配置将哪个存储模拟成`U盘`。
10. 通过`CubeMx`重新生成工程后，需要作如下修改：
```c
// USB_DEVICE/Target/usbd_conf.h modify as below
#define MSC_MEDIA_PACKET     512
//-------------- above change to below -------------
#define UDISK_SDCARD		  0     // [0]=[SPI Flash as Udisk]   [1]=[SDcard as Udisk]
#define UDISK_SPI_FLASH   (!UDISK_SDCARD)

#if UDISK_SDCARD
  #define MSC_MEDIA_PACKET     512
#elif UDISK_SPI_FLASH
  #define MSC_MEDIA_PACKET     4096
#endif
```
```c
// USB_DEVICE/App/usbd_storage_if.c
// this macro just used for SPI Flash
#define STORAGE_LUN_NBR                  1
#define STORAGE_BLK_NBR                  (2048)
#define STORAGE_BLK_SIZ                  (4096)
```

## HOW TO USE
1. clone this repo: `git clone https://github.com/ShadowThree/STM32V6_extFlash_SDcard_FatFS_Udisk_Demo.git`
2. update the submodele: `git submodule update --init`
3. modify the macro `UDISK_SDCARD` in [usbd_conf.h](./USB_DEVICE/Target/usbd_conf.h) as you need;
4. build and downlowd;