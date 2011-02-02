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
    struct edid_block0 block0;
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
dump_edid_block0(const struct edid_block0 * const block0)
{
    printf("%40s %02x %02x %02x %02x %02x %02x %02x %02x\n",
           "header:",
           block0->header[0], block0->header[1], block0->header[2],
           block0->header[3], block0->header[4], block0->header[5],
           block0->header[6], block0->header[7]);
    printf("%40s %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n",
           "vendor/product identification:",
           ((uint8_t *) &block0->manufacturer)[0],
           ((uint8_t *) &block0->manufacturer)[1],
           block0->product[0], block0->product[1],
           block0->serial_number[0], block0->serial_number[1],
           block0->serial_number[2], block0->serial_number[3],
           block0->manufacture_week, block0->manufacture_year);
    printf("%40s %02x %02x\n",
           "edid structure version/revision:",
           block0->version, block0->revision);
    printf("%40s %02x %02x %02x %02x %02x\n",
           "basic display parameters/features:",
           block0->video_input_definition,
           block0->maximum_horizontal_image_size,
           block0->maximum_vertical_image_size,
           block0->display_transfer_characteristics,
           block0->feature_support);
    printf("%40s %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n",
           "color characteristics:",

           block0->red_x_low   << 6 | block0->red_y_low   << 4 |
           block0->green_x_low << 2 | block0->green_y_low << 0,

           block0->blue_x_low  << 6 | block0->blue_y_low  << 4 |
           block0->white_x_low << 2 | block0->white_y_low << 0,

           block0->red_x, block0->red_y, block0->green_x, block0->green_y,
           block0->blue_x, block0->blue_y, block0->white_x, block0->white_y);
    printf("%40s %02x %02x %02x\n",
           "established timings:",
           ((uint8_t *) &block0->established_timings)[0],
           ((uint8_t *) &block0->established_timings)[1],
           block0->manufacturer_timings);
    printf("%40s %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n",
           "standard timing identification:",
           ((uint8_t *) &block0->standard_timing_id[0])[0],
           ((uint8_t *) &block0->standard_timing_id[0])[1],
           ((uint8_t *) &block0->standard_timing_id[1])[0],
           ((uint8_t *) &block0->standard_timing_id[1])[1],
           ((uint8_t *) &block0->standard_timing_id[2])[0],
           ((uint8_t *) &block0->standard_timing_id[2])[1],
           ((uint8_t *) &block0->standard_timing_id[3])[0],
           ((uint8_t *) &block0->standard_timing_id[3])[1],
           ((uint8_t *) &block0->standard_timing_id[4])[0],
           ((uint8_t *) &block0->standard_timing_id[4])[1],
           ((uint8_t *) &block0->standard_timing_id[5])[0],
           ((uint8_t *) &block0->standard_timing_id[5])[1],
           ((uint8_t *) &block0->standard_timing_id[6])[0],
           ((uint8_t *) &block0->standard_timing_id[6])[1],
           ((uint8_t *) &block0->standard_timing_id[7])[0],
           ((uint8_t *) &block0->standard_timing_id[7])[1]);
    printf("%40s %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n",
           "detailed timing 0:",
           ((uint8_t *) &block0->detailed_timings[0])[0],
           ((uint8_t *) &block0->detailed_timings[0])[1],
           ((uint8_t *) &block0->detailed_timings[0])[2],
           ((uint8_t *) &block0->detailed_timings[0])[3],
           ((uint8_t *) &block0->detailed_timings[0])[4],
           ((uint8_t *) &block0->detailed_timings[0])[5],
           ((uint8_t *) &block0->detailed_timings[0])[6],
           ((uint8_t *) &block0->detailed_timings[0])[7],
           ((uint8_t *) &block0->detailed_timings[0])[8],
           ((uint8_t *) &block0->detailed_timings[0])[9],
           ((uint8_t *) &block0->detailed_timings[0])[10],
           ((uint8_t *) &block0->detailed_timings[0])[11],
           ((uint8_t *) &block0->detailed_timings[0])[12],
           ((uint8_t *) &block0->detailed_timings[0])[13],
           ((uint8_t *) &block0->detailed_timings[0])[14],
           ((uint8_t *) &block0->detailed_timings[0])[15],
           ((uint8_t *) &block0->detailed_timings[0])[16],
           ((uint8_t *) &block0->detailed_timings[0])[17]);
    printf("%40s %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n",
           "detailed timing 1:",
           ((uint8_t *) &block0->detailed_timings[1])[0],
           ((uint8_t *) &block0->detailed_timings[1])[1],
           ((uint8_t *) &block0->detailed_timings[1])[2],
           ((uint8_t *) &block0->detailed_timings[1])[3],
           ((uint8_t *) &block0->detailed_timings[1])[4],
           ((uint8_t *) &block0->detailed_timings[1])[5],
           ((uint8_t *) &block0->detailed_timings[1])[6],
           ((uint8_t *) &block0->detailed_timings[1])[7],
           ((uint8_t *) &block0->detailed_timings[1])[8],
           ((uint8_t *) &block0->detailed_timings[1])[9],
           ((uint8_t *) &block0->detailed_timings[1])[10],
           ((uint8_t *) &block0->detailed_timings[1])[11],
           ((uint8_t *) &block0->detailed_timings[1])[12],
           ((uint8_t *) &block0->detailed_timings[1])[13],
           ((uint8_t *) &block0->detailed_timings[1])[14],
           ((uint8_t *) &block0->detailed_timings[1])[15],
           ((uint8_t *) &block0->detailed_timings[1])[16],
           ((uint8_t *) &block0->detailed_timings[1])[17]);
    printf("%40s %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n",
           "detailed timing 2:",
           ((uint8_t *) &block0->detailed_timings[2])[0],
           ((uint8_t *) &block0->detailed_timings[2])[1],
           ((uint8_t *) &block0->detailed_timings[2])[2],
           ((uint8_t *) &block0->detailed_timings[2])[3],
           ((uint8_t *) &block0->detailed_timings[2])[4],
           ((uint8_t *) &block0->detailed_timings[2])[5],
           ((uint8_t *) &block0->detailed_timings[2])[6],
           ((uint8_t *) &block0->detailed_timings[2])[7],
           ((uint8_t *) &block0->detailed_timings[2])[8],
           ((uint8_t *) &block0->detailed_timings[2])[9],
           ((uint8_t *) &block0->detailed_timings[2])[10],
           ((uint8_t *) &block0->detailed_timings[2])[11],
           ((uint8_t *) &block0->detailed_timings[2])[12],
           ((uint8_t *) &block0->detailed_timings[2])[13],
           ((uint8_t *) &block0->detailed_timings[2])[14],
           ((uint8_t *) &block0->detailed_timings[2])[15],
           ((uint8_t *) &block0->detailed_timings[2])[16],
           ((uint8_t *) &block0->detailed_timings[2])[17]);
    printf("%40s %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n",
           "detailed timing 3:",
           ((uint8_t *) &block0->detailed_timings[3])[0],
           ((uint8_t *) &block0->detailed_timings[3])[1],
           ((uint8_t *) &block0->detailed_timings[3])[2],
           ((uint8_t *) &block0->detailed_timings[3])[3],
           ((uint8_t *) &block0->detailed_timings[3])[4],
           ((uint8_t *) &block0->detailed_timings[3])[5],
           ((uint8_t *) &block0->detailed_timings[3])[6],
           ((uint8_t *) &block0->detailed_timings[3])[7],
           ((uint8_t *) &block0->detailed_timings[3])[8],
           ((uint8_t *) &block0->detailed_timings[3])[9],
           ((uint8_t *) &block0->detailed_timings[3])[10],
           ((uint8_t *) &block0->detailed_timings[3])[11],
           ((uint8_t *) &block0->detailed_timings[3])[12],
           ((uint8_t *) &block0->detailed_timings[3])[13],
           ((uint8_t *) &block0->detailed_timings[3])[14],
           ((uint8_t *) &block0->detailed_timings[3])[15],
           ((uint8_t *) &block0->detailed_timings[3])[16],
           ((uint8_t *) &block0->detailed_timings[3])[17]);
    printf("%40s %02x\n", "extensions:", block0->extensions);
    printf("%40s %02x\n", "checksum:", block0->extensions);
}

static void
dump_edid_data(const uint8_t * const data)
{
    dump_edid_block0((struct edid_block0 *) data);
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

