#include "msg.h"
using std::regex;
using std::vector;
using std::sregex_token_iterator;


vector<string> DV_Msg::decode(string string_to_be_decoded)
{
	regex ex_field("/|#");
	sregex_token_iterator pos(string_to_be_decoded.begin(),string_to_be_decoded.end(),ex_field,-1);
	decltype(pos) end;
	vector<string> res;
	for(;pos!=end;++pos)
	{
        if(pos->str()!="") res.push_back(pos->str());		
	}
    return res;
}
