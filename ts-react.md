### node 中使用 ts 编译为 module: commonjs

### 前端使用 ts 编译选项是 module: exnext

### 在前端中使用 ts 若是自己写第三方包的声明文件时要在 tsconfig.compileOptions 中添加 paths 属性

```
paths: {
    "*": ["types/*"],
    ....
}
```

### 若是不想写声明文件以及不设置 paths 路径,可以在 types 文件夹下写一个与包名相同的.d.ts 里面是

```
declare module 'cos-js-sdk-v5';
```

这样就可以了，但是导出的是 any 类型

### react 中使用 ts 声明 props 的时候他的属性中必须有一个是 optional 选项的

```
interface IProps extends FormProps {
    a?: number // 必须有一个
}
```

### 可是上面会引入另外一个问题https://github.com/Microsoft/TypeScript/issues/23812，https://github.com/DefinitelyTyped/DefinitelyTyped/issues/11640

大家看吧

下面是提到的一种解决方法

```
interface TestProps { x?: number}

class Test extends React.Component<TestProps, null> {

    static defaultProps: TestProps =  {x: 5};

    render() {
        const x: number = (this.props.x as number);
        return <p>{x}</p>;
    }
}
```

> 看最新的进展是要在 jsx 中添加新的声明来解决
