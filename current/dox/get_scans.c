// scan_times ��̃X�L�����f�[�^���擾

// urg_start_measurement() �֐��ŃX�L�����񐔂��w�肵
// urg_get_distance() �֐��Ŏw�肵���񐔂����f�[�^����M����B

const int scan_times = 123;
int i;

// �Z���T���狗���f�[�^���擾����B
ret = urg_start_measurement(&urg, URG_DISTANCE, scan_times, 0);
// \todo check error code

for (i = 0; i < scan_times; ++i) {
    length_data_size = urg_get_distance(&urg, length_data, NULL);
    // \todo process length_data array
}
