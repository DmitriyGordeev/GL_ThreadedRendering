# Threaded Rendering

This project is an example of rendering OpenGL scene 
in a separate thread along with the main thread updating
some game-related logic. 

Main idea is that OpenGL's context resides in
the RenderingThread. Game (main) thread pushes objects 
into a rendering queue in order for rendering thread 
to process and render them.

TODO:
1. вернуть матрицу трансформаций в шейдер
2. сделать загрузку текстуры Shader::loadTexture асинхронно
3. движение фигур с помощью клавиш
4. сетевая репликация
5. общий рефакторинг