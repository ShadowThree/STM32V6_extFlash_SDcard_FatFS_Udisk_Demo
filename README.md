## 功能说明
1. 本工程基于安富莱的`STM32V6`开发板；
2. 本工程通过`CubeMx`配置并生成初始化代码；
3. `MCU`可以通过`FatFS`同时访问`SDcard`和`extFlash`;
4. 本工程的外部`SPI Flash`驱动使用了[SFUD](https://github.com/armink/SFUD.git)，移植说明查看[这里](https://github.com/ShadowThree/STM32V6_SPI_FLASH_database)；

## 注意
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

## 使用
```sh
# 1. clone this repo
git clone https://github.com/ShadowThree/STM32V6_extFlash_SDcard_FatFS_Udisk_Demo.git
# 2. update the submodele
git submodule update --init
# 3. build and downlowd
```