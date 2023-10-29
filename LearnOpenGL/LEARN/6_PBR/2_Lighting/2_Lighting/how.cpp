//本文件不可运行，请从生成中排除
#include<mythings/shader.h>
#include<mythings/camera.h>
#include<mythings/model.h>

//BPR光照
namespace how
{
	//介绍
	void what()
	{
		//在上一个教程中，我们讨论了一些PBR的基础知识。
		//在本章节中，我们把重点放在将之前讨论的理论转化为实际的渲染器，
		//这个渲染器将使用直接的（或解析的）光源：比如点光源，定向灯或聚光灯。

		//我们先来看看上一个章提到的反射方程的最终版：
		// 	Lo(p,ωo) = ∫Ω(kd c/π + ks (D F G) / 4(ωo⋅n)(ωi⋅n))Li(p,ωi)n⋅ωi dωi

		//我们大致上清楚这个反射方程在干什么，但我们仍然留有一些迷雾尚未揭开。
		//比如说我们究竟将怎样表示场景上的辐照度(Irradiance), 辐射(Radiance) L？
		//我们知道辐射率L（在计算机图形领域中）表示光源的辐射通量(Radiant flux)ϕ，或光源在给定立体角ω下发出的光能。
		//在我们的情况下，不妨假设立体角ω无限小，这样辐射就表示光源在一条光线或单个方向向量上的辐射通量。

		//基于以上的知识，我们如何将其转化为之前的教程中所积累的一些光照知识呢？ 
		//那么想象一下，我们有一个点光源（一个在所有方向都具有相同亮度的光源），
		//它的辐射通量为用RGB表示为（23.47, 21.31, 20.79）。
		//该光源的辐射强度(Radiant Intensity)等于其在所有出射光线的辐射通量。
		//然而，当我们为一个表面上的特定的点p着色时，在其半球领域Ω的所有可能的入射方向中，
		//只有一个入射方向向量ωi直接来自于该点光源。
		//假设我们在场景中只有一个光源，位于空间中的某一个点，
		//因而对于p点的其他可能的入射光线方向上的辐射为0：https://learnopengl-cn.github.io/img/07/02/lighting_radiance_direct.png

		//请注意，这个假设成立的条件是点光源体积无限小，相当于在空间中的一个点。
		//如果我们认为该光源是具有体积的，它的辐射率会在不只一个入射光方向上非零。

		//如果从一开始，我们就假设点光源不受光线衰减（光照强度会随着距离变暗）的影响，
		//那么无论我们把光源放在哪，入射光线的辐射总是一样的（除去入射角cosθ对辐射的影响之外）。 
		//这是因为无论我们从哪个角度观察它，点光源总具有相同的辐射强度，
		//我们可以有效地将其辐射强度建模为其辐射通量 : 一个常量向量（23.47, 21.31, 20.79）。

		//然而，辐射也需要将位置p作为输入，正如所有现实的点光源都会受光线衰减影响一样，
		//点光源的辐射强度应该根据点p所在的位置和光源的位置以及他们之间的距离而做一些缩放。 
		//因此，根据原始的辐射方程，我们会根据表面法向量n和入射角度wi来缩放光源的辐射强度。

		//在实现上来说：对于直接点光源的情况，辐射率函数L先获取光源的颜色值， 
		//然后光源和某点p的距离衰减，接着按照n⋅wi缩放，
		//但是仅仅有一条入射角为wi的光线打在点p上， 这个wi同时也等于在p点光源的方向向量。
		//写成代码的话会是这样：
		/*
		vec3  lightColor  = vec3(23.47, 21.31, 20.79);
		vec3  wi          = normalize(lightPos - fragPos);               //lightDir
		float cosTheta    = max(dot(N, Wi), 0.0);                        //将mat相乘提前
		float attenuation = calculateAttenuation(fragPos, lightPos);
		float radiance    = lightColor * attenuation * cosTheta;
		*/

		//除了一些术语上的差异外，这段代码对你们来说应该很熟悉：
		//这正是我们一直以来怎么计算漫反射光照的！
		//说到直接光照，辐射度的计算方法与我们之前计算光照的方法类似，
		//因为只有单一的光照方向矢量会对表面的辐射度产生影响。

		//对于其它类型的从单点发出来的光源我们类似地计算出辐射率。
		//比如，定向光(directional light)拥有恒定的wi而不会有衰减因子；
		//而一个聚光灯光源则没有恒定的辐射强度，而是按照聚光灯的方向向量来缩放的。

		//这也让我们回到了对于表面的半球领域(hemisphere)Ω的积分∫上。
		//由于我们事先知道的所有贡献光源的位置，因此对物体表面上的一个点着色并不需要我们尝试去求解积分。
		//我们可以直接拿光源的（已知的）数目，去计算它们的总辐照度，
		//因为每个光源仅仅只有一个方向上的光线会影响物体表面的辐射。
		//这使得PBR对直接光源的计算相对简单，因为我们只需要有效地遍历所有有贡献的光源。
		//而当我们之后把环境照明也考虑在内的IBL教程中，我们就必须采取积分去计算了，这是因为光线可能会在任何一个方向入射。
	}

	//PBR表面模型
	void how()
	{
		//现在让我们开始写片段着色器来实现上述的PBR模型吧~ 
		//首先我们需要让片段着色器获取PBR相关的输入。
		/*
		#version 330 core
		out vec4 FragColor;
		in vec2 TexCoords;
		in vec3 WorldPos;
		in vec3 Normal;

		uniform vec3 camPos;

		uniform vec3  albedo;
		uniform float metallic;
		uniform float roughness;
		uniform float ao;
		*/

		//我们把通用的顶点着色器的输出作为输入的一部分。
		//另一部分输入则是物体表面模型的一些材质参数。
		//然后在片段着色器的开始部分我们做一下任何光照算法都需要做的计算:
		/*
		void main()
		{
			vec3 N = normalize(Normal);
			vec3 V = normalize(camPos - WorldPos);
			[...]
		}
		*/

		//直接光照----------------------------------------------------

		//在本教程的例子中我们会采用总共4个点光源来直接表示场景的辐照度。
		//为了满足反射率方程，我们循环遍历每一个光源，计算他们各自的辐射率然后求和，
		//接着根据BRDF和光源的入射角来缩放该辐射率。
		//我们可以把这个求和当作在物体的半球领域内对所有直接光源求积分。
		//首先我们来计算一些可以预计算的光照变量：
		/*
		vec3 Lo = vec3(0.0);
		for(int i = 0; i < 4; ++i)
		{
			vec3 L = normalize(lightPositions[i] - WorldPos);
			vec3 H = normalize(V + L);

			float distance    = length(lightPositions[i] - WorldPos);
			float attenuation = 1.0 / (distance * distance);
			vec3 radiance     = lightColors[i] * attenuation;
			[...]
		*/
		//当我们在线性空间内计算光照时（我们会在着色器的最后进行Gamma校正），
		//我们按照更符合物理原理的平方反比定律对光源进行衰减。

		//相对于物理上正确来说，你可能仍然想使用恒定线性二次衰减方程
		//他们在物理上相对不准确，却可以为您提供在光的能量衰减更多的控制。

		//然后，我们要为每个光源计算完整的 Cook-Torrance 镜面 BRDF 项：
		// (DFG) / 4(ωo⋅n)(ωi⋅n)

		//首先我们想计算的是镜面反射和漫反射之间的比值，
		//或者说表面反射光和折射光之间的比率。
		//我们从上一个教程知道可以使用菲涅尔方程计算
		//（注意这里用的clamp是为了限定cos值，避免黑点）：clamp译为夹具，其作用是是获得三个参数中大小处在中间的那个值
		/*
		vec3 fresnelSchlick(float cosTheta, vec3 F0)
		{
			return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
		}
		*/

		//Fresnel-Schlick近似法接收一个参数F0，即基础反射率，
		//被称为0°入射角的反射率，或者说是直接(垂直)观察表面的表面反射率。
		//这个参数F0会因为材料不同而不同，而且对于金属材质会带有颜色。
		//在PBR金属流中我们简单地认为大多数的绝缘体在F0为0.04的时候看起来视觉上是正确的，
		//而对于金属表面我们根据反射率特别地指定F0。 因此代码上看起来会像是这样：
		/*
		vec3 F0 = vec3(0.04);
		F0      = mix(F0, albedo, metallic);                //mix(x,y,a)函数，根据a值对x到y进行参数插值x*(1-a)+y*a)
		vec3 F  = fresnelSchlick(max(dot(H, V), 0.0), F0);
		*/
		//可以看到，对于非金属表面F0始终为0.04。对于金属表面，
		//我们根据初始的F0和表现金属属性的反射率进行线性插值。

		//算出了F，剩下的项就是计算法线分布函数D和几何遮蔽函数G了。

		//在直接PBR光照着色器中D和G的计算代码类似于：
		/*
		float DistributionGGX(vec3 N, vec3 H, float roughness)
		{
			float a      = roughness*roughness;
			float a2     = a*a;
			float NdotH  = max(dot(N, H), 0.0);
			float NdotH2 = NdotH*NdotH;

			float num   = a2;
			float denom = (NdotH2 * (a2 - 1.0) + 1.0);
			denom = PI * denom * denom;

			return num / denom;
		}

		float GeometrySchlickGGX(float NdotV, float roughness)
		{
			float r = (roughness + 1.0);
			float k = (r*r) / 8.0;

			float num   = NdotV;
			float denom = NdotV * (1.0 - k) + k;

			return num / denom;
		}
		float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
		{
			float NdotV = max(dot(N, V), 0.0);
			float NdotL = max(dot(N, L), 0.0);
			float ggx2  = GeometrySchlickGGX(NdotV, roughness);
			float ggx1  = GeometrySchlickGGX(NdotL, roughness);

			return ggx1 * ggx2;
		}
		*/

		//这里需要注意的是，与理论章节不同，我们直接将粗糙度参数传递给这些函数
		//这样我们就可以对原始粗糙度值进行一些特定的修改。
		//根据迪士尼公司的观察以及后来被Epic Games公司采用的光照模型，
		//在几何遮蔽函数和法线分布函数中将粗糙度平方后会让光照看起来更加自然。
		
		//现在两个函数都给出了定义，在计算反射的循环中计算NDF和G项就变得很简单：
		/*
		float NDF = DistributionGGX(N, H, roughness);       
		float G   = GeometrySmith(N, V, L, roughness);       
		*/

		//这样我们就凑够了足够的项来计算Cook-Torrance BRDF:
		/*
		vec3 nominator    = NDF * G * F;
		float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; 
		vec3 specular     = nominator / denominator;  
		*/
		//注意我们在分母项中加了一个0.001为了避免出现除零错误。

		//现在我们终于可以计算每个光源在反射率方程中的贡献值了！
		//因为菲涅尔值与 kS 直接对应， 我们可以使用F表示所有打在物体表面上的镜面反射光的贡献。
		//从kS我们很容易计算折射的比值kD：
		/*
		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;

		kD *= 1.0 - metallic;   
		*/

		//我们可以看作kS表示光能中被反射的能量的比例， 而剩下的光能会被折射， 比值即为kD。
		//更进一步来说，因为金属不会折射光线，因此不会有漫反射。
		//所以如果表面是金属的，我们会把系数kD变为0。 
		//这样，我们终于集齐所有变量来计算我们出射光线的值：
		/*
			const float PI = 3.14159265359;

			float NdotL = max(dot(N, L), 0.0);        
			Lo += (kD * albedo / PI + specular) * radiance * NdotL;
		}
		*/

		//最终的结果Lo，或者说是出射光线的辐射，是反射率方程的在半球领域Ω的积分的结果。
		//但是我们实际上不需要去求积，因为对于所有可能的入射光线方向我们知道只有4个方向的入射光线会影响片段的着色。
		//因为这样，我们可以直接循环N次计算这些入射光线的方向(N也就是场景中光源的数目)。
		
		//剩下的工作就是加一个环境光照项给Lo，然后我们就拥有了片段的最后颜色：
		/*
		vec3 ambient = vec3(0.03) * albedo * ao;
		vec3 color   = ambient + Lo;  
		*/
	}

	//线性空间和HDR渲染
	void how()
	{
		//直到现在，我们假设的所有计算都在线性的颜色空间中进行的，
		//为了考虑到这一点，我们需要在着色器的最后进行伽玛校正。
		//在线性空间中计算光照是非常重要的，因为PBR要求所有输入都是线性的，
		//如果不考虑这一点，就会导致光照不正确。
		//另外，我们希望所有光照的输入都尽可能的接近他们在物理上的取值，
		//这样他们的反射率或者说颜色值就会在色谱上有比较大的变化空间。
		//因此，Lo作为结果可能会变大得很快(超过1)，但是因为默认的低动态范围（LDR）而取值被截断。
		//所以在伽马矫正之前我们还要采用色调映射，
		//将 Lo 值和高动态范围 (HDR) 值的色调或曝光正确映射到 LDR，从而解决这个问题：。
		/*
		color = color / (color + vec3(1.0));
		color = pow(color, vec3(1.0/2.2)); 
		*/

		//这里我们采用的色调映射方法为Reinhard 操作，
		//以保留可能高度变化的辐照度的高动态范围，然后对颜色进行伽玛校正。
		//我们没有使用一个独立的帧缓冲或者采用后处理，
		//因此可以在前向片段着色器的末端直接应用色调映射和伽玛校正步骤。
		
		//https://learnopengl-cn.github.io/img/07/02/lighting_linear_vs_non_linear_and_hdr.png

		//在 PBR 管道中，将线性色彩空间和高动态范围考虑在内是非常重要的。
		//如果不考虑这些因素，就无法正确捕捉不同光照强度下的高低细节，
		//这最终会导致你的计算变得不正确，在视觉上看上去非常不自然。
	}

	//完整的直接光照PBR着色器
	void how()
	{
		//现在剩下的事情就是把做好色调映射和伽马矫正的颜色值传给片段着色器的输出，然后我们就拥有了自己的直接光照PBR着色器。
		//为了完整性，这里给出了完整的代码：
		/*
		#version 330 core
		out vec4 FragColor;
		in vec2 TexCoords;
		in vec3 WorldPos;
		in vec3 Normal;

		// material parameters
		uniform vec3  albedo;
		uniform float metallic;
		uniform float roughness;
		uniform float ao;

		// lights
		uniform vec3 lightPositions[4];
		uniform vec3 lightColors[4];

		uniform vec3 camPos;

		const float PI = 3.14159265359;

		float DistributionGGX(vec3 N, vec3 H, float roughness);
		float GeometrySchlickGGX(float NdotV, float roughness);
		float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
		vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness);

		void main()
		{       
			vec3 N = normalize(Normal);
			vec3 V = normalize(camPos - WorldPos);

			vec3 F0 = vec3(0.04); 
			F0 = mix(F0, albedo, metallic);

			// reflectance equation
			vec3 Lo = vec3(0.0);
			for(int i = 0; i < 4; ++i) 
			{
				// calculate per-light radiance
				vec3 L = normalize(lightPositions[i] - WorldPos);
				vec3 H = normalize(V + L);
				float distance    = length(lightPositions[i] - WorldPos);
				float attenuation = 1.0 / (distance * distance);
				vec3 radiance     = lightColors[i] * attenuation;        

				// cook-torrance brdf
				float NDF = DistributionGGX(N, H, roughness);        
				float G   = GeometrySmith(N, V, L, roughness);      
				vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);       

				vec3 kS = F;
				vec3 kD = vec3(1.0) - kS;
				kD *= 1.0 - metallic;     

				vec3 nominator    = NDF * G * F;
				float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; 
				vec3 specular     = nominator / denominator;

				// add to outgoing radiance Lo
				float NdotL = max(dot(N, L), 0.0);                
				Lo += (kD * albedo / PI + specular) * radiance * NdotL; 
			}   

			vec3 ambient = vec3(0.03) * albedo * ao;
			vec3 color = ambient + Lo;

			color = color / (color + vec3(1.0));
			color = pow(color, vec3(1.0/2.2));  

			FragColor = vec4(color, 1.0);
		}  
		*/

		//希望经过上一个教程的理论知识以及学习过关于渲染方程的一些知识后，这个着色器看起来不会太可怕。
		//如果我们采用这个着色器，加上4个点光源和一些球体，
		//同时令这些球体的金属性(metallic)和粗糙度(roughness)沿垂直和水平方向分别变化，
		//我们会得到这样的结果：https://learnopengl-cn.github.io/img/07/02/lighting_result.png

		//(上述图片)从下往上球体的金属性从0.0变到1.0， 从左到右球体的粗糙度从0.0变到1.0。
		//你可以看到仅仅改变这两个值，显示的效果会发生巨大的改变！

	}

	//带贴图的PBR
	void how()
	{
		//通过扩展系统，现在我们可以将表面参数设置为纹理，而不是统一的数值，
		//这样我们就可以按片段控制表面材料的属性：
		/*
		[...]
		uniform sampler2D albedoMap;
		uniform sampler2D normalMap;
		uniform sampler2D metallicMap;
		uniform sampler2D roughnessMap;
		uniform sampler2D aoMap;

		void main()
		{
			vec3 albedo     = pow(texture(albedoMap, TexCoords).rgb, 2.2);
			vec3 normal     = getNormalFromNormalMap();
			float metallic  = texture(metallicMap, TexCoords).r;
			float roughness = texture(roughnessMap, TexCoords).r;
			float ao        = texture(aoMap, TexCoords).r;
			[...]
		}
		*/

		//不过需要注意的是一般来说反射率(albedo)纹理在美术人员创建的时候就已经在sRGB空间了，
		//因此我们需要在光照计算之前先把他们转换到线性空间。
		//一般来说，环境光遮蔽贴图(ambient occlusion maps)也需要我们从 sRGB 转换到线性空间。
		//不过金属性(Metallic)和粗糙度(Roughness)贴图大多数时间都会保证在线性空间中。

		//只是把之前的球体的材质性质换成纹理属性，就在视觉上有巨大的提升：
		// https://learnopengl-cn.github.io/img/07/02/lighting_textured.png

		//注意金属表面会在场景中看起来有点黑，因为他们没有漫反射。
		//它们会在考虑环境镜面光照的时候看起来更加自然，不过这是我们下一个教程的事情了。

		//相比起在网上找到的其他PBR渲染结果来说，尽管在视觉上不算是非常震撼，
		//因为我们还没考虑到基于图像照明(IBL)，但我们现在也算是有了一个基于物理的渲染器了，
		//虽然还没考虑IBL，但你会发现你的光照看起来更加真实了。
	}
}