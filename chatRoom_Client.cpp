/**           Created by colorline on 20-5-13           **/
/**                      聊天室程序                      **/
/** 客户端功能                                           **/
/**     1.从标准输入终端读入用户数据 并将用户数据发送至服务器  **/
/**     2.往标准输出终端打印收到的服务器数据               **/
#define _GNU_SORCE 1

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <poll.h>
#include <fcntl.h>

#define BUFFER_SIZE 64

int main(int argc, char* argv[]) {
    if(argc <= 2) {
        printf("usage: %s ip_address port_number\n", basename(argv[0]));
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in server_address;
    bzero(&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &server_address.sin_addr);
    server_address.sin_port = htons(port);

    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(sockfd >= 0);
    if(connect(sockfd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        printf("Connection failed!\n");
        close(sockfd);
        return 1;
    }
    pollfd fds[2];
    /** 注册文件描述符0(标注输入)和文件描述符socket上的读写可读事件 **/
    /** fds被监听事件集合 **/
    fds[0].fd = 0;
    fds[0].events = POLLIN;
    fds[0].revents = 0;
    fds[1].fd = sockfd;
    fds[1].events = POLLIN | POLLRDHUP;
    fds[1].revents = 0;

    char read_buf[BUFFER_SIZE];
    /** pipe函数用于创建一个管道实现进程间的通讯 pipefd[1]写入  pipefd[0]读出 **/
    int pipefd[2];
    int ret = pipe(pipefd);
    assert(ret != -1);
    while(1) {
        ret = poll(fds, 2, -1);
        if(ret < 0) {
            printf("poll failure!\n");
            break;
        }
        if(fds[1].revents & POLLRDHUP) {
            printf("server close the connection!\n");
            break;
        }
        else if(fds[1].revents & POLLIN) {  //收到服务器发来的数据 直接显示在标准输出上
            memset(read_buf, '\0', BUFFER_SIZE);
            recv(fds[1].fd, read_buf, BUFFER_SIZE - 1, 0);
            printf("%s\n", read_buf);
        }
        if(fds[0].revents & POLLIN) {   //使用splice函数零拷贝 将数据写入到管道pipefd[1]
            /** 使用splice将用户输入的数据直接写到sockfd上(零拷贝) **/
            ret = splice(0, NULL, pipefd[1], NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);
            ret = splice(pipefd[0], NULL, sockfd, NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);
        }
    }
    close(sockfd);
    return 0;
}