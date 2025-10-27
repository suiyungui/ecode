/*********************************************************************************************************************
* TC264 Opensourec Library ����TC264 ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
*
* ���ļ��� TC264 ��Դ���һ����
*
* TC264 ��Դ�� ��������
* �����Ը��������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
*
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
*
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
*
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ���֤Э�� �����������Ϊ���İ汾
* �������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ���֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
*
* �ļ�����          zf_device_imu660rb
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          ADS v1.10.2
* ����ƽ̨          TC264D
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2022-09-15       pudding            first version
* 2023-04-28       pudding            ��������ע��˵��
********************************************************************************************************************/
/********************************************************************************************************************
* ���߶��壺
*                  ------------------------------------
*                  ģ��ܽ�             ��Ƭ���ܽ�
*                  // Ӳ�� SPI ����
*                  SCL/SPC            �鿴 zf_device_imu660rb.h �� IMU660RB_SPC_PIN �궨��
*                  SDA/DSI            �鿴 zf_device_imu660rb.h �� IMU660RB_SDI_PIN �궨��
*                  SA0/SDO            �鿴 zf_device_imu660rb.h �� IMU660RB_SDO_PIN �궨��
*                  CS                 �鿴 zf_device_imu660rb.h �� IMU660RB_CS_PIN  �궨��
*                  VCC                3.3V��Դ
*                  GND                ��Դ��
*                  ������������
*
*                  // ��� IIC ����
*                  SCL/SPC            �鿴 zf_device_imu660rb.h �� IMU660RB_SCL_PIN �궨��
*                  SDA/DSI            �鿴 zf_device_imu660rb.h �� IMU660RB_SDA_PIN �궨��
*                  VCC                3.3V��Դ
*                  GND                ��Դ��
*                  ������������
*                  ------------------------------------
********************************************************************************************************************/

#include "zf_common_debug.h"
#include "zf_driver_delay.h"
#include "zf_driver_spi.h"
#include "zf_driver_soft_iic.h"
#include "zf_device_imu660rb.h"

int16 imu660rb_gyro_x = 0, imu660rb_gyro_y = 0, imu660rb_gyro_z = 0;       // ��������������      GYRO (������)
int16 imu660rb_acc_x = 0,  imu660rb_acc_y = 0,  imu660rb_acc_z = 0;        // ������ٶȼ�����     ACC  (accelerometer ���ٶȼ�)
float imu660rb_transition_factor[2] = {4098, 14.3};                  // ת��ʵ��ֵ�ı���

#if IMU660RB_USE_SOFT_IIC
static soft_iic_info_struct imu660rb_iic_struct;

//-------------------------------------------------------------------------------------------------------------------
// �������     IMU660RB д�Ĵ���
// ����˵��     reg             �Ĵ�����ַ
// ����˵��     data            ����
// ���ز���     void
// ʹ��ʾ��     imu660rb_write_acc_gyro_register(IMU660RB_SLV0_CONFIG, 0x00);
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
#define imu660rb_write_acc_gyro_register(reg,data)       (soft_iic_write_8bit_register(&imu660rb_iic_struct,reg,data))

//-------------------------------------------------------------------------------------------------------------------
// �������     IMU660RB ���Ĵ���
// ����˵��     reg             �Ĵ�����ַ
// ���ز���     uint8           ����
// ʹ��ʾ��     imu660rb_read_acc_gyro_register(IMU660RB_STATUS_MASTER);
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
#define imu660rb_read_acc_gyro_register(reg)             (soft_iic_sccb_read_register(&imu660rb_iic_struct,reg))

//-------------------------------------------------------------------------------------------------------------------
// �������     IMU660RB ������ �ڲ�����
// ����˵��     reg             �Ĵ�����ַ
// ����˵��     data            ���ݻ�����
// ����˵��     len             ���ݳ���
// ���ز���     void
// ʹ��ʾ��     imu660rb_read_acc_gyro_registers(IMU660RB_OUTX_L_A, dat, 6);
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
#define imu660rb_read_acc_gyro_registers(reg,data,len)   (soft_iic_read_8bit_registers(&imu660rb_iic_struct,reg,data,len))
#else
//-------------------------------------------------------------------------------------------------------------------
// �������     IMU660RB д�Ĵ���
// ����˵��     reg             �Ĵ�����ַ
// ����˵��     data            ����
// ���ز���     void
// ʹ��ʾ��     imu660rb_write_acc_gyro_register(IMU660RB_SLV0_CONFIG, 0x00);
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
static void imu660rb_write_acc_gyro_register (uint8 reg, uint8 data)
{
    IMU660RB_CS(0);
    spi_write_8bit_register(IMU660RB_SPI, reg | IMU660RB_SPI_W, data);

    IMU660RB_CS(1);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     IMU660RB ���Ĵ���
// ����˵��     reg             �Ĵ�����ַ
// ���ز���     uint8           ����
// ʹ��ʾ��     imu660rb_read_acc_gyro_register(IMU660RB_STATUS_MASTER);
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
static uint8 imu660rb_read_acc_gyro_register (uint8 reg)
{
    uint8 data = 0;
    IMU660RB_CS(0);
    data = spi_read_8bit_register(IMU660RB_SPI, reg | IMU660RB_SPI_R);

    IMU660RB_CS(1);
    return data;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     IMU660RB ������ �ڲ�����
// ����˵��     reg             �Ĵ�����ַ
// ����˵��     data            ���ݻ�����
// ����˵��     len             ���ݳ���
// ���ز���     void
// ʹ��ʾ��     imu660rb_read_acc_gyro_registers(IMU660RB_OUTX_L_A, dat, 6);
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
static void imu660rb_read_acc_gyro_registers (uint8 reg, uint8 *data, uint32 len)
{
    IMU660RB_CS(0);
    spi_read_8bit_registers(IMU660RB_SPI, reg | IMU660RB_SPI_R, data, len);

    IMU660RB_CS(1);
}
#endif

//-------------------------------------------------------------------------------------------------------------------
// �������     IMU660RB �����Լ� �ڲ�����
// ����˵��     void
// ���ز���     uint8           1-�Լ�ʧ�� 0-�Լ�ɹ�
// ʹ��ʾ��     imu660rb_acc_gyro_self_check();
// ��ע��Ϣ     �ڲ�����
//-------------------------------------------------------------------------------------------------------------------
static uint8 imu660rb_acc_gyro_self_check (void)
{
    uint8 return_state = 0;
    uint8 dat = 0;
    uint16 timeout_count = 0;

    while(0x6B != dat)                                                          // �ж� ID �Ƿ���ȷ
    {
        if(IMU660RB_TIMEOUT_COUNT < timeout_count ++)
        {
            return_state = 1;
            break;
        }
        dat = imu660rb_read_acc_gyro_register(IMU660RB_WHO_AM_I);
        system_delay_ms(10);
    }
    return return_state;
}


//-------------------------------------------------------------------------------------------------------------------
// �������     ��ȡ IMU660RB ���ٶȼ�����
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     imu660rb_get_acc();
// ��ע��Ϣ     ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
//-------------------------------------------------------------------------------------------------------------------
void imu660rb_get_acc (void)
{
    uint8 dat[6];

    imu660rb_read_acc_gyro_registers(IMU660RB_OUTX_L_A, dat, 6);
    imu660rb_acc_x = (int16)(((uint16)dat[1]<<8 | dat[0]));
    imu660rb_acc_y = (int16)(((uint16)dat[3]<<8 | dat[2]));
    imu660rb_acc_z = (int16)(((uint16)dat[5]<<8 | dat[4]));
}


//-------------------------------------------------------------------------------------------------------------------
// �������     ��ȡ IMU660RB ����������
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     imu660rb_get_gyro();
// ��ע��Ϣ     ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
//-------------------------------------------------------------------------------------------------------------------
void imu660rb_get_gyro (void)
{
    uint8 dat[6];

    imu660rb_read_acc_gyro_registers(IMU660RB_OUTX_L_G, dat, 6);
    imu660rb_gyro_x = (int16)(((uint16)dat[1]<<8 | dat[0]));
    imu660rb_gyro_y = (int16)(((uint16)dat[3]<<8 | dat[2]));
    imu660rb_gyro_z = (int16)(((uint16)dat[5]<<8 | dat[4]));
}


//-------------------------------------------------------------------------------------------------------------------
// �������     ��ʼ�� IMU660RB
// ����˵��     void
// ���ز���     uint8           1-��ʼ��ʧ�� 0-��ʼ���ɹ�
// ʹ��ʾ��     imu660rb_init();
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
uint8 imu660rb_init (void)
{
    uint8 return_state = 0;
    system_delay_ms(10);                                                        // �ϵ���ʱ

#if IMU660RB_USE_SOFT_IIC
    soft_iic_init(&imu660rb_iic_struct, IMU660RB_DEV_ADDR, IMU660RB_SOFT_IIC_DELAY, IMU660RB_SCL_PIN, IMU660RB_SDA_PIN);
#else
    spi_init(IMU660RB_SPI, SPI_MODE0, IMU660RB_SPI_SPEED, IMU660RB_SPC_PIN, IMU660RB_SDI_PIN, IMU660RB_SDO_PIN, SPI_CS_NULL);
    gpio_init(IMU660RB_CS_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);
#endif

    do
    {
        imu660rb_write_acc_gyro_register(IMU660RB_FUNC_CFG_ACCESS, 0x00);       // �ر�HUB�Ĵ�������
        imu660rb_write_acc_gyro_register(IMU660RB_CTRL3_C, 0x01);               // ��λ�豸
        system_delay_ms(2);
        imu660rb_write_acc_gyro_register(IMU660RB_FUNC_CFG_ACCESS, 0x00);       // �ر�HUB�Ĵ�������
        if(imu660rb_acc_gyro_self_check())
        {
            zf_log(0, "IMU660RB acc and gyro self check error.");
            return_state = 1;
            break;
        }

        imu660rb_write_acc_gyro_register(IMU660RB_INT1_CTRL, 0x03);             // ���������� ���ٶ����ݾ����ж�

        // IMU660RB_CTRL1_XL �Ĵ���
        // ����Ϊ 0x30 ���ٶ�����Ϊ ��2  G    ��ȡ���ļ��ٶȼ����ݳ��� 16393  ����ת��Ϊ������λ������ ��λ g(m/s^2)
        // ����Ϊ 0x38 ���ٶ�����Ϊ ��4  G    ��ȡ���ļ��ٶȼ����ݳ��� 8197   ����ת��Ϊ������λ������ ��λ g(m/s^2)
        // ����Ϊ 0x3C ���ٶ�����Ϊ ��8  G    ��ȡ���ļ��ٶȼ����ݳ��� 4098   ����ת��Ϊ������λ������ ��λ g(m/s^2)
        // ����Ϊ 0x34 ���ٶ�����Ϊ ��16 G    ��ȡ���ļ��ٶȼ����ݳ��� 2049   ����ת��Ϊ������λ������ ��λ g(m/s^2)
        switch(IMU660RB_ACC_SAMPLE_DEFAULT)
        {
            default:
            {
                zf_log(0, "IMU660RB_ACC_SAMPLE_DEFAULT set error.");
                return_state = 1;
            }break;
            case IMU660RB_ACC_SAMPLE_SGN_2G:
            {
                imu660rb_write_acc_gyro_register(IMU660RB_CTRL1_XL, 0x30);
                imu660rb_transition_factor[0] = 16393;
            }break;
            case IMU660RB_ACC_SAMPLE_SGN_4G:
            {
                imu660rb_write_acc_gyro_register(IMU660RB_CTRL1_XL, 0x38);
                imu660rb_transition_factor[0] = 8197;
            }break;
            case IMU660RB_ACC_SAMPLE_SGN_8G:
            {
                imu660rb_write_acc_gyro_register(IMU660RB_CTRL1_XL, 0x3C);
                imu660rb_transition_factor[0] = 4098;
            }break;
            case IMU660RB_ACC_SAMPLE_SGN_16G:
            {
                imu660rb_write_acc_gyro_register(IMU660RB_CTRL1_XL, 0x34);
                imu660rb_transition_factor[0] = 2049;
            }break;
        }
        if(1 == return_state)
        {
            break;
        }

        // IMU660RB_CTRL2_G �Ĵ���
        // ����Ϊ 0x52 ����������Ϊ ��125  dps    ��ȡ�������������ݳ��� 228.6   ����ת��Ϊ������λ������ ��λΪ ��/s
        // ����Ϊ 0x50 ����������Ϊ ��250  dps    ��ȡ�������������ݳ��� 114.3   ����ת��Ϊ������λ������ ��λΪ ��/s
        // ����Ϊ 0x54 ����������Ϊ ��500  dps    ��ȡ�������������ݳ��� 57.1    ����ת��Ϊ������λ������ ��λΪ ��/s
        // ����Ϊ 0x58 ����������Ϊ ��1000 dps    ��ȡ�������������ݳ��� 28.6    ����ת��Ϊ������λ������ ��λΪ ��/s
        // ����Ϊ 0x5C ����������Ϊ ��2000 dps    ��ȡ�������������ݳ��� 14.3    ����ת��Ϊ������λ������ ��λΪ ��/s
        // ����Ϊ 0x51 ����������Ϊ ��4000 dps    ��ȡ�������������ݳ��� 7.1     ����ת��Ϊ������λ������ ��λΪ ��/s
        switch(IMU660RB_GYRO_SAMPLE_DEFAULT)
        {
            default:
            {
                zf_log(0, "IMU660RB_GYRO_SAMPLE_DEFAULT set error.");
                return_state = 1;
            }break;
            case IMU660RB_GYRO_SAMPLE_SGN_125DPS:
            {
                imu660rb_write_acc_gyro_register(IMU660RB_CTRL2_G, 0x52);
                imu660rb_transition_factor[1] = 228.6;
            }break;
            case IMU660RB_GYRO_SAMPLE_SGN_250DPS:
            {
                imu660rb_write_acc_gyro_register(IMU660RB_CTRL2_G, 0x50);
                imu660rb_transition_factor[1] = 114.3;
            }break;
            case IMU660RB_GYRO_SAMPLE_SGN_500DPS:
            {
                imu660rb_write_acc_gyro_register(IMU660RB_CTRL2_G, 0x54);
                imu660rb_transition_factor[1] = 57.1;
            }break;
            case IMU660RB_GYRO_SAMPLE_SGN_1000DPS:
            {
                imu660rb_write_acc_gyro_register(IMU660RB_CTRL2_G, 0x58);
                imu660rb_transition_factor[1] = 28.6;
            }break;
            case IMU660RB_GYRO_SAMPLE_SGN_2000DPS:
            {
                imu660rb_write_acc_gyro_register(IMU660RB_CTRL2_G, 0x5C);
                imu660rb_transition_factor[1] = 14.3;
            }break;
            case IMU660RB_GYRO_SAMPLE_SGN_4000DPS:
            {
                imu660rb_write_acc_gyro_register(IMU660RB_CTRL2_G, 0x51);
                imu660rb_transition_factor[1] = 7.1;
            }break;
        }
        if(1 == return_state)
        {
            break;
        }

        imu660rb_write_acc_gyro_register(IMU660RB_CTRL3_C, 0x44);                   // ʹ�����������ֵ�ͨ�˲���
        imu660rb_write_acc_gyro_register(IMU660RB_CTRL4_C, 0x02);                   // ʹ�����ֵ�ͨ�˲���
        imu660rb_write_acc_gyro_register(IMU660RB_CTRL5_C, 0x00);                   // ���ٶȼ�����������������
        imu660rb_write_acc_gyro_register(IMU660RB_CTRL6_C, 0x00);                   // �������ٶȼƸ�����ģʽ �����ǵ�ͨ�˲� 133hz
        imu660rb_write_acc_gyro_register(IMU660RB_CTRL7_G, 0x00);                   // ���������Ǹ�����ģʽ �رո�ͨ�˲�
        imu660rb_write_acc_gyro_register(IMU660RB_CTRL9_XL, 0x01);                  // �ر�I3C�ӿ�

    }while(0);
    return return_state;
}
