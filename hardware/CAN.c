#include "CAN.h"
#include "stm32f4xx.h"

void CAN_init(void) {
    // 初始化使能CAN时钟
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    CAN_InitTypeDef CAN_InitStructure;
    CAN_FilterInitTypeDef CAN_FilterInitStructure;

    // 1.开启GPIO和CAN的时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

    // 2.初始化GPIO为CAN复用功能
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;            // 复用功能
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          // 推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;  // PDO PD1
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;        // 无上拉下拉
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       // 4MHz
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    // 将PDO PD1复用功能映射到CAN1
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_CAN1);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_CAN1);

    /* 3.初始化CAN1，
    规则：can的比特率 = （can时钟）42M / （分频系数）4 / [bs1+bs2(等于CAN的ss+pts+pbs1+pbs2) + sjw(CAN的sjw)]
                        42MHz / 4 / [1+12 + 8] = 500Kbps
    */
    CAN_InitStructure.CAN_Prescaler = 4;           // 4yj
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;       // 1Tq
    CAN_InitStructure.CAN_BS1 = CAN_BS1_12tq;      // 12Tq
    CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;       // 8Tq
    CAN_InitStructure.CAN_Mode = CAN_Mode_LoopBack;  // 循环回式, 用于调试
    CAN_InitStructure.CAN_TTCM = DISABLE;
    CAN_InitStructure.CAN_ABOM = DISABLE;
    CAN_InitStructure.CAN_AWUM = DISABLE;
    CAN_InitStructure.CAN_NART = DISABLE;
    CAN_InitStructure.CAN_RFLM = DISABLE;
    CAN_InitStructure.CAN_TXFP = DISABLE;
    CAN_Init(CAN1, &CAN_InitStructure);

    // 4.过滤器初始化
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;                // 使能过滤器
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;  // FIFO接收
    CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0;
    CAN_FilterInitStructure.CAN_FilterIdLow = 0x0;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0;
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;   // ID掩码模式
    CAN_FilterInitStructure.CAN_FilterNumber = 0;                     // 过滤器0
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;  // 16位过滤器
    CAN_FilterInit(&CAN_FilterInitStructure);
    //中断配置
    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
CanTxMsg CAN_TX_MSG;
CanRxMsg CAN_RX_MSG;

// 发送函数
u8 can1_send_message(u8 *data, u8 len, u32 message_id){
    u8 i, mailbox;
    u32 timeout = 0;
    if(len > 8)return 1;
    // 根据id的值选择扩展帧/标准帧
    if (message_id > 0x7ff) {//超过标准帧的id范围, 则使用扩展帧
        CAN_TX_MSG.IDE = CAN_Id_Extended;  // 扩展帧
        CAN_TX_MSG.ExtId = message_id;
    }
    else {//否则使用标准帧
        CAN_TX_MSG.IDE = CAN_Id_Standard;  // 标准帧
        CAN_TX_MSG.StdId = message_id;
    }
    CAN_TX_MSG.RTR = CAN_RTR_Data;  // 数据帧
    CAN_TX_MSG.DLC = len;           // 数据长度
    // 填充数据字段
    for (i = 0; i < len; i++) {
        CAN_TX_MSG.Data[i] = data[i];
    }
    // 发送消息
    mailbox = CAN_Transmit(CAN1, &CAN_TX_MSG);
    
    // 检查是否成功获取邮箱
    if(mailbox == CAN_NO_MB){
        return 2;  // 没有可用邮箱
    }
    
    // 等待发送完成（使用正确的状态检查函数）
    while (CAN_TransmitStatus(CAN1, mailbox) != CAN_TxStatus_Ok && timeout < 10000) {
        timeout++;
    }
    if (timeout >= 10000) {
        return 3;
    }
    return 0;
}

void CAN1_RX0_IRQHandler(void)
{
    if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) == SET)
    {
        // 处理接收中断
        CAN_Receive(CAN1, CAN_FIFO0, &CAN_RX_MSG);
        if(CAN_RX_MSG.IDE == CAN_Id_Standard){
            // 处理标准帧
            printf("Standard Frame: ID = 0x%03X, DLC = %d\n", CAN_RX_MSG.StdId, CAN_RX_MSG.DLC);
            for(int i = 0; i < CAN_RX_MSG.DLC; i++){
                printf("0x%02X ", CAN_RX_MSG.Data[i]);
            }
            printf("\r\n");
        }
        else{
            // 处理扩展帧
            printf("Extended Frame: ID = 0x%08X, DLC = %d\n", CAN_RX_MSG.ExtId, CAN_RX_MSG.DLC);
            for(int i = 0; i < CAN_RX_MSG.DLC; i++){
                printf("0x%02X ", CAN_RX_MSG.Data[i]);
            }
            printf("\r\n");
        }
        // 清除中断标志位
        CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
    }
}



