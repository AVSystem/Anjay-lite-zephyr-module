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

#include <mbedtls/timing.h>

#include <anj/compat/time.h>
#include <anj/utils.h>

/**
 * HACK: MbedTLS socket implementation in Anjay Lite allocates a structure for
 * the timer of type mbedtls_timing_delay_context, which is not being directly
 * read by mbedTLS, but is provided as a void pointer to the timing functions.
 *
 * Since we don't want to force end users to provide a custom config of mbedTLS
 * that enabled MBEDTLS_TIMING_ALT which is expected to re-define these mbedTLS
 * types (seen both by mbedTLS and Anjay Lite), we'll just use
 * mbedtls_timing_delay_context as storage for our custom timing context.
 */

typedef struct anj_mbedtls_timing_delay_ctx_struct {
    anj_time_monotonic_t timer;
    uint32_t int_ms;
    uint32_t fin_ms;
} anj_mbedtls_timing_delay_ctx_t;

ANJ_STATIC_ASSERT(sizeof(anj_mbedtls_timing_delay_ctx_t) <= sizeof(mbedtls_timing_delay_context), anj_mbedtls_timing_delay_ctx_t_too_large);

/**
 * MbedTLS doesn't call this directly, but Anjay Lite MbedTLS socket
 * implementation does.
 */
void mbedtls_timing_set_delay(void *data, uint32_t int_ms, uint32_t fin_ms) {
    anj_mbedtls_timing_delay_ctx_t *ctx =
            (anj_mbedtls_timing_delay_ctx_t *) data;

    ctx->int_ms = int_ms;
    ctx->fin_ms = fin_ms;

    if (fin_ms != 0) {
        ctx->timer = anj_time_monotonic_now();
    }
}

/**
 * MbedTLS doesn't call this directly, but Anjay Lite MbedTLS socket
 * implementation does.
 */
int mbedtls_timing_get_delay(void *data) {
    anj_mbedtls_timing_delay_ctx_t *ctx =
            (anj_mbedtls_timing_delay_ctx_t *) data;

    if (ctx->fin_ms == 0) {
        return -1;
    }

    anj_time_duration_t diff = anj_time_monotonic_diff(
            anj_time_monotonic_now(), ctx->timer);
    if (!anj_time_duration_is_valid(diff)) {
        return -1;
    }

    int64_t elapsed_ms_signed = anj_time_duration_to_scalar(diff, ANJ_TIME_UNIT_MS);

    assert(elapsed_ms_signed >= 0);
    uint64_t elapsed_ms = (uint64_t) elapsed_ms_signed;
    if (elapsed_ms >= ctx->fin_ms) {
        return 2;
    } else if (elapsed_ms >= ctx->int_ms) {
        return 1;
    } else {
        return 0;
    }
}
