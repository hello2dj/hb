---
title: 用来angular的体系来写node之nestjs简介
tag:
- typescript
- nodejs
- angular2
---

先上图
![](https://camo.githubusercontent.com/18fe3feea5e3593c593e12e552494a3995eceacf/687474703a2f2f6b616d696c6d79736c69776965632e636f6d2f7075626c69632f6e6573742d6c6f676f2e706e672331)

这几年前端发展的很快，出现了很多优秀的框架。例如Angular2, React, Vue等他们大大提升了开发者的生产效率，以及快速创建可测试化，可扩展的前端应用。但是在server端nodejs到没有出现如此的框架都是一些基础的框架，工具等等，虽然目前有eggjs,thinkjs，私以为他们架构成熟性还有待进步，他依然也只是提供了一些工具和方法等，不过egg还是要胜think一筹的（都是我瞎掰的），当然他们也都在不同程度上的解决了一些初步的架构问题。

这里我看到了一个新近的框架nestjs(基于express)，他提供了一个开箱即用的架构体系，是啥样的架构体系呢？angular2的架构体系，可以说几乎是一样的架构体系，照着搬过来了，不过可以想一想用angular2的前端架构来写后端代码，也是一种酸爽！（typescript）在我看看来就是一个nodejs中ROR,flask或者是django。

### 概念普及
  * typescript js的超集，提供强类型校验，以及es6,7等js特性
  * express 一个node， httpserver,提供了最基础的框架如路由等。
  * 依赖注入，angular里面有个重要的概念就是依赖注入，nestjs中也是大量使用，有一个[IOC容器](http://www.cnblogs.com/DebugLZQ/archive/2013/06/05/3107957.html)。这里nest的注入和angular2的是一样的，注入可以是一个class, 也可以是一个值，可以是一个function等等不过不是一个class的就得用如下的方式
  ```
  {
    provide: 'PhotoRepositoryToken',
    useFactory: (connection: Connection) => connection.getRepository(Photo),
    inject: ['DbConnectionToken'],
  }
  ```

    其中inject是这个注入值要使用的依赖注入，provide是提供给其他人要使用的注入标识符，useFactory代表要执行的方法，也可以是useValue,此时是一个普通对象即可。还有循环依赖，此时就要使用forwardRef了，就不细讲了，大家可以具体[参见](https://docs.nestjs.com/fundamentals/dependency-injection)

  * 装饰器，要是写过angular2，或者flask等就会知道，都是一堆@name堆起来的了的
  * @Controller
  处理req的handler的。
  ```
  @Controller('cats')
  export class CatsController {
    @Get()
    findAll(@Req() user) {
      return [];
    }
  }
  ```
  * @Component
  就是service
  ```
  @Component()
  export class CatsService {
    private readonly cats: Cat[] = [];
    create(cat: Cat) {
      this.cats.push(cat);
    }
  }
  // 使用
  export class CatsController {
    constructor(private readonly catsService: CatsService) {} // 注入

    @Post()
    async create(@Body() createCatDto: CreateCatDto) {
      this.catsService.create(createCatDto);
    }
  }
  ```
  * @Module
  在angualr中代码是以module为粗粒度单元进行组织的，就是rootModule包含其他module比如orderModule, reportModule, userModule,等等，
  ```
  @Module({
      imports: [CatsModule, OrderModule, ReportModule, UserModule],
  })
  export class ApplicationModule {}
  ```
  * @Middleware
  同理与express的middleware,只是写法有变化，需要包裹一下
  ```
  @Middleware()
  export class LoggerMiddleware implements NestMiddleware {
    resolve(...args: any[]): ExpressMiddleware {
      return (req, res, next) => {
        console.log('Request...');
        next();
      };
    }
  }
  ```
  * @Filter 和 HttpException
  用来处理handler执行过程产生exception或者说错误（但是异常和错误应当是区分开来的）,Filter是区分全局和路由级的。 HttpException是nest提供的错误类型

  ![](https://docs.nestjs.com/assets/Filter_1.png)
  ```
  @Post()
  async create(@Body() createCatDto: CreateCatDto) {
    throw new HttpException({
      status: HttpStatus.FORBIDDEN,
      error: 'This is a custom message',
    });
  }
  /**
  {
    "status": 403,
    "error": "This is a custom message"
  }
  */
  ```

    expecptions的体系是可以扩展的，当然nest还提供很多的类型，如BadRequestExceptio, UnauthorizedExceptio等等

  ```
  export class ForbiddenException extends HttpException {
    constructor() {
      super('Forbidden', HttpStatus.FORBIDDEN);
    }
  }
  ```

    @Filter的正式称呼应当是Exception Filters。这次望文生义是正确的。是的就是处理Exception的。

  ```
  @Catch(HttpException)
  export class HttpExceptionFilter implements ExceptionFilter {
    catch(exception: HttpException, response) {
      const status = exception.getStatus();

      response
        .status(status)
        .json({
          statusCode: status,
          message: `It's a message from the exception filter`,
        });
    }
  }
  // 使用
  @Post()
  @UseFilters(new HttpExceptionFilter())
  async create(@Body() createCatDto: CreateCatDto) {
    throw new ForbiddenException();
  }

  // 全局的
  async function bootstrap() {
    const app = await NestFactory.create(ApplicationModule);
    app.useGlobalFilters(new HttpExceptionFilter());
    await app.listen(3000);
  }
  bootstrap(); 
  ```
  * @Guards
  用来决定请求是否要被handler处理，典型就是权限判断

  ![](https://docs.nestjs.com/assets/Guards_1.png)

  ```
  @Guard()
  export class RolesGuard implements CanActivate {
    canActivate(dataOrRequest, context: ExecutionContext): boolean | Promise<boolean> | Observable<boolean> {
      return true;
    }
  }
  // 使用
  @Controller('cats')
  @UseGuards(RolesGuard)
  export class CatsController {}
  ```

  我们可以自定义一些装饰器，给controller添加必要属性以供guard来使用

  ```
  // roles Deacator
  export const Roles = (...roles: string[]) => ReflectMetadata('roles', roles);
  // 使用
  @Post()
  @Roles('admin')
  async create(@Body() createCatDto: CreateCatDto) {
    this.catsService.create(createCatDto);
  }

  // Roles guards 改写
  @Guard()
  export class RolesGuard implements CanActivate {
    constructor(private readonly reflector: Reflector) {}

    canActivate(req, context: ExecutionContext): boolean {
      const { parent, handler } = context;
      const roles = this.reflector.get<string[]>('roles', handler);
      if (!roles) {
        return true;
      }

      const user = req.user;
      const hasRole = () => !!user.roles.find((role) => !!roles.find((item) => item === role));
      return user && user.roles && hasRole();
    }
  }
  // 使用同上
  ```
  * @Pipe
  使用来处理参数校验以及参数类型转换的,当然nest也提供了很多的内置pipe,
  参数解释一下，可能不太好看，value,是传进来的值，ArgumentMetadata的属性包含： type: 参数通过什么方式传进来的（body, query, param等等），metatype: 传进来的参数是啥类型，string, number .etc, data: 这个没搞太明白文档上说的是‘The string passed to the decorator, for example  @Body('string')’还没参悟处来。。。

  ![](https://docs.nestjs.com/assets/Pipe_1.png)
  ```
  @Pipe()
  export class ValidationPipe implements PipeTransform<any> {
    transform(value: any, metadata: ArgumentMetadata) {
      return value;
    }
  }
  // 使用
  @Post()
  // @UsePipes(new ValidationPipe())
  async create(@Body(new ValidationPipe()) createCatDto: CreateCatDto) {
    this.catsService.create(createCatDto);
  }
  ```
  * @Interceptor
  按照文档的说法就是受到AOP（面向切面编程）编程方式的启发。
  1. 在方法执行前后增加额外的逻辑(类似于koa中中间件的执行方式)
  2. 有了1，所以我们可以转换执行结果
  3. 转换执行时的异常
  4. 重写执行逻辑（比如根据缓存返回结果）
  这个例子太多了，就不贴代码了可以具体[参见](https://docs.nestjs.com/interceptors)

  * 可以自定义装饰器，在guard里面我们已经见到过了。

  * nest还集成了[graphql](https://docs.nestjs.com/graphql/quick-start), [websockets](https://docs.nestjs.com/websockets/gateways), [microservice](https://docs.nestjs.com/microservices/basics), 微服务这部分他提供两种通信方式，redis(pub/sub), tcp等等
### 目录结构

```
  app
    -- modules
      -- reports
        -- report.controller.ts
        -- report.service.ts
        -- report.entity.ts
        -- report.interface.ts
        -- dto(data transfer object)
          -- report.dto.ts(推荐class)
      -- orders
        ...
    -- common
      -- db.provice.ts
      ...
```
### 实践一把
1. git clone https://github.com/nestjs/typescript-starter
2. npm install 
3. npm run start
4. 看看实例代码就ok了。

### 最后 [项目地址](https://github.com/nestjs/nest)
nest项目也提供了很多的example, 总体来看写起来也还是很舒服的。集成了很多东西，点赞，希望下一个项目可以使用。

### 感悟
总会觉得自己不知道该怎么去更进一步的学习，一开始使用就觉得纯用express有很多问题，可是没有去思考怎么才能更好，总觉得见见世面，看看优秀的人都是怎么写的，可是，看完就完了，却没有想本项目做一样把看到的总结起来。不想动手写业余项目，因为就是觉得自己懂的太少了，还是得多看看，可是只看又有什么用呢？ps: 看的结果就是写出了屎一样的代码，还得努力啊！（有看的欢迎多多交流dj_amazing@sina.com）