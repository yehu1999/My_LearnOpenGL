//���ļ��������У�����������ų�
#include<mythings/shader.h>
#include<mythings/camera.h>
#include<stb_image.h>


//�߼�����
namespace how
{
	unsigned int data, buffer, vbo1, vbo2;
	void how()
	{
		//������OpenGL�д���ʹ�û��������������Ѿ��кܳ�ʱ���ˡ�
		//����������ʵ���и�����˼�ķ�ʽ������ʹ�������������ݴ�����ɫ��Ҳ�и���Ȥ�ķ�����
		//��һ���У����ǽ�����һЩ������˼�Ļ��庯�����Լ����Ǹ����ʹ�����������������������ݣ�����Ĳ��ֻ�û����ɣ���

		//OpenGL�еĻ���ֻ��һ�������ض��ڴ��Ķ���û����������Ĺ����ˡ�
		//�����ǽ����󶨵�һ������Ŀ��(Buffer Target)ʱ�����ǲŸ����������塣
		//�����ǰ�һ�����嵽GL_ARRAY_BUFFERʱ��������һ���������黺�壬������Ҳ���Ժ����׵ؽ���󶨵�GL_ELEMENT_ARRAY_BUFFER��
		//OpenGL�ڲ���Ϊÿ��Ŀ�괢��һ�����壬���һ����Ŀ��Ĳ�ͬ���Բ�ͬ�ķ�ʽ�����塣

		//��ĿǰΪֹ������һֱ�ǵ���glBufferData��������仺�������������ڴ棬
		//������������һ���ڴ棬����������ӵ�����ڴ��С�
		//������ǽ�����data��������ΪNULL����ô���������ֻ������ڴ棬����������䡣
		//����������ҪԤ��(Reserve)�ض���С���ڴ棬֮��ص��������һ��һ������ʱ�������á�

		//����ʹ��һ�κ������������������֮�⣬����Ҳ����ʹ��glBufferSubData����仺����ض�����
		//���������Ҫһ������Ŀ�ꡢһ��ƫ���������ݵĴ�С�����ݱ�����Ϊ���Ĳ�����
		//���������ͬ�ĵط����ڣ����ǿ����ṩһ��ƫ������ָ���Ӻδ���ʼ���������塣
		//���ܹ������ǲ�����߸��»����ڴ��ĳһ���֡�
		//Ҫע����ǣ�������Ҫ���㹻���ѷ����ڴ棬���Զ�һ���������glBufferSubData֮ǰ����Ҫ�ȵ���glBufferData��
		glBufferSubData(GL_ARRAY_BUFFER, 24, sizeof(data), &data);
		// ��Χ�� [24, 24 + sizeof(data)]

		//�����ݵ��뻺�������һ�ַ����ǣ����󻺳��ڴ��ָ�룬ֱ�ӽ����ݸ��Ƶ����嵱�С�
		//ͨ������glMapBuffer������OpenGL�᷵�ص�ǰ�󶨻�����ڴ�ָ�룬�����ǲ�����
		float data[] =
		{
			0.5f, 1.0f, -0.35f
			//...
		};
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		// ��ȡָ��
		void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		// �������ݵ��ڴ�
		memcpy(ptr, data, sizeof(data));
		// �ǵø���OpenGL���ǲ�����Ҫ���ָ����
		glUnmapBuffer(GL_ARRAY_BUFFER);

		//������ʹ��glUnmapBuffer����������OpenGL�����Ѿ����ָ�����֮��OpenGL�ͻ�֪�����Ѿ�����ˡ�
		//�ڽ��ӳ��(Unmapping)֮��ָ�뽫�᲻�ٿ��ã��������OpenGL�ܹ��ɹ�����������ӳ�䵽�����У�����������᷵��GL_TRUE��

		//���Ҫֱ��ӳ�����ݵ����壬�������Ƚ���洢����ʱ�ڴ��У�glMapBuffer�������������á�
		//����˵������Դ��ļ��ж�ȡ���ݣ���ֱ�ӽ����Ǹ��Ƶ������ڴ��С�

		//������������

		//ͨ��ʹ��glVertexAttribPointer�������ܹ�ָ���������黺�����ݵ����Բ��֡�
		//�ڶ������黺���У����Ƕ����Խ����˽���(Interleave)����
		//Ҳ����˵�����ǽ�ÿһ�������λ�á����ߺ�/������������ܷ�����һ��
		//��Ȼ���������Ѿ��Ի������˸�����˽⣬���ǿ��Բ�ȡ��һ�ַ�ʽ��

		//���ǿ��������ǣ���ÿһ���������͵��������ݴ��(Batch)Ϊһ��������飬�����Ƕ����ǽ��н����档
		//�뽻����123123123123��ͬ�����ǽ����÷���(Batched)�ķ�ʽ111122223333��

		//�����ļ��м��ض������ݵ�ʱ����ͨ����ȡ������һ��λ�����顢һ�����������/��һ�������������顣
		//������Ҫ�����������ܽ���Щ����ת��Ϊһ����Ľ����������顣
		//ʹ�÷����ķ�ʽ���Ǹ��򵥵Ľ�����������ǿ��Ժ�����ʹ��glBufferSubData����ʵ�֣�
		float positions[] = { NULL };
		float normals[] = { NULL };
		float tex[] = { NULL };
		// ��仺��
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positions), &positions);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(positions), sizeof(normals), &normals);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(positions) + sizeof(normals), sizeof(tex), &tex);

		//���������Ǿ���ֱ�ӽ�����������Ϊһ�����崫�ݸ����壬������Ҫ���ȴ��������ˡ�
		//�����Կ��Խ����Ǻϲ�Ϊһ��������飬��ʹ��glBufferData����仺�壬
		//���������ֹ�����ʹ��glBufferSubData�������һ�㡣

		//���ǻ���Ҫ���¶�������ָ������ӳ��Щ�ı䣺
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(sizeof(positions)));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(sizeof(positions) + sizeof(normals)));

		//ע��stride�������ڶ������ԵĴ�С����Ϊ��һ������������������3������2��������֮���ҵ���

		//������������ö������Ե���һ�ַ�����ʹ�����ַ����������OpenGL��ʲô���̵ĺô�����ֻ�����ö������Ե�һ�ָ�����ķ�ʽ��
		//����ʹ�õķ�������ȫȡ�������ϲ����������͡�

		//���ƻ���

		//����Ļ����Ѿ���������֮������ܻ����������Ļ��干�����е����ݣ�������Ҫ����������ݸ��Ƶ���һ�����嵱�С�
		//glCopyBufferSubData�ܹ�������������׵ش�һ�������и������ݵ���һ�������С����������ԭ�����£�
		void glCopyBufferSubData(GLenum readtarget, GLenum writetarget, GLintptr readoffset, GLintptr writeoffset, GLsizeiptr size);
		//readtarget��writetarget������Ҫ���븴��Դ�͸���Ŀ��Ļ���Ŀ�ꡣ
		//����˵�����ǿ��Խ�VERTEX_ARRAY_BUFFER���帴�Ƶ�VERTEX_ELEMENT_ARRAY_BUFFER���壬
		//�ֱ���Щ����Ŀ������Ϊ����д��Ŀ�ꡣ
		//��ǰ�󶨵���Щ����Ŀ��Ļ��彫�ᱻӰ�쵽��

		//������������д���ݵ�������ͬ���嶼Ϊ�������黺�����ô���أ�
		//���ǲ���ͬʱ����������󶨵�ͬһ������Ŀ���ϡ�
		//���ǳ������ԭ��OpenGL�ṩ������������������Ŀ�꣬����GL_COPY_READ_BUFFER��GL_COPY_WRITE_BUFFER��
		//���ǽ������Ϳ��Խ���Ҫ�Ļ���󶨵�����������Ŀ���ϣ�����������Ŀ����Ϊreadtarget��writetarget������

		//������glCopyBufferSubData���readtarget�ж�ȡsize��С�����ݣ�������д��writetarget�����writeoffsetƫ��������
		//�����������չʾ����θ��������������黺�壺
		float vertexData[] = { NULL };
		glBindBuffer(GL_COPY_READ_BUFFER, vbo1);
		glBindBuffer(GL_COPY_WRITE_BUFFER, vbo2);
		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, sizeof(vertexData));

		//����Ҳ����ֻ��writetarget�����Ϊ�µĻ���Ŀ������֮һ��
		float vertexData[] = { NULL };
		glBindBuffer(GL_ARRAY_BUFFER, vbo1);
		glBindBuffer(GL_COPY_WRITE_BUFFER, vbo2);
		glCopyBufferSubData(GL_ARRAY_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, sizeof(vertexData));

		//������Щ������β�������Ķ���֪ʶ�������Ѿ��ܹ��Ը�����˼�ķ�ʽʹ�������ˡ�
		//����Խ����OpenGLʱ����Щ�µĻ��巽�������ø������á�
		//����һ���У�����������Uniform�������(Uniform Buffer Object)ʱ�����ǽ���������glBufferSubData��
	}
}