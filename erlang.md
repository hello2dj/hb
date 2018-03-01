# 语句要以'.'结尾。
# erlang里的字符串是以ascii码数组形式展示。erlang会尽量尝试解释为字符串，只要其中有一个是无法解析成字符串的就都不显示为字符串。
```
[97,98]
// 'ab' 
```
# 在函数gurads中 ';' 是or的意思，','是and的意思
```
right_age(X) when X > 16, X < 104 ->
true;
right_age(_) ->
false.
```
这个表示的是大于16且小于106

```
wrong_age(X) when X < 16; X > 104 ->
true;
wrong_age(_) ->
false.
```
这个表示的是小于16或者大于104

# 用函数式编程语言来写快排，不可变数据属性，反而使得思想明了
```
quicksort([]) -> [];
quicksort([Pivot|Rest]) ->
  {Smaller, Larger} = partition(Pivot, Rest, [], []),
  quicksort(Smaller) ++ [Pivot] ++ quicksort(Larger).

partition(_, [], Smaller, Larger) -> {Smaller, Larger};
partition(Pivot, [H|T], Smaller, Larger) ->
  if H =< Pivot -> partition(Pivot, T, [H|Smaller], Larger);
     H > Pivot -> partition(Pivot, T, Smaller, [H|Larger])
  end.
```
erlang版的快排，很清晰的告诉了我们快排的核心思想~一次确定一个数字的位置

# erlang中的函数作为参数使用就得 使用 fun Module:Function/Arity这种形式
```
-module(hhfuns).
-compile(export_all).
one() -> 1.
two() -> 2.

add(X, Y) -> X() + Y().
// shell中使用
add(fun hhfuns:one/0, fun hhfuns:two/0).
```

# 匿名函数使用
```
fun() -> ... end.
```

# record 语法类似于结构体,使用-record来定义，可以有默认值， 使用#name{}来初始化赋值
```
-record(robot, {name,
  type=industrial,
  hobbies,
  details=[]}).

first_robot() ->
  #robot{name="Mechatron",
    type=handmade,
    details=["Moved by a small man inside"]}.
```

# rr(Module) 可以加载一个module中的record定义

# record结构 需要使用#recordName{}来进行
```
-record(user, {id, name, group, age}).
 
%% use pattern matching to filter
admin_panel(#user{name=Name, group=admin}) ->
  Name ++ " is allowed!";
admin_panel(#user{name=Name}) ->
  Name ++ " is not allowed".
```

# mailbox的使用
一旦得到了PID，就可以直接发消息给他，不管他在天涯海角，不管他或者还是死亡，erlang里消息的几个事实：
* 总是能发送成功，不管PID是否存在
* 发送消息是异步的，立即返回
* PID ! Message，是一个合法的表达式，返回值是Message，因此往多个进程发同样的消息可以：P1!P2!P3P4!Message
* 同一个进程发给另一个进程的消息保证其发送顺序

消息接收的过程：
* 当有新的消息进来时才触发以下操作
* receive语句会从mailbox里拿出第一条消息（最老的）
* 按顺序进行match，如果match上了，将从mailbox里把这条消息拿掉，然后执行pattern后的语句
* 如果没有match，就执行下一条match
* 如果所有pattern都没match，就取下一条消息
* 如果消息都没有match任何一个pattern，这些消息将按按原来的顺序放回mailbox

＊如果消息都没有match任何一个pattern，这些消息将按按原来的顺序放回mailbox

# receive 的超时问题
```
receive
  exp
after N ->
  exp
end.
```
带有after的后面跟一个N，代表接受数据等待时间若是超时了就停止接收。

# 错误处理也有try of catch

# make_ref返回一个几乎唯一的值，使用这个值来进行标识就意味一定是收到这个消息的人的回复。

```
  Tag = make_ref(),
  Pid ! {Tag, Message},
  receive
     {Tag, Response} ->
```