/* USER CODE BEGIN Header */
/**
 ******************************************************************************
  * @file    user_diskio.c
  * @brief   This file includes a diskio driver skeleton to be completed by the user.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
 /* USER CODE END Header */

#ifdef USE_OBSOLETE_USER_CODE_SECTION_0
/*
 * Warning: the user section 0 is no more in use (starting from CubeMx version 4.16.0)
 * To be suppressed in the future.
 * Kept to ensure backward compatibility with previous CubeMx versions when
 * migrating projects.
 * User code previously added there should be copied in the new user sections before
 * the section contents can be deleted.
 */
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */
#endif

/* USER CODE BEGIN DECL */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "ff_gen_drv.h"
#include "sfud.h"
#include "dbger.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SPI_FLASH_BLK_NUM	2048
#define SPI_FLASH_BLK_SIZE	4096	// erase unit
/* Private variables ---------------------------------------------------------*/
sfud_flash spi_flash_1 = {
    .name = SPI_FLASH_1_NAME,
    .spi.name = "SPI3",
    .chip = { "W25Q64JV", SFUD_MF_ID_WINBOND, 0x40, 0x17, SPI_FLASH_BLK_NUM * SPI_FLASH_BLK_SIZE, SFUD_WM_PAGE_256B, SPI_FLASH_BLK_SIZE, 0x20 }
};
/* Disk status */
static volatile DSTATUS Stat = STA_NOINIT;

/* USER CODE END DECL */

/* Private function prototypes -----------------------------------------------*/
DSTATUS USER_initialize (BYTE pdrv);
DSTATUS USER_status (BYTE pdrv);
DRESULT USER_read (BYTE pdrv, BYTE *buff, DWORD sector, UINT count);
#if _USE_WRITE == 1
  DRESULT USER_write (BYTE pdrv, const BYTE *buff, DWORD sector, UINT count);
#endif /* _USE_WRITE == 1 */
#if _USE_IOCTL == 1
  DRESULT USER_ioctl (BYTE pdrv, BYTE cmd, void *buff);
#endif /* _USE_IOCTL == 1 */

Diskio_drvTypeDef  USER_Driver =
{
  USER_initialize,
  USER_status,
  USER_read,
#if  _USE_WRITE
  USER_write,
#endif  /* _USE_WRITE == 1 */
#if  _USE_IOCTL == 1
  USER_ioctl,
#endif /* _USE_IOCTL == 1 */
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes a Drive
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
DSTATUS USER_initialize (
	BYTE pdrv           /* Physical drive nmuber to identify the drive */
)
{
  /* USER CODE BEGIN INIT */
	sfud_err sfud_sta;
	sfud_sta = sfud_device_init(&spi_flash_1);
	if(sfud_sta != SFUD_SUCCESS) {
		LOG_ERR("sfud device init err[%d]\n", sfud_sta);
		Stat = STA_NOINIT;
	} else {
		LOG_DBG("sfud device init succ\n");
		Stat = 0;
	}
    return Stat;
  /* USER CODE END INIT */
}

/**
  * @brief  Gets Disk Status
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
DSTATUS USER_status (
	BYTE pdrv       /* Physical drive number to identify the drive */
)
{
  /* USER CODE BEGIN STATUS */
    return Stat;
  /* USER CODE END STATUS */
}

/**
  * @brief  Reads Sector(s)
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT USER_read (
	BYTE pdrv,      /* Physical drive nmuber to identify the drive */
	BYTE *buff,     /* Data buffer to store read data */
	DWORD sector,   /* Sector address in LBA */
	UINT count      /* Number of sectors to read */
)
{
  /* USER CODE BEGIN READ */
	sfud_err sfud_sta;
	sfud_sta = sfud_read(&spi_flash_1, sector * SPI_FLASH_BLK_SIZE, count * SPI_FLASH_BLK_SIZE, buff);
	if(sfud_sta != SFUD_SUCCESS) {
		LOG_ERR("sfud read err[%d]\n", sfud_sta);
		return RES_ERROR;
	} else {
		LOG_VBS("sfud read succ\n");
	}
    return RES_OK;
  /* USER CODE END READ */
}

/**
  * @brief  Writes Sector(s)
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data to be written
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to write (1..128)
  * @retval DRESULT: Operation result
  */
#if _USE_WRITE == 1
DRESULT USER_write (
	BYTE pdrv,          /* Physical drive nmuber to identify the drive */
	const BYTE *buff,   /* Data to be written */
	DWORD sector,       /* Sector address in LBA */
	UINT count          /* Number of sectors to write */
)
{
  /* USER CODE BEGIN WRITE */
  /* USER CODE HERE */
    sfud_err sfud_sta;
	sfud_sta = sfud_erase(&spi_flash_1, sector * SPI_FLASH_BLK_SIZE, count * SPI_FLASH_BLK_SIZE);
	if(sfud_sta != SFUD_SUCCESS) {
		LOG_ERR("sfud erase err[%d]\n", sfud_sta);
		return RES_ERROR;
	}
	sfud_sta = sfud_write(&spi_flash_1, sector * SPI_FLASH_BLK_SIZE, count * SPI_FLASH_BLK_SIZE, buff);
	if(sfud_sta != SFUD_SUCCESS) {
		LOG_ERR("sfud write err[%d]\n", sfud_sta);
		return RES_ERROR;
	} else {
		LOG_VBS("sfud write succ\n");
	}
    return RES_OK;
  /* USER CODE END WRITE */
}
#endif /* _USE_WRITE == 1 */

/**
  * @brief  I/O control operation
  * @param  pdrv: Physical drive number (0..)
  * @param  cmd: Control code
  * @param  *buff: Buffer to send/receive control data
  * @retval DRESULT: Operation result
  */
#if _USE_IOCTL == 1
DRESULT USER_ioctl (
	BYTE pdrv,      /* Physical drive nmuber (0..) */
	BYTE cmd,       /* Control code */
	void *buff      /* Buffer to send/receive control data */
)
{
  /* USER CODE BEGIN IOCTL */
  DRESULT res = RES_ERROR;

  if (Stat & STA_NOINIT){
	  LOG_ERR("disk NOT init\n");
	  return RES_NOTRDY;
  }

  switch (cmd)
  {
  /* Make sure that no pending write process */
  case CTRL_SYNC :
    res = RES_OK;
    break;

  /* Get number of sectors on the disk (DWORD) */
  case GET_SECTOR_COUNT :
    *(DWORD*)buff = SPI_FLASH_BLK_NUM;
    res = RES_OK;
    break;

  /* Get R/W sector size (WORD) */
  case GET_SECTOR_SIZE :
    *(WORD*)buff = SPI_FLASH_BLK_SIZE;
    res = RES_OK;
    break;

  /* Get erase block size in unit of sector (DWORD) */
  case GET_BLOCK_SIZE :
    *(DWORD*)buff = SPI_FLASH_BLK_SIZE;
    res = RES_OK;
    break;

  default:
	  LOG_ERR("cmd[0x%02x] err\n", cmd);
	res = RES_PARERR;
  }

  return res;
  /* USER CODE END IOCTL */
}
#endif /* _USE_IOCTL == 1 */

