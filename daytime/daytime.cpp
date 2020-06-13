#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;

void errHandler(const char* why, const int exitCode = 1)
{
    cerr << why << endl;
    exit(exitCode);
    }
int main(int argc, char **argv)
{
    // подготовка стуктуры с адресом клиента
    sockaddr_in *selfAddr = new (sockaddr_in); // выделение памяти под структуру
    selfAddr->sin_family = AF_INET;            // выбор семейства адресов IPv4
    selfAddr->sin_port = 0;                    // любой порт на усмотрение ОС
    selfAddr->sin_addr.s_addr = 0;             // все адреса нашего компьютера
    
    // подготовка структуры с удаленном сервером
    sockaddr_in *remoteAddr = new (sockaddr_in); // выделение памяти под структуру
    remoteAddr->sin_family = AF_INET;            // выбор семейства адресов IPv4
    remoteAddr->sin_port = ntohs(13);            // порт 13
    remoteAddr->sin_addr.s_addr = inet_addr("82.179.90.12");
    
    // подготовка буфера для передачи и приема данных
    char *buf = new char [256];
    strcpy(buf, "Give us time!!!\n");
    int msgLen = strlen(buf);

    // создание сокета
    int mySocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (mySocket == -1) {
        close(mySocket);
        errHandler("Error open socket!", 11);
    }
    
    // привязака сокета к адресу
    int rc = bind(mySocket, (const sockaddr *) selfAddr, sizeof (sockaddr_in));
    if (rc == -1) {
        close(mySocket);
        errHandler("Error bind socket with locale addres", 12);
    }
    
    // установка соединения
    rc = connect(mySocket, (const sockaddr *) remoteAddr, sizeof (sockaddr_in));
    if (rc == -1) {
        close(mySocket);
        errHandler("Error connect socket with remote server", 13);
    }
    
    // передача данных
    rc = send(mySocket, buf, msgLen, 0);
    if (rc == -1) {
        close(mySocket);
        errHandler("Error send massage", 14);
    }
    cout << "We send: "<< buf << endl;
    
    // получение данных
    rc = recv(mySocket, buf, 256, 0);
    if (rc == -1) {
        close(mySocket);
        errHandler("Error receive ansver", 15);
    }
    buf[rc] = '\0';
    cout << "We receive: " << buf << endl;
    
    // закрытие сокета
    close(mySocket);
    
    delete selfAddr;
    delete remoteAddr;
    delete[] buf;
    return 0;
}