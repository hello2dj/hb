### keywords key 必须是原子，并且 key 可以不是唯一的,并且顺序是固定的， 两种形式

```
[foo: "dje", bar: "d", foo: "adsf"]
[{:foo, "dje"}, {:bar: "d"}, {:foo, "adsf"}]
```

### map 的键值可以是任意类型，并且没有排序，定义形式

```
%{:foo => "bar", "hello" => :world}
```

1.  添加重复的 key 值会覆盖之前的
2.  当 key 值只有原子时
    1.  可以使用：代替=>
    2.  map 有自己的获取元素的方法 a.foo
    3.  map 有自己的更新方法 %{map | foo: "bz"}

1. 可以有Map Module进行操作
2. elixir中不可以有大写开头的变量，那都是Module

