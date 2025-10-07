#include <stdio.h>
#include <string.h>
#include "steg.h"
typedef enum
    {
        SUCCESS,
        FAILURE
    }process_state;
typedef struct
    {
        int argc;
        char **argv;
    }program_data;
void _print_help();
process_state encode(program_data *app_info);
process_state decode(program_data *app_info);
int main(int argc,char **argv)
    {
        program_data app_info;
        app_info.argc=argc;
        app_info.argv=argv;
        switch(argc)
            {
                case 3:
                    decode(&app_info);
                    return 0;
                case 4:
                    encode(&app_info);
                    return 0;
                default:
                        _print_help();
                        return 0;
            }
            
    }
process_state encode(program_data *app_info)
    {
        int file_size;
        //////////////////////////////////////////////////////////////////////infile
        FILE *fp_enc=fopen(app_info->argv[2],"r");
            if(fp_enc==NULL)
                {
                    printf("Error opening %s\n",app_info->argv[2]);
                    return FAILURE;
                }
        fseek(fp_enc,0,SEEK_END);
        file_size=ftell(fp_enc);//////////get infilesize
        fseek(fp_enc,0,SEEK_SET);
        /////////////////////////////////////////////////////////////////////bmpin
        FILE *fp_steg=fopen(app_info->argv[1],"r");
            if(fp_steg==NULL)
                {
                    printf("Error opening %s\n",app_info->argv[1]);
                    return FAILURE;
                }
        
        ///////////////////////////////////////////////////////////////////bmpout
        FILE *fp_out=fopen("encoded.bmp","w");
            if(fp_out==NULL)
                {
                    printf("Error encoding, check: encoded.bmp\n");
                    return FAILURE;
                }

       
        char header[200];
        fread(header,54,1,fp_steg);
        if(strncmp(header,"BM",2))//check if in file is bmp
                {
                    printf("%s is not a bmp file\n",app_info->argv[1]);
                    return FAILURE;
                }
                
        fwrite(header,54,1,fp_out);

        fread(header,strlen(app_info->argv[3])*8,1,fp_steg);
        fwrite(string_encode(app_info->argv[3],header),strlen(app_info->argv[3])*8,1,fp_out);//write magic string-0000
       
        fread(header,8,1,fp_steg);
        *(unsigned long*)header=en_steg(*(unsigned long*)header,0x00);//towrite 0
        fwrite(header,8,1,fp_out);//write 0
        printf("Magic String encoded\n");
     
        strcpy(&header[100],get_extension(app_info->argv[2],'.'));//getextension
        fread(header,strlen(&header[100])*8,1,fp_steg);
        fwrite(string_encode(&header[100],header),strlen(&header[100])*8,1,fp_out);//write extension
        
     
        fread(header,8,1,fp_steg);
        *(unsigned long*)header=en_steg(*(unsigned long*)header,0x00);//towrite 0
        fwrite(header,8,1,fp_out);//write 0
        printf("Extension encoded\n");
       
        for(int i=0;i<4;i++)
                {
                   fread(header,8,1,fp_steg);
                   *(unsigned long*)header=en_steg(*(unsigned long*)header,((char*)&file_size)[i]);
                   fwrite(header,8,1,fp_out);
                }//dont use string_encode. only use it for pure strings, not for numerical values
        int writes=0;
        while((header[100]=fread(header,1,8,fp_steg))==8)
                {
                    
                    if(!fread(&header[199],1,1,fp_enc) || !header[100])//header[100]-->double check if true
                        break;
                    *(unsigned long*)header=en_steg(*(unsigned long*)header,header[199]);
                    fwrite(header,8,1,fp_out);
                    writes++;
                }
        while((header[100]=fread(header,1,64,fp_steg)))
                {
                    
                    fwrite(header,header[100],1,fp_out);
                }//write remaining
        if(writes<=file_size)
                printf("%s successfully encoded in %s\n",app_info->argv[2],app_info->argv[1]);
        else 
            printf("%s not fully encoded, %s size insufficient\n",app_info->argv[2],app_info->argv[1]);
        
        fclose(fp_enc);
        fclose(fp_out);
        fclose(fp_steg);
        return SUCCESS;
    }
process_state decode(program_data *app_info)
    {
        FILE *fp_bmp=fopen(app_info->argv[1],"r");
            if(fp_bmp==NULL)
                {
                    printf("Error opening %s\n",app_info->argv[2]);
                    return FAILURE;
                }
        FILE *fp_out=fopen("decode","w");
            if(fp_out==NULL)
                {
                    printf("Error decoding, check if decode(filename) exists\n");
                    return FAILURE;
                }
        char hold[55];
        fread(hold,54,1,fp_bmp);
        if(strncmp(hold,"BM",2))//check if in file is bmp
                {
                    printf("%s is not a bmp file\n",app_info->argv[1]);
                    return FAILURE;
                }    
        int i=0;
        while(fread(&hold[20],1,8,fp_bmp))
                {
                    hold[54]=de_steg(&hold[20]);
                    if(!hold[54])
                        {
                            hold[i]='\0';
                            if(strcmp(hold,app_info->argv[2]))
                                {
                                    printf("Magic String not match\n");
                                    fclose(fp_bmp);
                                    fclose(fp_out);
                                    return FAILURE;
                                }
                                
                            break;
                        }
                    hold[i++]=hold[54];
                }///check magicstring
        printf("Magic String match\n");
        i=0;
        while(fread(&hold[20],1,8,fp_bmp))
                {
                    hold[54]=de_steg(&hold[20]);
                    if(!hold[54])
                        {
                            hold[i]='\0'; 
                            break;
                        }
                    hold[i++]=hold[54];
                }//get extension
        printf(".%s file found\n",hold);
        char secretSize[4];
        for(int j=0;j<4;j++)
            {
                fread(&hold[20],1,8,fp_bmp);
                secretSize[j]=de_steg(&hold[20]);
            }
        // printf("%d\n",*(int*)secretSize);
        for(int j=0;j<*(int*)secretSize;j++)
            {
                fread(&hold[20],1,8,fp_bmp);
                hold[54]=de_steg(&hold[20]);
                fwrite(&hold[54],1,1,fp_out);
            }
        fclose(fp_bmp);
        fclose(fp_out);
        strncpy(&hold[20],"decode",7);
        strcat(&hold[20],".");
        strcat(&hold[20],hold);
        rename("decode",&hold[20]);
        printf("%s decoded succesfully\n",app_info->argv[1]);
        return SUCCESS; 
    }
void _print_help()
    {
        printf("\nTo encode:\v <exe/bin_file> <bmp_file> <encode_file> <magic_string>\n\n");
        printf("To decode:\v <exe/bin_file> <bmp_file> <magic_string>\n\n");
        printf("\nIMP:No * in magic string\n");
    }
