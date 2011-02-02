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

#include <stdio.h>
#include <stdlib.h>

#include <leds/edid.h>

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

static void
dump_edid_data(const uint8_t * const data)
{
    uint8_t i;
    const struct edid * const edid = (struct edid *) data;
    const struct edid_extension * const extensions =
        (struct edid_extension *) (data + sizeof(*edid));

    dump_edid(edid);

    printf("\n");

    for (i = 0; i < edid->extensions; i++)
        switch (extensions[i].tag) {
            case EDID_EXTENSION_TIMING:
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

