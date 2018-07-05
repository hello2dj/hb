### null 表示"没有对象"，即该处不应该有值。典型用法是：

（1） 作为函数的参数，表示该函数的参数不是对象。

（2） 作为对象原型链的终点。

```
Object.getPrototypeOf(Object.prototype)
// null
```

### undefined 表示"缺少值"，就是此处应该有一个值，但是还没有定义。典型用法是：

（1）变量被声明了，但没有赋值时，就等于 undefined。

（2) 调用函数时，应该提供的参数没有提供，该参数等于 undefined。

（3）对象没有赋值的属性，该属性的值为 undefined。

（4）函数没有返回值时，默认返回 undefined。

```
var i;
i // undefined

function f(x){console.log(x)}
f() // undefined

var  o = new Object();
o.p // undefined

var x = f();
x // undefined
```

### == 比较先转换成相同类型在进行 strict compare， null == undefined true

### for in 的一些介绍https://javascriptweblog.wordpress.com/2011/01/04/exploring-javascript-for-in-loops/

### for-in 循环也枚举原型链中的属性，Object.entries()不会枚举原型链中的属性，是返回一个给定对象自身可枚举属性的键值对数组

###

*   children 是指 DOM Object 类型的子对象，不包括 tag 之间隐形存在的 TextNode；

*   childNodes 包括 tag 之间隐形存在的 TextNode 对象。

###

*   firstChild 有可能返回的是文本节点（TextNode）
*   firstElementChild 返回的是 TagNode

### Dom nodeType 的文章（http://www.w3school.com.cn/jsref/prop_node_nodetype.asp）

*   1 element
*   2 attr
*   3 text

### https://github.com/YIXUNFE/blog/issues/63 js 中的数字

~ [按位取反的过程](https://es5.github.io/#x11.4.8)

```
let x = 0;

async function test() {
    x += await 2;
    console.log(x);
}

test();
x += 1;
console.log(x)
```
输出1， 2（test 函数输出因为词法作用域所以，test函数内的x是0)

### Number上接口 JavaScript 能够准确表示的整数范围在-2^53到2^53之间（不含两个端点）http://www.waibo.wang/bible/es6/html/6/6.6.html
```
Number.MAX_SAFE_INTEGER === Math.pow(2, 53) - 1
// true
Number.MAX_SAFE_INTEGER === 9007199254740991
// true
Number.MIN_SAFE_INTEGER === -Number.MAX_SAFE_INTEGER
// true
Number.MIN_SAFE_INTEGER === -9007199254740991
// true
```
* isInteger 判断整数
* Number.isSafeInteger()则是用来判断一个整数是否落在这个范围之内。

>   JavaScript能表示并进行精确算术运算的整数范围为：正负2的53次方，也即从最小值-9007199254740992到最大值+9007199254740992之间的范围；对于超过这个范围的整数，JavaScript依旧可以进行运算，但却不保证运算结果的精度。值得注意的是，对于整数的位运算（比如移位等操作），JavaScript仅支持32位整型数，也即从-2147483648到+2147483647之间的整数。

###

### array length 的问题
```
let colors = ["red", "green", "blue"];

console.log(colors.length);         // 3

colors[3] = "black";

console.log(colors.length);         // 4
console.log(colors[3]);             // "black"

colors.length = 2;

console.log(colors.length);         // 2
console.log(colors[3]);             // undefined
console.log(colors[2]);             // undefined
console.log(colors[1]);             // "green"
```

### js 结构赋值
1. 数组
2. 字符串
3. 对象
3. 数值和布尔值 解构赋值时，如果等号右边是数值和布尔值，则会先转为对象。
```
let {toString: s} = 123;
s === Number.prototype.toString // true
let {toString: s} = true;
s === Boolean.prototype.toString // true
```
用途
1. 交换变量
```
let x = 1;
let y = 2;
[x, y] = [y, x];
```

2. 返回多个值
```
// 返回一个数组
function example() {
 return [1, 2, 3];
}
let [a, b, c] = example();
// 返回一个对象
function example() {
 return {
 foo: 1,
 bar: 2
 };
}
let { foo, bar } = example();
```

3. 函数参数的定义
```
// 参数是一组有次序的值
function f([x, y, z]) { ... }
f([1, 2, 3]);
// 参数是一组无次序的值
function f({x, y, z}) { ... }
f({z: 3, y: 2, x: 1});
```
4. 提取对象中的数值
```
let jsonData = {
 id: 42,
 status: "OK",
 data: [867, 5309]
};
let { id, status, data: number } = jsonData;
console.log(id, status, number);
// 42, "OK", [867, 5309]
```

5. 函数默认参数
```
jQuery.ajax = function (url, {
 async = true,
 beforeSend = function () {},
 cache = true,
 complete = function () {},
 crossDomain = false,
 global = true,
 // ... more config
}) {
 // ... do stuff
};
```

6. 遍历map结构
```
const map = new Map();
map.set('first', 'hello');
map.set('second', 'world');
for (let [key, value] of map) {
 console.log(key + " is " + value);
}
// first is hello
// second is world

// 获取键名
for (let [key] of map) {
 // ...
}
// 获取键值
for (let [,value] of map) {
 // ...
}
```

7. 导入指定模块方法
```
const { SourceMapConsumer, SourceNode } = require("source-map");
```

### ecma 类型
1. ECMAScript 的类型分为语言类型和规范类型。

2. ECMAScript 语言类型是开发者直接使用 ECMAScript 可以操作的。其实就是我们常说的Undefined, Null, Boolean, String, Number, 和 Object。

3. 而规范类型相当于 meta-values，是用来用算法描述 ECMAScript 语言结构和 ECMAScript 语言类型的。规范类型包括：Reference, List, Completion, Property Descriptor, Property Identifier, Lexical Environment, 和 Environment Record。

### Reference
这里的 Reference 是一个 Specification Type，也就是 “只存在于规范里的抽象类型”。它们是为了更好地描述语言的底层行为逻辑才存在的，但并不存在于实际的 js 代码中。

### instanceof 
k instanceof v

if k, v 不是 object return false
```
2 instanceof Number // false 因为2 是基本类型之一 number
```
同上其他基本类型也是如此

### [js的面试题，关于this指向和运算符的问题的](http://www.cnblogs.com/xxcanghai/p/5189353.html)
1. new Foo() 高于 函数调用
2. new Foo() 高于 new Foo 这就是带参数的new和不带参数的new 

> https://developer.mozilla.org/zh-CN/docs/Web/JavaScript/Reference/Operators/Operator_Precedence

### js reduce 函数参数

```
a= [1,2,3,4]
a.reduce((a, b) => {console.log(a, b); return a;}, [optional])
// 1, 2
// 1, 3
// 1, 4
// 1
```

若提供了第二个参数，则reduce相当于提供了一个初始值，会从optional开始reduce, 否则从数组的第一个参数开始

### object VS Map
```
let gameChar1 = new Map([

["name","Aleria"],
["gender","female"],
["species","half-elf"],
["vocation","mage"]
]);

gameChar1.get('name'); // 输出Aleria
```
这就是一个键值对, Map的键值可以使任意值

```
> gameChar1.set(NaN, 123)
Map {
  'name' => 'Aleria',
  'gender' => 'female',
  'species' => 'half-elf',
  'vocation' => 'mage',
  null => 123,
  NaN => 123 }
> gameChar1.get(null, 123)
123
> gameChar1.get(NaN, 123)
```

Map可以被继承
```
clss AMap extends Map {}
```