# 这个是多线程并发Http服务器项目


# 在你运行这个项目之前，请认真阅读以下代码及注释！！！！！！！！！！！
```c
#define SERVER_PORT 80 
// 注意:如果你要用80端口,必须在root超级用户下运行代码,否则不可使用,普通用户一般用的是>1023的端口
```
注意:如果你要用**80端口**,必须**在root超级用户下**运行代码,否则不可使用,普通用户一般用的是>1023的端口




不存在：
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

存在：
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