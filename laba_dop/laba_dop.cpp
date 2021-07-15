#include <iostream>
#include <set>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

struct Coords {
	float lat, lon;
	bool operator <(const Coords& a) const {
		return lat < a.lat;
	}
};


std::string LDToStr(long double one)
{
	std::stringstream ss;
	ss << std::setprecision(10);
	ss << one;
	return ss.str();
}
float v(const std::string& str, int &i) {
	float a=1;
	std::string link_to_coord,coord;
	while (i < str.length() && str[i] != ';') {
		link_to_coord += str[i];
		i++;
	}
	
	for (int i = 0; i <9; i++) {
		if (i >= link_to_coord.length()) {
			coord.insert(0, "0");
		}
		else {
			coord += link_to_coord[i];
		}
	}
	coord.insert(2, ".");
	a = std::stod(coord);
	return a;
}

void find_lan_lon(float& c,const std::string& link, int &i) {
	c = v(link, i);
	while ( i<link.length() && link[i] != ';')
		i++;
	if(i-1 < link.length())
	  i++;
}

void fill_coords(const std::string& link,std::set<Coords> &way, int i,const float& x, const float& y) {
	Coords c;
	while (i < link.length()) {
		find_lan_lon(c.lat,link,i);
		find_lan_lon(c.lon, link, i);
		if (i < link.length() && link[i] == '0' && link[i + 1] == ';')
			i += 2;
		c.lat += x;
		c.lon += y;
		way.insert(c);
	}
}

void midle_coords(float& x, float&y, const std::string& link, int i) {
	while (link[i] != '&') {
		i++;
	}
	i += 5;
	std::string coord;
	while (link[i] != '&') {
		coord += link[i];
		i++;
	}
	i += 5;
	x = std::stof(coord);
	coord = "";
	while (link[i] != '&') {
		coord += link[i];
		i++;
	}
	y = std::stof(coord);
}

class Way {
public:

	Way(){
		std::string link;
		Read_link(link);
		int index_of_coord = 0, index_midle_coord=0;
		float x, y;
		midle_coords(x, y, link, index_midle_coord);
		while(link[index_of_coord] != ';') {
			index_of_coord++;
		}
		index_of_coord++;

		fill_coords(link,way,index_of_coord,x,y);

	}
	std::string Read_link(std::string &link) {
		std::ifstream file("link.txt");
		if (!file) {
			std::cout << "File don't open!" << std::endl;
		}
		else {
			std::cout << "File is open!" << std::endl;
		}
		file >> link;
		return link;
	}
	std::string Write_to_format_json() {
		std::string format_json;
		int nomer_coord = 1;
		format_json += "{ /n";
		format_json+="   \"Route\": [ /n" ;
		for (const auto& q : way) {
			format_json += "             { /n               \"lat\": "+ std::to_string(q.lat)+ " , \"lon\":  "+ std::to_string(q.lon)+"/n             },/n";
		}
		format_json += "   ]/n";
		std::cout << "Iformation write in json-file." << std::endl;
		format_json+= "}/n";
		return format_json;
	}
	void Write_json(std::string format_json) {
		std::ofstream file2("way.json");
		for (size_t i = 0; i < format_json.length(); i++){
			if (format_json[i + 1] == 'n' && format_json[i] == '/') {
				file2 <<std::endl;
				i ++;
			}
			else {
				file2 << format_json[i];
			}
		}
	}
	
private:
	std::set<Coords> way;
};

int main() {
	
	Way a;
	a.Write_json(a.Write_to_format_json());

	return 0;
}