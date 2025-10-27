/*
 * justfloat.c
 *
 *  Created on: 2025��4��29��
 *      Author: suiyungui
 */
/* ����Զ�̵���pid */
#include"justfloat.h"
typedef union
{
    float fdata;
    unsigned long ldata;
} FloatLongType;

/*
��������fת��Ϊ4���ֽ����ݴ����byte[4]��
*/
void Float_to_Byte(float f,unsigned char byte[])
{
    FloatLongType fl;
    fl.fdata=f;
    byte[0]=(unsigned char)fl.ldata;
    byte[1]=(unsigned char)(fl.ldata>>8);
    byte[2]=(unsigned char)(fl.ldata>>16);
    byte[3]=(unsigned char)(fl.ldata>>24);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ������float�������͵�����ͨ�����ڷ���
// ����˵��     a��Ҫ���͵���������
// ���ز���     void
// ʹ��ʾ��     JustFloat_Test_one(sin(t));
// ��ע��Ϣ     ͨ��vofa������ʾ����ͼ
//-------------------------------------------------------------------------------------------------------------------

void JustFloat_Test_one(float a)   //justfloat ����Э�����//�ɴ�����������
{
uint8 byte[4]={0};     //floatת��Ϊ4���ֽ�����
uint8 tail[4]={0x00, 0x00, 0x80, 0x7f};    //֡β
//����λ����������ͨ������
Float_to_Byte(a,byte);
//u1_printf("%f\r\n",a);
wireless_uart_send_buffer(byte, 4);   //1ת��Ϊ4�ֽ����� ����  0x00 0x00 0x80 0x3F
//����֡β
wireless_uart_send_buffer(tail, 4);   //֡βΪ 0x00 0x00 0x80 0x7f
}
//-------------------------------------------------------------------------------------------------------------------
// �������     ������float�������͵�����ͨ�����ڷ���
// ����˵��     a��b��Ҫ���͵���������
// ���ز���     void
// ʹ��ʾ��     JustFloat_Test_two(sin(t),sin(2*t));
// ��ע��Ϣ     ͨ��vofa������ʾ����ͼ
//-------------------------------------------------------------------------------------------------------------------

void JustFloat_Test_two(float a, float b)   //justfloat ����Э�����//�ɴ�����������
{

uint8 byte[4]={0};     //floatת��Ϊ4���ֽ�����
uint8 tail[4]={0x00, 0x00, 0x80, 0x7f};    //֡β

//����λ����������ͨ������
Float_to_Byte(a,byte);
wireless_uart_send_buffer(byte, 4);   //1ת��Ϊ4�ֽ����� ����  0x00 0x00 0x80 0x3F

Float_to_Byte(b,byte);
wireless_uart_send_buffer(byte, 4);   //2ת��Ϊ4�ֽ����� ����  0x00 0x00 0x00 0x40

//����֡β
wireless_uart_send_buffer(tail, 4);   //֡βΪ 0x00 0x00 0x80 0x7f

}


//-------------------------------------------------------------------------------------------------------------------
// �������     ������float�������͵�����ͨ�����ڷ���
// ����˵��     a��b��c��Ҫ���͵���������
// ���ز���     void
// ʹ��ʾ��     JustFloat_Test_three(sin(t),sin(2*t)��sin(3*t));
// ��ע��Ϣ     ͨ��vofa������ʾ����ͼ
//-------------------------------------------------------------------------------------------------------------------

void JustFloat_Test_three(float a, float b,float c)   //justfloat ����Э�����
{

uint8 byte[4]={0};     //floatת��Ϊ4���ֽ�����
uint8 tail[4]={0x00, 0x00, 0x80, 0x7f};    //֡β

//����λ����������ͨ������
Float_to_Byte(a,byte);
//u1_printf("%f\r\n",a);
wireless_uart_send_buffer(byte, 4);   //1ת��Ϊ4�ֽ����� ����  0x00 0x00 0x80 0x3F

Float_to_Byte(b,byte);
wireless_uart_send_buffer(byte, 4);   //2ת��Ϊ4�ֽ����� ����  0x00 0x00 0x00 0x40

Float_to_Byte(c,byte);
wireless_uart_send_buffer(byte, 4);   //2ת��Ϊ4�ֽ����� ����  0x00 0x00 0x00 0x40
//����֡β
wireless_uart_send_buffer(tail, 4);   //֡βΪ 0x00 0x00 0x80 0x7f
}

void JustFloat_Test_four(float a, float b, float c, float d) // justfloat ����Э�����//�ɴ�����������
{
    uint8 byte[4]={0};     // floatת��Ϊ4���ֽ�����
    uint8 tail[4]={0x00, 0x00, 0x80, 0x7f};    //֡β

    // ����λ�������ĸ�ͨ������
    Float_to_Byte(a, byte);
    wireless_uart_send_buffer(byte, 4);

    Float_to_Byte(b, byte);
    wireless_uart_send_buffer(byte, 4);

    Float_to_Byte(c, byte);
    wireless_uart_send_buffer(byte, 4);

    Float_to_Byte(d, byte); // ���͵��ĸ�����
    wireless_uart_send_buffer(byte, 4);

    // ����֡β
    wireless_uart_send_buffer(tail, 4);
}

