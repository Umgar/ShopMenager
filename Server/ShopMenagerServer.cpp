#include <iostream>
#include <thread>
#include <WinSock2.h>
#include<chrono>  
#include <vector>

auto readMSG = [](SOCKET clientSocket) {
    char buffer[1024];
    if (clientSocket == INVALID_SOCKET)
    {
        std::cout << "Accepting connection error.\n";
        return 1;
    }
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead > 0)
    {
        buffer[bytesRead - 1] = '\0';
        std::cout << "Received data: " << buffer << "\n";
    }
    else if (bytesRead == 0)
    {
        std::cout << "Client disconnected.\n";
    }
    else
    {
        std::cerr << "Receive error.\n";
    }
    char goodbyeMsg[] = "You will be disconnected\n";
    std::this_thread::sleep_for(std::chrono::seconds(10));
    send(clientSocket, goodbyeMsg, sizeof(goodbyeMsg), 0);

    closesocket(clientSocket);
};

int main()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "WSAStartup failed.\n";
        return 1;
    }

    SOCKET serverSocket, clientSocket;
    sockaddr_in serverAddr, clientAddr;
    int addrSize = sizeof(clientAddr);
    const int PORT = 21123;

    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        std::cerr << "Socket creation error.\n";
        WSACleanup();
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cerr << "Binding error.\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if (listen(serverSocket, 10) == SOCKET_ERROR)
    {
        std::cerr << "Listening error.\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server is running on port: " << PORT << "\n";

    const int MAX_THREADS = 10;
    std::vector<std::thread> threadPool;

    while (true)
    {
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrSize);
        if (clientSocket == INVALID_SOCKET)
        {
            std::cerr << "Accepting connection error.\n";
            continue;
        }
        if (threadPool.size() >= MAX_THREADS)
        {
            for (auto& thread : threadPool)
            {
                if (thread.joinable())
                {
                    thread.join();
                    break;
                }
            }
        }
        threadPool.emplace_back(std::thread(readMSG, clientSocket));
    }


    
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
