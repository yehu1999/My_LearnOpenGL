#version 330 core

void main()
{             
	// gl_FragDepth = gl_FragCoord.z;  //深度设置会默认执行
}

//由于没有颜色缓冲，最后的片段不需要任何处理，所以我们可以简单地使用一个空片段着色器

