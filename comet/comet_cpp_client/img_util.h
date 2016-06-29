/*
 * =====================================================================================
 *
 *       Filename:  util.h
 *
 *    Description:  util function
 *
 *        Version:  1.0
 *        Created:  2015年01月13日 12时33分36秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  liujun (qihoo inc.), liujun-xy@360.cn
 *        Company:  qihoo inc.
 *
 * =====================================================================================
 */

#ifndef __IMG_SPIDER_UTIL_
#define __IMG_SPIDER_UTIL_

#include<string>
#include<vector>
#include<map>
#include<sys/time.h>
#include<sys/types.h>
#include<assert.h>

using namespace std;

namespace img_util
{

//count表示最多切分成几块
void split_string(const string &text,vector<string> &parts,const string &delimiter=" ",int count=-1);

//将指定分隔符分割的vector,变成map
void from_vec2map(const vector<string> &vec,map<string,string> &parts,const string &delimiter=":");

//替换字符串,会改变原来的字符串,默认相当于replace_all
std::string replace(const std::string &str,const std::string &find,const std::string &rep,int count=-1);

//替换所有的字符串,如果find_vec.size()!=rep_vec.size(),直接返回,否则进行对等替换
std::string replace_vec(const std::string &str, std::vector<std::string> &find_vec,std::vector<std::string> &rep_vec,int count=-1);

double get_cur_sec();

}

#endif
