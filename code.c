//  author =Daylighttx

#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define NAME 20   //定义名字数组长度为20，可根据需要修改
#define STANDARD 20 //选课学分要求 
#define PASSWORD 1  //管理系统登录密码 
#define CLASSNUM 20 //可选课程数目 
//学生默认密码为学号后四位
 
int Course_NUM,Student_NUM;  //课程数目和学生数目 
struct course * c_head;   //结构体课程指针 
struct student * s_head;  //结构体学生指针 
char C_FILE[20]="course.txt"; //课程文件名称 
char S_FILE[20]="student.txt"; //学生文件名称 

struct course  //课程信息结构体
{
    int c_num;
    char c_name[NAME];
    int kind;//0
    float credit;
    int time;//上课时间：13 星期一第三节课
	char t_name[NAME];//任课教师： 
	char classroom[NAME]; 
    int selected_people;  //课程目前已选人数
    int Maxpeople;  //课程人数上限    
    struct course * next;
};
struct student//学生信息结构体
{
    int st_num;  //学生学号 
    int password;  //学生密码 
    char st_name[NAME];  //学生名字 
    int selected_course[CLASSNUM];  //已选课程编号
    int course_number;  //已选课程数量
    float all_credit;  //总学分 
    struct student * next;
};




/* 文件加载与存储模块 ------------------------------------------------------------------------------------------------*/
//包括 readfile_course()   savefile_course()	readfile_student()	savefile_student() 


void readfile_course()  //加载课程信息
{
    FILE * fp;
    struct course *p1,*p2;
    Course_NUM=0;    
    fp = fopen(C_FILE,"r");
    if((fp=fopen(C_FILE,"r"))==NULL)
    {
        printf("+---------------------------------------------------------+\n");
        printf("| 找不到课程文件!                                         |\n");
        exit(0);
    }
    p1=p2=(struct course*)malloc(sizeof(struct course));
    fscanf(fp, "%d%s%d%f%d%s%s%d%d", &p1->c_num, p1->c_name, &p1->kind, &p1->credit, &p1->time, p1->t_name, p1->classroom, &p1->selected_people, &p1->Maxpeople);
    c_head = NULL;
    while(!feof(fp))
    {
        Course_NUM = Course_NUM + 1;
        if(Course_NUM==1)
			c_head=p1;
        else 
			p2->next=p1;
        p2=p1;
        p1=(struct course*)malloc(sizeof(struct course));
        fscanf(fp, "%d%s%d%f%d%s%s%d%d", &p1->c_num, p1->c_name, &p1->kind, &p1->credit, &p1->time, p1->t_name, p1->classroom, &p1->selected_people, &p1->Maxpeople);
	}
    p2->next=NULL;
}

void savefile_course()//存储课程信息
{
    //调用文件指针将数据保存 
    FILE* fp;
    struct course* p;
    fp = fopen(C_FILE, "w+");
    if ((fp = fopen(C_FILE, "w+")) == NULL)
    {
        printf("+---------------------------------------------------------------------------------------------------+\n");
        printf("| 保存失败!                                               \n");
        exit(0);
    }
    p = c_head;
    while (p != NULL)
    {
        fprintf(fp, "%d %s %d %f %d %s %s %d %d\n", p->c_num, p->c_name, p->kind, p->credit, p->time, p->t_name, p->classroom, p->selected_people, p->Maxpeople);
        p = p->next;
    }
    fclose(fp);
    
}

void readfile_student()  //加载学生信息
{
    int i = 0;
    FILE* fp;
    struct student* p1, * p2;
    Student_NUM = 0;
    fp = fopen(S_FILE, "r");
    if ((fp = fopen(S_FILE, "r")) == NULL)
    {
        printf("+-----------------------------------------------------+\n");
        printf("|找不到student.txt文件!                               |\n");
        exit(0);
    }
    p1 = p2 = (struct student*)malloc(sizeof(struct student));
    fread(p1, sizeof(struct student), 1, fp);
    s_head = NULL;
    while (!feof(fp))
    {
        i = 0;
        Student_NUM = Student_NUM + 1;
        if (Student_NUM == 1)
			s_head = p1;
        else 
			p2->next = p1;
        p2 = p1;
        p1 = (struct student*)malloc(sizeof(struct student));
        fread(p1, sizeof(struct student), 1, fp);
    }
    p2->next = NULL;
}  

void savefile_student() //存储学生信息
{
    FILE* fp;
    struct student* p;
    fp = fopen(S_FILE, "w");
    if ((fp = fopen(S_FILE, "w")) == NULL)
    {
        printf("+---------------------------------------------------------+\n");
        printf("| 保存失败!                                               |\n");
        exit(0);
    }
    p = s_head;
    while (p != NULL)
    {
        fwrite(p, sizeof(struct student), 1, fp);
        p = p->next;
    }
    fclose(fp);

}
/* 文件加载与存储模块------------------------------------------------------------------------------------------------*/
/*  ^
	|
	|
*/


/* ----课程选择模块------------------------------------------------------------------------------------------------*/ 
//包括  list_selectedcourse(struct student *Stu)，Select_course(struct student *Stu)，Drop_course(struct student * Stu)，student_page() 

void list_selectedcourse(struct student *Stu)  //学生查询已选课程 
{
    struct course * p0;
    p0 = (struct course*)malloc(sizeof(struct student));
    int i=0,f=0;
    printf("+---------------------------------------------------------------------------------------------------+\n");
	printf("|                                           已选课程                                                |\n");
	printf("+--------+----------------+---------+----+-------------+--------+----------------+--------+---------+\n");
	printf("|课程编号|    课程名称    |选修/必修|学分|   上课时间  |课程教师|    上课地点    |已选人数| 人数上限|\n");
	printf("+--------+----------------+---------+----+-------------+--------+----------------+--------+---------+\n");
	char kind1[]="必修";
	char kind0[]="选修"; 
	char kinds[4]; 
	while(Stu->selected_course[i]!=0)
    {
    		p0 = c_head;
    		while(Stu->selected_course[i]!=p0->c_num)
    		{
    			p0=p0->next;
			}
			p0->kind ? strcpy(kinds, kind1):strcpy(kinds, kind0);
			printf("|%8d|%16s|%9s| %.1f|星期%d 第%d节课|%8s|%16s|%8d|%9d|\n",p0->c_num,p0->c_name,kinds,p0->credit,p0->time/10,p0->time%10,p0->t_name,p0->classroom,p0->selected_people,p0->Maxpeople);	
			printf("+--------+----------------+---------+----+-------------+--------+----------------+--------+---------+\n");
			i++;
	}
	printf("+---------------------------------------------------------------------------------------------------+\n");
    printf("|总学分: %.1f                                                                                       |\n",Stu->all_credit);
    if(Stu->all_credit>=STANDARD)
    {
    	printf("|已达到学分要求                                                                                     |\n"); 
	}
	else
	{
		printf("|未达到学分要求，还差%.1f分                                                                         |\n",STANDARD-Stu->all_credit); 
	}
    printf("+---------------------------------------------------------------------------------------------------+\n");
}


void Select_course(struct student *Stu) //学生选课函数
{
	while(1)
	{
			
	    struct course * c;
	    struct course *c1;
	    struct course *c2;
	    int st_num,i,j=0,t=0;
	    c = c_head;
	    int n=0;
	    int Num; //一个中间变量 
	    int mark;  //一个中间变量 
	    list_selectedcourse(Stu);
	    printf("+---------------------------------------------------------------------------------------------------+\n");
	    printf("|你的可选课程编号:                                                                                  |\n");
	    
		//选出未选的课 
		char kind1[]="必修";
		char kind0[]="选修"; 
		char kinds[4];
		while(c!=NULL)
	    {
	    	j=0;
			for(i=0;i<Stu->course_number;i++)
			{
				if(Stu->selected_course[i]==c->c_num)
				{
					j++;
				}
			}
			if(j==0)
			{
				c->kind ? strcpy(kinds, kind1):strcpy(kinds, kind0);
				printf("+--------+----------------+---------+----+-------------+--------+----------------+--------+---------+\n");
				printf("|%8d|%16s|%9s| %.1f|星期%d 第%d节课|%8s|%16s|%8d|%9d|\n",c->c_num,c->c_name,kinds,c->credit,c->time/10,c->time%10,c->t_name,c->classroom,c->selected_people,c->Maxpeople);	
				printf("+--------+----------------+---------+----+-------------+--------+----------------+--------+---------+\n");
	    		
			}
			c = c->next;
		}
		while(1)
		{
			j=0; //用j来做开关 
			printf("+---------------------------------------------------------------------------------------------------+\n");
			printf("|请输入你想选择的课程编号(输入0退出)：\n");
			scanf("%d",&n);
			if(n==0)
			{
				break;
			}
			else
			{
				c1 = c_head;
				c2 = c_head;
				while(c1!=NULL)
			    {
			    	
			    	j=0;
			    	if(c1->c_num==n)  //如果读取到的课程编号与课程链表中的课程一样的话 
			    	{ 
			    		
			    		for(i=0;i<Stu->course_number;i++)  
			    		{
			    			Num=Stu->selected_course[i];  //用两个指针遍历c_head 
			    			while(c2!=NULL)  //判断时间冲突 
			    			{
			    				if(c2->c_num==Num)
			    				{
			    					if(c1->time==c2->time)
			    					printf("+---------------------------------------------------------------------------------------------------+\n");
			    					printf("|选课时间冲突，不能选课！\n");
			    					j=1; 
								}
								c2 = c2->next;
							}
			    			if(n==Stu->selected_course[i])  //判断课程冲突 
			    			{
			    				printf("+---------------------------------------------------------------------------------------------------+\n");
			    				printf("|课程已选过！\n");
								j=1; 
							}
						}
			    		if(j==0)
			    		{
			    			Stu->selected_course[Stu->course_number] = n;
			    			mark = Stu->course_number + 1;  //使后面数组的值全为0 
			    			Stu->selected_course[mark]=0;
							Stu->course_number+=1;
							Stu->all_credit += c1->credit;
							c1->selected_people +=1;
							printf("+---------------------------------------------------------------------------------------------------+\n");
							printf("|选课成功！\n");
							system("pause");
							j=1;
							break;
						}
					}
					c1 = c1->next;
				}
			}
			if(j==1)
			{
				continue;
			}
			if(n==0)
			{
				break;
			} 
			if(c1==NULL)
			{
				printf("+---------------------------------------------------------------------------------------------------+\n");
				printf("|请输入正确的课程编号：\n");
				continue;
			}
			list_selectedcourse(Stu);
			system("pause");
		}	
		if(n==0)
		{
			break;		
		} 	
	}
	savefile_course();
	savefile_student();
}

void Drop_course(struct student * Stu)  //退课
{
	while(1)
	{
	    struct course * p1;
	    int c_num,i,j;
	    //system("cls");
	    list_selectedcourse(Stu);
	    printf("+---------------------------------------------------------------------------------------------------+\n");
	    printf("|请输入你要退掉的课程编号(输入0退出):                                                               |\n");
	    scanf("%d",&c_num);
	    if(c_num==0)
	    {
	    	break;
		}
	    p1=c_head;
	    while(p1->c_num!=c_num) 
			p1=p1->next;
		if(p1->kind==1)
		{
			printf("+---------------------------------------------------------------------------------------------------+\n");
			printf("|这门课为必修课，不能退选！！！                                                                     |\n");
			printf("|请重新输入！！                                                                                     |\n");
			printf("+---------------------------------------------------------------------------------------------------+\n");
			system("pause");
			system("cls");
			continue;
		}
	    for(i=0;Stu->selected_course[i]!=c_num;i++);
	    {
		
	    for(j=i;Stu->selected_course[j]!=0;j++) 
			Stu->selected_course[j]=Stu->selected_course[j+1];
		}
	    Stu->selected_course[--j]=0;
	    p1->selected_people -= 1;
	    Stu->course_number -= 1;
	    Stu->all_credit -= p1->credit; 
	    printf("+---------------------------------------------------------------------------------------------------+\n");
	    printf("| 退课成功!                                                                                         |\n");
	    printf("+---------------------------------------------------------------------------------------------------+\n");
	    savefile_student();
		savefile_course();                       
	    
	}
}

void Change_Password(struct student* Stu) //修改登录密码 
{
	while(1)
	{
		printf("欢迎您：%s\n",Stu->st_name);
		int old_p, new_p;
		printf("请输入原有密码：\n");
		scanf("%d",&old_p);
		if(old_p==Stu->password)
		{
			printf("请输入新密码：\n");
			scanf("%d",&new_p);
			Stu->password=new_p;
			//savefile_course();
			savefile_student(); 
			printf("新密码保存成功！即将返回\n");
			system("pause"); 
		}
		else
		{
			break;
		}
		break;
	}
}

void student_page()//学生登陆页面 
{
	struct student *Stu;
	Stu = (struct student *)malloc(sizeof(struct student));
	Stu = s_head;
	int StuNum,PassWord;
	//学生登陆系统 
	while(1)
	{
		system("cls");
		int n;
		int N=5; //密码错误能尝试的最大次数 
		printf("+---------------------------------------------------------+\n");
		printf("|                    学生选课系统                         |\n");
		printf("+---------------------------------------------------------+\n");
		printf("|请输入学号：(输入0退出)\n");
		scanf("%d",&StuNum);
		if(StuNum==0)
		{
			break;
		} 
		for(n=1;n<=Student_NUM;n++)
		{
			if(Stu->st_num==StuNum)
			{

				break;
			}
			else
			{
				Stu = Stu->next;
			}
		}
		if(Stu==NULL)
		{
			printf("|请输入正确的学号!\n");
			printf("|本系统中没有找到您的信息，请联系管理老师进行添加\n");
			system("pause");
			continue; 
		}
		else
		{
			while(N--)
			{
				
				printf("|请输入密码：\n");
				scanf("%d",&PassWord);
				if(Stu->password==PassWord)
				{
					printf("|密码正确！\n");
					printf("|欢迎您：%s同学！",Stu->st_name);
					system("pause"); 
					break;
				}
				else
				{
					printf("密码错误，您还有%d次机会\n",N);
					if(N==0)
					{
						break; 
					}
					continue; 
				}
			}
			if(N==0)
			{
				printf("|即将返回！\n");
				StuNum=0;
				system("pause");
				break;
			}
			else
			{
				break;
			} 
		}
	}
	if(StuNum!=0)
	{
		while(1)
		{
		    int i;
		    system("cls");
			
		    printf("+---------------------------------------------------------+\n");
		    printf("|                         学生选课                        |\n");
		    printf("+--+------------------------------------------------------+\n");
		    printf("|1 | 选择课程                                             |\n");
		    printf("+--+------------------------------------------------------+\n");
		    printf("|2 | 退课                                                 |\n");
		    printf("+--+------------------------------------------------------+\n");
		    printf("|3 | 查询已选课程                                         |\n");
		    printf("+--+------------------------------------------------------+\n");
		    printf("|4 | 修改密码                                             |\n");
		    printf("+--+------------------------------------------------------+\n");
		    printf("|5 | 返回上一级                                           |\n");
		    printf("+--+------------------------------------------------------+\n");
		    printf("请输入(1~5):                                              |\n");
		    scanf("%d",&i);
		    while(i<1||i>5)   // 规范输入 
		    { 
				printf("+---------------------------------------------------------+\n");
		    	printf("请输入正确的数字！                                        |\n");
		    	printf("+---------------------------------------------------------+\n");
		    	scanf("%d",&i);
		    } 
		    switch(i)
		    {
			    case(1):
				{
					system("cls");
					Select_course(Stu);
					continue;
					break;
				}
		
			    case(2):
				{
					system("cls");
					Drop_course(Stu);
					continue;
					break;
				}
				case(3):
				{
					system("cls");
					while(1)
					{ 
						char q;
						list_selectedcourse(Stu);
					    printf("按q返回上一级\n");
						scanf("%c",&q); 
						if(q=='q'||q=='Q')
						{
							break;
						}
						else
						system("cls");
					} 
					continue;
					break;
				} 
				case(4):
				{
					system("cls");
					Change_Password(Stu);
					continue;
					break;
				}
			    case(5):system("cls");break;
		    }
		    break;
		}
	}
}

/* ----课程选择模块------------------------------------------------------------------------------------------------*/
/*  ^
	|
	|
*/



/* ///---管理系统模块---------------------------------------------------------------------------------*/ 


/* 课程管理子模块----------------------------------------------------------------------------------------*/
//delete_course(int c_num)，getfile_course() ,list_course()，getfile_course()， management_course()

void delete_course(int c_num)  //课程管理子函数(删除课程)
{
    struct course *p1,*p2;
    if(c_head==NULL)
    {
        printf("+---------------------------------------------------------+\n");
        printf("| 没有课程,无法删除!                                      |\n");
        printf("+---------------------------------------------------------+\n");
        printf("\n");
        printf("\n");    
    }
    else
    {
	
	    p1=c_head;
	    while(c_num!=p1->c_num && p1->next!=NULL)
	    {
	        p2=p1;
	        p1=p1->next;
	    }
	    if(c_num==p1->c_num)
	    {
	        if(p1==c_head) c_head=p1->next;
	        else p2->next=p1->next;
	        printf("+---------------------------------------------------------+\n");
	        printf("|已删除该编号课程!                                        |\n");
	        printf("+---------------------------------------------------------+\n");
	        printf("\n");
	        printf("\n");
	        Course_NUM=Course_NUM-1;
	    }
	    else
	    {
	        printf("+---------------------------------------------------------+\n");
	        printf("| 无该编号的课程!                                         |\n");
	        printf("+---------------------------------------------------------+\n");
	        printf("\n");
	        printf("\n");
	    }
	}

}
 void list_course() //展示现在的课程表 
 {
	struct course *p;
	p = c_head;
	char kind1[]="必修";
	char kind0[]="选修"; 
	char kinds[4];
	printf("+--------+----------------+---------+----+-------------+--------+----------------+--------+---------+\n");
	printf("|课程编号|    课程名称    |选修/必修|学分|   上课时间  |课程教师|    上课地点    |已选人数| 人数上限|\n");
	printf("+--------+----------------+---------+----+-------------+--------+----------------+--------+---------+\n");
	while(p!=NULL)
	{
	p->kind ? strcpy(kinds, kind1):strcpy(kinds, kind0);
	printf("|%8d|%16s|%9s| %.1f|星期%d 第%d节课|%8s|%16s|%8d|%9d|\n",p->c_num,p->c_name,kinds,p->credit,p->time/10,p->time%10,p->t_name,p->classroom,p->selected_people,p->Maxpeople);	
	printf("+--------+----------------+---------+----+-------------+--------+----------------+--------+---------+\n");
	p = p->next;
	}
	printf("\n\n");
  } 


void getfile_course()  //读入新课程数据 
{
	while(1)
	{
	
		//-----------------------------------------
		//输入课程信息 
		struct course* incourse;
		incourse = (struct course* )malloc(sizeof(struct course));
		char Stu[5];
		int num1;
		int num2;
		
		printf("请输入课程编号：");
		scanf("%d",&incourse->c_num);
		printf("\n");
		
		printf("请输入课程名称：");
		scanf("%s",incourse->c_name);
		printf("\n");
		
		printf("请输入课程种类（选修/必修）：");
		scanf("%s",Stu);
		if(!strcmp(Stu,"选修")||(Stu[0]=='0'))
		{
			incourse->kind = 0;
		}
		else if(!strcmp(Stu,"必修")||(Stu[0]=='1')) 
		{
			incourse->kind = 1;
		}
		printf("\n");
	
		printf("请输入课程学分：");
		scanf("%f",&incourse->credit);
		printf("\n");
		
		printf("请输入课程时间：\n");
		printf("星期__"); 
		scanf("%d",&num1);
		printf("第__节课");
		scanf("%d",&num2);
		incourse->time = num1*10 + num2 ; 
		printf("\n");
		
		printf("请输入课程教师名称：");
		scanf("%s",incourse->t_name);
		printf("\n");
		
		printf("请输入课程教室：");
		scanf("%s",incourse->classroom);
		printf("\n");
		
		incourse->selected_people = 0; //选课人数默认为0 
		
		printf("请输入课程最大人数：");
		scanf("%d",&incourse->Maxpeople);
		printf("\n");
	
		//--------------------------------------------------
		//将获取的数据保存至链表中 
	    struct course* p0, * p1, * p2;
	    p1=p2 = c_head;
	    p0 = incourse;
	    if (c_head == NULL)
	    {
	        c_head = p0;
	        p0->next = NULL;
	    }
	    else
	    {
	        while ((p0->c_num > p1->c_num) && (p1->next != NULL))
	        {
	            p2 = p1;
	            p1 = p1->next;
	        }
	        if (p0->c_num <= p1->c_num)
	        {
	        	if(p0->c_num < p1->c_num)
	        	{
		            if (c_head == p1)
						c_head = p0;
		            else 
						p2->next = p0;
		            p0->next = p1;
	        	}
	        	else
	        	{
	        		system("cls");
					printf("课程编号重复，请重新输入！\n");
					list_course();
					continue;
				}
	        }
	        else
	        {
	            p1->next = p0;
	            p0->next = NULL;
	        }
	    }
	    Course_NUM = Course_NUM + 1;
	    
	    //-------------------- 
		//保存修改后的信息 
		savefile_course();
		//-------------------- 
		//显示修改后的课程信息 
	    list_course();
	    break;
	}
} 
 
 void management_course()  //课程管理主函数
{
	while(1)
	{
		system("cls");
	    struct course * newcourse;
	    int i,c_num;
	    printf("+---------------------------------------------------------+\n");
	    printf("|                        课程管理                         |\n");
	    printf("+--+------------------------------------------------------+\n");
	    printf("|1 | 新增课程                                             |\n");
	    printf("+--+------------------------------------------------------+\n");
	    printf("|2 | 删除课程                                             |\n");
	    printf("+--+------------------------------------------------------+\n");
	    printf("|3 | 查看课程                                             |\n");
	    printf("+--+------------------------------------------------------+\n");
	    printf("|4 | 返回上一级                                           |\n");
	    printf("+--+------------------------------------------------------+\n");
	    printf("| 请选择(1~4):                                            |\n");
	    scanf("%d",&i);
	    while(i<1||i>4)   // 规范输入 
	    { 
			printf("+--+------------------------------------------------------+\n");
	    	printf("请输入正确的数字！                                        |\n");
	    	printf("+--+------------------------------------------------------+\n");
	    	scanf("%d",&i);
	    } 
		
	    switch(i)
	    {
		    case(1):
		        {
					system("cls");
					list_course();
					getfile_course();
					system("pause");
		            continue;
		            break;
		        	
		        }

		        
		    case(2):
		        {
					while(1)
					{
					 	system("cls");
			            //输出目前的课程 
						list_course();
	               		printf("+---------------------------------------------------------------------------------------------------+\n");
			            printf("|请输入要删除课程的编号: (输入0退出)                           \n");
			            scanf("%d",&c_num);    
			            if(c_num)
			            {
			            	delete_course(c_num);
			            	list_course();
			            	savefile_course();
			            	system("pause");
			            	continue; 
			            }
			            else
			            {
			            	printf("+---------------------------------------------------------------------------------------------------+\n");
			            	printf("|!即将返回                        \n"); 
			            	system("pause");
			            	break;
						}
			        }
		            continue;
		            break;
		        }
		    case(3):
				{
					while(1)
					{ 
						char q;
						list_course();
					    printf("按q返回上一级\n");
						scanf("%c",&q); 
						if(q=='q'||q=='Q')
						{
							break;
						}
						else
						system("cls");
					} 
					continue;
					break;
				}
			 
		    case(4):
		    	{
					system("cls");
					break;
				}
	    }
	    break; 
    } 

}

/* 课程管理子模块----------------------------------------------------------------------------------------*/ 
/*  ^
	|
	|
*/


/* 学生信息管理子模块--------------------------------------------------------------------------- */
//list_student(), getfile_student(),delete_student(int st_num),management_student() 

void list_student()//输出学生信息
{
    struct student* p;
    p = s_head;
    printf("+--------+----------------+------------+------+\n");
    printf("|学生学号|    学生姓名    |已选课程数量|总学分|\n");
    printf("+--------+----------------+------------+------+\n");
    while (p != NULL)
    {
        printf("|%8d|%16s|%12d|%6.1f|\n", p->st_num, p->st_name, p->course_number,p->all_credit);
        printf("+--------+----------------+------------+------+\n");
        p = p->next;
    }
    printf("\n");
    printf("\n");
}

void getfile_student()  //读入新课程数据 
{
	while(1)
	{
		struct course* c1;//在添加学生时就直接自动选择了必修课 
		c1=c_head;
		int i=0;
		struct student* instudent;
		instudent = (struct student*)malloc(sizeof(struct student));
		
		
		printf("请输入学生学号(输入0退出)：");
		scanf("%d",&instudent->st_num);
		if(instudent->st_num==0)
		{
			break;
		}
		printf("\n");
		
		instudent->password = instudent->st_num % 10000 ;  //初始密码为学号后四位 
		
		printf("学生姓名：");
		scanf("%s",instudent->st_name);
		printf("\n");
		
		for(i=0;i<CLASSNUM;i++)
		{
			instudent->selected_course[i]=0;
		}
		i=0;
		//在添加学生时自动选择了必修课 
		instudent->course_number = 0;
		instudent->all_credit = 0;
		while(c1!=NULL)
		{
			if(c1->kind==1)
			{
				c1->selected_people +=1;
				instudent->all_credit += c1->credit;
				instudent->selected_course[i]=c1->c_num;
				instudent->course_number += 1;
				i++;
			}
			c1=c1->next;
		}
		 
		
		struct student *p0,*p1,*p2;
	    p1=s_head;
	    p0 = instudent;
	    if(s_head==NULL)
	    {
	        s_head=p0;
	        p0->next=NULL;
	    }
	    else
	    {
	        while((p0->st_num > p1->st_num) && (p1->next!=NULL))
	        {
	            p2=p1;
	            p1=p1->next;
	        }
	        if(p0->st_num <= p1->st_num)
	        {
	        	if(p0->st_num < p1->st_num)
		        {
				    if(s_head==p1) 
						s_head=p0;
		            else 
						p2->next=p0;
		            p0->next=p1;
		        }
		        else
		        { 
					system("cls");
					printf("学号重复，请重新输入！\n");
					list_student();
					continue;	 
		        }
		    }
	        else
	        {
	            p1->next=p0;
	            p0->next=NULL;
	        }
	    }
	    Student_NUM=Student_NUM+1;
	    savefile_course();
		savefile_student();
		printf("+---------------------------------------------+\n");
		printf("|学生信息已保存！                             |\n");
		printf("+---------------------------------------------+\n");
	}
}

void delete_student(int st_num)  //学生信息管理子函数（删除学生信息)
{
    struct student *p1,*p2;
    struct course *p0;
    int i;
    if(s_head==NULL)
    {
        printf("+---------------------------------------------------------+\n");
        printf("| 没有学生信息,无法删除!                                  |\n");
        printf("+---------------------------------------------------------+\n");
        printf("\n");
        printf("\n");
        
    }
	else
	{ 
		p0 = c_head;
	    p1 = s_head;
	    while(st_num!=p1->st_num && p1->next!=NULL)
	    {
	        p2=p1;
	        p1=p1->next;
	    }
	    if(st_num==p1->st_num)
	    {
	    	
	    	for(i=0;p1->selected_course[i]!=0;i++)
	    	{
	    		p0 =c_head;
	    		while(p0->c_num!=p1->selected_course[i])
	    		{
	    			p0 = p0->next;
				}
				p0->selected_people -= 1;
			}
			
	        if(p1==s_head) 
				s_head=p1->next;
	        else 
				p2->next=p1->next;
	        printf("+---------------------------------------------+\n");
	        printf("|已删除该学生信息!                            |\n");
	        Student_NUM = Student_NUM - 1;
	        savefile_course();
	        savefile_student();
	    }
	    else
	    {
	      printf("+---------------------------------------------+\n");
	      printf("|无该学号的学生!                              |\n");
	    }
    } 
    
}

void management_student()  //学生信息管理页面
{
	while(1)
	{
		system("cls");
	    struct student * newcourse;
	    int i,st_num;
	    printf("+---------------------------------------------------------+\n");
	    printf("|                      学生信息管理                       |\n");
	    printf("+--+------------------------------------------------------+\n");
	    printf("|1 | 增加学生                                             |\n");
	    printf("+--+------------------------------------------------------+\n");
	    printf("|2 | 删除学生                                             |\n");
	    printf("+--+------------------------------------------------------+\n");
	    printf("|3 | 查看学生信息                                         |\n");
	    printf("+--+------------------------------------------------------+\n");
	    printf("|4 | 返回上一级                                           |\n");
	    printf("+--+------------------------------------------------------+\n");
	    printf("|请选择(1~4):                                             |\n");
	    scanf("%d",&i);
	    while(i<1||i>4)   // 规范输入 
	    { 
			printf("+--+------------------------------------------------------+\n");
	    	printf("请输入正确的数字！                                        |\n");
	    	printf("+--+------------------------------------------------------+\n");
	    	scanf("%d",&i);
	    } 
		
	    switch(i)
	    {
		    case(1):
		        {
		            system("cls");
					//显示学生信息
					list_student();
					getfile_student();
					list_student();
					system("pause");
		            continue;
		            break;
		        }
		    case(2):
		        {
		        	while(1)
					{
					 	system("cls");
			            //输出目前的课程 
						list_student();
	               		printf("+---------------------------------------------+\n");
			            printf("|请输入要删除学生的学号: (输入0退出)          |\n");
			            scanf("%d",&st_num);    
			            if(st_num)
			            {
			            	delete_student(st_num);
			            	list_student();
			            	savefile_course();
			            	savefile_student();
							printf("+---------------------------------------------+\n");
							printf("已保存！"); 
			            	system("pause");
			            	continue; 
			            }
			            else
			            {
			            	printf("+---------------------------------------------+\n");
			            	printf("|!即将返回                        \n"); 
			            	system("pause");
			            	break;
						}
			        }
		            continue;
		            break;
		        }
		    case(3):
				{
					while(1)
					{
						char q;
						list_student();
			    		printf("\n");
			    	    printf("按q返回上一级\n");
						scanf("%c",&q); 
						if(q=='q'||q=='Q')
						{
							break;
						}
						else
						system("cls");
					}
					continue;	
					break;
				} 
		    case(4):
				system("cls");break;
	    }
	    break;
	}
}

/* 学生信息管理子模块--------------------------------------------------------------------------- */
/*  ^
	|
	|
*/


void manage_page()   //管理系统页面 
{
		
    int p,i;
    printf("+---------------------------------------------------------+\n");
    printf("| 管理系统                                                |\n" );
    printf("+---------------------------------------------------------|\n");
    printf("请输入密码                                                |\n");
    scanf("%d",&p);
    if(p==PASSWORD)
    {
	    	while(1)
	    {
			
	        system("cls");
	        printf("+---------------------------------------------------------+\n");
	        printf("|                       选课系统管理                      |\n");
	        printf("|---------------------------------------------------------|\n");
	        printf("|1 | 课程管理                                             |\n");
	        printf("|--+------------------------------------------------------|\n");
	        printf("|2 | 学生管理                                             |\n");
	        printf("|--+------------------------------------------------------|\n");
	        printf("|3 | 返回主菜单                                           |\n");
	        printf("|---------------------------------------------------------|\n");
	        printf("|请输入(1~3):                                             |\n");
	        scanf("%d",&i);
	        while(i<1||i>3)   // 规范输入 
		    { 
				printf("+--+------------------------------------------------------+\n");
		    	printf("请输入正确的数字！                                        |\n");
		    	printf("+--+------------------------------------------------------+\n");
		    	scanf("%d",&i);
		    } 
	        switch (i) {
	            case (1):
				{
					system("cls");
					management_course();
					continue;//返回这个界面 
					break;
				}
	            case (2):
				{
					system("cls");
					management_student();
					continue;
					break;
				} 
	            case (3):
				{	
					system("cls");
					break;
				} 
	        }
	        break;
	    }
    }
    else
    {
    	
        printf("+---------------------------------------------------------+\n");
        printf("| 密码错误     即将返回！！！                             |\n");
        printf("+---------------------------------------------------------+\n");
        system("pause");
    }
	
}

/* ///---管理系统模块---------------------------------------------------------------------------------*/ 
/*  ^
	|
	|
*/


int main()  //主函数（主页面） 
{
	while(1)
	{
		int i;
	    system("cls");
	    readfile_course();
	    readfile_student();
	
	    printf("+---------------------------------------------------------+\n");
	    printf("|                   欢迎使用学生选课系统!                 |\n");
	    printf("+---------------------------------------------------------+\n");
	    printf("|                           主菜单                        |\n");
	    printf("+---------------------------------------------------------+\n");
	    printf("|1 | 学生选课                                             |\n");
	    printf("+--+------------------------------------------------------+\n");
	    printf("|2 | 教师管理                                             |\n");
	    printf("+--+------------------------------------------------------+\n");
	    printf("|3 | 退出系统                                             |\n");
	    printf("+---------------------------------------------------------+\n");
	    printf("|请输入菜单选项:                                          |\n");
	    while(1)
	    {
			scanf("%d",&i);
		    if(i<1 || i>3)
		    {
		        printf("\n");
		        printf("+---------------------------------------------------------+\n");
		        printf("| 输入错误,请重输:                                        |\n");
		        continue;
		    }
		    else
		    	break;
		}
	    switch(i)
	    {
	    case(1):
	        {
	            system("cls"); // windows cls
	            student_page();
	            continue;
	            break;
	        }
	    case(2):
	        {
	            system("cls");
	            manage_page();
	            continue;
	            break;         
	        }
	        
	
	    case(3):
	        {
	            system("cls");
	            printf("+---------------------------------------------------------+\n");
	            printf("|  感谢使用本系统!再见!                                   |\n");
	            printf("+---------------------------------------------------------+\n");
	            exit(0); 
	        }
	    }
	}
    return(0);
}
