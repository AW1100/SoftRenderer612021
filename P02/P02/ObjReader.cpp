#include "ObjReader.h"
#include <vector>

void ObjReader::readObjectFile()
{
	std::ifstream myfile(filePath);

	std::string line;
	int countV = 0;
	int countN = 0;
	int countF = 0;
	while (std::getline(myfile, line))
	{
		if (line[0] == 'v' && line[1] == ' ')
		{
			std::stringstream ls(line);
			std::string data;
			double vx, vy, vz;
			std::getline(ls, data, ' ');
			ls >> vx >> vy >> vz;
			
			pos.push_back(Vec4(vx, vy, vz, 1.0));
		}
		else if (line[0] == 'v' && line[1] == 't')
		{
			std::stringstream ls(line);
			std::string data;
			double vx, vy;
			std::getline(ls, data, ' ');
			ls >> vx >> vy;
			uv.push_back(Vec2(vx, vy));
		}
		else if (line[0] == 'v' && line[1] == 'n')
		{
			std::stringstream ls(line);
			std::string data;
			double vx, vy, vz;
			std::getline(ls, data, ' ');
			ls >> vx >> vy >> vz;
			normal.push_back(Vec4(vx, vy, vz, 0.0));
		}
		else if (line[0] == 'f' && line[1] == ' ')
		{
			std::stringstream ls(line);
			std::string data;
			std::string s1, s2, s3;
			std::getline(ls, data, ' ');
			ls >> s1 >> s2 >> s3;

			std::stringstream str1(s1);
			std::string sf1;
			int f1, ft1, fn1;
			std::getline(str1, sf1, '/');
			f1 = std::stoi(sf1);
			str1 >> ft1;
			std::getline(str1, sf1, '/');
			str1 >> fn1;

			std::stringstream str2(s2);
			std::string sf2;
			int f2, ft2, fn2;
			std::getline(str2, sf2, '/');
			f2 = std::stoi(sf2);
			str2 >> ft2;
			std::getline(str2, sf2, '/');
			str2 >> fn2;

			std::stringstream str3(s3);
			std::string sf3;
			int f3, ft3, fn3;
			std::getline(str3, sf3, '/');
			f3 = std::stoi(sf3);
			str3 >> ft3;
			std::getline(str3, sf3, '/');
			str3 >> fn3;

			indices.push_back(f1);
			indices.push_back(ft1);
			indices.push_back(fn1);
			indices.push_back(f2);
			indices.push_back(ft2);
			indices.push_back(fn2);
			indices.push_back(f3);
			indices.push_back(ft3);
			indices.push_back(fn3);
		}
		else
		{
			continue;
		}
	}

	myfile.close();
}