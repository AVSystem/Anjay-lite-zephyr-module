/*
 * Copyright 2026 AVSystem <avsystem@avsystem.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANJAY_LITE_ZEPHYR_LOG_HANDLER_IMPL_H
#define ANJAY_LITE_ZEPHYR_LOG_HANDLER_IMPL_H

#ifndef CONFIG_LOG
#error "Anjay Lite Zephyr module default logger requires Zephyr logging subsystem to be enabled (CONFIG_LOG)."
#endif // CONFIG_LOG

#include <zephyr/logging/log.h>

#include <anj/utils.h>

LOG_MODULE_DECLARE(anjay_lite);

#define _ANJ_LOG_ZEPHYR_LOG_FOR_L_ERROR LOG_ERR
#define _ANJ_LOG_ZEPHYR_LOG_FOR_L_WARNING LOG_WRN
#define _ANJ_LOG_ZEPHYR_LOG_FOR_L_INFO LOG_INF
#define _ANJ_LOG_ZEPHYR_LOG_FOR_L_DEBUG LOG_DBG
#define _ANJ_LOG_ZEPHYR_LOG_FOR_L_TRACE LOG_DBG

#define _ANJ_LOG_MAP_LEVEL_TO_ZEPHYR(LogLevel) \
    ANJ_CONCAT(_ANJ_LOG_ZEPHYR_LOG_FOR_, LogLevel)

/**
 * NOTE: This uses two non-standard extensions:
 * - Statement-to-expression, since Anjay Lite expects an expression, while
 *   Zephyr logging macros are statements.
 * - The trailing comma in the variadic macro, to allow calls without any
 *   additional arguments.
 */
#define ANJ_LOG_HANDLER_IMPL_MACRO(Module, LogLevel, FormatStr, ...) \
    __extension__ ({ \
        _ANJ_LOG_MAP_LEVEL_TO_ZEPHYR(LogLevel)( \
            "[%s] " FormatStr, \
            ANJ_QUOTE_MACRO(Module), \
            ##__VA_ARGS__); \
        0; \
    })

#endif // ANJAY_LITE_ZEPHYR_LOG_HANDLER_IMPL_H
