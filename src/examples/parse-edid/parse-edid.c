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
#include <stdlib.h>
#include <string.h>

#include <leds/edid.h>

#define CM_2_MM(cm)                             ((cm) * 10)
#define CM_2_IN(cm)                             ((cm) * 0.3937)

#define ARRAY_SIZE(arr)                         (sizeof(arr) / sizeof((arr)[0]))

static unsigned int
get_block_count(FILE *edid)
{
    struct edid block0;
    size_t retval;

    if ((retval = fread(&block0, sizeof(block0), 1, edid)) != 1) {
        if (feof(edid))
            fprintf(stderr, "short read: edid data less than 1 block\n");
        else
            perror("fread");

        return 0;
    }

    return block0.extensions + 1;
}

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

    printf("  Power management......... %s%s%s\n",
           edid->feature_support.active_off ? "Active off " : "",
           edid->feature_support.suspend ? "Suspend " : "",
           edid->feature_support.standby ? "Standby " : "");

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
    unsigned int blocks;

    if (argc != 2) {
        printf("usage: %s <edid data file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    edid = fopen(argv[1], "rb");
    if (!edid) {
        perror("fopen");
        return EXIT_FAILURE;
    }

    blocks = get_block_count(edid);
    if (!blocks) {
        fclose(edid);
        fprintf(stderr, "failed to read edid block0\n");
        return EXIT_FAILURE;
    }

    rewind(edid);

    buffer = calloc(blocks, EDID_BLOCK_SIZE);
    if (!buffer) {
        fclose(edid);
        fprintf(stderr, "unable to allocate space for edid data\n");
        return EXIT_FAILURE;
    }

    if (fread(buffer, EDID_BLOCK_SIZE, blocks, edid) != blocks) {
        free(buffer);
        fclose(edid);
        perror("fread");
        return EXIT_FAILURE;
    }

    dump_edid_data(buffer);

    free(buffer);
    return EXIT_SUCCESS;
}

