# 这个是多线程并发Http服务器项目


# 在你运行这个项目之前，请认真阅读以下代码及注释！！！！！！！！！！！
1. 使用Microsoft Edge体验效果最佳
2. 请将"minihttp版本*.c",名字中的的"版本*"去掉后再编译
3. 请使用root权限再运行代码(代码中我们用的是80端口)
```c
#define SERVER_PORT 80 
// 注意:如果你要用80端口,必须在root超级用户下运行代码,否则不可使用,普通用户一般用的是>1023的端口
```
再次强调:如果你要用**80端口**,必须**在root超级用户下**运行代码,否则不可使用,普通用户一般用的是>1023的端口

# 创建子线程
```c
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                                  void *(*start_routine) (void *), void *arg);
1. thread:直接初始化一个pthread_t thread_id;它不用做任何初始化,直接&thread_id放进去即可;它的作用是标识当前线程的id,相当于当前线程的身份证
2. attr:置为NULL即可
3. start_routine:第三个参数是返回值为void*的函数
4. arg: 传递给start_routine函数的参数。它可以是任何类型的指针，只要将它强制转换为void*类型即可。
```
```c
eg:
void *do_http_request(void *pclient_sock);//具体代码参考minihttp版本2.c

int client_sock;
client_sock = accept(sock, (struct sockaddr *)&client, &client_addr_len);

pthread_t id;
int *pclient_sock = NULL;

pclient_sock = (int *)malloc(sizeof(int));
*pclient_sock = client_sock;

pthread_create(&id, NULL, do_http_request, (void *)pclient_sock);
```
# 截取某个网站
```bash
curl -L https://www.taobao.com -o ./html_docs/taobao.html

```
# 运行过程服务器打印的信息
当访问到服务器不存在的网页时：
```
等待客户端的连接
循环等待客户端中...
client ip: 192.168.8.1   port : 60296
read line:GET /baidu.html HTTP/1.1
request method: GET
正在处理的请求: method = GET
url: baidu.html
read: Host: 192.168.8.129
read: Connection: keep-alive
read: Upgrade-Insecure-Requests: 1
read: User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/113.0.0.0 Safari/537.36
read: Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7
read: Accept-Encoding: gzip, deflate
read: Accept-Language: zh-CN,zh;q=0.9
read: Cookie: cna=H6TrHCNLtgkCARKzzMnJZtfu; BD_UPN=12314753; isg=BLm5VCiZ-cyw6KVY_YMan-UJyCWTxq145fn_JNvuFuBfYtj0IxdpSONz4OaUQUWw
read: 
path: ./html_docs/baidu.html
stat ./html_docs/baidu.html failed. reason: No such file or directory
HTTP/1.0 404 NOT FOUND
Content-Type: text/html

<HTML lang="zh-CN">
<meta content="text/html; charset=utf-8" http-equiv="Content-Type">
<HEAD>
<TITLE>NOT FOUND</TITLE>
</HEAD>
<BODY>
    <P>文件不存在！
    <P>The server could not fulfill your request because the resource specified is unavailable or nonexistent.
</BODY>
</HTML>
循环等待客户端中...
client ip: 192.168.8.1   port : 60297
read line:GET /favicon.ico HTTP/1.1
request method: GET
正在处理的请求: method = GET
url: favicon.ico
read: Host: 192.168.8.129
read: Connection: keep-alive
read: User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/113.0.0.0 Safari/537.36
read: Accept: image/avif,image/webp,image/apng,image/svg+xml,image/*,*/*;q=0.8
read: Referer: http://192.168.8.129/baidu.html
read: Accept-Encoding: gzip, deflate
read: Accept-Language: zh-CN,zh;q=0.9
read: Cookie: cna=H6TrHCNLtgkCARKzzMnJZtfu; BD_UPN=12314753; isg=BLm5VCiZ-cyw6KVY_YMan-UJyCWTxq145fn_JNvuFuBfYtj0IxdpSONz4OaUQUWw
read: 
path: ./html_docs/favicon.ico
stat ./html_docs/favicon.ico failed. reason: No such file or directory
HTTP/1.0 404 NOT FOUND
Content-Type: text/html

<HTML lang="zh-CN">
<meta content="text/html; charset=utf-8" http-equiv="Content-Type">
<HEAD>
<TITLE>NOT FOUND</TITLE>
</HEAD>
<BODY>
    <P>文件不存在！
    <P>The server could not fulfill your request because the resource specified is unavailable or nonexistent.
</BODY>
</HTML>
循环等待客户端中...
```


当访问到服务器存在的网页时：
```
等待客户端的连接
循环等待客户端中...
client ip: 192.168.8.1   port : 60395
read line:GET /demo.html HTTP/1.1
request method: GET
正在处理的请求: method = GET
url: demo.html
read: Host: 192.168.8.129
read: Connection: keep-alive
read: Upgrade-Insecure-Requests: 1
read: User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/113.0.0.0 Safari/537.36
read: Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7
read: Accept-Encoding: gzip, deflate
read: Accept-Language: zh-CN,zh;q=0.9
read: Cookie: cna=H6TrHCNLtgkCARKzzMnJZtfu; BD_UPN=12314753; isg=BLm5VCiZ-cyw6KVY_YMan-UJyCWTxq145fn_JNvuFuBfYtj0IxdpSONz4OaUQUWw
read: 
path: ./html_docs/demo.html
header: HTTP/1.0 200 OK
Server: Martin Server
Content-Type: text/html
Connection: Close
Content-Length: 616


<html lang="zh-CN">

<head>
    <meta content="text/html; charset=utf-8" http-equiv="Content-Type">
    <title>This is a test</title>
</head>

<body>
    <div align=center height="500px">
        <br /><br /><br />
        <h2>大家好,欢迎来到我的网站!</h2><br /><br />
        <form action="commit" method="post">
            尊姓大名: <input type="text" name="name" />
            <br />芳龄几何: <input type="password" name="age" />
            <br /><br /><br /><input type="submit" value="提交" />
            <input type="reset" value="重置" />
        </form>
    </div>
</body>

循环等待客户端中...
client ip: 192.168.8.1   port : 60396
```
