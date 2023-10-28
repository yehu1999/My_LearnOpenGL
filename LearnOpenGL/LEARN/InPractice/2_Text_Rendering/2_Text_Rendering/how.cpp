//���ļ��������У�����������ų�
#include<mythings/shader.h>
#include<mythings/camera.h>
#include<mythings/model.h>
#include<mythings/debug.h>
#include <ft2build.h>
#include FT_FREETYPE_H  


//�ı���Ⱦ
namespace how
{
	//����
	void what()
	{
		//������ͼ�μ�������ð�յ���һ���׶��Ժ�����ܻ���ʹ��OpenGL�������ı���
		//Ȼ����������������Ĳ���һ����ʹ����OpenGL�����ĵײ�������ı���Ⱦ����Ļ�ϲ�����һ���򵥵����顣
		//�����ֻ��Ҫ����128�ֲ�ͬ���ַ�(Character)����ô������ܻ��һЩ��
		//���������Ҫ���Ƶ��ַ����Ų�ͬ�Ŀ��ߺͱ߾࣬�������Ͼ͸����ˡ�
		//������ʹ�����ԵĲ�ͬ������ܻ���Ҫ����128���ַ���
		//���ߣ������Ҫ�������ַ�����ѧ������Щ����ķ��ţ�
		//������Ⱦ�����ı��أ�
		//һ������ı���Щ���ӵ�������ǽ�������Ͳ������ΪʲôOpenGL�����ĵײ�APIû�а����ı������ˡ�

		//����OpenGL����û�а����κε��ı��������������Ǳ����Լ�����һ��ȫ�µ�ϵͳ��OpenGL�����ı�����Ļ�ϡ�
		//�����ı��ַ�û��ͼԪ�����Ǳ���Ҫ�е㴴�������С�
		//��Ҫʹ�õ�һЩ���������ǣ�ͨ��GL_LINES���������Σ������ı���3D����(Mesh)��
		//����3D�����н��ַ�������Ⱦ��2D�ı����ϡ�

		//��������õ�һ�ַ�ʽ�ǽ��ַ�������Ƶ��ı����ϡ�
		//������Щ�����ı��α�����ʵ�����Ǻܸ��ӣ�Ȼ������Ҫ�����ı�������ȴ�����һ������ս�ԵĹ�����
		//���̳̽�̽�������ı���Ⱦ��ʵ�ַ���������ʹ��FreeType��ʵ��һ�����Ӹ߼�����������Ⱦ�ı�������
	}

	//�����ı���Ⱦ��λͼ����
	void how()
	{
		//���ڵ�ʱ����Ⱦ�ı���ͨ��ѡ��һ����Ҫ������(Font)�������Լ�����һ������
		//����ȡ���������������ص��ַ��������Ƿŵ�һ�������Ĵ���������ʵ�ֵġ�
		//����һ���������λͼ����(Bitmap Font)�����������Ԥ���������а�����������Ҫʹ�õ������ַ���
		//�������Щ�ַ�����Ϊ����(Glyph)��
		//ÿ�����ζ�������һ���ض���������������
		//������Ҫ��Ⱦһ���ַ���ʱ����ֻ��Ҫͨ����Ⱦ��һ���ض���λͼ��������2D�ı����ϼ��ɡ�
		// https://learnopengl-cn.github.io/img/06/02/bitmapfont.png

		//����Կ���������ȡһ��λͼ���壬��ͨ����ϸѡ���������꣩�������в�����Ӧ�����Σ�
		//����Ⱦ���ǵ����2D�ı����ϣ�������Ⱦ����OpenGL���ı���
		//ͨ�����û�ϣ��ñ�������͸�������վ�����Ⱦһ���ַ�������Ļ�ϡ�
		//���λͼ������ͨ��Codehead��λͼ�������������ɵġ�

		//ʹ�����ַ�ʽ�����ı����������Ҳ�кܶ�ȱ�㡣
		//���ȣ��������˵������ʵ�֣�������Ϊλͼ�����Ѿ�Ԥ��դ���ˣ�����Ч��Ҳ�ܸߡ�
		//Ȼ�������ַ�ʽ������
		//������Ҫʹ�ò�ͬ������ʱ������Ҫ���±���һ��ȫ�µ�λͼ���壬������ĳ���ᱻ������һ���̶��ķֱ��ʡ�
		//��������Щ�ı��������ŵĻ���ῴ���ı������ر�Ե��
		//���⣬���ַ�ʽͨ��������ڷǳ�С���ַ������������������֧��Extended����Unicode�ַ��Ļ��ͺܲ���ʵ�ˡ�

		//���ֻ����ı��ķ�ʽ�������������ĸ��ٺͿ���ֲ�Զ��ǳ����У�Ȼ�������Ѿ����ָ������ķ�ʽ�ˡ�
		//����һ�������Ǽ������۵�ʹ��FreeType��������TrueType����ķ�ʽ��
	}

	//�ִ��ı���Ⱦ��FreeType
	void how()
	{
		//FreeType��һ���ܹ����ڼ������岢��������Ⱦ��λͼ�Լ��ṩ����������صĲ�������������⡣
		//����һ���ǳ��ܻ�ӭ�Ŀ�ƽ̨����⣬��������Mac OS X��Java��PlayStation������Linux��Android��ƽ̨��
		//FreeType�������������������ܹ�����TrueType���塣

		//TrueType���岻�������ػ������������ŵķ�ʽ������ģ�����ͨ����ѧ��ʽ�����ߵ���ϣ�������ġ�
		//������ʸ��ͼ����Щ��դ���������ͼ����Ը�����Ҫ������߶������ɡ�
		//ͨ��ʹ��TrueType���壬�����������Ⱦ��ͬ��С�����ζ�������κ�������ʧ��

		//FreeType���������ǵĹٷ���վ�����ص���
		//�����ѡ���Լ���Դ���������⣬���֧�����ƽ̨�Ļ�����Ҳ����ʹ������Ԥ����õĿ⡣
		//��ȷ���㽫freetype.lib��ӵ�����Ŀ�����ӿ��У�����ȷ�ϱ�����֪��ͷ�ļ���λ�á�

		//Ȼ����ȷ�ϰ������ʵ�ͷ�ļ���
		/*
		#include <ft2build.h>
		#include FT_FREETYPE_H
		*/

		//����FreeType�Ŀ�����ʽ����������д��ƪ���µ�ʱ�򣩣��㲻�ܽ����ǵ�ͷ�ļ��ŵ�һ���µ�Ŀ¼�¡�
		//����Ӧ�ñ�������includeĿ¼�ĸ�Ŀ¼�¡�
		//ͨ��ʹ���� #include <FreeType/ft2build.h> �����ķ�ʽ����FreeType���ܻ����һЩͷ�ļ���ͻ�����⡣

		//FreeType�������¾��Ǽ���TrueType���岢Ϊÿһ����������λͼ�Լ����㼸������ֵ(Metric)��
		//���ǿ�����ȡ�������ɵ�λͼ��Ϊ���ε�������ʹ����Щ����ֵ��λ�ַ������Ρ�

		//Ҫ����һ�����壬����ֻ��Ҫ��ʼ��FreeType�⣬���ҽ�����������Ϊһ��FreeType��֮Ϊ��(Face)�Ķ�����
		//����Ϊ���Ǽ���һ����Windows/FontsĿ¼�п�������TrueType�����ļ�arial.ttf��
		FT_Library ft;
		if (FT_Init_FreeType(&ft))
			std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

		FT_Face face;
		if (FT_New_Face(ft, "E:/Learn/Deps/Sources/fonts/arial.ttf", 0, &face))
			std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

		//��ЩFreeType�����ڳ��ִ���ʱ������һ�����������ֵ��

		//����������֮��������Ҫ���������С�����ʾ������Ҫ�������������ɶ������Σ�
		FT_Set_Pixel_Sizes(face, 0, 48);
		//�˺���������������Ŀ�Ⱥ͸߶ȣ������ֵ��Ϊ0��ʾ����Ҫ��������ͨ�������ĸ߶��ж�̬��������εĿ�ȡ�

		//һ��FreeType���а�����һ�����εļ��ϡ�
		//���ǿ��Ե���FT_Load_Char������������һ����������Ϊ�������Ρ�
		//��������ѡ������ַ����Ρ�X����
		if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
		//ͨ����FT_LOAD_RENDER��Ϊ���ر��֮һ�����Ǹ���FreeTypeȥ����һ��8λ�ĻҶ�λͼ��
		//���ǿ���ͨ��face->glyph->bitmap���������λͼ��

		//ʹ��FreeType���ص�ÿ������û����ͬ�Ĵ�С������λͼ������������
		//ʹ��FreeType���ɵ�λͼ�Ĵ�Сǡ���ܰ�������ַ��ɼ�����
		//�����������ڱ�ʾ��.����λͼ�Ĵ�СҪ�ȱ�ʾ��X����С�öࡣ
		//��ˣ�FreeTypeͬ��Ҳ������һЩ����ֵ��ָ��ÿ���ַ��Ĵ�С��λ�á�
		//��������ͼչʾ��FreeType��ÿһ���ַ����μ�������ж���ֵ��

		//ÿһ�����ζ�����һ��ˮƽ�Ļ�׼��(Baseline)�ϣ�����ͼ��ˮƽ��ͷָʾ�������ߣ���
		//һЩ����ǡ��λ�ڻ�׼���ϣ��硯X����������һЩ�����΢Խ����׼�����£��硯g����p����
		//����ע������Щ�������첿����ĸ�����Լ������
		//��Щ����ֵ��ȷ�����˰ڷ����������ÿ�����ξ����׼�ߵ�ƫ������
		//ÿ�����εĴ�С���Լ���ҪԤ�����ٿռ�����Ⱦ��һ�����Ρ�
		//����������г���������Ҫ���������ԡ�
		/*
		����	    ��ȡ��ʽ	                ����λͼ����
		width	    face->glyph->bitmap.width	λͼ��ȣ����أ�
		height	    face->glyph->bitmap.rows	λͼ�߶ȣ����أ�
		bearingX	face->glyph->bitmap_left	ˮƽ���룬��λͼ�����ԭ���ˮƽλ�ã����أ�
		bearingY	face->glyph->bitmap_top	    ��ֱ���룬��λͼ����ڻ�׼�ߵĴ�ֱλ�ã����أ�
		advance	    face->glyph->advance.x	    ˮƽԤ��ֵ����ԭ�㵽��һ������ԭ���ˮƽ���루��λ��1/64���أ�
		*/

		//����Ҫ��Ⱦ�ַ�ʱ�����ǿ��Լ���һ���ַ����Σ���ȡ���Ķ���ֵ��������һ��������ÿһ֡����������ǳ�û��Ч�ʡ�
		//����Ӧ����Щ���ɵ����ݴ����ڳ����ĳһ���ط�������Ҫ��Ⱦ�ַ���ʱ����ȥ���á�
		//���ǻᶨ��һ���ǳ�����Ľṹ�壬������Щ�ṹ��洢��һ��map�С�
		struct Character
		{
			GLuint     TextureID;  // ���������ID
			glm::ivec2 Size;       // ���δ�С
			glm::ivec2 Bearing;    // �ӻ�׼�ߵ�������/������ƫ��ֵ
			GLuint     Advance;    // ԭ�����һ������ԭ��ľ���
		};
		std::map<GLchar, Character> Characters;

		//��������̳���˵��������һ�м򵥵�Ŀ�ģ�����ֻ����ASCII�ַ�����ǰ128���ַ���
		//��ÿһ���ַ�����������һ�������������������Character�ṹ���У�֮���������Characters���ӳ����С�
		//�����ӣ���Ⱦһ���ַ�������������ݾͶ����������������ˡ�
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //�����ֽڶ�������
		for (GLubyte c = 0; c < 128; c++)
		{
			// �����ַ������� 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
				continue;
			}
			// ��������
			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// ��������ѡ��
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// �����ַ���֮��ʹ��
			Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				face->glyph->advance.x
			};
			Characters.insert(std::pair<GLchar, Character>(c, character));
		}
		//�����forѭ�������Ǳ�����ASCII���е�ȫ��128���ַ�������ȡ���Ƕ�Ӧ���ַ����Ρ�
		//��ÿһ���ַ�������������һ����������������ѡ������������Ķ���ֵ��
		//��Ȥ�����������ｫ�����internalFormat��format����ΪGL_RED��
		//ͨ���������ɵ�λͼ��һ��8λ�Ҷ�ͼ������ÿһ����ɫ����һ���ֽ�����ʾ��
		//���������Ҫ��λͼ�����ÿһ�ֽڶ���Ϊ�������ɫֵ��
		//����ͨ������һ�����������ʵ�ֵģ���������ÿһ�ֽڶ���Ӧ��������ɫ�ĺ�ɫ��������ɫ�����ĵ�һ���ֽڣ���
		//�������ʹ��һ���ֽ�����ʾ�������ɫ��������Ҫע��OpenGL��һ�����ƣ�
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		//OpenGLҪ�����е�������4�ֽڶ���ģ�������Ĵ�С��Զ��4�ֽڵı�����
		//ͨ���Ⲣ�������ʲô���⣬��Ϊ�󲿷�����Ŀ�ȶ�Ϊ4�ı�����/��ÿ����ʹ��4���ֽڣ�
		//������������ÿ������ֻ����һ���ֽڣ�������������Ŀ�ȡ�
		//ͨ���������ѹ���������Ϊ1����������ȷ�������ж������⣨�����ܻ���ɶδ��󣩡�

		//���㴦�������κ�Ҫ��������FreeType����Դ��
		FT_Done_Face(face);
		FT_Done_FreeType(ft);

		//��ɫ��---------------------------------------------------------------------------------------

		//���ǽ�ʹ������Ķ�����ɫ������Ⱦ���Σ�
		/*
		#version 330 core
		layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
		out vec2 TexCoords;

		uniform mat4 projection;

		void main()
		{
			gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
			TexCoords = vertex.zw;
		}
		*/
		//���ǽ�λ�ú�����������������ݺ���������һ��vec4�С�
		//���������ɫ����λ��������һ��ͶӰ������ˣ������������괫�ݸ�Ƭ����ɫ����
		/*
		#version 330 core
		in vec2 TexCoords;
		out vec4 color;

		uniform sampler2D text;
		uniform vec3 textColor;

		void main()
		{
			vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
			color = vec4(textColor, 1.0) * sampled;
		}
		*/

		//Ƭ����ɫ��������uniform������һ���ǵ���ɫͨ��������λͼ������һ������ɫuniform�����������������ı���������ɫ��
		//�������ȴ�λͼ�����в�����ɫֵ���������������н��洢�ź�ɫ���������ǾͲ��������r��������Ϊȡ����alphaֵ��
		//ͨ���任��ɫ��alphaֵ�����յ���ɫ�����α�����ɫ�ϻ���͸���ģ������������ַ��������ǲ�͸���ġ�
		//����Ҳ��RGB��ɫ��textColor���uniform��ˣ����任�ı���ɫ��

		//��Ȼ������Ҫ���û�ϲ�������һ����֮��Ч��
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//����ͶӰ�������ǽ�ʹ��һ������ͶӰ����(Orthographic Projection Matrix)��
		//�����ı���Ⱦ���ǣ�ͨ����������Ҫ͸�ӣ�ʹ������ͶӰͬ��������������Ļ����ϵ���趨���еĶ������꣬
		//�������ʹ�����·�ʽ���ã�
		glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
		//��������ͶӰ����ĵײ�����Ϊ0.0f������������������Ϊ���ڵĸ߶ȡ�
		//�������Ľ��������ָ����y����ķ�ΧΪ��Ļ�ײ�(0.0f)����Ļ����(600.0f)��
		//����ζ�����ڵ�(0.0, 0.0)��Ӧ���½ǣ���ע���������Ǵ������м䣩��

		//���Ҫ�������Ǵ���һ��VBO��VAO������Ⱦ�ı��Ρ�
		//���������ڳ�ʼ��VBOʱ�����㹻���ڴ棬�������ǿ�������Ⱦ�ַ���ʱ����������VBO���ڴ档
		GLuint VAO, VBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		//ÿ��2D�ı�����Ҫ6�����㣬ÿ������������һ��4float��������ע��һ�����������һ���������꣩��ɣ�
		//������ǽ�VBO���ڴ����Ϊ6 * 4��float�Ĵ�С��
		//�������ǻ��ڻ����ַ�ʱ��������VBO���ڴ棬�������ǽ��ڴ���������ΪGL_DYNAMIC_DRAW��

		//��Ⱦһ���ı�---------------------------------------------------------------------------------

		//Ҫ��Ⱦһ���ַ������Ǵ�֮ǰ������Charactersӳ�����ȡ����Ӧ��Character�ṹ�壬
		//�������ַ��Ķ���ֵ�������ı��ε�ά�ȡ�
		//�����ı��ε�ά�����Ǿ��ܶ�̬�����6�������ı��εĶ��㣬
		//��ʹ��glBufferSubData��������VBO�������ڴ�����ݡ�

		//���Ǵ���һ������RenderText�ĺ�����Ⱦһ���ַ����� ���Ѽ���render.h��
		/*
		void RenderText(Shader &s, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
		{
			// �����Ӧ����Ⱦ״̬
			s.Use();
			glUniform3f(glGetUniformLocation(s.Program, "textColor"), color.x, color.y, color.z);
			glActiveTexture(GL_TEXTURE0);
			glBindVertexArray(VAO);

			// �����ı������е��ַ�
			std::string::const_iterator c;
			for (c = text.begin(); c != text.end(); c++)
			{
				Character ch = Characters[*c];

				GLfloat xpos = x + ch.Bearing.x * scale;
				GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

				GLfloat w = ch.Size.x * scale;
				GLfloat h = ch.Size.y * scale;
				// ��ÿ���ַ�����VBO
				GLfloat vertices[6][4] = {
					{ xpos,     ypos + h,   0.0, 0.0 },            
					{ xpos,     ypos,       0.0, 1.0 },
					{ xpos + w, ypos,       1.0, 1.0 },

					{ xpos,     ypos + h,   0.0, 0.0 },
					{ xpos + w, ypos,       1.0, 1.0 },
					{ xpos + w, ypos + h,   1.0, 0.0 }           
				};
				// ���ı����ϻ�����������
				glBindTexture(GL_TEXTURE_2D, ch.textureID);
				// ����VBO�ڴ������
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				// �����ı���
				glDrawArrays(GL_TRIANGLES, 0, 6);
				// ����λ�õ���һ�����ε�ԭ�㣬ע�ⵥλ��1/64����
				x += (ch.Advance >> 6) * scale; // λƫ��6����λ����ȡ��λΪ���ص�ֵ (2^6 = 64)
			}
			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		*/
		//�������������Ӧ�÷ǳ������ˣ�
		//�������ȼ�����ı��ε�ԭ�����꣨Ϊxpos��ypos�������Ĵ�С��Ϊw��h����
		//������6�������γ����2D�ı��Σ�ע�����ǽ�ÿ������ֵ��ʹ��scale�������š�
		//���������Ǹ�����VBO�����ݡ�����Ⱦ������ı��Ρ�

		//�������д�����Ҫ�ӱ����⣺
		// GLfloat ypos = y - (ch.Size.y - ch.Bearing.y);

		//һЩ�ַ����硯p����q������Ҫ����Ⱦ����׼�����£�
		//��������ı���ҲӦ�ñ��ڷ���RenderText��yֵ���¡�
		//ypos��ƫ�������Դ����εĶ���ֵ�еó���https://learnopengl-cn.github.io/img/06/02/glyph_offset.png

		//Ҫ������ξ��룬��ƫ������������Ҫ�ҳ������ڻ�׼��֮����չ��ȥ�ľ��롣
		//����ͼ����ξ����ú�ɫ��ͷ������Ӷ���ֵ�п��Կ�����
		//���ǿ���ͨ�������εĸ߶ȼ�ȥbearingY��������������ĳ��ȡ�
		//������Щ����λ�ڻ�׼���ϵ��ַ����硯X���������ֵ������0.0��
		//��������Щ������׼�ߵ��ַ����硯g����j���������ֵ�������ġ�

		//�����ÿ���¶������ˣ���ô�������Ѿ�����ʹ����������ɹ���Ⱦ�ַ����ˣ�
		// RenderText(shader, "This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
		// RenderText(shader, "(C) LearnOpenGL.com", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));

		//��ȾЧ������ȥ��������https://learnopengl-cn.github.io/img/06/02/text_rendering.png

		//Ϊ��������������������ô�����ı��ζ���ģ����ǿ��Թرջ��������������Ⱦ�������ı�����ʲô���ӵģ�
		// https://learnopengl-cn.github.io/img/06/02/text_rendering_quads.png
		//���Կ������󲿷ֵ��ı��ζ�λ��һ���������еģ���׼���ϣ�����Ӧ��p����(�����ε��ı�������΢����ƫ����һЩ��
	}

	//����һ��
	/*
	���̳���ʾ�����ʹ��FreeType�����TrueType�ı���
	���ַ�ʽ�������Ų�֧�ֶ����ַ����롣
	Ȼ�����������Ƕ�ÿһ�����ζ����ɲ���Ⱦ���������Ӧ�ó�����ܲ�����Ҫ��ôǿ��Ĺ��ܡ�
	���ܸ��õ�λͼ����Ҳ���Ǹ���ȡ�ģ���Ϊ�����е���������ֻ��Ҫһ������
	��Ȼ����õķ�ʽ�ǽ�������ַ�ʽ����̬���ɰ��������ַ����ε�λͼ������������FreeType���ء�
	��Ϊ��Ⱦ����ʡ�˴��������л��Ŀ��������Ҹ������ε����н��̶ܳ�Ҳ���Խ�ʡ�ܶ�����ܿ�����

	��һ��ʹ��FreeType��������������������Ǵ���Ϊһ���̶��������С�ģ�
	���ֱ�Ӷ���Ŵ�ͻ���־�ݱ�Ե��
	���⣬�����ν�����ת����ʹ���ǿ���ȥ���ģ����
	����������ͨ������ÿ�����ؾ���������������ľ��룬�����ǹ�դ����������ɫ�������⡣
	���������Ϊ������볡(Signed Distance Fields)��Valve�ڼ���ǰ�����һƪ�����ģ�http://www.valvesoftware.com/publications/2007/SIGGRAPH2007_AlphaTestedMagnification.pdf
	����������ͨ�����������÷ǳ�����3D��ȾЧ����
	*/

	// https://www.websiteplanet.com/blog/best-free-fonts/
	//������һ�������壬��������Ŀ�����ڸ��˻���ҵ��;��
}