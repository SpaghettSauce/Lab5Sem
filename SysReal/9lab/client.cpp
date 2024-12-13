#include <iostream>
#include <string>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

void main()
{
    string ipAddress = "127.0.0.1";
    int port = 54000;

    WSAData data;
    WORD ver = MAKEWORD(2, 2);
    int wsResult = WSAStartup(ver, &data);
    if (wsResult != 0)
    {
        cerr << "Can't start Winsock" << wsResult << endl;
        return;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
        cerr << "Can't create socket" << WSAGetLastError() << endl;
        WSACleanup();
        return;
    }

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connResult == SOCKET_ERROR)
    {
        cerr << "Can't connect to server" << WSAGetLastError() << endl;
        closesocket(sock);
        WSACleanup();
        return;
    }

    char buf[4096];
    string userInput;

    do
    {
        cout << "> ";
        getline(cin, userInput);

        if (userInput == "exit")
        {
            cout << "Exiting" << endl;
            break;
        }

        if (userInput.size() > 0)
        {
            int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
            if (sendResult != SOCKET_ERROR)
            {
                ZeroMemory(buf, 4096);
                int bytesReceived = recv(sock, buf, 4096, 0);
                if (bytesReceived > 0)
                {
                    string receivedMessage(buf, 0, bytesReceived);
                    cout << "SERVER> " << receivedMessage << endl;

                    if (userInput != receivedMessage)
                    {
                        cout << "Message mismatch. Resending message" << endl;
                        send(sock, userInput.c_str(), userInput.size() + 1, 0);
                    }
                }
            }
        }

    } while (true);

    closesocket(sock);
    WSACleanup();
}
