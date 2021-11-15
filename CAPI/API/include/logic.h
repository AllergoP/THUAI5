#pragma once

#ifndef LOGIC_H
#define LOGIC_H

#include<iostream>
#include<thread>
#include<memory>
#include<condition_variable>
#include<tuple>
#include<atomic>

#include"base.h"
#include"CAPI.h"
#include"proto/Message2Clients.pb.h"
#include"concurrent_queue.hpp"



/// <summary>
/// ��װ��ͨ�������AI������в���
/// </summary>
class Logic
{
private:
    std::unique_ptr<ClientCommunication> pComm; // ͨ�����ָ��
    std::unique_ptr<AIBase> pAI; // ���ָ��
    std::shared_ptr<int> xx; // ���״̬

    std::thread tAI; // ��Ҫ����ҵ����߳�

    // ������
    std::mutex mtx_ai;
    std::mutex mtx_state; 
    std::mutex mtx_buffer;

    // ��������
    std::condition_variable cv_buffer;
    std::condition_variable cv_ai;
    
    // ��Ϣ����
    concurrency::concurrent_queue<std::string> MessageStorage;

    // ��¼״̬����Ϣ��(���ܺ��߳��й�)
    std::atomic<int> counter_state = 0;
    std::atomic<int> counter_buffer = 0;

    /// <summary>
    /// ִ��AI�߳�
    /// </summary>
    /// <param name="player"></param>
    void PlayerWrapper(std::function<void()> player);

    /// <summary>
    /// ������Ϣ
    /// </summary>
    /// <param name=""></param>
    void ProcessMessage(pointer_m2c);

    /// <summary>
    /// ������ϢPart1 �㲥
    /// </summary>
    /// <param name=""></param>
    void ProcessMessageToClient(std::shared_ptr<Protobuf::MessageToClient>);

    /// <summary>
    /// ������ϢPart2 ����
    /// </summary>
    /// <param name=""></param>
    void ProcessMessageToOneClient(std::shared_ptr<Protobuf::MessageToOneClient>);

    /// <summary>
    /// ������ϢPart3 ��ʼ��
    /// </summary>
    /// <param name=""></param>
    void ProcessMessageToInitialize(std::shared_ptr<Protobuf::MessageToInitialize>);

    /// <summary>
    /// ���ص�buffer
    /// </summary>
    /// <param name=""></param>
    void LoadBuffer(std::shared_ptr<Protobuf::MessageToClient>);

    /// <summary>
    /// ����״̬
    /// </summary>
    void UnBlockBuffer();
    
    /// <summary>
    /// ����AI�߳�
    /// </summary>
    void UnBlockAI();
    
public:
    Logic();
    ~Logic() = default;
    void Main();
};

#endif