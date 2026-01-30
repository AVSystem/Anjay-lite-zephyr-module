# Copyright 2026 AVSystem <avsystem@avsystem.com>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

if(CONFIG_ANJAY_LITE_ZEPHYR)
    zephyr_library()

    file(
        GLOB_RECURSE
        ANJAY_LITE_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/deps/anjay-lite/src/*.c"
    )
    zephyr_library_sources(${ANJAY_LITE_SOURCES})

    zephyr_library_sources_ifdef(CONFIG_ANJAY_LITE_ZEPHYR_COMPAT_LOG_DEFAULT
        "${CMAKE_CURRENT_SOURCE_DIR}/compat/log_macro_check.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/compat/log_module_instantiate.c"
    )
    zephyr_library_sources_ifdef(CONFIG_ANJAY_LITE_ZEPHYR_COMPAT_RNG_DEFAULT "${CMAKE_CURRENT_SOURCE_DIR}/compat/rng.c")
    zephyr_library_sources_ifdef(CONFIG_ANJAY_LITE_ZEPHYR_COMPAT_MBEDTLS_TIMING_ALT_DEFAULT "${CMAKE_CURRENT_SOURCE_DIR}/compat/mbedtls_timing_alt.c")

    # NOTE: This adds Anjay Lite headers (and headers of this module) to
    # "global" scope, but it's the most conventional way if enablind a Kconfig
    # flag is supposed to be enough to make a header be available.
    zephyr_include_directories(
        "${CMAKE_CURRENT_SOURCE_DIR}/include_public"
        "${CMAKE_CURRENT_SOURCE_DIR}/deps/anjay-lite/include_public"
    )
endif()
