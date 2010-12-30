#ifndef URG_SERIAL_H
#define URG_SERIAL_H

/*!
  \file
  \brief �V���A���ʐM

  \author Satofumi KAMIMURA

  $Id$
*/

#include "urg_detect_os.h"

#if defined(URG_WINDOWS_OS)
#include <windows.h>
#else
#include <termios.h>
#endif
#include "urg_ring_buffer.h"


enum {
    RING_BUFFER_SIZE_SHIFT = 7,
    RING_BUFFER_SIZE = 1 << RING_BUFFER_SIZE_SHIFT,

    ERROR_MESSAGE_SIZE = 256,
};


typedef struct
{
#if defined(URG_WINDOWS_OS)
    HANDLE hCom;                /*!< �ڑ����\�[�X */
    int current_timeout;        /*!< �^�C���A�E�g�̐ݒ莞�� [msec] */
#else
    int fd;
    struct termios sio;
#endif

    ring_buffer_t ring;         /*!< �����O�o�b�t�@ */
    char buffer[RING_BUFFER_SIZE];
    char has_last_ch;          /*!< �����߂������������邩�̃t���O */
    char last_ch;              /*!< �����߂����P���� */
} urg_serial_t;


extern int serial_open(urg_serial_t *serial, const char *device, long baudrate);
extern void serial_close(urg_serial_t *serial);
extern int serial_set_baudrate(urg_serial_t *serial, long baudrate);
extern int serial_write(urg_serial_t *serial, const char *data, int size);
extern int serial_read(urg_serial_t *serial,
                       char *data, int max_size, int timeout);
extern int serial_readline(urg_serial_t *serial,
                           char *data, int max_size, int timeout);
extern int serial_error(urg_serial_t *serial, char *error_message, int max_size);

#endif /* !URG_SERIAL_H */
