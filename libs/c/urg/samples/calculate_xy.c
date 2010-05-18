/*!
  \example calculate_xy.c X-Y 座標系での位置を計算する

  \author Satofumi KAMIMURA

  $Id$
*/

#include "urg_sensor.h"
#include "urg_utils.h"
#include "math.h"
#include <stdio.h>
#include <stdlib.h>


int main(void)
{
    urg_t urg;
    long *data;
    long max_distance;
    long min_distance;
    long timestamp;
    int i;
    int n;

    // 接続
    urg_initialize(&urg);
    if (urg_open(&urg, "/dev/ttyACM0", 115200, URG_SERIAL) < 0) {
        printf("urg_open: %s\n", urg_error(&urg));
        return 1;
    }
    data = malloc(urg_data_max(&urg) * sizeof(data[0]));

    // データ取得
    urg_laser_on(&urg);
    n = urg_get_distance(&urg, data, &timestamp);
    if (n < 0) {
        printf("urg_distance: %s\n", urg_error(&urg));
        urg_close(&urg);
        return 1;
    }

    // X-Y 座標系の値を出力
    min_distance = urg_distance_min(&urg);
    max_distance = urg_distance_max(&urg);
    for (i = 0; i < n; ++i) {
        long distance = data[i];
        double radian;
        double x;
        double y;

        if ((distance < min_distance) || (distance > max_distance)) {
            continue;
        }

        radian = urg_index2rad(&urg, i);
        x = distance * cos(radian);
        y = distance * sin(radian);

        printf("%.1f, %.1f\n", x, y);
    }
    printf("\n");

    // 切断
    urg_close(&urg);

    return 0;
}
