mkdir ..\..\Release\shaders\ 1>NUL 2>NUL
mkdir ..\..\Debug\shaders\ 1>NUL 2>NUL

copy Output\CircleShader.vk.vert.spv ..\..\Release\shaders\CircleShader.vk.vert.spv 1>NUL
copy Output\CircleShader.vk.vert.spv ..\..\Debug\shaders\CircleShader.vk.vert.spv 1>NUL

copy Output\CoordinateColorShader.vk.frag.spv ..\..\Release\shaders\CoordinateColorShader.vk.frag.spv 1>NUL
copy Output\CoordinateColorShader.vk.frag.spv ..\..\Debug\shaders\CoordinateColorShader.vk.frag.spv 1>NUL