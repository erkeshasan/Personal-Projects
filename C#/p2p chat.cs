using System.Net.Sockets;
using System.Text;

static void ReceiveMessages(NetworkStream stream)
{
    while (true)
    {
        byte[] buffer = new byte[1024];
        int bytes = stream.Read(buffer, 0, buffer.Length);

        if (bytes > 0)
        {
            string message = Encoding.ASCII.GetString(buffer, 0, bytes);
            Console.WriteLine(message);
        }
    }
}

static void StartServer(int port)
{
    TcpListener listener = new TcpListener(System.Net.IPAddress.Any, port);
    listener.Start();

    Console.WriteLine("Listening");

    while (true)
    {
        TcpClient client = listener.AcceptTcpClient();
        NetworkStream stream = client.GetStream();

        Thread receiveThread = new Thread(() => ReceiveMessages(stream));
        receiveThread.Start();
    }
}

static void SendMessages(NetworkStream stream)
{
    while (true)
    {
        string message = Console.ReadLine();
        byte[] buffer = Encoding.ASCII.GetBytes(message);
        
        stream.Write(buffer, 0, buffer.Length);
    }
}

static void StartClient(string peerIP, int peerPort)
{
    TcpClient client = new TcpClient();

    while (!client.Connected)
    {
        try
        {
            client.Connect(System.Net.IPAddress.Parse(peerIP), peerPort);

            Console.WriteLine($"Connected to {peerIP}:{peerPort}");
        }
        catch
        {
            Console.WriteLine("Peer unavailable");

            Thread.Sleep(1000);
        }
    }

    NetworkStream stream = client.GetStream();
    SendMessages(stream);
}

Thread listenThread = new Thread(() => StartServer(5000));
listenThread.Start();

StartClient("127.0.0.1", 5000);
