#include "OBJImporter.h"

void OBJImporter::readMaterial(string& lineBuffer)
{
	stringstream lineStream(lineBuffer);//
	string skip;
	string materialFilename;
	lineStream >> skip >> materialFilename;
	// �� .obj ͬ·���¶�ȡ .mtl �ļ�
	ifstream mtl;
	mtl.open(path.substr(0, path.rfind('/') + 1) + materialFilename, ifstream::in);
	if (!mtl.is_open()) {
		cerr << "Can't open file.\n";
		return;

	}
	// ���� .mtl
	string lineBufferMtl;
	string currentMaterialName;
	while (getline(mtl, lineBufferMtl).good())
	{
		if (lineBuffer.empty()) continue;
		stringstream lineStreamMtl(lineBufferMtl);
		string skip;
		switch (lineBufferMtl[0])
		{
		case '#':
			break;
		case 'n': // newmtl
			lineStreamMtl >> skip >> currentMaterialName;
			break;
		case 'K': // Ka Kd Ks
			switch (lineBufferMtl[1])
			{


			case 'a': // ambient
				lineStreamMtl >> skip;
				for (int i = 0; i < 3; i++)
				{
					lineStreamMtl >> materialTable[currentMaterialName].ambient[i];
				}
				break;
			case 'd': // diffuse
				lineStreamMtl >> skip;
				for (int i = 0; i < 3; i++)
				{
					lineStreamMtl >> materialTable[currentMaterialName].diffuse[i];
				}
				break;
			case 's': // specular
				lineStreamMtl >> skip;
				for (int i = 0; i < 3; i++)
				{
					lineStreamMtl >> materialTable[currentMaterialName].specular[i];
				}
				break;
			default:
				break;
			}
			break;
		case 'N': //shininess
			lineStreamMtl >> skip >> materialTable[currentMaterialName].shininess;
			/* wavefront shininess is from [0, 1000], so scale for OpenGL */
			materialTable[currentMaterialName].shininess /= 1000;
			materialTable[currentMaterialName].shininess *= 128;
			break;
		default:
			break;
		}
	}
	mtl.close();
}

void OBJImporter::useMaterial(string& lineBuffer, string& currentMaterialName)

{
	stringstream lineStream(lineBuffer);
	string skip;
	lineStream >> skip >> currentMaterialName;
}

void OBJImporter::readVertex(string& lineBuffer)
{
	stringstream lineStream(lineBuffer);
	string skip;
	switch (lineBuffer[1])
	{
	case ' ': // v 0 0 0
	{
		Vertex temp;
		lineStream >> skip >> temp.x >> temp.y >> temp.z;
		vertexList.push_back(temp);
	}
	break;
	case 't': // vt 0 0 // δ֧������
	{
		/*float x, y;
			lineStream >> skip >> x >> y;
			textureList.push_back(x, y);*/
	}
	break;
	case 'n': // vn 0 0 0
	{
		Normal temp;
		lineStream >> skip >> temp.x >> temp.y >> temp.z;
		normalList.push_back(temp);
	}
	break;
	default:
		break;
	}
}

void OBJImporter::readFace(string& lineBuffer, string& currentMaterialName)
{
	// can be one of %d, %d//%d, %d/%d, %d/%d/%d
	int flag = 0;
	for (int i = 0; i <lineBuffer.size(); i++)
	{


		if (lineBuffer[i] == '/')
		{
			if (i + 1 < lineBuffer.size() && lineBuffer[i + 1] == '/')
			{
				flag = 1; // "v//n"
			}
			else
			{
				for (int j = i + 1; j < lineBuffer.size(); j++)
				{
					if (lineBuffer[j] == ' ')
					{
						flag = 2; // "v/t"
						break;
					}
					if (lineBuffer[j] == '/')
					{
						flag = 3; // "v/t/n"
						break;
					}
				}
			}
			break;
		}
	}

	// �滻 '/' Ϊ ' ' �����ڴ���
	for (auto& c : lineBuffer)
	{
		if (c == '/')
		{
			c = ' ';
		}
	}

	stringstream lineStream(lineBuffer);
	string skip;
	lineStream >> skip;

	switch (flag)
	{
	case 0: // f v v v (v)
	{
		int a, b, c, d;

		lineStream >> a >> b >> c;
		Face temp = { a - 1, b - 1, c -
	   1, 0, 0, 0, 0, 0, 0, currentMaterialName }; // �����±�ƫ��
		faceList.push_back(temp);
		// ֻ֧������������ı����棬�ı����浱�������������洦��
		if (lineStream.good())
		{
			lineStream >> d;
			Face temp = { a - 1, c - 1, d -
		   1, 0, 0, 0, 0, 0, 0, currentMaterialName };
			faceList.push_back(temp);
		}
	}
	break;
	case 1: // f v//n v//n v//n (v//n)
	{
		int a, b, c, d;
		int an, bn, cn, dn;
		lineStream >> a >> an >> b >> bn >> c >> cn;
		Face temp = { a - 1, b - 1, c - 1, 0, 0, 0, an - 1, bn - 1, cn - 1, currentMaterialName };
		faceList.push_back(temp);
		if (lineStream.good())
		{
			lineStream >> d >> dn;
			Face temp = { a - 1, c - 1, d - 1, 0, 0, 0, an - 1, cn - 1, dn -
		   1, currentMaterialName };
			faceList.push_back(temp);
		}
	}
	break;
	case 2: // f v/t v/t v/t (v/t)
	{
		// δ֧�� vt
		int a, b, c, d;
		lineStream >> a >> b >> c;
		Face temp = { a - 1, b - 1, c -
	   1, 0, 0, 0, 0, 0, 0, currentMaterialName }; // �����±�ƫ��
		faceList.push_back(temp);
		if (lineStream.good())
		{
			lineStream >> d;
			Face temp = { a - 1, c - 1, d -
		   1, 0, 0, 0, 0, 0, 0, currentMaterialName };


			faceList.push_back(temp);
		}
	}
	break;
	case 3: // f v/t/n v/t/n v/t/n (v/t/n)
	{
		// δ֧�� vt
		int a, b, c, d;
		int an, bn, cn, dn;
		int skip;
		lineStream >> a >> skip >> an >> b >> skip >> bn >> c >> skip >> cn;

		Face temp = { a - 1, b - 1, c - 1, 0, 0, 0, an - 1, bn - 1, cn -
	   1, currentMaterialName };
		faceList.push_back(temp);
		if (lineStream.good())
		{
			lineStream >> d >> skip >> dn;
			Face temp = { a - 1, c - 1, d - 1, 0, 0, 0, an - 1, cn - 1, dn -
		   1, currentMaterialName };
			faceList.push_back(temp);
		}
	}
	break;
	default:
		break;
	}
}

void OBJImporter::smoothNormals()
{
	// �����淨��
	vector<Normal> faceNormalList;
	faceNormalList.resize(faceList.size());
	for (int i = 0; i < faceList.size(); i++)
	{
		Face& f = faceList[i];
		Normal& fn = faceNormalList[i];
		// ���
		Normal Vab = {
		vertexList[f.b].x - vertexList[f.a].x,
		vertexList[f.b].y - vertexList[f.a].y,
		vertexList[f.b].z - vertexList[f.a].z
		};

		Normal Vac = {
		vertexList[f.c].x - vertexList[f.a].x,
		vertexList[f.c].y - vertexList[f.a].y,
		vertexList[f.c].z - vertexList[f.a].z
		};
		fn = {
		Vab.y * Vac.z - Vab.z * Vac.y,
		Vab.z * Vac.x - Vab.x * Vac.z,
		Vab.x * Vac.y - Vab.y * Vac.x
		};
		// ����
		GLfloat l = (GLfloat)sqrt(fn.x * fn.x + fn.y * fn.y + fn.z * fn.z);
		fn = {
	fn.x / l,
	fn.y / l,
	fn.z / l
		};
	}

	// ����Ҫԭ�еķ���
	if (!normalList.empty())
	{
		normalList.clear();
	}

	// Ϊÿ��������ӳ�Ա�棨���а������������棩
	vector<vector<int>> members;
	members.resize(vertexList.size());
	for (int i = 0; i < faceList.size(); i++)
	{
		members[faceList[i].a].push_back(i);
		members[faceList[i].b].push_back(i);
		members[faceList[i].c].push_back(i);
	}

	// ����ƽ�����㷨��
	for (int i = 0; i < vertexList.size(); i++)
	{
		Normal avg = { 0, 0, 0 };
		Normal fn1 = { 0, 0, 0 };
		for (auto& findex : members[i])
		{
			// ֻ�е�һ�λ������һ����ļн�С�� 90 �Ȳ���Ӹ��淨��
			Normal& fn = faceNormalList[findex];

			if (fn1.x * fn1.x + fn1.y * fn1.y + fn1.z * fn1.z == 0)
			{
				fn1 = { fn.x, fn.y, fn.z };
				avg.x += fn.x;
				avg.y += fn.y;
				avg.z += fn.z;
			}
			else if (fn1.x * fn.x + fn1.y * fn.y + fn1.z * fn.z)
			{
				avg.x += fn.x;
				avg.y += fn.y;
				avg.z += fn.z;
			}
		}
		GLfloat l = (GLfloat)sqrt(avg.x * avg.x + avg.y * avg.y + avg.z * avg.z);
		avg.x /= l;
		avg.y /= l;
		avg.z /= l;
		normalList.push_back(avg);
	}
}
