/*
 * Ensures VFX and the switched LED VCC rail are enabled after ZMK's
 * settings_load() finishes, preventing persisted off or ext_power states from
 * leaving the LED strip disabled or unpowered after boot.
 */

#include <zephyr/init.h>
#include <zephyr/kernel.h>

#include <zmk/vfx/vfx.h>

static struct k_work_delayable boot_fix_work;

static void vfx_boot_fix_handler(struct k_work *work) {
    ARG_UNUSED(work);

    zmk_vfx_on(ZMK_VFX_CH_ALL);

    if (zmk_vfx_get_brightness(ZMK_VFX_CH_ALL) == 0) {
        zmk_vfx_set_brightness(ZMK_VFX_CH_ALL,
                               (uint8_t)((uint16_t)CONFIG_ZMK_VFX_BRT_START * 255U / 100U));
    }
}

static int vfx_boot_fix_init(void) {
    k_work_init_delayable(&boot_fix_work, vfx_boot_fix_handler);
    k_work_schedule(&boot_fix_work, K_MSEC(150));

    return 0;
}

SYS_INIT(vfx_boot_fix_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
