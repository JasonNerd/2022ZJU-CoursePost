
#include <vector>
#include <string>
using namespace std;
class Util{
public:
	Util(){}
	~Util(){}

	void split(vector<string>& res, string s){
		res.clear();
		int a = 0, b = 0;
		while(s.length()>0 && s[s.length()-1] == ' ') s.erase(s.end()-1);
		while(s.length()>0 && s[0] == ' ') s.erase(s.begin());
		for(int i = 0; i < s.length(); i++){
			if(' ' != s[i]){
				b++;
			}else if(b == 0){
				a = i+1;
			}
			else if(b != 0){
				res.push_back(s.substr(a,b));
				a = i+1;
				b = 0;
			}
		}
		res.push_back(s.substr(a,b));
	}
};