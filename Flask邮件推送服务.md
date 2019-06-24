[TOC]

# SMTP邮件推送以及验证码

## 验证码的生成方式

- 在进行登陆注册的时候，往往需要有验证的功能，以及很多地方都需要有类似的功能

- 其原理主要是利用random函数的sample函数而实现的
- 代码如下

```python
code_element_list = []
# 添加0-9数字
for i in range(10):
    code_element_list.append(str(i))
# 添加A-Z数字
for i in range(65, 91):
    code_element_list.append(chr(i))
# 添加a-z数字
for i in range(97, 123):
    code_element_list.append(chr(i))

def generate_verification_code():
    '''生成6位验证码
    参数：
        input:None
        output: code
    '''
    return ''.join(random.sample(code_element_list, 6))
```

## SMTP邮件推送

- 构造一个邮件对象就是一个`Messag`对象，如果构造一个`MIMEText`对象，就表示一个文本邮件对象，如果构造一个`MIMEImage`对象，就表示一个作为附件的图片，要把多个对象组合起来，就用`MIMEMultipart`对象，而`MIMEBase`可以表示任何对象。它们的继承关系如下

```
Message
	MIMEBase
        MIMEMultipart
        MIMENonMultipart
            MIMEMessage
            MIMEText
            MIMEImage
```

以下是阿里云邮件推送服务的python API接口：是一份十分不错的样例，可以参照着使用。

```python
# -*- coding:utf-8 -*-
import smtplib
import email
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
from email.mime.image import MIMEImage
from email.mime.base import MIMEBase
from email.mime.application import MIMEApplication
from email.header import Header
# 发件人地址，通过控制台创建的发件人地址
username = '***'
# 发件人密码，通过控制台创建的发件人密码
password = '***'
# 自定义的回复地址
replyto = '***'
# 收件人地址或是地址列表，支持多个收件人，最多30个
#rcptto = '***,***'
rcptto = '***'
# 构建alternative结构
msg = MIMEMultipart('alternative')
msg['Subject'] = Header('自定义信件主题'.decode('utf-8')).encode()
msg['From'] = '%s <%s>' % (Header('自定义发信昵称'.decode('utf-8')).encode(), username)
msg['To'] = rcptto
msg['Reply-to'] = replyto
msg['Message-id'] = email.utils.make_msgid()
msg['Date'] = email.utils.formatdate() 
# 构建alternative的text/plain部分
textplain = MIMEText('自定义TEXT纯文本部分', _subtype='plain', _charset='UTF-8')
msg.attach(textplain)
# 构建alternative的text/html部分
texthtml = MIMEText('自定义HTML超文本部分', _subtype='html', _charset='UTF-8')
msg.attach(texthtml)
# 发送邮件
try:
    client = smtplib.SMTP()
    #python 2.7以上版本，若需要使用SSL，可以这样创建client
    #client = smtplib.SMTP_SSL()
    #SMTP普通端口为25或80
    client.connect('smtpdm.aliyun.com', 25)
    #开启DEBUG模式
    client.set_debuglevel(0)
    client.login(username, password)
    #发件人和认证地址必须一致
    #备注：若想取到DATA命令返回值,可参考smtplib的sendmaili封装方法:
    #      使用SMTP.mail/SMTP.rcpt/SMTP.data方法
    client.sendmail(username, rcptto, msg.as_string())
    client.quit()
    print '邮件发送成功！'
except smtplib.SMTPConnectError, e:
    print '邮件发送失败，连接失败:', e.smtp_code, e.smtp_error
except smtplib.SMTPAuthenticationError, e:
    print '邮件发送失败，认证错误:', e.smtp_code, e.smtp_error
except smtplib.SMTPSenderRefused, e:
    print '邮件发送失败，发件人被拒绝:', e.smtp_code, e.smtp_error
except smtplib.SMTPRecipientsRefused, e:
    print '邮件发送失败，收件人被拒绝:', e.smtp_code, e.smtp_error
except smtplib.SMTPDataError, e:
    print '邮件发送失败，数据接收拒绝:', e.smtp_code, e.smtp_error
except smtplib.SMTPException, e:
    print '邮件发送失败, ', e.message
except Exception, e:
    print '邮件发送异常, ', str(e)
```

- 如果想要比较好看的模板，可以直接打开自己的邮件挑选比较好看的之前网站发送的验证界面
- 然后按F12，然后选择元素



- 将代码copy过来就好了