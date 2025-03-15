#include <GL/glut.h>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
#include <algorithm>
using namespace std;

struct Vertex
{
	GLfloat x, y, z;
};//����ṹ��

struct Texture
{
	GLfloat x, y;
};//����ṹ��

struct Normal
{
	GLfloat x, y, z;
};//���߽ṹ��

struct Material
{
	GLfloat ambient[4] = { 0.2, 0.2, 0.2, 1.0 };// ������
	GLfloat diffuse[4] = { 0.8, 0.8, 0.8, 1.0 };// ������
	GLfloat specular[4] = { 0.0, 0.0, 0.0, 1.0 };// ���淴��
	GLfloat shininess = 65.0;// �߹�ָ��
};//���ʽṹ��

struct Face
{
	GLint a, b, c; // �����±�
	GLint ta, tb, tc; // ���������±�
	GLint na, nb, nc; // ���㷨���±�
	string mtl_name; // ������
};//��ṹ��

class OBJImporter
{
public:
	OBJImporter(string path)
		: path(path)
	{
		ifstream ifs;
		ifs.open(path, ifstream::in);
		if (!ifs.is_open()) {
			cerr << "Can't open file.\n";
			return;

		}// ���캯����ȡ���㡢�������ߡ�������
		parseStream(ifs);// �����ļ���
		ifs.close();// �ر��ļ���
		smoothNormals();// ƽ������
	}
	~OBJImporter() = default;// ��������

	string path;
	vector<Vertex> vertexList;// �����б�
	vector<Texture> textureList;// �����б�
	vector<Normal> normalList;// �����б�
	vector<Face> faceList;// ���б�
	unordered_map<string, Material> materialTable;// ���ʱ�

private:
	void parseStream(std::ifstream& ifs) {// �����ļ���
		string lineBuffer;// ��ʱ������
		string currentMaterialName;// ��ǰ������
		while (getline(ifs, lineBuffer).good())
		{
			if (lineBuffer.empty()) continue;// ��������
			switch (lineBuffer[0])// �������ַ��ж�����
			{
			case '#': // comment
				break;
			case 'm': // mtllib
				readMaterial(lineBuffer);
				break;
			case 'u': // usemtl
				useMaterial(lineBuffer, currentMaterialName);
				break;
			case 'v': // v, vt, vn
				readVertex(lineBuffer);
				break;
			case 'f': // face
				readFace(lineBuffer, currentMaterialName);
				break;
			default: // δ֧�� group
				break;
			}
		}

	}
	void readMaterial(string& lineBuffer);// ��ȡ����
	void useMaterial(string& lineBuffer, string& currentMaterialName);// ѡ�����
	void readVertex(string& lineBuffer);// ��ȡ����

	void readFace(string& lineBuffer, string& currentMaterialName);// ��ȡ��
	void smoothNormals();// ƽ������
};
