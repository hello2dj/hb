### 什么jsx
Fundamentally, JSX just provides syntactic sugar for the React.createElement(component, props, ...children) function. The JSX code:

### react element vs component
#### element

> Simply put, a React element describes what you want to see on the screen. Not so simply put, a React element is an object representation of a DOM node.’
### react element 简单说就是描述了你在屏幕上所看到的，复杂点说就是一个DOM node 的 js object表现代理

> In order to create our object representation of a DOM node (aka React element), we can use React’s createElement method.
### 为了创建一个DOM node 的js object 表现代理，我们需要使用 react 的createElement方法
```
const element = React.createElement(
  'div',
  {id: 'login-btn'},
  'Login'
)
```
### 而我们所看到的所有如图片里的写法都是 jsx, 当他们被babel转义后就是这个
```
React.createElement(Icon, null)
```

> So finally, what do we call it when we write out our component like this, <Icon/>? We can call it “creating an element” because after the JSX is transpiled, that’s exactly what’s happening.
也就是我们在jsx文件里的所有的类似的写法都是在写 createElement()方法（jsx会被babel转义）

### 如下
```
function Button ({ addFriend }) {
  return (
    <button onClick={addFriend}>Add Friend</button>
  )
}

function User ({ name, addFriend }) {
  return (
    <div>
      <p>{name}</p>
      <Button addFriend={addFriend}/>
    </div>
  )
}
```
### 会被转为
```
function Button ({ addFriend }) {
  return React.createElement(
    "button",
    { onClick: addFriend },
    "Add Friend"
  )
}

function User({ name, addFriend }) {
  return React.createElement(
    "div",
    null,
    React.createElement(
      "p",
      null,
      name
    ),
    React.createElement(Button, { addFriend })
  )
}
```
### component

> “Components are the building blocks of React”. Notice, however, that we started this post with elements. The reason for this is because once you understand elements, understanding components is a smooth transition. A component is a function or a Class which optionally accepts input and returns a React element.
### 理解了上面的react element就知道component了，对上段英文翻译一下就是， component 是构建react应用的基础，理解了element 就知道了，component就是一个类或者是一个函数，他接收输入 并且返回一个element以供react使用


1. createElement(type, props, [...children])
type是tagName string, 或者component(class, function) 又或者是fragment
```
createElement -> 解析props, 生成key或者ref, ->生成vnode
```