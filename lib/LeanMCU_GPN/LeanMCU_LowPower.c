/*
 * SPDX-License-Identifier: GPL-2.0-only OR LicenseRef-Commercial
 *
 * Copyright (c) 2025 LeanMCU
 *
 * This file is part of <PROJECT NAME>.
 *
 * Dual Licensing Notice:
 *
 * This source code is licensed under the terms of the GNU General Public
 * License version 2 only (GPL-2.0-only), as published by the Free Software
 * Foundation.
 *
 * Alternatively, this software may be licensed under a separate commercial
 * license obtained from the copyright holder. Use of this software in
 * proprietary or closed-source products, or under terms incompatible with
 * the GPL-2.0-only, requires a valid commercial license.
 *
 * Commercial licensing inquiries:
 *   leanmcu(at)gmail(dot)com
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License version 2 for more details.
 *
 * You should have received a copy of the GNU General Public License
 * version 2 along with this program. If not, see:
 *   https://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 */

#include "LeanMCU_LowPower.h"
#include <Arduino.h>

void Disable_GPIO(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    // Enable all GPIO ports
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();

    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;

    // Configure all pins as analog to reduce leakage
    GPIO_InitStruct.Pin = GPIO_PIN_All;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
#ifdef DEBUG
    // Enable GPIOA clock for SWD pins
    GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP; // Alternate Function Push-Pull
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF0_SWCLK; // AF0 is for SWD on STM32L0
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
#endif

#ifndef DEBUG
    __HAL_RCC_GPIOA_CLK_DISABLE(); // GPIOA is used for SWD pins in debug mode
#endif
    __HAL_RCC_GPIOB_CLK_DISABLE();
    __HAL_RCC_GPIOC_CLK_DISABLE();
    __HAL_RCC_GPIOH_CLK_DISABLE();
}

void Disable_RTC(void)
{
    HAL_PWR_EnableBkUpAccess();
    __HAL_RCC_RTC_DISABLE();
    if (__HAL_RCC_GET_RTC_SOURCE() == RCC_RTCCLKSOURCE_LSE)
    {
        __HAL_RCC_LSE_CONFIG(RCC_LSE_OFF);
        while (__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY))
        {
        }
    }
    __HAL_RCC_RTC_DISABLE();
    HAL_PWR_DisableBkUpAccess();
}

void Disable_Peripherals(void)
{
    __HAL_RCC_CRC_CLK_DISABLE();
    __HAL_RCC_ADC1_CLK_DISABLE();
    __HAL_RCC_I2C1_CLK_DISABLE();
    __HAL_RCC_SPI1_CLK_DISABLE();
    __HAL_RCC_USART1_CLK_DISABLE();
    __HAL_RCC_TIM2_CLK_DISABLE();
    __HAL_RCC_TIM3_CLK_DISABLE();
    __HAL_RCC_TIM6_CLK_DISABLE();
    __HAL_RCC_TIM7_CLK_DISABLE();
    __HAL_RCC_TIM21_CLK_DISABLE();
    __HAL_RCC_TIM22_CLK_DISABLE();

    HAL_PWR_DisablePVD();
}
