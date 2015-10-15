#ifndef SAFETY_CRC_H
#define SAFETY_CRC_H

/*!
  \file
  \~japanese
  \brief ���S�Z���T�[��CRC�v�Z

  ���S�Z���T�[�p�̊�{�I��CRC�v�Z�Ɋւ���֐���񋟂��܂��B

  \~english
  \brief Safety sensor CRC

  Provides the basic functions for Safety sensor's CRC calculation

  \~
  \author Mehrez KRISTOU

  $Id$
*/

#ifdef __cplusplus
extern "C" {
#endif

	    /*!
      \~japanese
      \brief CRC�e�[�u��������������

      \param[in] �Ȃ�

      \retval �Ȃ�

      \~english
      \brief CRC table initialization

      \param[in] None

      \retval None
    */
	void safety_init_crc();

	    /*!
      \~japanese
      \brief CRC�̌v�Z

      \param[in] data ������
      \param[in] size data �� byte �T�C�Y

      \retval CRC�̌v�Z��̐��l

      \~english
      \brief Claculates the CRC code of a message

      \param[in] data the message to calculate its CRC
      \param[in] size of the data

      \retval CRC value
    */
	unsigned short safety_calc_crc(char *src, int length);

#ifdef __cplusplus
}
#endif

#endif /* !SAFETY_CRC_H */