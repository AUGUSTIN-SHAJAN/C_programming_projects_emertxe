//Author: Augustin Shajan
//Description:CRUD operations on student records

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio_ext.h>

typedef struct
    {
        char name[96],email[96];
        long int phone_number;
    } student;

typedef struct
{
    int currentSize,maxSize;
    char file[100],stsmsg[300];
    student *addHold;
} programDetail;
    
void create(programDetail *info);
void search(programDetail *info);
void edit(programDetail *info);
void delete(programDetail *info);
void listAll(programDetail *info);
void saveDetails(programDetail *info);
int init(programDetail *info);
int retrieve(programDetail *info);

int main(int argc, char **argv)
    {
        if(argc<2)
            {
                printf("Input File Required\n");
                return 0;
            }
        programDetail info;
        strcpy(info.file,argv[1]);

        if(!init(&info))
            return 0;
            
        info.maxSize=info.currentSize+100;
        student addressBook[info.maxSize];
        info.addHold=addressBook;

        retrieve(&info);
        
        while(1)
            {
                system("clear");
                int option;
                printf("-------------------Menu\n1)Create\t\t\t\tstatus->%s\n2)Search\n3)Edit\n4)Delete\n5)List all\n6)Save Contacts and Exit\n",info.stsmsg);
                scanf("%d",&option);
                system("clear");
                switch(option)
                    {
                        case 1:
                            create(&info);
                            break;
                        case 2:
                            search(&info);
                            break;
                        case 3:
                            edit(&info);
                            break;
                        case 4:
                            delete(&info);
                            break;
                        case 5:
                            listAll(&info);
                            break;
                        case 6:
                            saveDetails(&info);
                            printf("Details saved to %s\n",info.file);
                            return 0;
                        default:
                            printf("Invalid option\n");
                            break;
                    }
            }
    }

void create(programDetail *info)
    {
        if(info->currentSize<info->maxSize)
            {
                student temp;
                do{
                    printf("phone number(10 digit),name,email:");
                    __fpurge(stdin);
                }while(scanf("%ld,%95[^,],%95[^\n]",&temp.phone_number,temp.name,temp.email)!=3 || temp.phone_number>9999999999L || temp.phone_number<1000000000L);
                int i=0,flag=0;
                for(i=0;i<info->currentSize;i++)
                    {
                        if(temp.phone_number==info->addHold[i].phone_number)
                        {
                            flag=1;
                            break;
                        }
                     }
                if(flag)
                    strcpy(info->stsmsg,"Phone number already exists");
                else
                {
                    info->addHold[i].phone_number=temp.phone_number;
                    strcpy(info->addHold[i].name,temp.name);
                    strcpy(info->addHold[i].email,temp.email);
                    info->currentSize++;
                    sprintf(info->stsmsg,"added to %s",info->file);
                }
            
            }
        else
            strcpy(info->stsmsg,"Alloted memory full, rerun program");
    }
void search(programDetail *info)
    {
        student temp;
        do{
            printf("phone number(10 digit):");
            __fpurge(stdin);
        }while(scanf("%ld",&temp.phone_number)!=1 || temp.phone_number>9999999999L || temp.phone_number<1000000000L);
        //should i do getchar() for \n;
        int i=0,flag=0;
        for(i=0;i<info->currentSize;i++)
            {
                if(temp.phone_number==info->addHold[i].phone_number)
                    {
                        flag=1;
                        break;
                    }
            }
        if(flag)
            sprintf(info->stsmsg,"phone:%ld,name:%s,email:%s",info->addHold[i].phone_number,info->addHold[i].name,info->addHold[i].email);
        else
            strcpy(info->stsmsg,"Not found");
    }
void edit(programDetail *info)
    {
        student temp;
        do{
            printf("phone number(10 digit),name,email:");
            __fpurge(stdin);
        }while(scanf("%ld,%95[^,],%95[^\n]",&temp.phone_number,temp.name,temp.email)!=3 || temp.phone_number>9999999999L || temp.phone_number<1000000000L);
        int i=0,flag=0;
        for(i=0;i<info->currentSize;i++)
            {
                if(temp.phone_number==info->addHold[i].phone_number)
                    {
                        flag=1;
                        break;
                    }
            }
        if(flag)
            {
                info->addHold[i].phone_number=temp.phone_number;
                strcpy(info->addHold[i].name,temp.name);
                strcpy(info->addHold[i].email,temp.email);
                strcpy(info->stsmsg,"Edited");
            }
        else
            sprintf(info->stsmsg,"%ld  doesnt exist",temp.phone_number);
    }
void delete(programDetail *info)
    {
        student temp;
        do{
            printf("phone number(10 digit):");
            __fpurge(stdin);
        }while(scanf("%ld",&temp.phone_number)!=1 || temp.phone_number>9999999999L || temp.phone_number<1000000000L);
        //should i do getchar() for \n;
        int i=0,flag=1;
        for(i=0;i<info->currentSize;i++)
            {
                if(temp.phone_number==info->addHold[i].phone_number)
                    {
                        flag=0;
                        break;
                    }
            }
        if(flag)
            sprintf(info->stsmsg,"%ld  doesnt exist",temp.phone_number);
        else
            {
                for(int j=i;j<(info->currentSize-1);j++)
                    {
                        info->addHold[j].phone_number=info->addHold[j+1].phone_number;
                        strcpy(info->addHold[j].name,info->addHold[j+1].name);
                        strcpy(info->addHold[j].email,info->addHold[j+1].email);
                    }
                info->currentSize--;
                sprintf(info->stsmsg,"%ld  deleted:(",temp.phone_number);
            }
    }
void saveDetails(programDetail *info)
    {
        FILE *fp=NULL;
        if((fp=fopen(info->file,"w"))==NULL)
            {
                sprintf(info->stsmsg,"Save to file failed,Error opening %s",info->file);
            }
        else
            {
                fprintf(fp,"Phone Number,Name,EmailID,Size:%d\n",info->currentSize);
                for(int i=0;i<info->currentSize;i++)
                    {
                        fprintf(fp,"%ld,%s,%s\n",info->addHold[i].phone_number,info->addHold[i].name,info->addHold[i].email);
                    }
                sprintf(info->stsmsg,"Saved to %s",info->file);
                fclose(fp);
            }
    }
void listAll(programDetail *info)
    {
        sprintf(info->stsmsg,"Enter 1 to go back to options:)");
        printf("listing available data\t\t\t\tstatus: %s\n",info->stsmsg);
        for(int i=0;i<info->currentSize;i++)
            {
                printf("phone:%ld,name:%s,email:%s\n",info->addHold[i].phone_number,info->addHold[i].name,info->addHold[i].email);
            }
        int i=0;
        scanf("%d",&i);
        if(i!=1)
            {
                __fpurge(stdin);
                system("clear");
                listAll(info);
            }
    }
int retrieve(programDetail *info)
    {
        FILE *fp=NULL;
        if((fp=fopen(info->file,"r"))==NULL)
            {
                sprintf(info->stsmsg,"File retrieved");
                return 0;
            }
            
        fscanf(fp,"Phone Number,Name,EmailID,Size:%d\n",&info->currentSize);
        for(int i=0;i<info->currentSize;i++)
            {
                fscanf(fp,"%ld,%95[^,],%95[^\n]\n",&(info->addHold[i].phone_number),info->addHold[i].name,info->addHold[i].email);
            }
        sprintf(info->stsmsg,"Retrieved data from %s",info->file);
        fclose(fp);
        return 1;
    }
int init(programDetail *info)
    {
        FILE *fp=NULL;
        if((fp=fopen(info->file,"r"))==NULL)
            {
                printf("Initialization failed.\nError opening \"%s\".\n",info->file);
                printf("Check if \"%s\" is in the same directory.\n",info->file);
                return 0;
            }
            
        if(!(fscanf(fp,"Phone Number,Name,EmailID,Size:%d\n",&info->currentSize)==1))
                    {
                        fclose(fp);
                        info->currentSize=0;
                    }
                return 1;
    }