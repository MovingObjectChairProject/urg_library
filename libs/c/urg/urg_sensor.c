/*!
  \brief URG センサ制御

  URG 用の基本皁E��関数を提供します、E
  \author Satofumi KAMIMURA

  $Id$
*/

#include "urg_sensor.h"
#include "urg_errno.h"
#include <stddef.h>
#include <string.h>

enum {
    URG_FALSE = 0,
    URG_TRUE = 1,

    BUFFER_SIZE = 64,

    EXPECTED_END = -1,

    RECEIVE_DATA_TIMEOUT,
    RECEIVE_DATA_COMPLETE,      /*!< チE�Eタを正常に受信 */
    RECEIVE_DATA_QT,            /*!< QT 応答を受信 */
};


// !!! ボ�Eレートを変更しながら接��を行う
static int connect_serial_device(urg_t *urg, long baudrate)
{
    (void)urg;
    (void)baudrate;
    // !!!

    return -1;
}


static int receive_parameter(urg_t *urg)
{
    (void)urg;
    // !!!

    return -1;
}


static int response(urg_t *urg, const char* command, int command_size,
                    const int expected_ret[],
                    char receive_buffer[], int receive_buffer_max_size)
{
    (void)urg;
    (void)command;
    (void)command_size;
    (void)expected_ret;
    (void)receive_buffer;
    (void)receive_buffer_max_size;
    // !!!

    // !!! receive_buffer は '\0' 終端させめE    // !!! 改行�E取り除ぁE
    return -1;
}


//! チェチE��サムの計箁Estatic char checksum(const char buffer[], int size)
{
    int i;
    unsigned char sum = '\0';

    for (i = 0; i < size; ++i) {
        sum += buffer[i];
    }

    // SCIP 仕様書参�E
    sum &= 0x3f;
    sum += 0x30;

    return sum;
}


//! SCIP 斁E���EのチE��ーチEstatic long decode(const char buffer[], int size)
{
    (void)buffer;
    (void)size;
    // !!!

    return -1;
}


//! 距離チE�Eタの取征Estatic int receive_data(urg_t *urg, long data[], unsigned short intensity[],
                        long *time_stamp)
{
    (void)urg;
    (void)data;
    (void)intensity;
    (void)time_stamp;

    // !!! 戻り値
    // !!! - エラー
    // !!! - 受信したチE�Eタ数
    // !!! - QT, RS, RT の応答を受信した
    // !!! (RS, RT で受信が中断するかを確認すること。まぁ、使わなぁE��ど)

    return -1;
}


int urg_open(urg_t *urg, connection_type_t connection_type,
             const char *device, long baudrate)
{
    int ret;

    urg->is_active = URG_FALSE;

    // チE��イスへの接綁E    if (! connection_open(&urg->connection, connection_type,
                          device, baudrate)) {
        switch (connection_type) {
        case URG_SERIAL:
            urg->last_errno = URG_SERIAL_OPEN_ERROR;
            break;
        case URG_ETHERNET:
            urg->last_errno = URG_ETHERNET_OPEN_ERROR;
        }
        return urg->last_errno;
    }

    // 持E��した�Eーレートで URG と通信できるように調整
    if (connection_type == URG_SERIAL) {
        ret = connect_serial_device(urg, baudrate);
        if (ret < 0) {
            return ret;
        }
    }

    // 変数の初期匁E    urg->last_errno = URG_NO_ERROR;
    urg->communication_data_size = URG_COMMUNICATION_3_BYTE;

    // パラメータ惁E��を取征E    return receive_parameter(urg);
}


void urg_close(urg_t *urg)
{
    connection_close(&urg->connection);
    urg->is_active = URG_FALSE;
}


int urg_start_time_stamp_mode(urg_t *urg)
{
    const int expected[] = { 0, EXPECTED_END };

    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    // TM0 を発行すめE    return response(urg, "TM0\n", 4, expected, NULL, 0);
}


long urg_time_stamp(urg_t *urg)
{
    const int expected[] = { 0, EXPECTED_END };
    char buffer[BUFFER_SIZE];
    int ret;

    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    ret = response(urg, "TM1\n", 4, expected, buffer, BUFFER_SIZE);
    if (ret < 0) {
        return ret;
    }

    // buffer からタイムスタンプを取得し、デコードして返す
    if (strlen(buffer) != 5) {
        return URG_RECEIVE_ERROR;
    }
    if (buffer[5] == checksum(buffer, 4)) {
        return URG_CHECKSUM_ERROR;
    }
    return decode(buffer, 4);
}


void urg_stop_time_stamp_mode(urg_t *urg)
{
    int expected[] = { 0, EXPECTED_END };

    if (!urg->is_active) {
        return;
    }

    // TM2 を発行すめE    response(urg, "TM2\n", 4, expected, NULL, 0);
}


int urg_start_measurement(urg_t *urg, measurement_type_t type,
                          int scan_times, int skip_scan)
{
    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    // 持E��されたタイプ�EパケチE��を生成し、E��信する
    // !!! GD, GS, (GI),
    // !!! MD, MS, MI
    // !!! (HD), (HS), (HI)
    // !!! ND, NS, NI

    (void)type;
    (void)scan_times;
    (void)skip_scan;
    // !!!

    return -1;
}


int urg_get_distance(urg_t *urg, long data[], long *time_stamp)
{
    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }
    return receive_data(urg, data, NULL, time_stamp);
}


int urg_get_distance_intensity(urg_t *urg,
                               long data[], unsigned short intensity[],
                               long *time_stamp)
{
    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    return receive_data(urg, data, intensity, time_stamp);
}


int urg_get_multiecho(urg_t *urg, long data_multi[], long *time_stamp)
{
    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    return receive_data(urg, data_multi, NULL, time_stamp);
}


int urg_get_multiecho_intensity(urg_t *urg,
                                long data_multi[],
                                unsigned short intensity_multi[],
                                long *time_stamp)
{
    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    return receive_data(urg, data_multi, intensity_multi, time_stamp);
}


int urg_stop_measurement(urg_t *urg)
{
    int ret;

    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    // QT を発行すめE    connection_write(&urg->connection, "QT\n", 3);
    do {
        // QT の応答が返されるまで、距離チE�Eタを読み捨てめE        ret = receive_data(urg, NULL, NULL, NULL);
        if (ret == RECEIVE_DATA_QT) {
            // 正常応筁E            return 0;
        }
    } while (ret != RECEIVE_DATA_TIMEOUT);

    return ret;
}


int urg_set_scanning_parameter(urg_t *urg, int first_step, int last_step,
                               int skip_step)
{
    // 設定�E篁E��外を持E��したとき�E、エラーを返す
    if (((skip_step < 0) || (skip_step >= 100)) ||
        (first_step > last_step) ||
        (first_step < urg->scanning_first_step) ||
        (last_step > urg->scanning_last_step)) {
        return URG_SCANNING_PARAMETER_ERROR;
    }

    urg->scanning_first_step = first_step;
    urg->scanning_last_step = last_step;
    urg->scanning_skip_step = skip_step;

    return 0;
}


int urg_set_communication_data_size(urg_t *urg, range_byte_t data_size)
{
    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    if ((data_size != URG_COMMUNICATION_3_BYTE) ||
        (data_size != URG_COMMUNICATION_2_BYTE)) {
        return URG_DATA_SIZE_PARAMETER_ERROR;
    }

    urg->communication_data_size = data_size;

    return 0;
}


int urg_laser_on(urg_t *urg)
{
    int expected[] = { 0, 2, EXPECTED_END };

    if (!urg->is_active) {
        return URG_NOT_CONNECTED;
    }

    // 既にレーザが発光してぁE��とき�E、コマンドを送信しなぁE��ぁE��する
    // !!!

    return response(urg, "BM\n", 3, expected, NULL, 0);
}


int urg_laser_off(urg_t *urg)
{
    return urg_stop_measurement(urg);
}


int urg_reboot(urg_t *urg)
{
    (void)urg;
    // !!!

    return -1;
}
