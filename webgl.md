### What Is a Shader?
HelloPoint1.js is our first WebGL program that uses shaders. As mentioned earlier, shader programs are necessary when you want to draw something on the screen in WebGL. Essentially, shader programs are “embedded” in the JavaScript file and, in this case, set up at the start. This seems at first sight to be complicated, but let’s take it one step at a time.
WebGL needs the following two types of shaders

* Vertex shader: Vertex shaders are programs that describe the traits (position, colors, and so on) of a vertex. The vertex is a point in 2D/3D space, such as the corner or intersection of a 2D/3D shape.
* Fragment shader: A program that deals with per-fragment processing such as light- ing (see Chapter 8, “Lighting Objects”). The fragment is a WebGL term that you can consider as a kind of pixel (picture element).

### vertex shader
1. vec4 gl_Position Specifies the position of a vertex 
2.  float gl_PointSize Specifies the size of a point (in pixels)

### fragment shader
1. vec4 gl_FragColor Specify the color of a fragment (in RGBA)

### gl.drawArrays(mode, first, count)
Now, when the program makes a call to gl.drawArrays(), the vertex shader is executed count times, each time working with the next vertex. In this sample program, the shader is executed once (count is set to 1) because we only have one vertex: our point. When the shader is executed, the function main() in the shader is called, and then each line in the function is executed sequentially, resulting in (0.0, 0.0, 0.0, 1.0) being assigned to gl_Position (line 5) and then 10.0 assigned to gl_PointSize (line 6).

Once the vertex shader executes, the fragment shader is executed by calling its main() function which, in this example, assigns the color value (red) to gl_FragColor (line 12). As a result, a red point of 10 pixels is drawn at (0.0, 0.0, 0.0, 1.0), or the center of the drawing area (see Figure 2.15).

count指定的是几个点，shader会执行count次（helloPoint.js）

shader 会为每个vertex，每个fragment执行一次，最后渲染

### webgl与canvas 坐标对应

1. The center position of a <canvas>: (0.0, 0.0, 0.0)
2. The two edges of the x-axis of the <canvas>: (–1.0, 0.0, 0.0) and (1.0, 0.0, 0.0) 
3. The two edges of the y-axis of the <canvas>: (0.0, –1.0, 0.0) and (0.0, 1.0, 0.0)

### Attribute and uniform
Our goal is to pass a position from the JavaScript program to the vertex shader. There are two ways to pass data to a vertex shader: attribute variable and uniform variable (see Figure 2.20). The one you use depends on the nature of the data. The attribute variable passes data that differs for each vertex, whereas the uniform variable passes data that is the same (or uniform) in each vertex. In this program, you will use the attribute variable because each vertex generally has different coordinates.

attribute每个vertex的

### Attribute Variables
The Naming Rules for WebGL-Related Methods

You may be wondering what 3f in gl.vertexAttrib3f() actually means. WebGL bases its method names on the function names in OpenGL ES 2.0, which as you now know,
is the base specification of WebGL. The function names in OpenGL comprise the three components: <base function name> <number of parameters> < parameter type>. The name of each WebGL method also uses the same components: <base method name> <number of parameters> <parameter type>

```
var positions = new Float32Array([1.0, 2.0, 3.0, 1.0]); gl.vertexAttrib4fv(a_Position, positions);
```

### Uniform Variables
You likely remember how to use an attribute variable to pass data from a JavaScript program to a vertex shader. Unfortunately, the attribute variable is only available in a vertex shader, and when using a fragment shader, you need to use a uniform variable. There is an alternative mechanism, a varying variable

### 
There are five steps needed to pass multiple data values to a vertex shader through a buffer object. Because WebGL uses a similar approach when dealing with other objects such as texture objects (Chapter 4) and framebuffer objects (Chapter 8, “Lighting Objects”), let’s explore these in detail so you will be able to apply the knowledge later:
1. Create a buffer object (gl.createBuffer()).
2. Bind the buffer object to a target (gl.bindBuffer()). // After creating a buffer object, the second step is to bind it to a “target.” The target tells WebGL what type of data the buffer object contains, allowing it to deal with the contents correctly
3. Write data into the buffer object (gl.bufferData()).
4. Assign the buffer object to an attribute variable (gl.vertexAttribPointer()). 
5. Enable assignment (gl.enableVertexAttribArray()).

### 模型矩阵
综上可以看到，模型矩阵描述了三维空间物体的一系列变换的信息。在物体为刚体模型（刚体是在运动中和受力作用后，形状和大小不变，而且内部各点的相对位置不变的物体）的情况下，物体的所有顶点通过左乘模型矩阵，就可以实现物体的指定运动，所以模型矩阵大大降低了运动模型的复杂度。


### 过渡矩阵
a*p = c
p = c*(a的逆)
若a是单位矩阵则p过渡矩阵就是c
事实上过渡矩阵就是视图矩阵