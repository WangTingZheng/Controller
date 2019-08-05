
#include <Tree.h>

char* return_parent_name(char  *name){
    static char parentName[MaxPage];
    int flag=0;
    for(int i=0;i<MaxPage;i++){
        if(name[i]=='.'&&((int)name[i+2]<=49||(int)name[i+2]>=57)&&(int)name[i+2]!=46){//如果当前的字符为.而且它后面两个字符不是数字也不是.的话
            flag=i-1;
        }
    }
    for(int i=0;i<=flag;i++){
        parentName[i]=name[i];
    }
    return parentName;
}
