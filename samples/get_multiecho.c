 /*!
  \example get_multiecho.c 距離データ(マルチエコー)を取得する

  \author Satofumi KAMIMURA

  $Id$
*/


#include "urg_sensor.h"
#include "urg_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static void print_data(urg_t *urg, long data[], int data_n, long time_stamp)
{
#if 1
    (void)data_n;

    // 前方のデータのみを表示
    int front_index = 3 * urg_step2index(urg, 0);

    // [mm], [mm], [mm], [msec]
    printf("%ld, %ld, %ld, %ld\n",
           data[(3 * front_index) + 0],
           data[(3 * front_index) + 1],
           data[(3 * front_index) + 2],
           time_stamp);

#else
    int i;

    // 全てのデータを表示
    printf("# n = %d, time_stamp = %d\n", data_n, time_stamp);
    for (i = 0; i < data_n; ++i) {

        // [mm], [mm], [mm]
        printf("%ld, %ld, %ld\n",
               data[(3 * front_index) + 0],
               data[(3 * front_index) + 1],
               data[(3 * front_index) + 2]);
    }
    printf("\n");
#endif
}


int main(int argc, char *argv[])
{
    enum {
        CAPTURE_TIMES = 1,
    };
    urg_t urg;
    connection_type_t connection_type = URG_SERIAL;
    long *data = NULL;
    long time_stamp;
    //int ret;
    int n;
    int i;

#if defined(URG_WINDOWS_OS)
    const char device[] = "COM3";
#elif defined(URG_LINUX_OS)
    const char device[] = "/dev/ttyACM0";
#else
#endif

    // 接続タイプの切替え
    for (i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "-e")) {
            connection_type = URG_ETHERNET;
        }
    }

    // 接続
    if (urg_open(&urg, URG_SERIAL, device, 115200) < 0) {
        printf("urg_open: %s\n", urg_error(&urg));
        return 1;
    }
    data = malloc(urg_max_data_size(&urg) * 3 * sizeof(data[0]));
    if (!data) {
        perror("urg_max_index()");
        return 1;
    }

    // データ取得
    urg_start_measurement(&urg, URG_MULTIECHO, CAPTURE_TIMES, 0);
    for (i = 0; i < CAPTURE_TIMES; ++i) {
        n = urg_get_distance(&urg, data, &time_stamp);
        if (n <= 0) {
            printf("urg_distance: %s\n", urg_error(&urg));
            free(data);
            urg_close(&urg);
            return 1;
        }
        print_data(&urg, data, n, time_stamp);
    }

    // 切断
    free(data);
    urg_close(&urg);

    return 0;
}
