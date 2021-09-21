using System;
using System.Collections.Concurrent;
using Google.Protobuf;
using HPSocket;
using HPSocket.Tcp;
using Communication.Proto;
using System.Threading;

namespace ClientCommunication
{
    public delegate void OnReceiveCallback();

    /// <summary>
    /// �ͻ�����
    /// </summary>
    class ClientCommunication : IDisposable
    {
        private TcpPackClient client;
        private BlockingCollection<IGameMessage> msgQueue;
        public event OnReceiveCallback OnReceive;
        private readonly int maxtimeout = 30000; // ��ʱ�綨ʱ��
        
        public ClientCommunication()
        {
            client = new TcpPackClient();
            msgQueue = new BlockingCollection<IGameMessage>();
            client.OnReceive += delegate (IClient sender, byte[] bytes)
            {
                Message message = new Message();
                message.Deserialize(bytes); // ������Ϣ
                try
                {
                    msgQueue.Add(message);
                }
                catch (Exception e)
                {
                    Console.WriteLine("Exception occured when adding an item to the msgQueue:" + e.Message);
                }
                OnReceive?.Invoke();
                return HandleResult.Ok;
            };
        }

        /// <summary>
        /// ���ӵ�server�Ĳ���
        /// </summary>
        /// <param name="IP"></param>
        /// <param name="port"></param>
        /// <returns></returns>
        public bool Connect(string IP, ushort port)
        {
            client.Address = IP;
            client.Port = port;
            if (!client.Connect())
            {
                return false;
            }
            for (int i = 0; i < maxtimeout / 100; i++)
            {
                if (!client.IsConnected)
                {
                    Thread.Sleep(100);
                }
                else
                {
                    return true;
                }
            }
            return false;
        }
        /// <summary>
        /// ��server���͵��û�����
        /// </summary>
        /// <param name="msg"></param>
        public void SendMessage(MessageToServer msg)
        {
            Message message = new Message();
            message.Content = msg;
            message.PacketType = PacketType.MessageToServer;
            byte[] bytes;
            message.WriteTo(out bytes);
            SendOperation(bytes);
        }
        /// <summary>
        /// ��server������Ϣ�����õ���ʵ����
        /// </summary>
        /// <param name="bytes"></param>
        private void SendOperation(byte[] bytes)
        {
            if (!client.Send(bytes, bytes.Length))
            {
                Console.WriteLine("failed to send to server.");
            }
        }


        /// <summary>
        /// ��������ʽ������Ϣ
        /// </summary>
        /// <param name="msg"></param>
        /// <returns></returns>
        public bool TryTake(out IGameMessage msg)
        {
            try
            {
                return msgQueue.TryTake(out msg);
            }
            catch (Exception e)
            {
                Console.WriteLine("Exception occured when using 'TryTake' method in client:" + e.Message);
                msg = null;
                return false;
            }
        }

        /// <summary>
        /// �Է���ֵ��ʽ������Ϣ
        /// </summary>
        /// <returns></returns>
        public IGameMessage Take()
        {
            try
            {
                return msgQueue.Take();
            }
            catch (Exception e)
            {
                Console.WriteLine("Exception occured when using 'Take' method in client:" + e.Message);
                return null;
            }
        }

        /// <summary>
        /// ֹͣclient��
        /// </summary>
        /// <returns></returns>
        public bool Stop()
        {
            return client.Stop();
        }

        /// <summary>
        /// ��������
        /// </summary>
        public void Dispose()
        {
            client.Dispose();
            GC.SuppressFinalize(this);
        }
    }
}
