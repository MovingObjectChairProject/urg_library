/*!
  \~japanese
  \example get_multiecho_intensity.c 距離・強度データ(マルチエコー)を取得する

  \author Satofumi KAMIMURA

  $Id$
*/

#include "urg_sensor.h"
#include "urg_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static void print_echo_data(long data[], unsigned short intensity[],
                            int index)
{
    int i;

    // [mm]
    for (i = 0; i < URG_MAX_ECHO; ++i) {
        printf("%ld, ", data[(URG_MAX_ECHO * index) + i]);
    }

    // [1]
    for (i = 0; i < URG_MAX_ECHO; ++i) {
        printf("%d, ", intensity[(URG_MAX_ECHO * index) + i]);
    }
}


// \~japanese 距離、強度のデータを表示する
static void print_data(urg_t *urg, long data[],
                       unsigned short intensity[], int data_n, long time_stamp)
{
#if 1
    (void)data_n;

    // \~japanese 前方のデータのみを表示
    int front_index = urg_step2index(urg, 0);
    print_echo_data(data, intensity, front_index);
    printf("%ld\n", time_stamp);

#else
    (void)urg;
    int i;

    // \~japanese 全てのデータを表示
    printf("# n = %d, time_stamp = %ld\n", data_n, time_stamp);
    for (i = 0; i < data_n; ++i) {
        print_echo_data(data, intensity, i);
        printf("\n");
    }
#endif
}


int main(int argc, char *argv[])
{
    enum {
        CAPTURE_TIMES = 10,
    };
    urg_t urg;
    urg_connection_type_t connection_type = URG_SERIAL;
    int max_data_size;
    long *data = NULL;
    unsigned short *intensity = NULL;
    long time_stamp;
    int n;
    int i;

#if defined(URG_WINDOWS_OS)
    const char *device = "COM3";
#elif defined(URG_LINUX_OS)
    const char *device = "/dev/ttyACM0";
#else
#endif
    long baudrate_or_port = 115200;
    const char *ip_address = "192.168.0.10";

    // \~japanese 接続タイプの切替え
    for (i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "-e")) {
            connection_type = URG_ETHERNET;
            baudrate_or_port = 10940;
            device = ip_address;
        }
    }

    // \~japanese 接続
    if (urg_open(&urg, connection_type, device, baudrate_or_port) < 0) {
        printf("urg_open: %s\n", urg_error(&urg));
        return 1;
    }

    max_data_size = urg_max_data_size(&urg);
    data = malloc(max_data_size * 3 * sizeof(data[0]));
    intensity = malloc(max_data_size * 3 * sizeof(intensity[0]));

    if (!data) {
        perror("urg_max_index()");
        return 1;
    }

    // \~japanese データ取得
    urg_start_measurement(&urg, URG_MULTIECHO_INTENSITY, CAPTURE_TIMES, 0);
    for (i = 0; i < CAPTURE_TIMES; ++i) {
        n = urg_get_distance_intensity(&urg, data, intensity, &time_stamp);
        if (n <= 0) {
            printf("urg_distance: %s\n", urg_error(&urg));
            free(data);
            free(intensity);
            urg_close(&urg);
            return 1;
        }
        print_data(&urg, data, intensity, n, time_stamp);
    }

    // \~japanese 切断
    free(data);
    free(intensity);
    urg_close(&urg);

#if defined(URG_MSC)
    getchar();
#endif
    return 0;
}
