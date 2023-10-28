//���ļ��������У�����������ų�
#include<vector>
using namespace std;
#include<mythings/shader.h>

namespace how
{
	//����
	//ͨ��ʹ��Assimp�����ǿ��Լ��ز�ͬ��ģ�͵������У�������������Ƕ�������ΪAssimp�����ݽṹ��
	//����������Ҫ����Щ����ת��ΪOpenGL�ܹ����ĸ�ʽ������������Ⱦ������塣
	//���Ǵ���һ����ѧ��������(Mesh)������ǵ����Ŀɻ���ʵ�壬����������������һ�������Լ��������ࡣ

	//�����������ع�һ������Ŀǰѧ����֪ʶ������һ������������Ҫʲô���ݡ�
	//һ������Ӧ��������Ҫһϵ�еĶ��㣬ÿ���������һ��λ��������һ����������һ����������������
	//һ������Ӧ�ð��������������Ƶ������Լ�������ʽ�Ĳ������ݣ�������/�������ͼ����

	//��Ȼ��������һ�������������������ǿ�����OpenGL�ж���һ�������ˣ�
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};
	//���ǽ�������Ҫ���������浽һ������Vertex�Ľṹ���У����ǿ�������������ÿ���������ԡ�

	//����Vertex�ṹ��֮�⣬���ǻ���Ҫ��������������һ��Texture�ṹ���С�
	struct Texture
	{
		unsigned int id;
		string type;
	};
	//���Ǵ����������id�Լ��������ͣ���������������ͼ�����Ǿ������ͼ��

	//֪���˶���������ʵ�֣����ǿ��Կ�ʼ����������Ľṹ�ˣ�
	class Mesh
	{
	public:
		/*  ��������  */
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;
		/*  ����  */
		Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
		void Draw(Shader shader);
	private:
		/*  ��Ⱦ����  */
		unsigned int VAO, VBO, EBO;
		/*  ����  */
		void setupMesh();
	};
	Mesh mesh;
	//����Կ�������ಢ�����ӡ��ڹ������У����ǽ����б�������ݸ���������
	//������setupMesh�����г�ʼ�����壬������ʹ��Draw��������������
	//ע�����ǽ�һ����ɫ��������Draw�����У�����ɫ�������������п����������ڻ���֮ǰ����һЩuniform���������Ӳ�����������Ԫ����

	//�����������ݷǳ�������⡣����ֻ��Ҫʹ�ù������Ĳ���������Ĺ��б����Ϳ����ˡ�
	//�����ڹ������л�������setupMesh������
	Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;

		setupMesh();  //��ʼ��
	}
	//����ûʲô��˵�ġ����ǽ���������setupMesh������

	//��ʼ��
	//�������˹�����������������һ���е���������������Ⱦ��
	//�ڴ�֮ǰ���ǻ�����������ȷ�Ļ��壬��ͨ����������ָ�붨�嶥����ɫ���Ĳ��֡�
	//������Ӧ�ö���Щ�������Ϥ�ˣ���������λ���΢��һ��䶯��ʹ�ýṹ���еĶ������ݣ�
	void Mesh::setupMesh()
	{
		//����
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		//��
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//�ϴ�����
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
		//����
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		//���ö�������
		// ����λ��
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// ���㷨��
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		// ������������
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

		//���
		glBindVertexArray(0);
	}
	//����Ӧ�ú��������ûʲô��ͬ��������Vertex�ṹ��İ���������ʹ����һЩС���ɡ�

	//C++�ṹ����һ���ܰ������ԣ����ǵ��ڴ沼����������(Sequential)��
	//Ҳ����˵��������ǽ��ṹ����Ϊһ����������ʹ�ã���ô��������˳�����нṹ��ı�����
	//�⽫��ֱ��ת��Ϊ���������黺��������Ҫ��float��ʵ�������ֽڣ����顣
	//����˵�����������һ�������Vertex�ṹ�壬��ô�����ڴ沼�ֽ�����ڣ�
	void what()
	{
		Vertex vertex;
		vertex.Position = glm::vec3(0.2f, 0.4f, 0.6f);
		vertex.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
		vertex.TexCoords = glm::vec2(1.0f, 0.0f);
		// = [0.2f, 0.4f, 0.6f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f];
		//��������������õ����ԣ������ܹ�ֱ�Ӵ���һ���е�Vertex�ṹ���ָ����Ϊ��������ݣ�
		//���ǽ���������ת��ΪglBufferData�����õĲ�����
		glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), &mesh.vertices[0], GL_STATIC_DRAW);
		//��Ȼsizeof����Ҳ�������ڽṹ���������������ֽڴ�С��
		//���Ӧ����32�ֽڵģ�8��float* ÿ��4�ֽڣ���

		//�ṹ�������һ���ܺõ���;������Ԥ����ָ��offsetof(s, m)��
		//���ĵ�һ��������һ���ṹ�壬�ڶ�������������ṹ���б��������֡�
		//�����᷵���Ǹ�������ṹ��ͷ�����ֽ�ƫ����(Byte Offset)��
		//�����ÿ������ڶ���glVertexAttribPointer�����е�ƫ�Ʋ�����
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		
		//ƫ����������ʹ��offsetof�������ˣ����������Ὣ���������ֽ�ƫ��������Ϊ�ṹ���з�������ƫ������Ҳ����3��float����12�ֽڡ�
		//ע�⣬����ͬ����������������Ϊ��Vertex�ṹ��Ĵ�С��
	
		//ʹ��������һ���ṹ�岻���ܹ��ṩ�ɶ��Ը��ߵĴ��룬Ҳ�������Ǻ����׵���չ����ṹ��
		//�������ϣ�������һ���������ԣ�����ֻ��Ҫ������ӵ��ṹ���оͿ����ˡ�
		//������������ԣ���Ⱦ�Ĵ��벻�ᱻ�ƻ���
	}
	
	//��Ⱦ
	/*
	������ҪΪMesh�ඨ�����һ������������Draw������
	��������Ⱦ�������֮ǰ��������Ҫ�ڵ���glDrawElements����֮ǰ�Ȱ���Ӧ������
	Ȼ������ʵ������Щ���ѣ�����һ��ʼ����֪�������������еĻ����ж�������������ʲô���͵ġ�
	�������Ǹ��������ɫ������������Ԫ�Ͳ������أ�
	
	Ϊ�˽��������⣬������Ҫ�趨һ��������׼��
	ÿ����������������Ϊtexture_diffuseN��ÿ�����������Ӧ�ñ�����Ϊtexture_specularN��
	����N�ķ�Χ��1����������������������֡�
	����˵���Ƕ�ĳһ��������3������������2��������������ǵ����������Ӧ��֮��ᱻ���ã�
	uniform sampler2D texture_diffuse1;
	uniform sampler2D texture_diffuse2;
	uniform sampler2D texture_diffuse3;
	uniform sampler2D texture_specular1;
	uniform sampler2D texture_specular2;

	���������׼�����ǿ�������ɫ���ж���������Ҫ��������������������һ��������İ����ˣ���ô�ࣩ��������Ҳ��֪�����ǵ�������ʲô��
	����Ҳ�ܸ��������׼��һ�������д�����������������������Ҳ��������ѡ����Ҫʹ�õ���������ֻ��Ҫ������ȷ�Ĳ������Ϳ����ˣ���Ȼ�����ٵĻ����е��˷Ѱ󶨺�uniform���ã���

	�������������кܶ��ֲ�ͬ�Ľ��������
	����㲻ϲ��������������������Լ���һ�����Լ��Ľ���취��
	*/
	//���յ���Ⱦ�����������ģ�
	void Mesh::Draw(Shader shader)
	{
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); // �ڰ�֮ǰ������Ӧ������Ԫ
			// ��ȡ������ţ�diffuse_textureN �е� N��
			string number;
			string name = textures[i].type;
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++);

			shader.setTex(("material." + name + number).c_str(), i);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}
		glActiveTexture(GL_TEXTURE0);

		// ��������
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	//�������ȼ�����ÿ���������͵�N-������������ƴ�ӵ����������ַ����ϣ�����ȡ��Ӧ��uniform���ơ�
	//���������ǲ��Ҷ�Ӧ�Ĳ�������������λ��ֵ����Ϊ��ǰ���������Ԫ����������
	//��Ҳ��������Draw��������Ҫ��ɫ����ԭ��
	//����Ҳ��"material."��ӵ������յ�uniform�����У���Ϊ����ϣ������������һ�����ʽṹ���У�����ÿ��ʵ���п��ܶ���ͬ����

	//ע�������ڽ�������������;�������������stringstreamʱ�������ǽ����˵�����
	//��C++�У��������������variable++���᷵�ر�������֮���ٵ�������++variable�����ȵ������ٷ���ֵ��
	//�����ǵ������������Ƚ�ԭ���ļ�����ֵ����stringstream��֮���ٵ�����������һ��ѭ��ʹ�á�

	//���Ǹն����Mesh��������֮ǰ���۵ĺܶ໰��ĳ�������
	//����һ���У����ǽ�����һ��ģ�ͣ���Ϊ�������������������������ʵ��Assimp�ļ��ؽӿڡ�
}