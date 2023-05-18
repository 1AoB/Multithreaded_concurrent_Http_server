#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

#define SERVER_PORT 80 // 注意:如果你要用80端口,必须在root超级用户下运行代码,否则不可使用,普通用户一般用的是>1023的端口

static int debug = 1;

// 按行读取请求头部
int get_line(int sock, char *buf, int size);
// 处理http 请求,读取客户端发送的数据
void do_http_request(int client_sock);
void do_http_response(int client_sock, const char *path);
// 发送http 头部
int headers(int client_sock, FILE *resource);
// 发送http body
void cat(int client_sock, FILE *resource);

void not_found(int client_sock);
void inner_error(int client_sock);

void unimplemented(client_sock)
{
}
void bad_request(client_sock)
{
}

int main(void)
{

    int sock; // 代表信箱
    struct sockaddr_in server_addr;

    // 1.美女创建信箱
    sock = socket(AF_INET, SOCK_STREAM, 0);

    // 2.清空标签，写上地址和端口号
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;                // 选择协议族IPV4
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 监听本地所有IP地址
    server_addr.sin_port = htons(SERVER_PORT);       // 绑定端口号

    // 端口复用
    int optval = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval)); // sock监听的文件描述符

    // 实现标签贴到收信得信箱上
    bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // 把信箱挂置到传达室，这样，就可以接收信件了
    listen(sock, 128);

    // 万事俱备，只等来信
    printf("等待客户端的连接\n");

    int done = 1;

    while (done)
    {
        printf("循环等待客户端中...\n");
        struct sockaddr_in client;
        int client_sock, len, i;
        char client_ip[64];
        char buf[256];

        socklen_t client_addr_len;
        client_addr_len = sizeof(client);
        // printf("client accept函数正在运行中...\n");
        client_sock = accept(sock, (struct sockaddr *)&client, &client_addr_len);

        // printf("client 来喽!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        //  http://192.168.8.129:80
        //  http://192.168.8.129/demo.html
        // http://192.168.8.129/qiniu.html
        // http://192.168.8.129/taobao.html

        // 打印客服端IP地址和端口号
        printf("client ip: %s\t port : %d\n",
               inet_ntop(AF_INET, &client.sin_addr.s_addr, client_ip,
                         sizeof(client_ip)),
               ntohs(client.sin_port));
        /*处理http 请求,读取客户端发送的数据*/
        do_http_request(client_sock);
        //    我们设计为答完就关闭
        close(client_sock);
    }
    close(sock);
    return 0;
}

void do_http_response(int client_sock, const char *path)
{
    int ret = 0;
    FILE *resource = NULL;

    // 在这种情况下，模式是“r”，代表“read”。这意味着文件将以只读模式打开，您将无法修改其内容。
    resource = fopen(path, "r");

    if (resource == NULL)
    {
        not_found(client_sock);
        return;
    }

    // 1.发送http 头部
    ret = headers(client_sock, resource);

    // 2.发送http body .
    if (!ret)
    {
        cat(client_sock, resource);
    }

    fclose(resource);
}

/****************************
 *返回关于响应文件信息的http 头部
 *输入：
 *     client_sock - 客服端socket 句柄
 *     resource    - 文件的句柄
 *返回值： 成功返回0 ，失败返回-1
 ******************************/

int headers(int client_sock, FILE *resource)
{
    struct stat st;
    int fileid = 0;
    char tmp[64];
    char buf[1024] = {0};
    strcpy(buf, "HTTP/1.0 200 OK\r\n");
    strcat(buf, "Server: Martin Server\r\n"); // 追加
    strcat(buf, "Content-Type: text/html\r\n");
    strcat(buf, "Connection: Close\r\n");

    // 函数fileno（）检查参数流并返回其文件描述符。
    fileid = fileno(resource);

    /*
    stat的玩法
    int stat(const char *path, struct stat *buf);//通过字符串路径
    int fstat(int fd, struct stat *buf);//通过文件描述符fd
    int lstat(const char *path, struct stat *buf);
    */
    if (fstat(fileid, &st) == -1)
    { // 拿到文件描述符,去拿该文件的信息时却出错,这个属于服务器内部的错误
        inner_error(client_sock);
        return -1;
    }

    snprintf(tmp, 64, "Content-Length: %ld\r\n\r\n", st.st_size);
    strcat(buf, tmp);

    if (debug)
        fprintf(stdout, "header: %s\n", buf);

    // send的最后一个参数暂时只设为0就行
    if (send(client_sock, buf, strlen(buf), 0) < 0)
    {
        fprintf(stderr, "send failed. data: %s, reason: %s\n", buf, strerror(errno));
        return -1;
    }

    return 0;
}

/****************************
 *说明：实现将html文件的内容按行
        读取并送给客户端
 ****************************/
void cat(int client_sock, FILE *resource)
{
    char buf[1024];
    // 1. 首先使用 fgets 函数从文件中读取一行文本，并将其存储在缓冲区 buf 中
    fgets(buf, sizeof(buf), resource);
    // printf("=============测试=======================\n");
    //  feof 函数用于判断文件指针所指向的文件是否已经到达了文件末尾。
    //  2. 然后，它使用 while 循环读取剩余行，并将它们发送到客户端。
    //  循环条件是当文件的末尾被检测到时，feof 函数将返回非零值，循环将终止。
    while (!feof(resource))
    {
        // printf("测试:%s\n", buf);
        //  3.在循环体内，
        //  函数使用 write 函数将缓冲区的内容发送到客户端，然后使用 fgets 函数读取下一行内容。
        //  如果发送过程中出现了错误，该函数会在标准错误流上输出一条错误消息，并终止循环。
        int len = write(client_sock, buf, strlen(buf));

        if (len < 0)
        { // 发送body 的过程中出现问题,怎么办？1.重试？ 2.
            fprintf(stderr, "send body error. reason: %s\n", strerror(errno));
            break;
        }

        if (debug)
            fprintf(stdout, "%s", buf);
        fgets(buf, sizeof(buf), resource);
        // 可以发现最后一行没有写入,因为最后一行是</html>,我们只需要将</body>之前的读完即可
    }
    // printf("=============测试=======================\n");
}

// 这个函数是回复固定的内容,目前已被do_http_response函数替换
void do_http_response1(int client_sock)
{
    const char *main_header = "HTTP/1.0 200 OK\r\nServer: Martin Server\r\nContent-Type: text/html\r\nConnection: Close\r\n";

    const char *welcome_content = "\
<html lang=\"zh-CN\">\n\
<head>\n\
<meta content=\"text/html; charset=utf-8\" http-equiv=\"Content-Type\">\n\
<title>This is a test</title>\n\
</head>\n\
<body>\n\
<div align=center height=\"500px\" >\n\
<br/><br/><br/>\n\
<h2>大家好，欢迎来到我的网站！</h2><br/><br/>\n\
<form action=\"commit\" method=\"post\">\n\
尊姓大名: <input type=\"text\" name=\"name\" />\n\
<br/>芳龄几何: <input type=\"password\" name=\"age\" />\n\
<br/><br/><br/><input type=\"submit\" value=\"提交\" />\n\
<input type=\"reset\" value=\"重置\" />\n\
</form>\n\
</div>\n\
</body>\n\
</html>";

    // 1. 发送main_header
    int len = write(client_sock, main_header, strlen(main_header));
    if (debug)
        fprintf(stdout, "... do_http_response...\n");
    if (debug)
        fprintf(stdout, "write[%d]: %s", len, main_header);

    // 2. 生成Content-Length
    char send_buf[64];
    int wc_len = strlen(welcome_content);
    len = snprintf(send_buf, 64, "Content-Length: %d\r\n\r\n", wc_len);
    len = write(client_sock, send_buf, len);

    if (debug)
        fprintf(stdout, "write[%d]: %s", len, send_buf);

    len = write(client_sock, welcome_content, wc_len);
    if (debug)
        fprintf(stdout, "write[%d]: %s", len, welcome_content);
}

void do_http_request(int client_sock)
{
    int len = 0;
    char buf[256];   // 每行的内容
    char method[64]; // 请求方法
    char url[256];   // url资源
    char path[256];  // 存放净化后的url

    struct stat st;

    // 读取客户端发送的http 请求
    //  读取的单位是:一行一行的读
    //   1.读取请求行

    len = get_line(client_sock, buf, sizeof(buf)); // 请求首行读到buf中

    printf("read line:%s\n", buf);

    if (len > 0) // 读到了请求行
    {
        int i = 0, j = 0; // method[i] = buf[j];url[i] = buf[j]; i表示已经赋值过去的
        // 白空格isspace
        while (!isspace(buf[j]) && (i < sizeof(method) - 1))
        {
            method[i] = buf[j];
            i++;
            j++;
        }
        method[i] = '\0'; // 行位
        if (debug)
            printf("request method: %s\n", method);

        // 暂时只处理get请求
        // case表示不区分大小写,i==strlen(method)
        if (strncasecmp(method, "GET", i) == 0) // 不等于0就不相等
        {
            // 只处理get请求
            if (debug)
                printf("正在处理的请求: method = GET\n");

            // 获取url
            while (isspace(buf[j++]))
                ; // 跳过白空格

            i = 0;

            // 不断的读直到遇到白空格，url就读到了
            while (!isspace(buf[j]) && (i < sizeof(url) - 1))
            {
                url[i] = buf[j];
                i++;
                j++;
            }

            url[i] = '\0';

            if (debug)
            {
                printf("url: %s\n", url);
            }

            // 继续读取http 头部
            do
            {
                len = get_line(client_sock, buf, sizeof(buf));
                if (debug)
                    printf("read: %s\n", buf);

            } while (len > 0);

            ///////////////////////////////////////////////////////////////////
            // 定位服务器本地的html文件

            // 将?后面的东西过滤掉
            // 处理url 中的?
            {
                // printf("进入 处理url 中的?\n");
                //  strchr(url, '?');查找字符串url中有没有?
                char *pos = strchr(url, '?'); // pos指向?的位置
                // printf("pos:%s\n", pos);
                if (pos)
                {
                    *pos = '\0'; // 将?后面的东西过滤掉
                    printf("real url: %s\n", url);
                }
                // printf("结束 处理url 中的?\n");
            }

            sprintf(path, "./html_docs/%s", url);
            if (debug)
                printf("path: %s\n", path);

            // 执行http 响应
            // 判断文件是否存在，如果存在就响应200 OK，同时发送相应的html 文件,如果不存在，就响应 404 NOT FOUND.
            if (stat(path, &st) == -1) // stat函数拿到path路径的文件,并保存在st中(st在前面struct stat st;定义过了)
            {                          // 文件不存在或是出错
                fprintf(stderr, "stat %s failed. reason: %s\n", path, strerror(errno));
                not_found(client_sock); // 显示404
            }
            else
            { // 文件存在

                if (S_ISDIR(st.st_mode)) // 如果是一个文件夹
                {
                    strcat(path, "/index.html"); // 追加一个index.html
                }

                do_http_response(client_sock, path); // 回复http的请求
            }
        }
        else
        { // 非get请求, 读取http 头部，并响应客户端 501 	Method Not Implemented
            // 501 方法未实现
            fprintf(stderr, "warning! other request [%s]\n", method);
            do
            {
                len = get_line(client_sock, buf, sizeof(buf));
                if (debug)
                    printf("read: %s\n", buf);
            } while (len > 0);

            unimplemented(client_sock); // 在响应时再实现
        }
    }
    else
    {                             // 请求格式有问题，出错处理
        bad_request(client_sock); // 在响应时再实现
    }
}

// 返回值： -1 表示读取出错， 等于0表示读到一个空行， 大于0 表示成功读取一行
// HTTP的空行是指HTTP协议中请求头和响应头之间的一行空行，通常表示请求头或响应头的结束。
int get_line(int sock, char *buf, int size)
{
    int count = 0;  // 一共读了多少字符
    char ch = '\0'; // 当前读到的字符是什么
    int len = 0;    // 本次read读到的长度

    // 本来=size就已经越界了,所以要<size,还要加一个字符串结束符,
    // 所以<size-1
    while ((count < size - 1) && ch != '\n')
    {
        len = read(sock, &ch, 1);

        if (len == 1) // 成功读取一个字符
        {
            if (ch == '\r') // 回车符 忽略掉
            {
                continue;
            }
            else if (ch == '\n') // 换行符
            {
                // buf[count] = '\0'; // 已经读完
                break;
            }

            // 这里处理一般的字符(非\r \n)
            buf[count] = ch;
            count++;
        }
        else if (len == -1)
        { // 读取出错
            perror("read failed");
            count = -1; // 表示没读到
            break;
        }
        else
        { // read 返回0,客户端关闭sock 连接.
            fprintf(stderr, "client close.\n");
            count = -1; // 表示没读到
            break;
        }
    }

    if (count >= 0)
        buf[count] = '\0';

    return count;
}

void not_found(int client_sock)
{
    const char *reply = "HTTP/1.0 404 NOT FOUND\r\n\
Content-Type: text/html\r\n\
\r\n\
<HTML lang=\"zh-CN\">\r\n\
<meta content=\"text/html; charset=utf-8\" http-equiv=\"Content-Type\">\r\n\
<HEAD>\r\n\
<TITLE>NOT FOUND</TITLE>\r\n\
</HEAD>\r\n\
<BODY>\r\n\
    <P>文件不存在！\r\n\
    <P>The server could not fulfill your request because the resource specified is unavailable or nonexistent.\r\n\
</BODY>\r\n\
</HTML>";

    int len = write(client_sock, reply, strlen(reply));
    if (debug)
        fprintf(stdout, "%s\n", reply);

    if (len <= 0)
    {
        fprintf(stderr, "send reply failed. reason: %s\n", strerror(errno));
    }
}

void inner_error(int client_sock)
{
    const char *reply = "HTTP/1.0 500 Internal Sever Error\r\n\
Content-Type: text/html\r\n\
\r\n\
<HTML lang=\"zh-CN\">\r\n\
<meta content=\"text/html; charset=utf-8\" http-equiv=\"Content-Type\">\r\n\
<HEAD>\r\n\
<TITLE>Inner Error</TITLE>\r\n\
</HEAD>\r\n\
<BODY>\r\n\
    <P>服务器内部出错.\r\n\
</BODY>\r\n\
</HTML>";

    int len = write(client_sock, reply, strlen(reply));
    if (debug)
        fprintf(stdout, "%s\n", reply);

    if (len <= 0)
    {
        fprintf(stderr, "send reply failed. reason: %s\n", strerror(errno));
    }
}

// http://192.168.8.129/demo.html
