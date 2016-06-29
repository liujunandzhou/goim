/*
 * =====================================================================================
 *
 *       Filename:  util.cc
 *
 *    Description:  implement the func in the util.h
 *
 *        Version:  1.0
 *        Created:  2015年01月13日 12时37分37秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liujun (qihoo inc.), liujun-xy@360.cn
 *        Company:  qihoo inc.
 *
 * =====================================================================================
 */

#include<string.h>
#include<stdlib.h>
#include<limits>
#include<iostream>

#include"img_util.h"

namespace img_util
{

void split_string(const string &text,vector<string> &parts,const string &delimiter,int count)
{
	assert(count==-1 || count>1);

	parts.clear();
	size_t delimiterPos=text.find(delimiter);
	size_t lastPos=0;
	if(delimiterPos == string::npos){
		parts.push_back(text);
		return;
	}

	if(count==-1)
		count=std::numeric_limits<int>::max();

	int split_cnt=0;

	while(delimiterPos != string::npos && split_cnt++ <count-1 ){

		parts.push_back(text.substr(lastPos, delimiterPos - lastPos));
		lastPos = delimiterPos + delimiter.size();
		delimiterPos = text.find(delimiter, lastPos);
	}
	parts.push_back(text.substr(lastPos));
}

void from_vec2map(const vector<string> &vec,map<string,string> &parts,const string &delimiter)
{
	int nsize=vec.size();
	for(int i=0;i<nsize;i++){

		vector<string> temp;

		split_string(vec[i],temp,delimiter,2);

		if(temp.size()<2)
			continue;

		parts[temp[0]]=temp[1];
	}
}

std::string replace(const std::string &str,const std::string &find,const std::string &rep,int count)
{
	std::string result;

	assert(count==-1 || count>0);

	if(str.length()<=0 || find.length()<=0 || str.length()<find.length())
		return str;

	size_t find_pos=str.find(find);
	if(find_pos==string::npos)
		return str;

	if(count==-1)
		count=std::numeric_limits<int>::max();

	int rep_count=0;

	const char *data=str.data();
	const char *fdata=find.data();

	int find_len=find.length();

	int length=str.length();

	int i=0;

	while(i<=length-find_len){

		if(strncmp(data+i,fdata,find_len)==0){

			result+=rep;

			i+=find_len;

			rep_count++;

			if(rep_count>=count)
				break;

			continue;
		}

		result+=str[i++];
	}

	if(i<length)
	{
		result+=str.substr(i);
	}

	return result;
}

//效率实现得比较差,一般不用用
std::string  replace_vec(const std::string &str, std::vector<std::string> &find_vec,std::vector<std::string> &rep_vec,int count)
{
	int find_size=find_vec.size();
	int rep_size=rep_vec.size();

	if(find_size<=0 || rep_size<=0 || find_size!=rep_size)
		return str;

	std::string temp=str;

	for(int i=0;i<find_size;i++){

		temp=replace(temp,find_vec[i],rep_vec[i],count);
	}

	return temp;
}


double get_cur_sec()
{
	struct timeval tv; 
	gettimeofday( &tv, NULL );
	return tv.tv_sec + (double)tv.tv_usec/1000000.0f;
}

}
