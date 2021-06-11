#include "ObjReader.h"
#include <vector>

/*
struct Index
{
	unsigned int v;
	unsigned int vt;
	unsigned int vn;

	Index(unsigned int v_, unsigned int vt_, unsigned int vn_) :
		v(v_),
		vt(vt_),
		vn(vn_)
	{}
};
*/
void ObjReader::readObjectFile(std::string filePath)
{
	std::vector<Vec4> pos;
	std::vector<Vec4> normal;
	//std::vector<Index> indices;
	
	std::ifstream myfile(filePath);
	Vec4 cc(0.0, 0.0, 0.0);

	std::string line;
	int countV = 0;
	int countN = 0;
	int countF = 0;
	while (std::getline(myfile, line))
	{
		if (line[0] == 'v' && line[1] == ' ')
		{
			countV++;
			std::stringstream ls(line);
			std::string data;
			double vx, vy, vz;
			std::getline(ls, data, ' ');
			ls >> vx >> vy >> vz;
			//vx /= 1000;
			//vy /= 1000;
			//vz /= 1000;
			vertices.push_back(VertexIn(Vec4(vx, -vy+200, vz),Vec4(0.0,0.0,0.0),Vec4(0.0,0.0,0.0)));
		}
		else if (line[0] == 'v' && line[1] == 'n')
		{
			countN++;
			std::stringstream ls(line);
			std::string data;
			double vx, vy, vz;
			std::getline(ls, data, ' ');
			ls >> vx >> vy >> vz;
			Vec4 temp(vx, vy, vz);
			normal.push_back(temp);
		}
		else if (line[0] == 'f' && line[1] == ' ')
		{
			countF++;
			std::stringstream ls(line);
			std::string data;
			std::string s1, s2, s3;
			std::getline(ls, data, ' ');
			ls >> s1 >> s2 >> s3;

			std::stringstream str1(s1);
			std::string sf1;
			int f1;
			std::getline(str1, sf1, '/');
			f1 = std::stoi(sf1);
			std::getline(str1, sf1, '/');
			int fn1;
			str1 >> fn1;

			std::stringstream str2(s2);
			std::string sf2;
			int f2;
			std::getline(str2, sf2, '/');
			f2 = std::stoi(sf2);
			std::getline(str2, sf2, '/');
			int fn2;
			str2 >> fn2;

			std::stringstream str3(s3);
			std::string sf3;
			int f3;
			std::getline(str3, sf3, '/');
			f3 = std::stoi(sf3);
			std::getline(str3, sf3, '/');
			int fn3;
			str3 >> fn3;

			//indices.push_back(Index(f1, 0, fn1));
			//indices.push_back(Index(f2, 0, fn2));
			//indices.push_back(Index(f3, 0, fn3));

			vertices[f1 - 1].normal = normal[fn1-1];
			vertices[f2 - 1].normal = normal[fn2-1];
			vertices[f3 - 1].normal = normal[fn3-1];

			indices.push_back((f1 - 1));
			indices.push_back((f2 - 1));
			indices.push_back((f3 - 1));
		}
		else
		{
			continue;
		}
	}

	myfile.close();
	//std::vector<VertexIn> vertices;
	//std::vector<int> indices2;
	//for(int i=0; i< )
}