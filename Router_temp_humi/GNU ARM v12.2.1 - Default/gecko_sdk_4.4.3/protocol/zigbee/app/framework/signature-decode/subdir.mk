################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/simplicity/sdks/protocol/zigbee/app/framework/signature-decode/sl_signature_decode.c 

OBJS += \
./gecko_sdk_4.4.3/protocol/zigbee/app/framework/signature-decode/sl_signature_decode.o 

C_DEPS += \
./gecko_sdk_4.4.3/protocol/zigbee/app/framework/signature-decode/sl_signature_decode.d 


# Each subdirectory must supply rules for building sources it contributes
gecko_sdk_4.4.3/protocol/zigbee/app/framework/signature-decode/sl_signature_decode.o: D:/simplicity/sdks/protocol/zigbee/app/framework/signature-decode/sl_signature_decode.c gecko_sdk_4.4.3/protocol/zigbee/app/framework/signature-decode/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m33 -mthumb -std=c99 '-DEFR32MG24B310F1536IM48=1' '-DSL_APP_PROPERTIES=1' '-DHARDWARE_BOARD_DEFAULT_RF_BAND_2400=1' '-DHARDWARE_BOARD_SUPPORTS_1_RF_BAND=1' '-DHARDWARE_BOARD_SUPPORTS_RF_BAND_2400=1' '-DHFXO_FREQ=39000000' '-DSL_BOARD_NAME="BRD2601B"' '-DSL_BOARD_REV="A01"' '-DSL_COMPONENT_CATALOG_PRESENT=1' '-DCORTEXM3=1' '-DCORTEXM3_EFM32_MICRO=1' '-DCORTEXM3_EFR32=1' '-DPHY_RAIL=1' '-DPLATFORM_HEADER="platform-header.h"' '-DSL_LEGACY_HAL_ENABLE_WATCHDOG=1' '-DMBEDTLS_CONFIG_FILE=<sl_mbedtls_config.h>' '-DMBEDTLS_PSA_CRYPTO_CONFIG_FILE=<psa_crypto_config.h>' '-DSL_RAIL_LIB_MULTIPROTOCOL_SUPPORT=0' '-DSL_RAIL_UTIL_PA_CONFIG_HEADER=<sl_rail_util_pa_config.h>' '-DCUSTOM_TOKEN_HEADER="sl_token_manager_af_token_header.h"' '-DUSE_NVM3=1' '-DUC_BUILD=1' '-DEMBER_MULTI_NETWORK_STRIPPED=1' '-DSL_ZIGBEE_PHY_SELECT_STACK_SUPPORT=1' '-DSL_ZIGBEE_STACK_COMPLIANCE_REVISION=22' '-DCONFIGURATION_HEADER="app/framework/util/config.h"' -I"C:\Users\Admin\SimplicityStudio\v5_workspace\Router_temp_humi\config" -I"C:\Users\Admin\SimplicityStudio\v5_workspace\Router_temp_humi\config\zcl" -I"D:/simplicity/sdks//platform/Device/SiliconLabs/EFR32MG24/Include" -I"D:/simplicity/sdks//platform/common/inc" -I"D:/simplicity/sdks//hardware/board/inc" -I"D:/simplicity/sdks//platform/bootloader" -I"D:/simplicity/sdks//platform/bootloader/api" -I"D:/simplicity/sdks//platform/driver/button/inc" -I"D:/simplicity/sdks//platform/service/cli/inc" -I"D:/simplicity/sdks//platform/service/cli/src" -I"D:/simplicity/sdks//platform/CMSIS/Core/Include" -I"D:/simplicity/sdks//hardware/driver/configuration_over_swo/inc" -I"D:/simplicity/sdks//platform/driver/debug/inc" -I"D:/simplicity/sdks//platform/service/device_init/inc" -I"D:/simplicity/sdks//platform/emdrv/dmadrv/inc" -I"D:/simplicity/sdks//platform/emdrv/common/inc" -I"D:/simplicity/sdks//platform/emlib/inc" -I"D:/simplicity/sdks//platform/emdrv/gpiointerrupt/inc" -I"D:/simplicity/sdks//platform/service/hfxo_manager/inc" -I"D:/simplicity/sdks//platform/driver/i2cspm/inc" -I"D:/simplicity/sdks//platform/service/iostream/inc" -I"D:/simplicity/sdks//platform/driver/leddrv/inc" -I"D:/simplicity/sdks//platform/service/legacy_hal/inc" -I"D:/simplicity/sdks//platform/service/legacy_hal_wdog/inc" -I"D:/simplicity/sdks//platform/service/legacy_printf/inc" -I"D:/simplicity/sdks//platform/security/sl_component/sl_mbedtls_support/config" -I"D:/simplicity/sdks//platform/security/sl_component/sl_mbedtls_support/config/preset" -I"D:/simplicity/sdks//platform/security/sl_component/sl_mbedtls_support/inc" -I"D:/simplicity/sdks//util/third_party/mbedtls/include" -I"D:/simplicity/sdks//util/third_party/mbedtls/library" -I"D:/simplicity/sdks//hardware/driver/mx25_flash_shutdown/inc/sl_mx25_flash_shutdown_usart" -I"D:/simplicity/sdks//platform/emdrv/nvm3/inc" -I"D:/simplicity/sdks//platform/peripheral/inc" -I"D:/simplicity/sdks//platform/service/power_manager/inc" -I"D:/simplicity/sdks//util/third_party/printf" -I"D:/simplicity/sdks//util/third_party/printf/inc" -I"D:/simplicity/sdks//platform/security/sl_component/sl_psa_driver/inc" -I"D:/simplicity/sdks//platform/radio/rail_lib/common" -I"D:/simplicity/sdks//platform/radio/rail_lib/protocol/ble" -I"D:/simplicity/sdks//platform/radio/rail_lib/protocol/ieee802154" -I"D:/simplicity/sdks//platform/radio/rail_lib/protocol/wmbus" -I"D:/simplicity/sdks//platform/radio/rail_lib/protocol/zwave" -I"D:/simplicity/sdks//platform/radio/rail_lib/chip/efr32/efr32xg2x" -I"D:/simplicity/sdks//platform/radio/rail_lib/protocol/sidewalk" -I"D:/simplicity/sdks//platform/radio/rail_lib/plugin/rail_util_built_in_phys/efr32xg24" -I"D:/simplicity/sdks//platform/radio/rail_lib/plugin/rail_util_ieee802154" -I"D:/simplicity/sdks//platform/radio/rail_lib/plugin/pa-conversions" -I"D:/simplicity/sdks//platform/radio/rail_lib/plugin/pa-conversions/efr32xg24" -I"D:/simplicity/sdks//platform/radio/rail_lib/plugin/rail_util_power_manager_init" -I"D:/simplicity/sdks//platform/radio/rail_lib/plugin/rail_util_pti" -I"D:/simplicity/sdks//platform/security/sl_component/se_manager/inc" -I"D:/simplicity/sdks//platform/security/sl_component/se_manager/src" -I"D:/simplicity/sdks//util/plugin/security_manager" -I"D:/simplicity/sdks//hardware/driver/si70xx/inc" -I"D:/simplicity/sdks//util/silicon_labs/silabs_core/memory_manager" -I"D:/simplicity/sdks//platform/common/toolchain/inc" -I"D:/simplicity/sdks//platform/service/system/inc" -I"D:/simplicity/sdks//platform/service/sleeptimer/inc" -I"D:/simplicity/sdks//platform/service/token_manager/inc" -I"D:/simplicity/sdks//platform/service/udelay/inc" -I"D:/simplicity/sdks//protocol/zigbee/app/framework/common" -I"D:/simplicity/sdks//protocol/zigbee/app/framework/plugin/basic" -I"D:/simplicity/sdks//protocol/zigbee/app/util/serial" -I"D:/simplicity/sdks//protocol/zigbee/app/framework/service-function" -I"D:/simplicity/sdks//protocol/zigbee/app/framework/plugin/counters" -I"D:/simplicity/sdks//protocol/zigbee/app/framework/plugin/debug-print" -I"D:/simplicity/sdks//protocol/zigbee/app/framework/plugin/find-and-bind-initiator" -I"D:/simplicity/sdks//protocol/zigbee/app/framework/plugin/identify" -I"D:/simplicity/sdks//protocol/zigbee/app/framework/plugin/network-steering" -I"D:/simplicity/sdks//protocol/zigbee/app/framework/plugin/scan-dispatch" -I"D:/simplicity/sdks//protocol/zigbee/stack/security" -I"D:/simplicity/sdks//protocol/zigbee/stack/include" -I"D:/simplicity/sdks//protocol/zigbee/app/framework/signature-decode" -I"D:/simplicity/sdks//platform/radio/rail_lib/plugin" -I"D:/simplicity/sdks//protocol/zigbee" -I"D:/simplicity/sdks//protocol/zigbee/stack" -I"D:/simplicity/sdks//protocol/zigbee/stack/gp" -I"D:/simplicity/sdks//protocol/zigbee/stack/zigbee" -I"D:/simplicity/sdks//platform/radio/mac/rail_mux" -I"D:/simplicity/sdks//platform/radio/mac" -I"D:/simplicity/sdks//util/silicon_labs/silabs_core" -I"D:/simplicity/sdks//protocol/zigbee/stack/core" -I"D:/simplicity/sdks//protocol/zigbee/stack/mac" -I"D:/simplicity/sdks//protocol/zigbee/app/em260" -I"D:/simplicity/sdks//protocol/zigbee/app/framework/plugin/update-tc-link-key" -I"D:/simplicity/sdks//protocol/zigbee/app/framework/include" -I"D:/simplicity/sdks//protocol/zigbee/app/framework/util" -I"D:/simplicity/sdks//protocol/zigbee/app/framework/security" -I"D:/simplicity/sdks//protocol/zigbee/app/util/zigbee-framework" -I"D:/simplicity/sdks//protocol/zigbee/app/util/counters" -I"D:/simplicity/sdks//protocol/zigbee/app/framework/cli" -I"D:/simplicity/sdks//protocol/zigbee/app/util/common" -I"D:/simplicity/sdks//protocol/zigbee/app/util/security" -I"C:\Users\Admin\SimplicityStudio\v5_workspace\Router_temp_humi\autogen" -Os -Wall -Wextra -ffunction-sections -fdata-sections -imacrossl_gcc_preinclude.h -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mcmse -fno-builtin-printf -fno-builtin-sprintf --specs=nano.specs -Wno-unused-parameter -Wno-missing-field-initializers -Wno-missing-braces -c -fmessage-length=0 -MMD -MP -MF"gecko_sdk_4.4.3/protocol/zigbee/app/framework/signature-decode/sl_signature_decode.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


