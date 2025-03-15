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
};//顶点结构体

struct Texture
{
	GLfloat x, y;
};//纹理结构体

struct Normal
{
	GLfloat x, y, z;
};//法线结构体

struct Material
{
	GLfloat ambient[4] = { 0.2, 0.2, 0.2, 1.0 };// 环境光
	GLfloat diffuse[4] = { 0.8, 0.8, 0.8, 1.0 };// 漫反射
	GLfloat specular[4] = { 0.0, 0.0, 0.0, 1.0 };// 镜面反射
	GLfloat shininess = 65.0;// 高光指数
};//材质结构体

struct Face
{
	GLint a, b, c; // 顶点下标
	GLint ta, tb, tc; // 顶点纹理下标
	GLint na, nb, nc; // 顶点法线下标
	string mtl_name; // 材质名
};//面结构体

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

		}// 构造函数读取顶点、纹理、法线、面数据
		parseStream(ifs);// 解析文件流
		ifs.close();// 关闭文件流
		smoothNormals();// 平滑法线
	}
	~OBJImporter() = default;// 析构函数

	string path;
	vector<Vertex> vertexList;// 顶点列表
	vector<Texture> textureList;// 纹理列表
	vector<Normal> normalList;// 法线列表
	vector<Face> faceList;// 面列表
	unordered_map<string, Material> materialTable;// 材质表

private:
	void parseStream(std::ifstream& ifs) {// 解析文件流
		string lineBuffer;// 临时缓冲区
		string currentMaterialName;// 当前材质名
		while (getline(ifs, lineBuffer).good())
		{
			if (lineBuffer.empty()) continue;// 空行跳过
			switch (lineBuffer[0])// 根据首字符判断类型
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
			default: // 未支持 group
				break;
			}
		}

	}
	void readMaterial(string& lineBuffer);// 读取材质
	void useMaterial(string& lineBuffer, string& currentMaterialName);// 选择材质
	void readVertex(string& lineBuffer);// 读取顶点

	void readFace(string& lineBuffer, string& currentMaterialName);// 读取面
	void smoothNormals();// 平滑法线
};
