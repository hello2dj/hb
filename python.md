### object和type的关系 https://www.zhihu.com/question/38791962

1. object的父子关系的顶端，也就是继承关系的顶端，object也是个类型
2. type是类型关系的顶端，所有的类型都是type实例化来的，type也是个对象

> 因此利用type就可以动态创建类型，利用object就可以动态实例化对象
```
>>> def fn(self, name='world'): # 先定义函数
...     print('Hello, %s.' % name)
...
>>> Hello = type('Hello', (object,), dict(hello=fn)) # 创建Hello class
>>> h = Hello()
>>> h.hello()
Hello, world.
>>> print(type(Hello))
<type 'type'>
>>> print(type(h))
<class '__main__.Hello'>


# metaclass是创建类，所以必须从`type`类型派生：这是python2的写法
class ListMetaclass(type):
    def __new__(cls, name, bases, attrs):
        print(cls, name, bases, attrs)
        attrs['add'] = lambda self, value: self.append(value)
        return type.__new__(cls, name, bases, attrs)

class MyList(list):
    __metaclass__ = ListMetaclass # 指示使用ListMetaclass来定制类


# python3的写法
class ListMetaclass(type):
    def __new__(cls, name, bases, attrs):
        c.append((cls, name, bases, attrs))
        attrs['des'] = lambda self, value: self.append(value)
        return type.__new__(cls, name, bases, attrs)

class MyList(list, metaclass = ListMetaclass):
    pass
```



### python super使用方法 http://www.runoob.com/python/python-func-super.html

### https://stackoverflow.com/questions/14301967/python-bare-asterisk-in-function-argument
函数定义带星意味着强制使用命名参数


#### https://stackoverflow.com/questions/9432719/python-how-can-i-inherit-from-the-built-in-list-type
继承内置类型时调用父类型的初始化方法要注意传递self, 还有关于super的使用和其他language不同
```
List.__init__(self, [1,2,3])
```


### mysql-Python 安装（2.X）
1. brew install mysql
2. brew install mysq-connector-c
3. brew install mysql_config
    修改mysql_config的 (https://stackoverflow.com/questions/43543483/pip-install-mysql-python-fails-with-indexerror)
    ```
    #Create options
    Libs = "-L $ pkglibdir "
    Libs = " $ libs   -l"

    > change into:
    
    #Create options
    Libs = "- L $ pkglibdir"
    Libs = "$ libs -lmysqlclient -lssl -lcrypto"
    ```