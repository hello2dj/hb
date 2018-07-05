### nginx 白名单

*   Module ngx_http_access_module
*   Module ngx_http_realip_module

见 https://leo108.com/pid-2132/
使用

```
real_ip_header X-Forwarded-For
set_real_ip_from ...
```

若是 nginx 也在代理的后面的话是拿不到真实访问 ip 的，因此需要使用 ngx_http_realip_module 来获取真实 ip, set_real_ip_from 是指 nginx 的代理是谁，real_ip_header 是指真实 ip 在哪里
