/*
 * ---------------------------------------------------------------------------
 *  Copyright (c) 2012 Qualcomm Technologies, Inc.  All Rights Reserved.
 *  Qualcomm Technologies Proprietary and Confidential.
 * ---------------------------------------------------------------------------
 *
 */

#include <stdio.h>
#include <linux/input.h>
#include <fcntl.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define PWR_KEY_DEVICE "/dev/input/event0"
#define USEC_IN_SEC 1000000
#define SHUTDOWN_TIME_US 400000
#define NO_ERROR 0
#define ERROR -1

#define LOGI(...) fprintf(stderr, "I:" __VA_ARGS__)

int time_diff_us(struct timeval *press, struct timeval *release)
{
        int time;
        if (release->tv_usec > press->tv_usec) {
                release->tv_usec += USEC_IN_SEC;
                release->tv_sec--;
        }
        time = (int) (release->tv_sec - press->tv_sec) * USEC_IN_SEC +
                release->tv_usec - press->tv_usec;
        return time;
}

int main(int argc, char* argv[])
{
        int rc, fd, read_size, keypress=0;
        struct input_event event;
        struct timeval press, release;

        /* wait for power key event */
        fd = open(PWR_KEY_DEVICE, O_RDONLY);
        if (fd < 0) {
                LOGI("open pwr key device failed: %s\n", PWR_KEY_DEVICE);
                return ERROR;
        }

        memset(&press, 0, sizeof(struct timeval));
        memset(&release, 0, sizeof(struct timeval));

        while (1) {

                read_size = read(fd, &event, sizeof(struct input_event));
                if (read_size < sizeof(struct input_event)) {
                        LOGI("read input event error!\n");
                        return ERROR;
                }

                if (event.type == EV_KEY && event.code == KEY_POWER) {
                        LOGI("ger event KEY_POWER, event.value: %d.\n", event.value);
                        if (event.value == 1){
                                memcpy(&press, &event.time, sizeof(struct timeval));
                                keypress=1;
                        }  else {
                                memcpy(&release, &event.time, sizeof(struct timeval));
                                /* Sometimes timeval is negative when reading the
                                 * event time from pmic. Add a check to process suspend or
                                 * shutdown request only when the values are non-negative.
                                 */
                                if((press.tv_sec >= 0) && (release.tv_sec >= 0) && (keypress==1)) {
                                        keypress=0;
                                        LOGI("key event time: %d.\n", time_diff_us(&press, &release));
                                        if (time_diff_us(&press, &release) < SHUTDOWN_TIME_US) {

                                                LOGI("short time press, do nothing.\n");

                                        } else {
                                                LOGI("long time press, shut down.\n");
                                                break;

                                        }
                                }
                        }
                }
        }

        system("halt");

        return 0;
}
