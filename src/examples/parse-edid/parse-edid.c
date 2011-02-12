/* vim: set et fde fdm=syntax ts=4 sts=4 sw=4 : */
/*
 * Copyright © 2011 Saleem Abdulrasool <compnerd@compnerd.org>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#define _GNU_SOURCE

#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <leds/edid.h>
#include <leds/hdmi.h>
#include <leds/cea861.h>

#define CM_2_MM(cm)                             ((cm) * 10)
#define CM_2_IN(cm)                             ((cm) * 0.3937)

#define ARRAY_SIZE(arr)                         (sizeof(arr) / sizeof((arr)[0]))


static void
dump_edid(const struct edid * const edid)
{
    printf("%40s %02x %02x %02x %02x %02x %02x %02x %02x\n",
           "header:",
           edid->header[0], edid->header[1], edid->header[2],
           edid->header[3], edid->header[4], edid->header[5],
           edid->header[6], edid->header[7]);
    printf("%40s %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n",
           "vendor/product identification:",
           ((uint8_t *) &edid->manufacturer)[0],
           ((uint8_t *) &edid->manufacturer)[1],
           edid->product[0], edid->product[1],
           edid->serial_number[0], edid->serial_number[1],
           edid->serial_number[2], edid->serial_number[3],
           edid->manufacture_week, edid->manufacture_year);
    printf("%40s %02x %02x\n",
           "edid structure version/revision:",
           edid->version, edid->revision);
    printf("%40s %02x %02x %02x %02x %02x\n",
           "basic display parameters/features:",
           edid->video_input_definition,
           edid->maximum_horizontal_image_size,
           edid->maximum_vertical_image_size,
           edid->display_transfer_characteristics,
           edid->feature_support);
    printf("%40s %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n",
           "color characteristics:",

           edid->red_x_low   << 6 | edid->red_y_low   << 4 |
           edid->green_x_low << 2 | edid->green_y_low << 0,

           edid->blue_x_low  << 6 | edid->blue_y_low  << 4 |
           edid->white_x_low << 2 | edid->white_y_low << 0,

           edid->red_x, edid->red_y, edid->green_x, edid->green_y,
           edid->blue_x, edid->blue_y, edid->white_x, edid->white_y);
    printf("%40s %02x %02x %02x\n",
           "established timings:",
           ((uint8_t *) &edid->established_timings)[0],
           ((uint8_t *) &edid->established_timings)[1],
           edid->manufacturer_timings);
    printf("%40s %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n",
           "standard timing identification:",
           ((uint8_t *) &edid->standard_timing_id[0])[0],
           ((uint8_t *) &edid->standard_timing_id[0])[1],
           ((uint8_t *) &edid->standard_timing_id[1])[0],
           ((uint8_t *) &edid->standard_timing_id[1])[1],
           ((uint8_t *) &edid->standard_timing_id[2])[0],
           ((uint8_t *) &edid->standard_timing_id[2])[1],
           ((uint8_t *) &edid->standard_timing_id[3])[0],
           ((uint8_t *) &edid->standard_timing_id[3])[1],
           ((uint8_t *) &edid->standard_timing_id[4])[0],
           ((uint8_t *) &edid->standard_timing_id[4])[1],
           ((uint8_t *) &edid->standard_timing_id[5])[0],
           ((uint8_t *) &edid->standard_timing_id[5])[1],
           ((uint8_t *) &edid->standard_timing_id[6])[0],
           ((uint8_t *) &edid->standard_timing_id[6])[1],
           ((uint8_t *) &edid->standard_timing_id[7])[0],
           ((uint8_t *) &edid->standard_timing_id[7])[1]);
    printf("%40s %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n",
           "detailed timing 0:",
           ((uint8_t *) &edid->detailed_timings[0])[0],
           ((uint8_t *) &edid->detailed_timings[0])[1],
           ((uint8_t *) &edid->detailed_timings[0])[2],
           ((uint8_t *) &edid->detailed_timings[0])[3],
           ((uint8_t *) &edid->detailed_timings[0])[4],
           ((uint8_t *) &edid->detailed_timings[0])[5],
           ((uint8_t *) &edid->detailed_timings[0])[6],
           ((uint8_t *) &edid->detailed_timings[0])[7],
           ((uint8_t *) &edid->detailed_timings[0])[8],
           ((uint8_t *) &edid->detailed_timings[0])[9],
           ((uint8_t *) &edid->detailed_timings[0])[10],
           ((uint8_t *) &edid->detailed_timings[0])[11],
           ((uint8_t *) &edid->detailed_timings[0])[12],
           ((uint8_t *) &edid->detailed_timings[0])[13],
           ((uint8_t *) &edid->detailed_timings[0])[14],
           ((uint8_t *) &edid->detailed_timings[0])[15],
           ((uint8_t *) &edid->detailed_timings[0])[16],
           ((uint8_t *) &edid->detailed_timings[0])[17]);
    printf("%40s %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n",
           "detailed timing 1:",
           ((uint8_t *) &edid->detailed_timings[1])[0],
           ((uint8_t *) &edid->detailed_timings[1])[1],
           ((uint8_t *) &edid->detailed_timings[1])[2],
           ((uint8_t *) &edid->detailed_timings[1])[3],
           ((uint8_t *) &edid->detailed_timings[1])[4],
           ((uint8_t *) &edid->detailed_timings[1])[5],
           ((uint8_t *) &edid->detailed_timings[1])[6],
           ((uint8_t *) &edid->detailed_timings[1])[7],
           ((uint8_t *) &edid->detailed_timings[1])[8],
           ((uint8_t *) &edid->detailed_timings[1])[9],
           ((uint8_t *) &edid->detailed_timings[1])[10],
           ((uint8_t *) &edid->detailed_timings[1])[11],
           ((uint8_t *) &edid->detailed_timings[1])[12],
           ((uint8_t *) &edid->detailed_timings[1])[13],
           ((uint8_t *) &edid->detailed_timings[1])[14],
           ((uint8_t *) &edid->detailed_timings[1])[15],
           ((uint8_t *) &edid->detailed_timings[1])[16],
           ((uint8_t *) &edid->detailed_timings[1])[17]);
    printf("%40s %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n",
           "detailed timing 2:",
           ((uint8_t *) &edid->detailed_timings[2])[0],
           ((uint8_t *) &edid->detailed_timings[2])[1],
           ((uint8_t *) &edid->detailed_timings[2])[2],
           ((uint8_t *) &edid->detailed_timings[2])[3],
           ((uint8_t *) &edid->detailed_timings[2])[4],
           ((uint8_t *) &edid->detailed_timings[2])[5],
           ((uint8_t *) &edid->detailed_timings[2])[6],
           ((uint8_t *) &edid->detailed_timings[2])[7],
           ((uint8_t *) &edid->detailed_timings[2])[8],
           ((uint8_t *) &edid->detailed_timings[2])[9],
           ((uint8_t *) &edid->detailed_timings[2])[10],
           ((uint8_t *) &edid->detailed_timings[2])[11],
           ((uint8_t *) &edid->detailed_timings[2])[12],
           ((uint8_t *) &edid->detailed_timings[2])[13],
           ((uint8_t *) &edid->detailed_timings[2])[14],
           ((uint8_t *) &edid->detailed_timings[2])[15],
           ((uint8_t *) &edid->detailed_timings[2])[16],
           ((uint8_t *) &edid->detailed_timings[2])[17]);
    printf("%40s %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n",
           "detailed timing 3:",
           ((uint8_t *) &edid->detailed_timings[3])[0],
           ((uint8_t *) &edid->detailed_timings[3])[1],
           ((uint8_t *) &edid->detailed_timings[3])[2],
           ((uint8_t *) &edid->detailed_timings[3])[3],
           ((uint8_t *) &edid->detailed_timings[3])[4],
           ((uint8_t *) &edid->detailed_timings[3])[5],
           ((uint8_t *) &edid->detailed_timings[3])[6],
           ((uint8_t *) &edid->detailed_timings[3])[7],
           ((uint8_t *) &edid->detailed_timings[3])[8],
           ((uint8_t *) &edid->detailed_timings[3])[9],
           ((uint8_t *) &edid->detailed_timings[3])[10],
           ((uint8_t *) &edid->detailed_timings[3])[11],
           ((uint8_t *) &edid->detailed_timings[3])[12],
           ((uint8_t *) &edid->detailed_timings[3])[13],
           ((uint8_t *) &edid->detailed_timings[3])[14],
           ((uint8_t *) &edid->detailed_timings[3])[15],
           ((uint8_t *) &edid->detailed_timings[3])[16],
           ((uint8_t *) &edid->detailed_timings[3])[17]);
    printf("%40s %02x\n", "extensions:", edid->extensions);
    printf("%40s %02x\n", "checksum:", edid->checksum);
}

static inline double
_edid_fraction(uint16_t value)
{
    double result = 0.0;
    uint8_t i;

    assert(~value & 0xfc00);    /* edid fraction is 10 bits */

    for (i = 0; value && (i < 10); i++, value >>= 1)
        result += (value & 0x1) * (1.0 / (1 << (10 - i)));

    return result;
}

static inline const char * const
_aspect_ratio(const double hres, const double vres)
{
    const double ratio = hres / vres;

    if (ratio == (16.0 / 10.0))
        return "16:10";
    if (ratio == (4.0 / 3.0))
        return "4:3";
    if (ratio == (5.0 / 4.0))
        return "5:4";
    if (ratio == (16.0 / 9.0))
        return "16:9";

    return "unknown";
}

static inline char *
_edid_timing_string(const struct edid_detailed_timing_descriptor * const dtb)
{
    char *timing = NULL;
    const uint16_t hres = edid_timing_horizontal_active(dtb);
    const uint16_t vres = edid_timing_vertical_active(dtb);
    const uint32_t htotal = hres + edid_timing_horizontal_blanking(dtb);
    const uint32_t vtotal = vres + edid_timing_vertical_blanking(dtb);

    asprintf(&timing,
             "%ux%u%c at %.fHz (%s)",
             hres,
             vres,
             dtb->interlaced ? 'i' : 'p',
             (double) edid_timing_pixel_clock(dtb) / (vtotal * htotal),
             _aspect_ratio(hres, vres));

    return timing;
}

static inline char *
_edid_mode_string(const struct edid_detailed_timing_descriptor * const dtb)
{
    char *modestr = NULL;
    const uint16_t xres = edid_timing_horizontal_active(dtb);
    const uint16_t yres = edid_timing_vertical_active(dtb);
    const uint32_t pixclk = edid_timing_pixel_clock(dtb);
    const uint16_t lower_margin = edid_timing_vertical_sync_offset(dtb);
    const uint16_t right_margin = edid_timing_horizontal_sync_offset(dtb);

    asprintf(&modestr,
             "\"%ux%u\" %.3f %u %u %u %u %u %u %u %u %chsync %cvsync",
             /* resolution */
             xres, yres,

             /* doct clock frequence (MHz) */
             pixclk / 1000000.0,

             /* horizontal timings */
             xres,
             xres + right_margin,
             xres + right_margin + edid_timing_horizontal_sync_pulse_width(dtb),
             xres + edid_timing_horizontal_blanking(dtb),

             /* vertical timings */
             yres,
             yres + lower_margin,
             yres + lower_margin + edid_timing_vertical_sync_pulse_width(dtb),
             yres + edid_timing_vertical_blanking(dtb),

             /* sync direction */
             dtb->signal_pulse_polarity ? '+' : '-',
             dtb->signal_serration_polarity ? '+' : '-');

    return modestr;
}

static void
parse_edid(const struct edid * const edid)
{
    char manufacturer[4] = {0};
    char monitor_model_name[14] = {0};
    char monitor_serial_number[14] = {0};
    const struct edid_monitor_range_limits *monitor_range_limits = NULL;
    struct edid_color_characteristics_data characteristics;
    const uint8_t vlen = edid->maximum_vertical_image_size;
    const uint8_t hlen = edid->maximum_horizontal_image_size;
    uint8_t i;

    static const char * const display_type[] = {
        [EDID_DISPLAY_TYPE_MONOCHROME] = "Monochrome or greyscale",
        [EDID_DISPLAY_TYPE_RGB]        = "sRGB colour",
        [EDID_DISPLAY_TYPE_NON_RGB]    = "Non-sRGB colour",
        [EDID_DISPLAY_TYPE_UNDEFINED]  = "Undefined",
    };

    edid_manufacturer(edid, manufacturer);
    characteristics = edid_color_characteristics(edid);


    for (i = 0; i < ARRAY_SIZE(edid->detailed_timings); i++) {
        const struct edid_monitor_descriptor * const desc =
            &edid->detailed_timings[i].monitor;

        if (desc->flag0 != 0x0000 || desc->flag1 != 0x00 || desc->flag2 != 0x00)
            continue;

        switch (desc->tag) {
        case EDID_MONITOR_DESCRIPTOR_ASCII_STRING:
            /* This is an arbitrary string, unless we can identify it, just
               silently ignore it. */
            break;
        case EDID_MONITOR_DESCRIPTOR_MONITOR_NAME:
            strncpy(monitor_model_name, (char *) desc->data,
                    sizeof(monitor_model_name) - 1);
            *strchrnul(monitor_model_name, '\n') = '\0';
            break;
        case EDID_MONITOR_DESCRIPTOR_MONITOR_RANGE_LIMITS:
            monitor_range_limits = (struct edid_monitor_range_limits *) &desc->data;
            break;
        case EDID_MONITOR_DESCRIPTOR_MONITOR_SERIAL_NUMBER:
            strncpy(monitor_serial_number, (char *) desc->data,
                    sizeof(monitor_serial_number) - 1);
            *strchrnul(monitor_serial_number, '\n') = '\0';
            break;
        default:
            fprintf(stderr, "unknown monitor descriptor type 0x%02x\n",
                    desc->tag);
            break;
        }
    }

    printf("Monitor\n");

    printf("  Model name............... %s\n",
           *monitor_model_name ? monitor_model_name : "n/a");

    printf("  Manufacturer............. %s\n",
           manufacturer);

    printf("  Product code............. %u\n",
           *(uint16_t *) edid->product);

    printf("  Product serial........... %u\n",
           *(uint32_t *) edid->serial_number);

#if defined(DISPLAY_UNKNOWN)
    printf("  Plug and Play ID......... %s\n", NULL);
#endif

    printf("  Serial number............ %s\n",
           *monitor_serial_number ? monitor_serial_number : "n/a");

    printf("  Manufacture date......... %u, ISO week %u\n",
           edid->manufacture_year + 1990, edid->manufacture_week);

    printf("  EDID revision............ %u.%u\n",
           edid->version, edid->revision);

    printf("  Input signal type........ %s\n",
           edid->video_input_definition.digital.digital ? "Digital" : "Analog");

    if (edid->video_input_definition.digital.digital) {
        printf("  VESA DFP 1.x supported... %s\n",
               edid->video_input_definition.digital.dfp_1x ? "Yes" : "No");
    } else {
        /* TODO print analog flags */
    }

#if defined(DISPLAY_UNKNOWN)
    printf("  Color bit depth.......... %s\n", NULL);
#endif

    printf("  Display type............. %s\n",
           display_type[edid->feature_support.display_type]);

    printf("  Screen size.............. %u mm x %u mm (%.1f in)\n",
           CM_2_MM(hlen), CM_2_MM(vlen),
           CM_2_IN(sqrt(hlen * hlen + vlen * vlen)));

    printf("  Power management......... %s%s%s%s\n",
           edid->feature_support.active_off ? "Active off, " : "",
           edid->feature_support.suspend ? "Suspend, " : "",
           edid->feature_support.standby ? "Standby, " : "",

           (edid->feature_support.active_off ||
            edid->feature_support.suspend    ||
            edid->feature_support.standby) ? "\b\b  " : "n/a");

    printf("  Extension blocks......... %u\n",
           edid->extensions);

#if defined(DISPLAY_UNKNOWN)
    printf("  DDC/CI................... %s\n", NULL);
#endif

    printf("\n");

    printf("Color characteristics\n");

    printf("  Default color space...... %ssRGB\n",
           edid->feature_support.standard_default_color_space ? "" : "Non-");

    printf("  Display gamma............ %.2f\n",
           edid_gamma(edid));

    printf("  Red chromaticity......... Rx %0.3f - Ry %0.3f\n",
           _edid_fraction(characteristics.red.x),
           _edid_fraction(characteristics.red.y));

    printf("  Green chromaticity....... Gx %0.3f - Gy %0.3f\n",
           _edid_fraction(characteristics.green.x),
           _edid_fraction(characteristics.green.y));

    printf("  Blue chromaticity........ Bx %0.3f - By %0.3f\n",
           _edid_fraction(characteristics.blue.x),
           _edid_fraction(characteristics.blue.y));

    printf("  White point (default).... Wx %0.3f - Wy %0.3f\n",
           _edid_fraction(characteristics.white.x),
           _edid_fraction(characteristics.white.y));

#if defined(DISPLAY_UNKNOWN)
    printf("  Additional descriptors... %s\n", NULL);
#endif

    printf("\n");

    printf("Timing characteristics\n");

    if (monitor_range_limits) {
        printf("  Horizontal scan range.... %u - %u kHz\n",
               monitor_range_limits->minimum_horizontal_rate,
               monitor_range_limits->maximum_horizontal_rate);

        printf("  Vertical scan range...... %u - %u Hz\n",
               monitor_range_limits->minimum_vertical_rate,
               monitor_range_limits->maximum_vertical_rate);

        printf("  Video bandwidth.......... %u MHz\n",
               monitor_range_limits->maximum_supported_pixel_clock * 10);
    }

#if defined(DISPLAY_UNKNOWN)
    printf("  CVT standard............. %s\n", NULL);
#endif

    printf("  GTF standard............. %sSupported\n",
           edid->feature_support.default_gtf ? "" : "Not ");

#if defined(DISPLAY_UNKNOWN)
    printf("  Additional descriptors... %s\n", NULL);
#endif

    printf("  Preferred timing......... %s\n",
           edid->feature_support.preferred_timing_mode ? "Yes" : "No");

    if (edid->feature_support.preferred_timing_mode) {
        char *string = NULL;

        string = _edid_timing_string(&edid->detailed_timings[0].timing);
        printf("  Native/preferred timing.. %s\n", string);
        free(string);

        string = _edid_mode_string(&edid->detailed_timings[0].timing);
        printf("    Modeline............... %s\n", string);
        free(string);
    } else {
        printf("  Native/preferred timing.. n/a\n");
    }

    printf("\n");

    printf("Standard timings supported\n");
    if (edid->established_timings.timing_720x400_70)
        printf("   720 x  400p @ 70Hz - IBM VGA\n");
    if (edid->established_timings.timing_720x400_88)
        printf("   720 x  400p @ 88Hz - IBM XGA2\n");
    if (edid->established_timings.timing_640x480_60)
        printf("   640 x  480p @ 60Hz - IBM VGA\n");
    if (edid->established_timings.timing_640x480_67)
        printf("   640 x  480p @ 67Hz - Apple Mac II\n");
    if (edid->established_timings.timing_640x480_72)
        printf("   640 x  480p @ 72Hz - VESA\n");
    if (edid->established_timings.timing_640x480_75)
        printf("   640 x  480p @ 75Hz - VESA\n");
    if (edid->established_timings.timing_800x600_56)
        printf("   800 x  600p @ 56Hz - VESA\n");
    if (edid->established_timings.timing_800x600_60)
        printf("   800 x  600p @ 60Hz - VESA\n");

    if (edid->established_timings.timing_800x600_72)
        printf("   800 x  600p @ 72Hz - VESA\n");
    if (edid->established_timings.timing_800x600_75)
        printf("   800 x  600p @ 75Hz - VESA\n");
    if (edid->established_timings.timing_832x624_75)
        printf("   832 x  624p @ 75Hz - Apple Mac II\n");
    if (edid->established_timings.timing_1024x768_87)
        printf("  1024 x  768i @ 87Hz - VESA\n");
    if (edid->established_timings.timing_1024x768_60)
        printf("  1024 x  768p @ 60Hz - VESA\n");
    if (edid->established_timings.timing_1024x768_70)
        printf("  1024 x  768p @ 70Hz - VESA\n");
    if (edid->established_timings.timing_1024x768_75)
        printf("  1024 x  768p @ 75Hz - VESA\n");
    if (edid->established_timings.timing_1280x1024_75)
        printf("  1280 x 1024p @ 75Hz - VESA\n");

    for (i = 0; i < ARRAY_SIZE(edid->standard_timing_id); i++) {
        const struct edid_standard_timing_descriptor * const desc =
            &edid->standard_timing_id[i];

        uint16_t hres, vres;

        if (!memcmp(desc, EDID_STANDARD_TIMING_DESCRIPTOR_INVALID, sizeof(*desc)))
            continue;

        hres = (desc->horizontal_active_pixels + 31) << 3;

        switch (desc->image_aspect_ratio) {
        case EDID_ASPECT_RATIO_16_10:
            vres = (hres * (10.0 / 16.0));
            break;
        case EDID_ASPECT_RATIO_4_3:
            vres = (hres * (3.0 / 4.0));
            break;
        case EDID_ASPECT_RATIO_5_4:
            vres = (hres * (4.0 / 5.0));
            break;
        case EDID_ASPECT_RATIO_16_9:
            vres = (hres * (9.0 / 16.0));
            break;
        }

        printf("  %4u x %4u%c @ %uHz - VESA STD\n",
               hres, vres, 'p', desc->refresh_rate + 60);
    }
}

static void
dump_cea_timing_block(const struct edid_extension * const ext)
{
    const struct cea861_timing_block * const ctb = (struct cea861_timing_block *) ext;
    const uint8_t offset = offsetof(struct cea861_timing_block, data);
    const struct edid_detailed_timing_descriptor *dtd = NULL;
    uint8_t i;

    printf("%40s %02x %02x %02x %02x\n",
           "cea extension header:",
           ctb->tag, ctb->revision, ctb->dtd_offset, ext->extension_data[1]);

    printf("%40s ", "data block collection:");
    for (i = 0; i < ctb->dtd_offset - offset; i++) {
        if (((i * 3) % 78) == 39)
            printf("\n%40s ", "");
        printf("%02x ", ctb->data[i]);
    }
    printf("\n");

    dtd = (struct edid_detailed_timing_descriptor *) ((uint8_t *) ctb + ctb->dtd_offset);
    for (i = 0; dtd->pixel_clock; i++, dtd++) {
        const uint8_t * const data = (uint8_t *) dtd;

        printf("%35s %03u: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n",
               "detailed timing descriptor", i,
               data[0x00], data[0x01], data[0x02], data[0x03], data[0x04],
               data[0x05], data[0x06], data[0x07], data[0x08], data[0x09],
               data[0x0a], data[0x0b], data[0x0c], data[0x0d], data[0x0e],
               data[0x0f], data[0x10], data[0x11]);
    }

    printf("%40s ", "padding:");
    for (i = sizeof(*dtd) * i + ctb->dtd_offset; i < sizeof(ctb->data); i++) {
        if (((i * 3) % 78) == 39)
            printf("\n%40s ", "");
        printf("%02x ", ctb->data[i]);
    }
    printf("\n");

    printf("%40s %02x", "checksum:", ctb->checksum);
}

static void
parse_cea_timing_block(const struct edid_extension * const ext)
{
    const struct edid_detailed_timing_descriptor *dtd = NULL;
    const struct cea861_timing_block * const ctb = (struct cea861_timing_block *) ext;
    const uint8_t offset = offsetof(struct cea861_timing_block, data);
    uint8_t index = 0, i;

    printf("CEA-861 Information\n");
    printf("  Revision number.......... %u\n",
           ctb->revision);
    printf("  IT underscan............. %supported\n",
           ctb->underscan_supported ? "S" : "Not s");
    printf("  Basic audio.............. %supported\n",
           ctb->basic_audio_supported ? "S" : "Not s");
    printf("  YCbCr 4:4:4.............. %supported\n",
           ctb->yuv_444_supported ? "S" : "Not s");
    printf("  YCbCr 4:2:2.............. %supported\n",
           ctb->yuv_422_supported ? "S" : "Not s");
    printf("  Native formats........... %u\n",
           ctb->native_dtds);

    dtd = (struct edid_detailed_timing_descriptor *) ((uint8_t *) ctb + ctb->dtd_offset);
    for (i = 0; dtd->pixel_clock; i++, dtd++) {
        char *string;

        string = _edid_timing_string(dtd);
        printf("  Detailed timing #%u....... %s\n", i + 1, string);
        free(string);

        string = _edid_mode_string(dtd);
        printf("    Modeline............... %s\n", string);
        free(string);
    }

    printf("\n");

    do {
        const struct cea861_data_block_header * const header =
            (struct cea861_data_block_header *) &ctb->data[index];

        switch (header->tag) {
        case CEA861_DATA_BLOCK_TYPE_AUDIO:
            {
                const struct cea861_audio_data_block * const adb =
                    (struct cea861_audio_data_block *) header;

                printf("CE audio data (formats supported)\n");
                for (i = 0; i < header->length / sizeof(*adb->sad); i++) {
                    const struct cea861_short_audio_descriptor * const sad =
                        (struct cea861_short_audio_descriptor *) &adb->sad[i];

                    switch (sad->audio_format) {
                    case CEA861_AUDIO_FORMAT_LPCM:
                        printf("  LPCM    %u-channel, %s%s%s\b bit depths at %s%s%s%s%s%s%s\b kHz\n",
                               sad->channels + 1,
                               sad->flags.lpcm.bitrate_16_bit ? "16/" : "",
                               sad->flags.lpcm.bitrate_20_bit ? "20/" : "",
                               sad->flags.lpcm.bitrate_24_bit ? "24/" : "",
                               sad->sample_rate_32_kHz ? "32/" : "",
                               sad->sample_rate_44_1_kHz ? "44.1/" : "",
                               sad->sample_rate_48_kHz ? "48/" : "",
                               sad->sample_rate_88_2_kHz ? "88.2/" : "",
                               sad->sample_rate_96_kHz ? "96/" : "",
                               sad->sample_rate_176_4_kHz ? "176.4/" : "",
                               sad->sample_rate_192_kHz ? "192/" : "");
                        break;
                    case CEA861_AUDIO_FORMAT_AC_3:
                        printf("  AC-3    %u-channel, %4uk max. bit rate at %s%s%s%s%s%s%s\b kHz\n",
                               sad->channels + 1,
                               (sad->flags.maximum_bit_rate << 3),
                               sad->sample_rate_32_kHz ? "32/" : "",
                               sad->sample_rate_44_1_kHz ? "44.1/" : "",
                               sad->sample_rate_48_kHz ? "48/" : "",
                               sad->sample_rate_88_2_kHz ? "88.2/" : "",
                               sad->sample_rate_96_kHz ? "96/" : "",
                               sad->sample_rate_176_4_kHz ? "176.4/" : "",
                               sad->sample_rate_192_kHz ? "192/" : "");
                        break;
                    default:
                        fprintf(stderr, "unknown audio format 0x%02x\n",
                                sad->audio_format);
                        break;
                    }
                }
            }
            break;
        case CEA861_DATA_BLOCK_TYPE_VIDEO:
            {
                const struct cea861_video_data_block * const vdb =
                    (struct cea861_video_data_block *) header;

                printf("CE video identifiers (VICs) - timing/formats supported\n");
                for (i = 0; i < header->length; i++) {
                    const struct cea861_timing * const timing =
                        &cea861_timings[vdb->svd[i].video_identification_code];

                    printf(" %s CEA Mode %02u: %4u x %4u%c @ %.fHz\n",
                           vdb->svd[i].native ? "*" : " ",
                           vdb->svd[i].video_identification_code,
                           timing->hactive, timing->vactive,
                           (timing->mode == INTERLACED) ? 'i' : 'p',
                           timing->vfreq);
                }
            }
            break;
        case CEA861_DATA_BLOCK_TYPE_VENDOR_SPECIFIC:
            {
                const struct cea861_vendor_specific_data_block * const vsdb =
                    (struct cea861_vendor_specific_data_block *) header;
                const uint8_t oui[] = { vsdb->ieee_registration[2],
                                        vsdb->ieee_registration[1],
                                        vsdb->ieee_registration[0] };

                printf("CEA vendor specific data (VSDB)\n");
                printf("  IEEE registration number. 0x%02X%02X%02X\n",
                       vsdb->ieee_registration[2], vsdb->ieee_registration[1],
                       vsdb->ieee_registration[0]);

                if (!memcmp(oui, HDMI_OUI, sizeof(HDMI_OUI))) {
                    const struct hdmi_vendor_specific_data_block * const hdmi =
                        (struct hdmi_vendor_specific_data_block *) vsdb;

                    printf("  CEC physical address..... %u.%u.%u.%u\n",
                           hdmi->port_configuration_a,
                           hdmi->port_configuration_b,
                           hdmi->port_configuration_c,
                           hdmi->port_configuration_d);

                    if (header->length >= HDMI_VSDB_EXTENSION_FLAGS_OFFSET) {
                        printf("  Supports AI (ACP, ISRC).. %s\n",
                               hdmi->audio_info_frame ? "Yes" : "No");
                        printf("  Supports 48bpp........... %s\n",
                               hdmi->colour_depth_48_bit ? "Yes" : "No");
                        printf("  Supports 36bpp........... %s\n",
                               hdmi->colour_depth_36_bit ? "Yes" : "No");
                        printf("  Supports 30bpp........... %s\n",
                               hdmi->colour_depth_30_bit ? "Yes" : "No");
                        printf("  Supports YCbCr 4:4:4..... %s\n",
                               hdmi->yuv_444_supported ? "Yes" : "No");
                        printf("  Supports dual-link DVI... %s\n",
                               hdmi->dvi_dual_link ? "Yes" : "No");
                    }

                    if (header->length >= HDMI_VSDB_MAX_TMDS_OFFSET) {
                        if (hdmi->max_tmds_clock)
                            printf("  Maximum TMDS clock....... %uMHz\n",
                                   hdmi->max_tmds_clock * 5);
                        else
                            printf("  Maximum TMDS clock....... n/a\n");
                    }

                    if (header->length >= HDMI_VSDB_LATENCY_FIELDS_OFFSET) {
                        if (hdmi->latency_fields) {
                            printf("  Video latency %s........ %ums\n",
                                   hdmi->interlaced_latency_fields ? "(p)" : "...",
                                   (hdmi->video_latency - 1) << 1);
                            printf("  Audio latency %s........ %ums\n",
                                   hdmi->interlaced_latency_fields ? "(p)" : "...",
                                   (hdmi->audio_latency - 1) << 1);
                        }

                        if (hdmi->interlaced_latency_fields) {
                            printf("  Video latency (i)........ %ums\n",
                                   hdmi->interlaced_video_latency);
                            printf("  Audio latency (i)........ %ums\n",
                                   hdmi->interlaced_audio_latency);
                        }
                    }
                }
            }
            break;
        case CEA861_DATA_BLOCK_TYPE_SPEAKER_ALLOCATION:
            {
                const struct cea861_speaker_allocation_data_block * const sadb =
                    (struct cea861_speaker_allocation_data_block *) header;
                const struct cea861_speaker_allocation * const sa =
                    &sadb->payload;
                const uint8_t * const channel_configuration = (uint8_t *) sa;

                printf("CEA speaker allocation data\n");
                printf("  Channel configuration.... %u.%u\n",
                       (__builtin_popcountll(channel_configuration[0] & 0xe9) << 1) +
                       (__builtin_popcountll(channel_configuration[0] & 0x14) << 0) +
                       (__builtin_popcountll(channel_configuration[1] & 0x01) << 1) +
                       (__builtin_popcountll(channel_configuration[1] & 0x06) << 0),
                       (channel_configuration[0] & 0x02));
                printf("  Front left/right......... %s\n",
                       sa->front_left_right ? "Yes" : "No");
                printf("  Front LFE................ %s\n",
                       sa->front_lfe ? "Yes" : "No");
                printf("  Front center............. %s\n",
                       sa->front_center ? "Yes" : "No");
                printf("  Rear left/right.......... %s\n",
                       sa->rear_left_right ? "Yes" : "No");
                printf("  Rear center.............. %s\n",
                       sa->rear_center ? "Yes" : "No");
                printf("  Front left/right center.. %s\n",
                       sa->front_left_right_center ? "Yes" : "No");
                printf("  Rear left/right center... %s\n",
                       sa->rear_left_right_center ? "Yes" : "No");
                printf("  Front left/right wide.... %s\n",
                       sa->front_left_right_wide ? "Yes" : "No");
                printf("  Front left/right high.... %s\n",
                       sa->front_left_right_high ? "Yes" : "No");
                printf("  Top center............... %s\n",
                       sa->top_center ? "Yes" : "No");
                printf("  Front center high........ %s\n",
                       sa->front_center_high ? "Yes" : "No");
            }
            break;
        default:
            fprintf(stderr, "unknown CEA-861 data block type 0x%02x\n",
                    header->tag);
            break;
        }

        printf("\n");

        index = index + header->length + sizeof(*header);
    } while (index < ctb->dtd_offset - offset);
}

static void
dump_edid_data(const uint8_t * const data)
{
    uint8_t i;
    const struct edid * const edid = (struct edid *) data;
    const struct edid_extension * const extensions =
        (struct edid_extension *) (data + sizeof(*edid));

    dump_edid(edid);
    printf("\n");
    parse_edid(edid);

    printf("\n");

    for (i = 0; i < edid->extensions; i++) {
        switch (extensions[i].tag) {
        case EDID_EXTENSION_TIMING:
            printf("edid block %u is an extension block (type: %#04x)\n",
                   i + 1, extensions[i].tag);
            break;
        case EDID_EXTENSION_CEA:
            dump_cea_timing_block(&extensions[i]);
            printf("\n");
            parse_cea_timing_block(&extensions[i]);
            break;
        case EDID_EXTENSION_VTB:
        case EDID_EXTENSION_EDID_2_0:
        case EDID_EXTENSION_DI:
        case EDID_EXTENSION_LS:
        case EDID_EXTENSION_MI:
        case EDID_EXTENSION_DTCDB_1:
        case EDID_EXTENSION_DTCDB_2:
        case EDID_EXTENSION_DTCDB_3:
        case EDID_EXTENSION_DDDB:
            printf("edid block %u is an extension block (type: %#04x)\n",
                   i + 1, extensions[i].tag);
            break;
        case EDID_EXTENSION_BLOCK_MAP:
            printf("edid block %u is a block map\n", i + 1);
            break;
        default:
            printf("WARNING: Unknown extension %#04x at block %u\n",
                   extensions[i].tag, i + 1);
            break;
        }
    }
}

int
main(int argc, char **argv)
{
    FILE *edid = NULL;
    uint8_t *buffer = NULL;
    long length = 0;

    if (argc != 2) {
        printf("usage: %s <edid data file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    edid = fopen(argv[1], "rb");
    if (!edid) {
        perror("fopen");
        return EXIT_FAILURE;
    }

    fseek(edid, 0, SEEK_END);
    length = ftell(edid);
    fseek(edid, 0, SEEK_SET);

    buffer = calloc(length, 1);
    if (!buffer) {
        fclose(edid);
        fprintf(stderr, "unable to allocate space for edid data\n");
        return EXIT_FAILURE;
    }

    if (fread(buffer, 1, length, edid) != length) {
        perror("fread");
        free(buffer);
        fclose(edid);
        return EXIT_FAILURE;
    }

    fclose(edid);

    dump_edid_data(buffer);

    free(buffer);
    return EXIT_SUCCESS;
}

