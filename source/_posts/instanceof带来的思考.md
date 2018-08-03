---
title: instanceof带来的思考与es规范的研究
date: 2018-06-06
tags:
---

源码面前了无秘密，规范面前也一样啊！

<!-- more -->

### 问题
今天在jjc的群里正美大大抛出了一个问题

{% asset_img question.jpeg 问题 %}

答案全是false

### 郁闷点
我很郁闷了，我觉得按照instanceof的执行过程应该是true啊！
为啥呢？因为一下两点
* 我记忆中instanceof的流程是，比如： "k instanceof v"就是验证v.prototype 在 k的原型链中

证明如下
{% asset_img instanceof验证问题.jpg 验证 %}

* 而在REPL中查看 如下
```
(2).\__proto\__ === Number.prototype
// true
```
那么为啥 2 instanceof Number 是false呢

### 接下来我要开始装逼了

> 下面的es规范是基于[ecma-262/5.1](https://www.ecma-international.org/ecma-262/5.1/)的，es6的规范有了一些变化，有了更多的新东西，更加严谨，但原理是一致的。

* 先来讲讲js里的原始类型

我们知道js中的原始类型(primitive) 只有如下几种
```
Undefined, Null, Boolean, String, Number
```
>  es6中多了一个原始类型 Symbol

要注意，Number是指原始类型而不是内置的Number对象, 其他类似

因此 3, '123', true, false, null, undefined等是原始类型，不是对象(Object)哦。

也就是说3 和 new Number(3)或者Number(3)不是一个类型哦，后两者是对象
参见下图

{% asset_img number.png 验证 %}

> 各位看官有没有看到里面的另一句话啊: Number object 可以通过是用Number函数方式的调用在转换为Number value。 Number(new Number(3)) instanceof Number === false

剩下的就是对象了Object，各种对象，就不缺对象

### 接下来上 instanceof 的规范, 一步一步揭开秘密

例子
```
3 instanceof Number
```
我们一步一步讲来
#### 1. The instanceof operator
> The production RelationalExpression : RelationalExpression instanceof ShiftExpression is evaluated as follows:

1. Let lref be the result of evaluating RelationalExpression.
> 计算左侧 为3
2. Let lval be GetValue(lref).
> GetValue 调用返回值赋值给lval(此处我们只要知道GetValue(3) === 3就好了，后面会细讲)
3. Let rref be the result of evaluating ShiftExpression.
> 计算Number ，那就是Number
4. Let rval be GetValue(rref).
> 同2 这里rval = Number
5. If Type(rval) is not Object, throw a TypeError exception.
> Type 调用会返回rval的类型，此处是对象Object[1]。
6. If rval does not have a [[HasInstance]] internal method, throw a TypeError exception.
> 判断rval 是否有HasInstance 方法
7. Return the result of calling the [[HasInstance]] internal method of rval with argument lval.
使用lval作为参数调用rval的[[HasInstance方法]] 就是 Number[[HasInstance]] (3)

接下来就该[[HasInstance]]了
#### 2. [[HasInstance]] (V)
Assume F is a Function object.
> 假设F是一个函数对象

When the [[HasInstance]] internal method of F is called with value V, the following steps are taken:

> 当我们调用F的[[HasInstance]]方法时走如下步骤

1. If V is not an object, return false.
> 如果v不是对象，返回false
> 很明显我们上面的调用就结束了。。。因为3不是个对象，而是个Number value
2. Let O be the result of calling the [[Get]] internal method of F with property name "prototype".
> 获取 F的prototype 给 O
3. If Type(O) is not Object, throw a TypeError exception.
4. Repeat
    * Let V be the value of the [[Prototype]] internal property of V.
    * If V is null, return false.
    * If O and V refer to the same object, return true.
    > 找到V的原型链，然后依次向上查找，直到结束，若O与其中一个原型是同一个对象返回true, 否则false

> NOTE Function objects created using Function.prototype.bind have a different implementation of [[HasInstance]] defined in 15.3.4.5.3.

##### 到此我们已经搞定为了为啥 3 instanceof Number 是false了，打完，不收工。
还有一个Symbol啊，在此有个很魔性的地方，我上面引入的ecma262/5.1是没有Symbol的，我们去看ecma262/6.0，里面是有Symbol的，可是在那里我们可以看到Symbol也进级为了原始类型，因此Symbol('a') instanceof Symbol自然也是 false了，你说魔性不魔性。

### 结束了？no 不结束！GetValue我们还没讲呢

#### 在此我们先来看看 Reference是啥
官方解释如下
```
The Reference type is used to explain the behaviour of such operators
as delete, typeof, the assignment operators, the super keyword and
other language features. For example, the left-hand operand of an
assignment is expected to produce a reference.
```
我只看明白是为了解释某些操作而存在的，例如delete, typeof, super, left-hand operand等等,还有super keyword等等

Reference 含有3个component
1. base value: any 甚至是Environment Record
2. referenced name: string or symbol
3. strict reference flag: true or false

#### 那什么时候会创建一个Reference呢 规范并没有说怎么设置，我在stackoverflow 上看到了以下三个会创建并返回Reference的情况
1. [identifier reference](https://www.ecma-international.org/ecma-262/5.1/#sec-11.1.2) expressions, that [resolve the identifier](https://www.ecma-international.org/ecma-262/5.1/#sec-10.3.1) in the current lexical environment (or one of its parents)
标识符，解析标识符时会创建一个reference, base value is envRec, referenced name is identifier

2. [property accessor expressions](http://es5.github.io/#x11.2.1), i.e. the .… and […] operators
属性获取时会创建一个reference(这个就是我们后面要用到的哦)

3. [function calls](http://www.ecma-international.org/ecma-262/5.1/#sec-15.3.4.4) to host functions are permitted to return them, [but such don't exist](https://stackoverflow.com/q/13124417/1048572).
这个在es5规范里有说，是可以返回，但没有规定一定返回

#### 接下来说说我的另一个问题了，就是下面的类型转换是怎么发生的？Number value => Number object
```
(2).\__proto\__
// [Number: 0]
```
上面的转换很显然是获取属性发生的转换，我们一步一步解析，请看下面
###### 1. "(2).\__proto\__" 是啥, 从表达式走起
```
11 Expressions

11.1 Primary Expressions
Syntax
    PrimaryExpression :
        this
        Identifier
        Literal
        ArrayLiteral
        ObjectLiteral
        ( Expression )

```
参见上面，我们"(2)"是PrimaryExpression, 再来看看 (Expression)是啥

```
11.1.6 The Grouping Operator
    The production PrimaryExpression : ( Expression ) is evaluated as follows:

        1. Return the result of evaluating Expression. This may be of type Reference.
```
> NOTE This algorithm does not apply GetValue to the result of evaluating Expression. The principal motivation for this is so that operators such as delete and typeof may be applied to parenthesised expressions.
很显然我们"(2)"是group expression,按照规范的描述，group expression返回的有可能是Reference

###### 2. "2" 咋计算
看了上面我们知道了"(2)"是啥，那按照规范所说的，"(2)"属于Express, 也可以看出来"2" 属于PrimaryExpression 中的Literal, 于是就有了下面的
```
11.1.3 Literal Reference
    A Literal is evaluated as described in 7.8.
```
看到了Literal 参见[7.8](http://www.ecma-international.org/ecma-262/5.1/#sec-7.8) 具体就是规定了Number literal的语法

###### 3. (2)我们解决了，那么就该"(2).\__proto\__"了， 这是一个[属性获取](http://www.ecma-international.org/ecma-262/5.1/#sec-11.2.1)，于是有了下面的规范, 规范先描述了属性获取的语法"."和"[]",然后是算法描述

The production MemberExpression : MemberExpression [ Expression ] is evaluated as follows:

1. Let baseReference be the result of evaluating MemberExpression.
> 左侧计算 得到 2 赋值给baseReference
2. Let baseValue be GetValue(baseReference).
> 调用GetValue, 对2 ，依然返回2
3. Let propertyNameReference be the result of evaluating Expression.
> 计算属性Expression 赋值给 propertyNameReference， 对于'.'调用会变为'[<identifer-name-string>]' 于是 就是"\__proto\__"字符串
4. Let propertyNameValue be GetValue(propertyNameReference).
> 调用GetValue, 对于propertyNameReference ，依然返回"\__proto\__"字符串
5. Call CheckObjectCoercible(baseValue).
> 判断baseValue不是Null或者Undefined
6. Let propertyNameString be ToString(propertyNameValue).
> 转为String
7. If the syntactic production that is being evaluated is contained in strict mode code, let strict be true, else let strict be false.

8. Return a value of type Reference whose base value is baseValue and whose referenced name is propertyNameString, and whose strict mode flag is strict.
> 返回 Reference, base value是baseValue，对于我们的baseValue就是2，referenced name 是 propertyNameString，对于我们就是"\__proto\__"

The production CallExpression : CallExpression [ Expression ] is evaluated in exactly the same manner, except that the contained CallExpression is evaluated in step 1.
> 这句话的意思就是 CallExpress和属性获取类似。

到这里我们就完成了 "(2).\__proto\__"的完整解析，这个完整解析返回了一个Reference, 可是类型转换呢？

###### 4. 对属性获取返回的Reference的使用
上面的步骤我们是完成了整个表达式的解析，但还没有使用表达式返回的结果，那我们来使用一下，比如 "(2).\__proto\__ === 2"
11.9.4 The Strict Equals Operator ( === )
    The production EqualityExpression : EqualityExpression === RelationalExpression is evaluated as follows:

1. Let lref be the result of evaluating EqualityExpression.
> 解析 EqualityExpression ,我们从前面可以得知，"(2).\__proto\__"， 解析之后是一个Reference, 类似于 {baseValue: 2, referencedName: "\__proto\__", strict:...}
2. Let lval be GetValue(lref).
> 重头戏来了 把我们上一步得到的Reference 作为参数调用 GetValue会返回什么呢？ 见下

3. Let rref be the result of evaluating RelationalExpression.
4. Let rval be GetValue(rref).
5. Return the result of performing the strict equality comparison rval === lval. (See 11.9.6)

###### 5. GetValue调用
上面的严格等于我们就不分析了，不是重点，来看看GetValue吧。很长很高能

8.7.1 GetValue (V)
1. If Type(V) is not Reference, return V.
> 判断V是不是一个Reference,不是直接返回V， 现在明白了GetValue(2)为啥返回2了吧。
2. Let base be the result of calling GetBase(V).
> 获取base value, 对我们的Ref 就是2
3. If IsUnresolvableReference(V), throw a ReferenceError exception.
> 这是用来判断base value为非null和undefined的
4. If IsPropertyReference(V), then
    1. If HasPrimitiveBase(V) is false, then let get be the [[Get]] internal method of base, otherwise let get be the special [[Get]] internal method defined below.
    > base value 不是原始类型，则让get为base的内部[[Get]]方法，若是原始类型则参见下面的internal [[GET]]方法的逻辑，嗯，我们的base value是2，于是就得走下面的internal [[GET]]方法了
    2. Return the result of calling the get internal method using base as its this value, and passing GetReferencedName(V) for the argument. 
    > 调用get，使用referenceName作为参数, base value作为this
> 判断V的base value是不是object, number, string, Boolean
    
5. Else, base must be an environment record.
> 不是 上面列举的几类，那就一定是environment record
    1. Return the result of calling the GetBindingValue (see 10.2.1) concrete method of base passing GetReferencedName(V) and IsStrictReference(V) as arguments.

The following [[Get]] internal method is used by GetValue when V is a property reference with a primitive base value. It is called using base as its this value and with property P as its argument. The following steps are taken:
> internal [[GET]]method

1. Let O be ToObject(base).
> 我的神啊，神啊，神啊，神啊，神啊，神啊，神啊，神啊，神啊，神啊，神啊，神啊，神啊，神啊，神啊， 终于看到了类型转换啊！！！ ToObject(base), 这个很简单了，把原始类型转换为对应Object类型，比如2(Number Value), 转换为Number Object, 其他类似 [参见](http://www.ecma-international.org/ecma-262/5.1/#sec-9.9)

2. Let desc be the result of calling the [[GetProperty]] internal method of O with property name P.
> ...不解释了，往下就很容易理解了
3. If desc is undefined, return undefined.
4. If IsDataDescriptor(desc) is true, return desc.[[Value]].
5. Otherwise, IsAccessorDescriptor(desc) must be true so, let getter be desc.[[Get]] (see 8.10).
6. If getter is undefined, return undefined.
7. Return the result calling the [[Call]] internal method of getter providing base as the this value and providing no arguments.

> NOTE The object that may be created in step 1 is not accessible outside of the above method. An implementation might choose to avoid the actual creation of the object. The only situation where such an actual property access that uses this internal method can have visible effect is when it invokes an accessor function. 这些说的就是step 1创建的object，不应当被外部访问到。

### 总结
源码面前了无秘密，规范面前也一样啊！其实但我们理解了Reference 类型，那么this绑定的问题也就迎刃而解了，大家可以再去看看[this的绑定问题](https://www.ecma-international.org/ecma-262/5.1/#sec-11.2.3)也是基于Reference的

注
[1][Type(v)解释](http://www.ecma-international.org/ecma-262/6.0/#sec-ecmascript-data-types-and-values)
> 根据具体的内容返回相应的类型，如字面3,返回Number Type，new Number(3) 返回Object 类型