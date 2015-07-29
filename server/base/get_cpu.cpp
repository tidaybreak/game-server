/*
 * get_cpu.cpp
 *
 *  Created on: Dec 25, 2014
 *      Author: ti
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   //头文件
#include <assert.h>
#include <string.h>
#include <sys/sysinfo.h>
#include "get_cpu.h"
#include <dirent.h>
#include <ctype.h>
#define READ_BUF_SIZE 256

int get_phy_mem(const pid_t p)
{
    char file[64] = {0};//文件名

    FILE *fd;         //定义文件指针fd
    char line_buff[256] = {0};  //读取行的缓冲区
    sprintf(file,"/proc/%d/status",p);//文件中第11行包含着

    //fprintf (stderr, "current pid:%d\n", p);
    fd = fopen (file, "r"); //以R读的方式打开文件再赋给指针fd

    //获取vmrss:实际物理内存占用
    int i;
    char name[32];//存放项目名称
    int vmrss;//存放内存峰值大小
    for (i=0;i<VMRSS_LINE-1;i++) {
        fgets (line_buff, sizeof(line_buff), fd);
    }//读到第15行
    fgets (line_buff, sizeof(line_buff), fd);//读取VmRSS这一行的数据,VmRSS在第16行
    sscanf (line_buff, "%s %d", name,&vmrss);
    //fprintf (stderr, "====%s：%d====\n", name,vmrss);
    fclose(fd);     //关闭文件fd
    return vmrss;
}

int get_total_mem()
{
    const char* file = "/proc/meminfo";//文件名

    FILE *fd;         //定义文件指针fd
    char line_buff[256] = {0};  //读取行的缓冲区
    fd = fopen (file, "r"); //以R读的方式打开文件再赋给指针fd

    //获取memtotal:总内存占用大小
    char name[32];//存放项目名称
    int memtotal;//存放内存峰值大小
    fgets (line_buff, sizeof(line_buff), fd);//读取memtotal这一行的数据,memtotal在第1行
    sscanf (line_buff, "%s %d", name,&memtotal);
    //fprintf (stderr, "====%s：%d====\n", name,memtotal);
    fclose(fd);     //关闭文件fd
    return memtotal;
}

float get_pmem(pid_t p)
{
    int phy = get_phy_mem(p);
      int total = get_total_mem();
      float occupy = (phy*1.0)/(total*1.0);
      fprintf(stderr,"====process mem occupy:%.6f\n====",occupy);
      return occupy;
}

unsigned int get_cpu_process_occupy(const pid_t p)
{
    char file[64] = {0};//文件名
    process_cpu_occupy_t t;

    FILE *fd;         //定义文件指针fd
    char line_buff[1024] = {0};  //读取行的缓冲区
    sprintf(file,"/proc/%d/stat",p);//文件中第11行包含着

    fprintf (stderr, "current pid:%d\n", p);
    fd = fopen (file, "r"); //以R读的方式打开文件再赋给指针fd
    fgets (line_buff, sizeof(line_buff), fd); //从fd文件中读取长度为buff的字符串再存到起始地址为buff这个空间里

    sscanf(line_buff,"%u",&t.pid);//取得第一项
    const char* q = get_items(line_buff,PROCESS_ITEM);//取得从第14项开始的起始指针
    sscanf(q,"%u %u %u %u",&t.utime,&t.stime,&t.cutime,&t.cstime);//格式化第14,15,16,17项

    fprintf (stderr, "====pid%u:%u %u %u %u====\n", t.pid, t.utime,t.stime,t.cutime,t.cstime);
    fclose(fd);     //关闭文件fd
    return (t.utime + t.stime + t.cutime + t.cstime);
}


unsigned int get_cpu_total_occupy()
{
    FILE *fd;         //定义文件指针fd
    char buff[1024] = {0};  //定义局部变量buff数组为char类型大小为1024
    total_cpu_occupy_t t;

    fd = fopen ("/proc/stat", "r"); //以R读的方式打开stat文件再赋给指针fd
    fgets (buff, sizeof(buff), fd); //从fd文件中读取长度为buff的字符串再存到起始地址为buff这个空间里
    /*下面是将buff的字符串根据参数format后转换为数据的结果存入相应的结构体参数 */
    char name[16];//暂时用来存放字符串
    sscanf (buff, "%s %u %u %u %u", name, &t.user, &t.nice,&t.system, &t.idle);


    fprintf (stderr, "====%s:%u %u %u %u====\n", name, t.user, t.nice,t.system, t.idle);
    fclose(fd);     //关闭文件fd
    return (t.user + t.nice + t.system + t.idle);
}


float get_pcpu(pid_t p)
{
    unsigned int totalcputime1,totalcputime2;
    unsigned int procputime1,procputime2;
    totalcputime1 = get_cpu_total_occupy();
    procputime1 = get_cpu_process_occupy(p);
    usleep(500000);//延迟500毫秒
    totalcputime2 = get_cpu_total_occupy();
    procputime2 = get_cpu_process_occupy(p);
    float pcpu = 100.0*(procputime2 - procputime1)/(totalcputime2 - totalcputime1);
    //fprintf(stderr,"====pcpu:%.6f\n====",pcpu);
    // 占单核的百分比
    return pcpu * get_nprocs();
}

const char* get_items(const char* buffer,int ie) {
    assert(buffer);
    const char* p = buffer;//指向缓冲区
    int len = strlen(buffer);
    int count = 0;//统计空格数
    if (1 == ie || ie < 1)
    {
        return p;
    }
    int i;

    for (i=0; i<len; i++)
    {
        if (' ' == *p)
        {
            count++;
            if (count == ie-1)
            {
                p++;
                break;
            }
        }
        p++;
    }

    return p;
}

long get_mpid(const char* path) {
	// 函数返回0，并不是说进程号0不存在，是因为mongod进程号不可能为0，故做无效判断
	DIR *dir = NULL;
	struct dirent *next = NULL;
	dir = opendir("/proc");
	if (NULL == dir || NULL == path)
	    return 0;
	while((next = readdir(dir)) != NULL) {

	    FILE *status = NULL;
	    char filename[READ_BUF_SIZE] = {0};
	    char buffer[READ_BUF_SIZE] = {0};
	    char name[READ_BUF_SIZE] = {0};
	    if (strcmp(next->d_name,".") == 0 || strcmp(next->d_name,"..") == 0)
	        continue;

	    if (!isdigit(*next->d_name))
	        continue;

	    sprintf(filename,"/proc/%s/status", next->d_name);
	    if (!(status = fopen(filename, "r")))
	        continue;

	    if (fgets(buffer,READ_BUF_SIZE-1, status) == NULL) {
	        fclose(status);
	        continue;
	    }
	    fclose(status);
	    sscanf(buffer, "%*s %s", name);
	    if ( name != NULL && name[0] != '\0') {
	        if (strcmp(name,"mongod") == 0) {
	        	char softfile[READ_BUF_SIZE] = {0};
	            long tmp_pid = strtol(next->d_name, NULL, 0);

	            memset(filename,0,READ_BUF_SIZE);

	            sprintf(filename,"/proc/%s/cwd", next->d_name);
	            int rslt = readlink(filename, softfile, READ_BUF_SIZE - 1);
	            if ( rslt < 0 || (rslt >= READ_BUF_SIZE - 1) )
	                continue;

	            if( strstr( softfile,path) ) {
	            	closedir(dir);
	            	return tmp_pid;
	            }else
	                continue;
	        }
	    }
	}
	closedir(dir);
	return 0;
}
