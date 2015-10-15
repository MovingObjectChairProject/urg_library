/*!
  \~japanese
  \example get_distance.c �����f�[�^���擾����
  \~english
  \example get_distance.c Obtains distance data
  \~
  \author Satofumi KAMIMURA

  $Id$
*/

#include "urg_sensor.h"
#include "urg_utils.h"
#include "open_urg_sensor.h"
#include <stdlib.h>
#include <stdio.h>


static void print_data(urg_t *urg, long data[], int data_n, safety_data_t *safety_data)
{
#if 1
    int front_index;

    (void)data_n;

    // \~japanese �O���̃f�[�^�݂̂�\��
    // \~english Shows only the front step
    front_index = urg_step2index(urg, 0);
	printf("%ld [mm], (%ld [on])\n", data[front_index], safety_data->is_ossd1_1_on);

#else
    (void)time_stamp;

    int i;
    long min_distance;
    long max_distance;

    // \~japanese �S�Ẵf�[�^�� X-Y �̈ʒu��\��
    // \~english Prints the X-Y coordinates for all the measurement points
    urg_distance_min_max(urg, &min_distance, &max_distance);
    for (i = 0; i < data_n; ++i) {
        long l = data[i];
        double radian;
        long x;
        long y;

        if ((l <= min_distance) || (l >= max_distance)) {
            continue;
        }
        radian = urg_index2rad(urg, i);
        x = (long)(l * cos(radian));
        y = (long)(l * sin(radian));
        printf("(%ld, %ld), ", x, y);
    }
    printf("\n");
#endif
}


int main(int argc, char *argv[])
{
    enum {
        CAPTURE_TIMES = 10,
    };
    urg_t urg;
    long *data = NULL;
	safety_data_t safety_data;
    int n;
    int i;

    if (open_urg_sensor(&urg, argc, argv) < 0) {
        return 1;
    }

    data = (long *)malloc(urg_max_data_size(&urg) * sizeof(data[0]));
    if (!data) {
        perror("urg_max_index()");
        return 1;
    }

    // \~japanese �f�[�^�擾
    // \~english Gets measurement data
#if 0
    // \~japanese �f�[�^�̎擾�͈͂�ύX����ꍇ
    // \~english Case where the measurement range (start/end steps) is defined
    urg_set_scanning_parameter(&urg,
                               urg_deg2step(&urg, -90),
                               urg_deg2step(&urg, +90), 0);
#endif

    safety_start_measurement(&urg, URG_DISTANCE, URG_CONTINUOUS);
    for (i = 0; i < CAPTURE_TIMES; ++i) {
        n = safety_get_distance(&urg, data, &safety_data);
        if (n <= 0) {
            printf("urg_get_distance: %s\n", urg_error(&urg));
            //free(data);
            //urg_close(&urg);
            //return 1;
        }
		else{
			print_data(&urg, data, n, &safety_data);
		}
    }

	// \~japanese ���ꗬ�����~����
    // \~english Stops continuous mode
	safety_stop_measurement(&urg);

    // \~japanese �ؒf
    // \~english Disconnects
    free(data);
    urg_close(&urg);

#if defined(URG_MSC)
    getchar();
#endif
    return 0;
}
