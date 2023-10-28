//���ļ��������У�����������ų�
#include<vector>
using namespace std;
#include<mythings/shader.h>
#include<mythings/mesh.h>
#include<mythings/camera.h>
#include<stb_image.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

unsigned int TextureFromFile(const char* path, const string& directory);

namespace how
{
	//ģ��
	//������ʱ��Ӵ�Assimp������ʵ�ʵļ��غ�ת�������ˡ�
	//����̵̳�Ŀ���Ǵ�����һ�����������ر�ʾһ��ģ�ͣ�����˵�ǰ���������������Ƕ�������ģ�͡�
	//һ������ľ����̨����¥��������Ӿ�صķ��ӿ����Իᱻ����Ϊһ��ģ�͡�
	//���ǻ�ʹ��Assimp������ģ�ͣ�������ת��(Translate)���������һ���д�����Mesh����

	//�²��˳٣��һ��Ȱ�Model��Ľṹ���㣺
	class Model
	{
	public:
		/*  ����   */
		Model(char* path)
		{
			loadModel(path);
		}
		void Draw(Shader shader);
	private:
		/*  ģ������  */
		vector<Mesh> meshes;
		string directory;
		/*  ����   */
		void loadModel(string path);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
			string typeName);
	};

	//Model�������һ��Mesh�����vector����ע������ָ����C++�е�vectorģ���֮࣬�����������룩����������Ҫ���Ǹ���һ���ļ�·����
	//�ڹ������У�����ֱ��ͨ��loadModel�������ļ���˽�к������ᴦ��Assimp��������е�һ���֣����Ǻܿ�ͻ�������ǡ�
	//���ǻ��������ļ�·����Ŀ¼����֮����������ʱ�򻹻��õ�����

	//Draw����û��ʲô�ر�֮���������Ͼ��Ǳ������������񣬲��������Ǹ��Ե�Draw������
	void Model::Draw(Shader shader)
	{
		for (unsigned int i = 0; i < meshes.size(); i++)
			meshes[i].Draw(shader);
	}

	//����3Dģ�͵�OpenGL
	//Ҫ�뵼��һ��ģ�ͣ�������ת���������Լ������ݽṹ�еĻ�������������Ҫ����Assimp��Ӧ��ͷ�ļ��������������Ͳ��ᱧԹ�����ˡ�
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//������Ҫ���õĺ�����loadModel������ӹ�������ֱ�ӵ��á�
//��loadModel�У�����ʹ��Assimp������ģ����Assimp��һ������scene�����ݽṹ�С�
//����ܻ��ǵ���ģ�ͼ����½ڵĵ�һ�ڽ̳��У�����Assimp���ݽӿڵĸ�����
//һ������������������������Ǿ��ܷ��ʵ����غ��ģ������������������ˡ�

//Assimp�ܰ���һ�����ڣ���������˼��ز�ͬ�ļ���ʽ�����м���ϸ�ڣ�ֻ��Ҫһ�д������������еĹ�����
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	//��������������Assimp�����ռ��ڵ�һ��Importer��֮�����������ReadFile������
	//���������Ҫһ���ļ�·�������ĵڶ���������һЩ���ڴ���(Post-processing)��ѡ�
	//���˼����ļ�֮�⣬Assimp���������趨һЩѡ����ǿ�����Ե����������һЩ����ļ���������
	//ͨ���趨aiProcess_Triangulate�����Ǹ���Assimp�����ģ�Ͳ��ǣ�ȫ��������������ɣ�����Ҫ��ģ�����е�ͼԪ��״�任Ϊ�����Ρ�
	//aiProcess_FlipUVs���ڴ����ʱ��תy����������꣨����ܻ��ǵ�����������̳���˵������OpenGL�д󲿷ֵ�ͼ���y�ᶼ�Ƿ��ģ�����������ڴ���ѡ����޸��������
	//����һЩ�Ƚ����õ�ѡ���У�
	//aiProcess_GenNormals�����ģ�Ͳ������������Ļ�����Ϊÿ�����㴴�����ߡ�
	//aiProcess_SplitLargeMeshes�����Ƚϴ������ָ�ɸ�С����������������Ⱦ����󶥵������ƣ�ֻ����Ⱦ��С��������ô����ǳ����á�
	//aiProcess_OptimizeMeshes�����ϸ�ѡ���෴�����Ὣ���С����ƴ��Ϊһ��������񣬼��ٻ��Ƶ��ôӶ������Ż���

	//Assimp�ṩ�˺ܶ����õĺ��ڴ���ָ�������������ҵ�ȫ����ָ� http://assimp.sourceforge.net/lib_html/postprocess_8h.html
	//ʵ����ʹ��Assimp����ģ���Ƿǳ����׵ģ���Ҳ���Կ�������
	//���ѵ���֮��ʹ�÷��صĳ������󽫼��ص�����ת����һ��Mesh��������顣

	//������loadModel���������������ģ�
	void Model::loadModel(string path)
	{
		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
			return;
		}
		directory = path.substr(0, path.find_last_of('/'));

		processNode(scene->mRootNode, scene);
	}
	//�����Ǽ�����ģ��֮�����ǻ��鳡��������ڵ㲻Ϊnull�����Ҽ��������һ�����(Flag)�����鿴���ص������ǲ��ǲ������ġ�
	//����������κδ������Ƕ���ͨ����������GetErrorString������������󲢷��ء�
	//����Ҳ��ȡ���ļ�·����Ŀ¼·����

	//���ʲô����û�з���������ϣ���������е����нڵ㣬�������ǽ���һ���ڵ㣨���ڵ㣩�����˵ݹ��processNode������
	//��Ϊÿ���ڵ㣨���ܣ������ж���ӽڵ㣬����ϣ�����ȴ�������еĽڵ㣬�ټ�������ýڵ����е��ӽڵ㣬�Դ����ơ�
	//��������һ���ݹ�ṹ���������ǽ�����һ���ݹ麯����
	//�ݹ麯������һЩ����֮��ʹ�ò�ͬ�Ĳ����ݹ���������������ֱ��ĳ������������ֹͣ�ݹ顣
	//�����ǵ��������˳�����(Exit Condition)�����еĽڵ㶼��������ϡ�

	//���ܻ��ǵ�Assimp�Ľṹ�У�ÿ���ڵ������һϵ�е�����������ÿ������ָ�򳡾������е��Ǹ��ض�����
	//���ǽ���������ȥ��ȡ��Щ������������ȡÿ�����񣬴���ÿ�����񣬽��Ŷ�ÿ���ڵ���ӽڵ��ظ���һ���̡�
	//processNode�������������£�
	void Model::processNode(aiNode* node, const aiScene* scene)
	{
		// ����ڵ����е���������еĻ���
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}
		// �������������ӽڵ��ظ���һ����
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}
	//�������ȼ��ÿ���ڵ������������������������mMeshes��������ȡ��Ӧ������
	//���ص����񽫻ᴫ�ݵ�processMesh�����У����᷵��һ��Mesh�������ǿ��Խ����洢��meshes�б�/vector��

	//�������񶼱�����֮�����ǻ�����ڵ�������ӽڵ㣬�������ǵ�����ͬ��processMesh������
	//��һ���ڵ㲻�����κ��ӽڵ�֮�������������ִֹͣ�С�

	/*
	����Ķ��߿��ܻᷢ�֣����ǿ��Ի��������������κεĽڵ㣬ֻ��Ҫ��������������������񣬾Ͳ���ҪΪ����������һ�Ѹ��ӵĶ����ˡ�
	��������ô����ԭ���ǣ�ʹ�ýڵ������뷨�ǽ�����֮�䶨��һ�����ӹ�ϵ��
	ͨ�������ݹ�ر�������ϵ�����Ǿ��ܽ�ĳ��������Ϊ��һ������ĸ������ˡ�

	���ϵͳ��һ��ʹ�ð����ǣ�������λ��һ������������ʱ������Ա�֤�������������񣨱����������񡢷�����������̥���񣩶�������һ��λ�ơ�
	������ϵͳ�ܹ��ø��ӹ�ϵ�����׵ش���������

	Ȼ�����������ǲ�û��ʹ������һ��ϵͳ������������������������и���Ŀ��ƣ�ͨ�����ǽ���ʹ����һ�ַ����ġ�
	������ڵ�Ĺ�ϵ�Ͼ����ɴ��������ģ�͵������������塣
	*/
	//��һ�����ǽ�Assimp�����ݽ�������һ���д�����Mesh���С�

	//��Assimp������
	//��һ��aiMesh����ת��Ϊ�����Լ��������������ô���ѡ�
	//����Ҫ����ֻ�Ƿ��������������Բ������Ǵ��浽�����Լ��Ķ����С�processMesh�����Ĵ���ṹ���£�
	Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			// ������λ�á����ߺ���������
			//...
			vertices.push_back(vertex);
		}
		// ��������
		//...
			// �������
		if (mesh->mMaterialIndex >= 0)
		{
			//...
			;
		}

		return Mesh(vertices, indices, textures);
	}

	//��������Ĺ�����Ҫ�������֣���ȡ���еĶ������ݣ���ȡ���ǵ���������������ȡ��صĲ������ݡ�
	//���������ݽ��ᴢ��������vector���У����ǻ��������ǹ���һ��Mesh���󣬲��������������ĵ��������

	Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			// ������λ�á����ߺ���������
			//
			//��ȡ�������ݷǳ��򵥣����Ƕ�����һ��Vertex�ṹ�壬���ǽ���ÿ������֮�����ӵ�vertices�����С�
			//���ǻ���������е����ж��㣨ʹ��mesh->mNumVertices����ȡ����
			//��ÿ�������У�����ϣ��ʹ�����е���������������ṹ�塣
			//�����λ������������ģ�
			glm::vec3 vector;
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;
			//ע������Ϊ�˴���Assimp�����ݣ����Ƕ�����һ��vec3����ʱ������
			//ʹ������һ����ʱ������ԭ����Assimp�������������ַ����ȶ����Լ���һ���������ͣ����ǲ�����������ת����GLM�����������С�
			//Assimp�����Ķ���λ���������mVertices������ʵ��������ôֱ�ۡ�

			//�����ߵĲ���Ҳ�ǲ��ģ�
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;

			//��������Ĵ���Ҳ�������ƣ���Assimp����һ��ģ����һ�������������8����ͬ���������꣬���ǲ����õ���ô�࣬����ֻ���ĵ�һ���������ꡣ
			//����ͬ��Ҳ���������Ƿ���İ������������꣨���ܲ�����һֱ��ˣ�
			if (mesh->mTextureCoords[0]) //������������������
			{
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			//vertex�ṹ�������Ѿ���������Ҫ�Ķ������ԣ����ǻ��ڵ����������ѹ��vertices���vector��β����
			//������̻��ÿ������Ķ��㶼�ظ�һ�顣
			vertices.push_back(vertex);
		}
		// ��������
		//
		//Assimp�Ľӿڶ�����ÿ��������һ����(Face)���飬ÿ���������һ��ͼԪ�������ǵ������У�����ʹ����aiProcess_Triangulateѡ������������Ρ�
		//һ��������˶�����������Ƕ�������ÿ��ͼԪ�У�����Ӧ�û����ĸ����㣬����ʲô˳����ƣ�����������Ǳ��������е��棬�����������������indices���vector�оͿ����ˡ�
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		//���е��ⲿѭ���������ˣ�������������һϵ�еĶ�����������ݣ����ǿ�������ͨ��glDrawElements��������������
		//Ȼ����Ϊ�˽���������⣬���Ҷ������ṩһЩϸ�ڣ����ǻ���Ҫ��������Ĳ��ʡ�

		// �������
		// 
		//�ͽڵ�һ����һ������ֻ������һ��ָ����ʶ����������
		//�����Ҫ��ȡ���������Ĳ��ʣ����ǻ���Ҫ����������mMaterials���顣
		//�����������λ������mMaterialIndex�����У�����ͬ���������������һ�������Ƿ�����в��ʣ�
		if (mesh->mMaterialIndex >= 0)//��������������
		{
			//��ȡ����
			aiMaterial* material =
				scene->mMaterials[mesh->mMaterialIndex];
			//������������ͼ
			vector<Texture> diffuseMaps =
				loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			//�����model�����Textures�ṹ���Ա��
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			//���ؾ��淴����ͼ
			vector<Texture> specularMaps =
				loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			//�����model�����Textures�ṹ���Ա��
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}
		return Mesh(vertices, indices, textures);
	}
	//loadMaterialTextures����
		//���������˸����������͵���������λ�ã���ȡ��������ļ�λ�ã�
		//�����ز�����������������Ϣ��������һ��Vertex�ṹ���С�������������������
	vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
	{
		vector<Texture> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
		}
		return textures;
	}
	/*
	��������ͨ��GetTextureCount������鴢���ڲ�������������������������Ҫһ���������͡�
	���ǻ�ʹ��GetTexture��ȡÿ��������ļ�λ�ã����Ὣ���������һ��aiString�С�
	���ǽ�����ʹ������һ������TextureFromFile�Ĺ��ߺ����������ᣨ��stb_image.h������һ���������ظ������ID��
	����㲻ȷ�������Ĵ��������д�����Ļ������Բ鿴�����������롣

	ע�⣬���Ǽ�����ģ���ļ��������ļ���·���������ģ���ļ��ı���(Local)·����
	����˵��ģ���ļ�����ͬһĿ¼�¡����ǿ��Խ�����λ���ַ���ƴ�ӵ�֮ǰ����loadModel�У���ȡ��Ŀ¼�ַ����ϣ�
	����ȡ����������·������Ҳ��ΪʲôGetTexture����Ҳ��Ҫһ��Ŀ¼�ַ�������

	���������ҵ���ĳЩģ�ͻ������λ��ʹ�þ���(Absolute)·������Ͳ�����ÿ̨�����϶������ˡ�
	����������£�����ܻ���Ҫ�ֶ��޸�����ļ���������������ʹ�ñ���·����������ܵĻ�����
	*/

	/*
		�������ȴӳ�����mMaterials�����л�ȡaiMaterial����
		����������ϣ������������������/�������ͼ��
		һ�����ʶ�����ڲ���ÿ���������Ͷ��洢��һ������λ�����顣
		��ͬ���������Ͷ���aiTextureType_Ϊǰ׺��
		����ʹ��һ������loadMaterialTextures�Ĺ��ߺ������Ӳ����л�ȡ����
		����������᷵��һ��Texture�ṹ���vector�����ǽ���ģ�͵�textures vector��β��֮��洢����
		*/



	//�ش��Ż�
	/*
	�⻹û����ȫ��������Ϊ���ǻ�������һ���ش�ģ���������ȫ����ģ��Ż���
	��������������ڶ�����������ò���������������һ�����ӣ�����ǽ�����Ż����ҵ�����
	�������Ҳ���Ա�Ӧ�õ��ذ塢�컨�塢¥�ݡ����ӣ������Ǹ�����һ�ھ��ϡ�
	������������һ����������Ĳ����������ǵ�ǰ��ʵ���У�
	����ͬ���������Ѿ������ع��ܶ���ˣ���ÿ�������Ի���ز�����һ���µ�������ܿ�ͻ���ģ�ͼ���ʵ�ֵ�����ƿ����

	�������ǻ��ģ�͵Ĵ�����е����������м��ع�������ȫ�ִ��棬
	ÿ�����������һ�������ʱ������ȥ�������û�б����ع���
	����еĻ������ǻ�ֱ��ʹ���Ǹ����������������������̣���Ϊ����ʡ�ºܶദ��������
	Ϊ���ܹ��Ƚ��������ǻ���Ҫ�������ǵ�·����
	struct Texture
	{
		unsigned int id;
		string type;
		aiString path;  // ���Ǵ��������·������������������бȽ�
	};
	���������ǽ����м��ع�������������һ��vector�У�
	��ģ����Ķ�������Ϊһ��˽�б�����
	vector<Texture> textures_loaded;

	֮����loadMaterialTextures�����У�����ϣ���������·���봢����textures_loaded���vector�е�����������бȽϣ�
	������ǰ�����·���Ƿ������е�һ����ͬ������ǵĻ����������������/���ɵĲ��֣�
	ֱ��ʹ�ö�λ��������ṹ��Ϊ������������º�ĺ������£�
	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
	{
		vector<Texture> textures;
		for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			bool skip = false;
			for(unsigned int j = 0; j < textures_loaded.size(); j++)
			{
				if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
				{
					textures.push_back(textures_loaded[j]);
					skip = true;
					break;
				}
			}
			if(!skip)
			{   // �������û�б����أ��������
				Texture texture;
				texture.id = TextureFromFile(str.C_Str(), directory);
				texture.type = typeName;
				texture.path = str.C_Str();
				textures.push_back(texture);
				textures_loaded.push_back(texture); // ��ӵ��Ѽ��ص�������
			}
		}
		return textures;
	}
	�����������ǲ������˸�����ģ�ͼ���ϵͳ������Ҳ�����һ�����ض���ܿ���Ż��汾��

	��Щ�汾��Assimp��ʹ�õ��԰汾����ʹ��IDE�ĵ���ģʽ�¼���ģ�ͻ�ǳ�������
	�����������������ļ����ٶ�ʱ����������ʹ�÷����汾��
	*/
}

//����Ϊmain.cpp
//������ģ�͸��
/*
���ԣ������ǵ���һ�����������������������ģ�ͣ�����������ŵ���Ʒ
����Ҫ���ϣ���Щ���ӿ������㿴������Ư�����������ˣ�������һ�����ǵ�ʵ�ְɡ�
�����Ҳ�������ռ̫��Ĺ��ͣ��һ�ż���ñ��������Ҳ�������ǣ�
������ǽ������Crytek����Ϸ�µ�Σ��(Crysis)�е�ԭ������װ(Nanosuit)��
���ģ�ͱ����Ϊһ��.obj�ļ��Լ�һ��.mtl�ļ���
.mtl�ļ�������ģ�͵������䡢�����ͷ�����ͼ��������ں���ѧϰ������

ע�����е������ģ���ļ�Ӧ��λ��ͬһ��Ŀ¼�£��Թ���������

�����ڴ����У�����һ��Model���󣬽�ģ�͵��ļ�λ�ô��롣
������ģ��Ӧ�û��Զ����ز������û�д���Ļ�������Ⱦѭ����ʹ������Draw�������������壬
�����Ϳ����ˡ�������Ҫ������䡢����ָ�����Ⱦָ�ֻ��Ҫһ�д���Ϳ����ˡ�
����������㴴��һϵ����ɫ��������Ƭ����ɫ��������������������������ɫ
*/

