### what FRP does
FRP is the best prescription[处方] to deal with asynchronous and event-based data sources effectively。

### inversion of control form runtime
Unlike the previous code that makes a blocking HTTP call that you must wait for, using callbacks with asynchronous (AJAX) requests creates an inversion of control that permits your application to continue executing the next lines of code. Inversion of control in this sense refers to the way in which certain parts of your code receive the flow of control back from the runtime system. In this case, the runtime calls you (or returns control to you) via the function handler when the data is ready to be pro- cessed; hence, the term callback. Look at this alternative:

```
ajax('/data',  // No explicit return value
    items => { // Declaration of callback function
       items.forEach(item => {
        // process each item 
        // All processing is carried out within the callback body after the data has been fetched from the server.
    }); 
});

beginUiRendering(); // This function begins immediately after AJAX is called.
```

> 同步等待意味着我们把代码的控制交给了runtime, 比如  a = http.get('www.baidu.com'); 同步时我们把代码的控制权交给了http.get第三方代码。
http.get('www.baidu.com', () => {})

Callback functions allow you to invoke code asynchronously, so that the application can return control to you later.

* Familiar control flow structures (like for and while loops) with asynchronous functions don’t work well together because they’re not async aware; that is, they’re oblivious of wait time or latency between iterations.
* Error-handling strategies become easily convoluted when you begin nesting try/catch blocks within each callback. In chapter 7, we’ll approach error han- dling from a functional perspective. Also, if you want to implement some level of retry logic at every step, this will be incredibly difficult even with the help of other libraries.
* Business logic is tightly coupled within the nested callback structure you need to support. It’s plain to see that the more nested your code is, the harder it is to reason about. Functions that are deeply nested become entangled with other variables and functions, which is problematic in terms of readability and com- plexity. It would be ideal to be able to create reusable and modular components in order to have loosely coupled business logic that can be maintained and unit tested independently. We’ll cover unit testing with RxJS in chapter 9.
* You want to avoid excessive use of closures, but functions in JavaScript create a closure around the scope in which they’re declared. Nesting them means that you need to be concerned about not just the state of the variables passed in as arguments but also the state of all external variables surrounding each function declaration, causing side effects to occur. In the next chapter, you’ll learn how detrimental side effects can be and how FP addresses this problem. Side effects increase the cognitive load of the state of your application, making it virtually impossible to keep track of what’s going on in your programs. Throw a few loops and conditional if-else statements into the mix, and you’ll regret the day a bug occurs that impacts this functionality.
* It’s difficult to detect when events or long-running operations go rogue and need to be cancelled. Consider the case of a remote HTTP request that’s taking too long to process. Is the script unresponsive or is the server just slow? It would be ideal to have an easy mechanism to cancel events cleanly after some prede- termined amount of time. Implementing your own cancellation mechanism can be very challenging and error prone even with the help of third-party libraries.
* One good quality of responsive design is to always throttle a user’s interaction with any UI components, so that the system isn’t unnecessarily overloaded. In chapter 4, you’ll learn how to use throttling and debouncing to your advantage. Manual solutions for achieving this are typically very hard to get right and involve functions that access data outside their local scope, which breaks the sta- bility of your entire program.

* It’s rare to be concerned about memory management in JavaScript applica- tions, especially client-side code. After all, the browser takes care of most of these low-level details. But as UIs become larger and richer, we can begin to see that lingering event listeners may cause memory leaks and cause the size of the browser process to grow. It’s true that this was more prevalent in older browsers; nevertheless, the complexity of today’s JavaScript applications is no match for the applications of years past.

### Reactive programming is oriented around data flows and propa- gation.

Unlike in OOP where state or data is held in variables or collections, state in RP is transient, which means that data never remains stored but actually flows through the streams that are being subscribed to, which makes event handling easy to reason about and test

### imperatively(how) and declaratively(what)
Another noticeable difference is the style used in both paradigms. On one hand, OOP is typically written imperatively. In other words, you instantiate objects that keep track of state while running through a sequence of statements revealing how those objects interact and transform to arrive at your desired solution.
On the other hand, RxJS code encourages you to write declaratively, which means your code expresses the what and not the how of what you’re trying to accomplish. RxJS follows a simple and declarative design inspired by FP. No longer will you be required to create variables to track the progress of your callbacks or worry about inadvertently corrupting some closed-over outer state causing side effects to occur. Besides, with RxJS it becomes easy to manage multiple streams of data, filtering and transforming them at will. By creating operations that can be chained together, you can also fluently create pipelines of logic that sound very much like spoken sentences like this: “When I receive a magazine for the month of July, notify me.”

### fp 的特点
* Declarative 声明式
* immutable 不可变
* side-effect free 无副作用
### In general, mutations and side effects make functions unreliable and unpredictable. That is to say, if a function alters the contents of an object inadvertently, it will com- promise other functions that expect this object to keep its original state. The OO solu- tion to this is to encapsulate state and protect it from direct access from other components of the system. In contrast, FP deals with state by eliminating it, so that your functions can confidently rely on it to run.

Streams are immutable data types

### lazy evaluation if no subscribers no compute

### rxjs 6.* from 可以接受observable 和promise

pull or push
### This is extremely important to understand, because most of the literature for RxJS defines observables as objects that represent push-based collections

Also, observable pipelines should be self- contained, which essentially means they’re side effect–free (keep in mind that if your business logic functions are pure, your entire program is pure and stable as well)

### referential transparency
An observable must always produce the same results given the same events passing through it (that is, pressing the same key combination should always yield the same data to the observers), a quality known in FP as referential transparency.