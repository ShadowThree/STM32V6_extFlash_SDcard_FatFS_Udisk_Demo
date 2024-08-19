## ����˵��
1. �����̻��ڰ�������`STM32V6`�����壻
2. ������ͨ��`CubeMx`���ò����ɳ�ʼ�����룻
3. `MCU`����ͨ��`FatFS`ͬʱ����`SDcard`��`extFlash`;
4. �����̵��ⲿ`SPI Flash`����ʹ����[SFUD](https://github.com/armink/SFUD.git)����ֲ˵���鿴[����](https://github.com/ShadowThree/STM32V6_SPI_FLASH_database)��

## ע��
1. ����`ST`��˾��`HAL`���е����⣬`SDIO`��ʼ���������������޸�(���ﲻ��Ҫ����`SD`����ʼ�������������`FatFS`�����)��
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
2. Ϊ�˱������壬���Խ�`ThirdUtils\sfud\sfud\inc\sfud_cfg.h`�ļ�ɾ��(��Ϊ��ֲ`SFUD`ʱ���Ʋ��޸�������ļ�)��
3. ��ǰ��[fatfs_file_handler.c](./ThirdUtils/fatfs_file_handler/fatfs_file_handler.c)�ļ��У�`FH_mkfs()`��`FH_get_space()`����������`SPI Flash`ִ��ʧ�ܣ�
4. ��`SDcard`��ص�`FATFS`���ܣ�����ͨ��`CubeMX`���ɣ���Ҫ��[sd_diskio.c](./FATFS/Target/sd_diskio.c)�У�
5. ��`SPI Flash`��ص�`FATFS`���ܣ������ͨ��`CubeMX`���ɣ���Ҫ�Լ�ʵ�ֳ�ʼ������д���ܣ���Ҫ��[user_diskio.c](./FATFS/Target/user_diskio.c)�У�

## ʹ��
```sh
# 1. clone this repo
git clone https://github.com/ShadowThree/STM32V6_extFlash_SDcard_FatFS_Udisk_Demo.git
# 2. update the submodele
git submodule update --init
# 3. build and downlowd
```