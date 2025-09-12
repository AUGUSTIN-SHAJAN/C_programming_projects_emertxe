#include <stdio.h>
#include <string.h>
typedef struct
    {
        //char avilable_option[10];
        int argument_number;
        char **inputs;
        int property_size;
        char edit_option[5];
    }program_data;
int endian_conv(int num);
void edit(program_data *app_data);
void view(program_data *app_data);
int init(program_data *app_data);
void print_string(char *str,int size, char str_extend[]);
void print_help();
int main(int argc,char **argv)
    {
         program_data app_data;
        app_data.inputs=argv;
        app_data.argument_number=argc;
        
        switch (argc)
            {
                case 2:
                    if(!strcmp(argv[1],"--help"))
                        goto exit;
                    else
                        {
                            view(&app_data);
                            return 0;
                        }

                case 4:
                    if(!init(&app_data))
                        goto exit;
                    edit(&app_data);
                    return 0;
                default:
                    exit:
                        print_help();
                        return 0;
                    }
        
    }
int init(program_data *app_data)
    {
        if(!strcmp(app_data->inputs[2],"--title"))
            {
                strcpy(app_data->edit_option,"TIT2");
                return 1;
            }
            
        else if(!strcmp(app_data->inputs[2],"--artist"))
            {
                strcpy(app_data->edit_option,"TPE1");
                return 1;
            }
            
        else if(!strcmp(app_data->inputs[2],"--album"))
            {
                strcpy(app_data->edit_option,"TALB");
                return 1;
            }
        else if(!strcmp(app_data->inputs[2],"--year"))
            {
                strcpy(app_data->edit_option,"TYER");
                return 1;
            }
        else
            return 0;

    }
void print_help()
    {

        printf("./<bin_exe> <filename> to view\n");
        printf("./<bin_exe> <filename> <option> <change_string> to edit\n");
        printf("options are:\v --title,--artist,--album,--year\n");
    }
void print_string(char *str,int size, char str_extend[])
    {
        for(int i=0;i<size;i++)
            printf("%c",str[i]);
        printf("%s",str_extend);
    }
int endian_conv(int num)
    {
        return (num & 0x000000ff)<<24|(num & 0xff000000)>>24|(num & 0x0000ff00)<<8|(num & 0x00ff0000)>>8;
    }
void view(program_data *app_data)
    {
        FILE *fp=fopen(app_data->inputs[1],"r");
            if(fp==NULL)
                {
                    printf("Error opening file\n");
                    return;
                }
        char extract[200];
        char head[7]={'I','D','3',3,0};
        fread(extract,10,1,fp);
        if(strncmp(extract,head,5))
                {
                    printf("Wrong File/version\n");
                    return;
                }
        app_data->property_size=endian_conv(*(int*)&extract[6]);
        int i=0;
        int size=0;
        while(i<app_data->property_size)
                {
                    fread(extract,11,1,fp);
                    i+=11;
                    if(!strncmp(extract,"APIC",4))
                        return;
                    print_string(extract,4,"-->");
                    size=endian_conv(*(int*)&extract[4]);
                    size--;//to remove '\0' from the begining
                    fread(extract,size,1,fp);
                    print_string(extract,size,"\n");
                    i+=size;
                }
        fclose(fp);
    }
void edit(program_data *app_data)
    {
        FILE *fpr=fopen(app_data->inputs[1],"r");
            if(fpr==NULL)
                {
                    printf("Error opening file\n");
                    return;
                }
        FILE *fpw=fopen("output.mp3","w");
            if(fpw==NULL)
                {
                    fclose(fpr);
                    printf("Error opening file\n");
                    return;
                }
        char extract[200];
        char head[7]={'I','D','3',3,0};
        fread(extract,10,1,fpr);
        if(strncmp(extract,head,5))
                {
                    fclose(fpr);
                    fclose(fpw);
                    remove("output.mp3");
                    printf("Wrong File/version\n");
                    return;
                }
        fwrite(extract,10,1,fpw);
        int size=0;
        while(1)
                {
                    fread(extract,11,1,fpr);
                    fwrite(extract,4,1,fpw);
                    size=endian_conv(*(int*)&extract[4]);
                    size--;//to remove '\0' from the begining
                    if(!strncmp(extract,app_data->edit_option,4))
                        {
                            int isize=endian_conv(strlen(app_data->inputs[3])+1);
                            fwrite((char*)&isize,4,1,fpw);
                            isize=0;
                            fwrite((char*)&isize,3,1,fpw);
                            fwrite(app_data->inputs[3],strlen(app_data->inputs[3]),1,fpw);
                            fread(extract,size,1,fpr);
                            break;
                        }
                    if(!strncmp(extract,"APIC",4))
                        break;
                    fwrite(&extract[4],7,1,fpw);
                    fread(extract,size,1,fpr);
                    fwrite(extract,size,1,fpw);
                }
            while((size=fread(extract,1,200,fpr)))
                {
                    fwrite(extract,size,1,fpw);
                }
        fclose(fpw);
        fclose(fpr);
        remove(app_data->inputs[1]);
        rename("output.mp3", app_data->inputs[1]);
    }
